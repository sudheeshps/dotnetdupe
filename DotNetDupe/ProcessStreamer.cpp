#include "pch.h"
#include "System/Diagnostics/ProcessStreamer.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/SystemException.h"
#include "System/Threading/Thread.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include "System/InvalidOperationException.h"
#include <vector>
#include <string>
#include <atomic>
#include <algorithm>
#include <cctype>

#if defined(_WIN32)
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#else
#include <dirent.h>
#include <fstream>
#include <unistd.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

#if defined(_WIN32)
            static void FastPopulateProc(PROCESSENTRY32W* pe32, ProcessInfo& proc) {
                proc.iProcessId = pe32->th32ProcessID;
                proc.sName = String(pe32->szExeFile);
                proc.memory.lPhysicalMemoryBytes = 0;
                proc.memory.lPrivateBytes = 0;
                DWORD dwSess = 0;
                if (::ProcessIdToSessionId(pe32->th32ProcessID, &dwSess)) proc.iSessionId = static_cast<int>(dwSess);
                if (pe32->th32ProcessID == 0) return;
                HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe32->th32ProcessID);
                if (!hProc) return;
                WCHAR szPath[MAX_PATH] = { 0 }; DWORD dwLen = MAX_PATH;
                if (::QueryFullProcessImageNameW(hProc, 0, szPath, &dwLen)) proc.sPath = String(szPath);
                PROCESS_MEMORY_COUNTERS pmc;
                if (::GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))) {
                    proc.memory.lPhysicalMemoryBytes = static_cast<long long>(pmc.WorkingSetSize);
                    proc.memory.lPrivateBytes = static_cast<long long>(pmc.PagefileUsage);
                }
                ::CloseHandle(hProc);
            }

            static void CollectTier1Processes(std::vector<ProcessInfo>& vecProcs, int iSessionId) {
                HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnapshot == INVALID_HANDLE_VALUE) throw SystemException("Failed to create system process snapshot.");
                PROCESSENTRY32W pe32; pe32.dwSize = sizeof(PROCESSENTRY32W);
                if (::Process32FirstW(hSnapshot, &pe32)) {
                    do {
                        if (pe32.th32ProcessID == 0) continue;
                        ProcessInfo proc;
                        FastPopulateProc(&pe32, proc);
                        if (proc.iProcessId > 0 && (iSessionId == -1 || proc.iSessionId == iSessionId)) {
                            vecProcs.push_back(proc);
                        }
                    } while (::Process32NextW(hSnapshot, &pe32));
                }
                ::CloseHandle(hSnapshot);
            }
#else
            static void FastPopulateLinuxProc(const std::string& dname, ProcessInfo& proc) {
                proc.iProcessId = std::stoi(dname);
                std::ifstream commFile("/proc/" + dname + "/comm");
                std::string procComm;
                if (commFile.is_open() && std::getline(commFile, procComm)) proc.sName = String(procComm.c_str());
                std::ifstream statmFile("/proc/" + dname + "/statm");
                unsigned long size = 0, resident = 0;
                if (statmFile >> size >> resident) {
                    long pageSize = sysconf(_SC_PAGE_SIZE);
                    proc.memory.lPhysicalMemoryBytes = static_cast<long long>(resident * pageSize);
                }
            }

            static void CollectTier1Processes(std::vector<ProcessInfo>& vecProcs, int iSessionId) {
                DIR* dir = ::opendir("/proc");
                if (!dir) throw SystemException("Failed to open /proc directory.");
                struct dirent* entry = nullptr;
                while ((entry = ::readdir(dir)) != nullptr) {
                    if (entry->d_type == DT_DIR) {
                        std::string dname = entry->d_name;
                        if (std::all_of(dname.begin(), dname.end(), ::isdigit)) {
                            ProcessInfo proc;
                            FastPopulateLinuxProc(dname, proc);
                            if (iSessionId == -1 || proc.iSessionId == iSessionId) vecProcs.push_back(proc);
                        }
                    }
                }
                ::closedir(dir);
            }
#endif

            static void DeepEnrichProc(ProcessInfo& proc, bool bIncludeNetwork) {
                SystemMetrics::EnrichProcessInfo(proc, bIncludeNetwork);
            }

            class ProcessStreamer::Impl : public Object {
            public:
                ProcessStreamOptions m_options;
                Action<const ProcessInfo&> m_fnOnProcess;
                Action<const Collections::Generic::List<ProcessInfo>&> m_fnOnBatch;
                Action<const ProcessInfo&> m_fnOnUpdated;
                Action<> m_fnOnCompleted;
                Action<const Exception&> m_fnOnError;
                SmartPointer<IProcessObserver> m_spObserver;
                Threading::CriticalSection m_csLock;
                std::atomic<bool> m_bRunning;
                std::atomic<bool> m_bCancelled;
                SmartPointer<Threading::Thread> m_spWorkerThread;

                Impl(const ProcessStreamOptions& options)
                    : m_options(options), m_bRunning(false), m_bCancelled(false) {}

                ~Impl() override {
                    Cancel();
                }

                void DispatchProcess(const ProcessInfo& proc) {
                    if (m_fnOnProcess) m_fnOnProcess(proc);
                    if (m_spObserver) m_spObserver->OnProcessDiscovered(proc);
                }

                void DispatchBatch(const Collections::Generic::List<ProcessInfo>& lstBatch) {
                    if (m_fnOnBatch) m_fnOnBatch(lstBatch);
                    if (m_spObserver) m_spObserver->OnBatchReady(lstBatch);
                }

                void DispatchUpdated(const ProcessInfo& proc) {
                    if (m_fnOnUpdated) m_fnOnUpdated(proc);
                    if (m_spObserver) m_spObserver->OnProcessUpdated(proc);
                }

                void DispatchCompleted() {
                    if (m_fnOnCompleted) m_fnOnCompleted();
                    if (m_spObserver) m_spObserver->OnCompleted();
                }

                void DispatchError(const Exception& ex) {
                    if (m_fnOnError) m_fnOnError(ex);
                    if (m_spObserver) m_spObserver->OnError(ex);
                }

                void RunTier1(std::vector<ProcessInfo>& vecProcs) {
                    Collections::Generic::List<ProcessInfo> batch;
                    int iBatchLimit = (m_options.iBatchSize > 0) ? m_options.iBatchSize : 25;
                    for (size_t i = 0; i < vecProcs.size(); ++i) {
                        if (m_bCancelled.load()) break;
                        DispatchProcess(vecProcs[i]);
                        batch.Add(vecProcs[i]);
                        if (batch.GetCount() >= iBatchLimit) {
                            DispatchBatch(batch);
                            batch.Clear();
                            if (m_options.iBatchIntervalMs > 0) Threading::Thread::Sleep(m_options.iBatchIntervalMs);
                        }
                    }
                    if (batch.GetCount() > 0 && !m_bCancelled.load()) DispatchBatch(batch);
                }

                void RunTier2(std::vector<ProcessInfo>& vecProcs) {
                    for (size_t i = 0; i < vecProcs.size(); ++i) {
                        if (m_bCancelled.load()) break;
                        DeepEnrichProc(vecProcs[i], m_options.bIncludeNetworkInfo);
                        DispatchUpdated(vecProcs[i]);
                    }
                }

                void ExecuteStream() {
                    try {
                        std::vector<ProcessInfo> vecProcs;
                        CollectTier1Processes(vecProcs, m_options.iSessionId);
                        RunTier1(vecProcs);
                        if (!m_bCancelled.load() && m_options.eDetailLevel != ProcessMetricsDetail::FastDiscoveryOnly) {
                            RunTier2(vecProcs);
                        }
                        m_bRunning.store(false);
                        if (!m_bCancelled.load()) DispatchCompleted();
                    } catch (const Exception& ex) {
                        m_bRunning.store(false);
                        DispatchError(ex);
                    }
                }

                void Start(const SmartPointer<Impl>& spSelf) {
                    if (m_options.iBatchSize < 0 || m_options.iBatchIntervalMs < 0) {
                        throw ArgumentException("ProcessStreamOptions batch parameters cannot be negative.");
                    }
                    if (m_bRunning.exchange(true)) throw InvalidOperationException("ProcessStreamer is already running.");
                    m_bCancelled.store(false);
                    m_spWorkerThread = SmartPointer<Threading::Thread>::NewShared([spSelf]() {
                        if (spSelf) spSelf->ExecuteStream();
                    });
                    m_spWorkerThread->Start();
                }

                void Cancel() {
                    m_bCancelled.store(true);
                    if (m_spWorkerThread && m_spWorkerThread->IsAlive()) {
                        if (Threading::Thread::GetCurrentThreadId() != m_spWorkerThread->GetCurrentThreadId()) {
                            m_spWorkerThread->Join();
                        }
                    }
                    m_bRunning.store(false);
                }
            };

            ProcessStreamer::ProcessStreamer(const ProcessStreamOptions& options)
                : m_pImpl(SmartPointer<Impl>::NewShared(options)) {}

            ProcessStreamer::~ProcessStreamer() {}

            void ProcessStreamer::OnProcess(const Action<const ProcessInfo&>& fnOnProcess) {
                if (m_pImpl) m_pImpl->m_fnOnProcess = fnOnProcess;
            }

            void ProcessStreamer::OnBatch(const Action<const Collections::Generic::List<ProcessInfo>&>& fnOnBatch) {
                if (m_pImpl) m_pImpl->m_fnOnBatch = fnOnBatch;
            }

            void ProcessStreamer::OnProcessUpdated(const Action<const ProcessInfo&>& fnOnUpdated) {
                if (m_pImpl) m_pImpl->m_fnOnUpdated = fnOnUpdated;
            }

            void ProcessStreamer::OnCompleted(const Action<>& fnOnCompleted) {
                if (m_pImpl) m_pImpl->m_fnOnCompleted = fnOnCompleted;
            }

            void ProcessStreamer::OnError(const Action<const Exception&>& fnOnError) {
                if (m_pImpl) m_pImpl->m_fnOnError = fnOnError;
            }

            void ProcessStreamer::Subscribe(const SmartPointer<IProcessObserver>& pObserver) {
                if (!pObserver) throw ArgumentNullException("pObserver cannot be null.");
                if (m_pImpl) m_pImpl->m_spObserver = pObserver;
            }

            void ProcessStreamer::Start() {
                if (!m_pImpl) throw InvalidOperationException("ProcessStreamer implementation is null.");
                m_pImpl->Start(m_pImpl);
            }

            void ProcessStreamer::Cancel() {
                if (m_pImpl) m_pImpl->Cancel();
            }

            bool ProcessStreamer::IsRunning() const {
                return m_pImpl ? m_pImpl->m_bRunning.load() : false;
            }

            ProcessStreamOptions ProcessStreamer::GetOptions() const {
                return m_pImpl ? m_pImpl->m_options : ProcessStreamOptions();
            }

        }
    }
}
