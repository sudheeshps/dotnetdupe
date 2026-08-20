#include "pch.h"
#include "System/Diagnostics/Process.h"
#include "System/ArgumentException.h"
#include "System/UnauthorizedAccessException.h"
#include "System/IO/FileNotFoundException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/List.h"
#include "System/Utils/StringConvert.h"

#if defined(_WIN32)
#include <windows.h>
#include <tlhelp32.h>
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>
#include <spawn.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cerrno>
extern char** environ;
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            ProcessStartInfo::ProcessStartInfo() 
                : CreateNoWindow(false), UseShellExecute(false) {}

            ProcessStartInfo::ProcessStartInfo(const String& sFileName) 
                : FileName(sFileName), CreateNoWindow(false), UseShellExecute(false) {}

            ProcessStartInfo::ProcessStartInfo(const String& sFileName, const String& sArguments) 
                : FileName(sFileName), Arguments(sArguments), CreateNoWindow(false), UseShellExecute(false) {}

            Process::Process() 
                : m_iId(0), m_iExitCode(0), m_bHasExited(true), m_pProcessHandle(nullptr) {}

            Process::Process(int iId, const String& sProcessName, void* pProcessHandle)
                : m_iId(iId), m_sProcessName(sProcessName), m_iExitCode(0), m_bHasExited(false), m_pProcessHandle(pProcessHandle) {}

            Process::~Process() {
#if defined(_WIN32)
                if (m_pProcessHandle != nullptr) {
                    CloseHandle((HANDLE)m_pProcessHandle);
                }
#endif
            }

#if defined(_WIN32)
            static std::wstring BuildCommandLine(const ProcessStartInfo& info) {
                std::wstring sWFileName = StringConvertInternal::Utf8ToWChar(info.FileName.GetRawString());
                std::wstring sWArgs = StringConvertInternal::Utf8ToWChar(info.Arguments.GetRawString());
                std::wstring sCmd = L"\"" + sWFileName + L"\"";
                if (info.Arguments.GetLength() > 0) {
                    sCmd += L" " + sWArgs;
                }
                return sCmd;
            }

            static bool CreateWin32Process(const ProcessStartInfo& info, PROCESS_INFORMATION& pi) {
                STARTUPINFOW si;
                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));

                std::wstring sCmd = BuildCommandLine(info);
                DWORD dwFlags = info.CreateNoWindow ? CREATE_NO_WINDOW : 0;
                if (::CreateProcessW(NULL, (LPWSTR)sCmd.c_str(), NULL, NULL, FALSE, dwFlags, NULL, NULL, &si, &pi)) {
                    return true;
                }

                DWORD dwErr = ::GetLastError();
                if (dwErr == ERROR_FILE_NOT_FOUND || dwErr == ERROR_PATH_NOT_FOUND) {
                    throw IO::FileNotFoundException("The system cannot find the file specified.");
                }
                if (dwErr == ERROR_ACCESS_DENIED) {
                    throw UnauthorizedAccessException("Access denied starting process. Higher privileges required.");
                }
                return false;
            }
#else
            static std::vector<std::string> ParsePosixArgs(const ProcessStartInfo& info) {
                std::vector<std::string> argStrings;
                argStrings.push_back(info.FileName.GetRawString());

                std::string sArgs = info.Arguments.GetRawString();
                std::string sCurrentArg;
                bool bInQuotes = false;

                for (size_t i = 0; i < sArgs.length(); ++i) {
                    char c = sArgs[i];
                    if (c == '\"') {
                        bInQuotes = !bInQuotes;
                    } else if (c == ' ' && !bInQuotes) {
                        if (!sCurrentArg.empty()) {
                            argStrings.push_back(sCurrentArg);
                            sCurrentArg.clear();
                        }
                    } else {
                        sCurrentArg += c;
                    }
                }
                if (!sCurrentArg.empty()) argStrings.push_back(sCurrentArg);
                return argStrings;
            }

            static bool SpawnPosixProcess(const ProcessStartInfo& info, pid_t& pid) {
                auto argStrings = ParsePosixArgs(info);
                std::vector<char*> argv;
                for (auto& s : argStrings) argv.push_back((char*)s.c_str());
                argv.push_back(NULL);

                int err = posix_spawn(&pid, info.FileName.GetRawString(), NULL, NULL, argv.data(), environ);
                if (err == 0) return true;
                if (err == ENOENT) throw IO::FileNotFoundException("The system cannot find the file specified.");
                if (err == EACCES || err == EPERM) throw UnauthorizedAccessException("Access denied starting process.");
                return false;
            }
#endif

#if defined(_WIN32)
            static bool StartProcessPlatform(const ProcessStartInfo& info, int& iId, void*& pHandle, bool& bExited) {
                PROCESS_INFORMATION pi;
                if (CreateWin32Process(info, pi)) {
                    iId = static_cast<int>(pi.dwProcessId);
                    pHandle = (void*)pi.hProcess;
                    ::CloseHandle(pi.hThread);
                    bExited = false;
                    return true;
                }
                return false;
            }

            static bool WaitForExitPlatform(void* pHandle, int iMilliseconds) {
                DWORD dwTimeout = (iMilliseconds == -1) ? INFINITE : (DWORD)iMilliseconds;
                return (WaitForSingleObject((HANDLE)pHandle, dwTimeout) == WAIT_OBJECT_0);
            }

            static void KillProcessPlatform(void* pHandle) {
                if (!::TerminateProcess((HANDLE)pHandle, 1)) {
                    DWORD dwErr = ::GetLastError();
                    if (dwErr == ERROR_ACCESS_DENIED) throw UnauthorizedAccessException("Cannot terminate target process: access denied.");
                    if (dwErr == ERROR_INVALID_HANDLE) throw InvalidOperationException("Process has already exited.");
                }
            }

            static bool RefreshProcessPlatform(void* pHandle, int& iExitCode) {
                DWORD dwCode;
                if (GetExitCodeProcess((HANDLE)pHandle, &dwCode) && dwCode != STILL_ACTIVE) {
                    iExitCode = static_cast<int>(dwCode);
                    return true;
                }
                return false;
            }
#else
            static bool StartProcessPlatform(const ProcessStartInfo& info, int& iId, void*& pHandle, bool& bExited) {
                pid_t pid;
                if (SpawnPosixProcess(info, pid)) {
                    iId = static_cast<int>(pid);
                    pHandle = (void*)(intptr_t)pid;
                    bExited = false;
                    return true;
                }
                return false;
            }

            static bool WaitForExitPlatform(void* pHandle, int iMilliseconds, int& iExitCode) {
                int iStatus;
                pid_t pid = (pid_t)(intptr_t)pHandle;
                if (iMilliseconds == -1) {
                    if (waitpid(pid, &iStatus, 0) == pid) {
                        iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                        return true;
                    }
                    return false;
                }
                int iElapsed = 0;
                while (iElapsed < iMilliseconds) {
                    pid_t res = waitpid(pid, &iStatus, WNOHANG);
                    if (res == pid) {
                        iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                        return true;
                    }
                    if (res != 0 && errno == ECHILD) return true;
                    usleep(10000); iElapsed += 10;
                }
                return false;
            }

            static void KillProcessPlatform(void* pHandle, int& iExitCode) {
                pid_t pid = (pid_t)(intptr_t)pHandle;
                if (kill(pid, SIGKILL) != 0) {
                    if (errno == EPERM) throw UnauthorizedAccessException("Cannot terminate target process: access denied.");
                    if (errno == ESRCH) throw InvalidOperationException("Process has already exited.");
                }
                int iStatus;
                if (waitpid(pid, &iStatus, 0) == pid) {
                    iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                }
            }

            static bool RefreshProcessPlatform(void* pHandle, int& iExitCode) {
                int iStatus;
                pid_t res = waitpid((pid_t)(intptr_t)pHandle, &iStatus, WNOHANG);
                if (res > 0) {
                    iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                    return true;
                }
                return (res == -1 && errno == ECHILD);
            }
#endif

            bool Process::Start() {
                if (m_objStartInfo.FileName.GetLength() == 0) return false;
                return StartProcessPlatform(m_objStartInfo, m_iId, m_pProcessHandle, m_bHasExited);
            }

            SmartPointer<Process> Process::Start(const String& sFileName) {
                return Start(ProcessStartInfo(sFileName));
            }

            SmartPointer<Process> Process::Start(const String& sFileName, const String& sArguments) {
                return Start(ProcessStartInfo(sFileName, sArguments));
            }

            SmartPointer<Process> Process::Start(const ProcessStartInfo& objStartInfo) {
                SmartPointer<Process> pProcess = SmartPointer<Process>::NewShared();
                pProcess->SetStartInfo(objStartInfo);
                try {
                    if (pProcess->Start()) return pProcess;
                } catch (const IO::FileNotFoundException&) {
                    return SmartPointer<Process>(nullptr);
                }
                return SmartPointer<Process>(nullptr);
            }

            void Process::WaitForExit() {
                WaitForExit(-1);
            }

            bool Process::WaitForExit(int iMilliseconds) {
                if (m_bHasExited || m_pProcessHandle == nullptr) return true;
#if defined(_WIN32)
                if (WaitForExitPlatform(m_pProcessHandle, iMilliseconds)) {
                    Refresh();
                    return true;
                }
                return false;
#else
                if (WaitForExitPlatform(m_pProcessHandle, iMilliseconds, m_iExitCode)) {
                    m_bHasExited = true;
                    return true;
                }
                return m_bHasExited;
#endif
            }

            bool Process::GetHasExited() const {
                Refresh();
                return m_bHasExited;
            }

            void Process::Kill() {
                if (m_bHasExited || m_pProcessHandle == nullptr) return;
#if defined(_WIN32)
                KillProcessPlatform(m_pProcessHandle);
                Refresh();
#else
                KillProcessPlatform(m_pProcessHandle, m_iExitCode);
                m_bHasExited = true;
#endif
            }

            int Process::GetCurrentProcessId() {
#if defined(_WIN32)
                return static_cast<int>(::GetCurrentProcessId());
#else
                return static_cast<int>(getpid());
#endif
            }

            void Process::Refresh() const {
                if (m_bHasExited || m_pProcessHandle == nullptr) return;
                if (RefreshProcessPlatform(m_pProcessHandle, m_iExitCode)) {
                    m_bHasExited = true;
                }
            }

#if defined(_WIN32)
            static bool QueryProcessNameById(int iProcessId, String& sOutName, HANDLE& hOutProc) {
                hOutProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE, FALSE, static_cast<DWORD>(iProcessId));
                if (hOutProc == NULL) return false;
                WCHAR szPath[MAX_PATH] = { 0 }; DWORD dwLen = MAX_PATH;
                if (::QueryFullProcessImageNameW(hOutProc, 0, szPath, &dwLen)) {
                    std::wstring ws(szPath);
                    size_t pos = ws.find_last_of(L"\\/");
                    std::wstring wsName = (pos != std::wstring::npos) ? ws.substr(pos + 1) : ws;
                    sOutName = StringConvertInternal::WCharToUtf8(wsName.c_str()).c_str();
                    return true;
                }
                return false;
            }
#else
            static bool QueryLinuxProcessNameById(int iProcessId, String& sOutName) {
                std::string sPath = "/proc/" + std::to_string(iProcessId) + "/comm";
                std::ifstream commFile(sPath);
                std::string sComm;
                if (commFile.is_open() && std::getline(commFile, sComm)) {
                    sOutName = String(sComm.c_str());
                    return true;
                }
                return false;
            }
#endif

#if defined(_WIN32)
            static void EnumerateWin32Processes(Collections::Generic::List<SmartPointer<Process>>& lstProcs) {
                HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnapshot == INVALID_HANDLE_VALUE) return;
                PROCESSENTRY32W pe32; pe32.dwSize = sizeof(PROCESSENTRY32W);
                if (::Process32FirstW(hSnapshot, &pe32)) {
                    do {
                        if (pe32.th32ProcessID == 0) continue;
                        lstProcs.Add(SmartPointer<Process>::NewShared(static_cast<int>(pe32.th32ProcessID), String(pe32.szExeFile), nullptr));
                    } while (::Process32NextW(hSnapshot, &pe32));
                }
                ::CloseHandle(hSnapshot);
            }
#else
            static void EnumerateLinuxProcesses(Collections::Generic::List<SmartPointer<Process>>& lstProcs) {
                DIR* dir = ::opendir("/proc");
                if (!dir) return;
                struct dirent* entry = nullptr;
                while ((entry = ::readdir(dir)) != nullptr) {
                    if (entry->d_type == DT_DIR) {
                        std::string dname = entry->d_name;
                        if (std::all_of(dname.begin(), dname.end(), ::isdigit)) {
                            std::ifstream commFile("/proc/" + dname + "/comm");
                            std::string sComm;
                            if (commFile.is_open() && std::getline(commFile, sComm)) {
                                lstProcs.Add(SmartPointer<Process>::NewShared(std::stoi(dname), String(sComm.c_str()), nullptr));
                            }
                        }
                    }
                }
                ::closedir(dir);
            }
#endif

            Array<SmartPointer<Process>> Process::GetProcesses() {
                Collections::Generic::List<SmartPointer<Process>> lstProcs;
#if defined(_WIN32)
                EnumerateWin32Processes(lstProcs);
#else
                EnumerateLinuxProcesses(lstProcs);
#endif
                Array<SmartPointer<Process>> arrProcs(lstProcs.GetCount());
                for (int i = 0; i < lstProcs.GetCount(); ++i) arrProcs[i] = lstProcs[i];
                return arrProcs;
            }

            static bool FindProcessInSnapshot(int iProcessId, String& sOutName) {
                auto arrProcs = Process::GetProcesses();
                for (int i = 0; i < arrProcs.GetLength(); ++i) {
                    if (arrProcs[i]->GetId() == iProcessId) {
                        sOutName = arrProcs[i]->GetProcessName();
                        return true;
                    }
                }
                return false;
            }

            SmartPointer<Process> Process::GetProcessById(int iProcessId) {
                if (iProcessId <= 0) throw ArgumentException("Process ID must be greater than zero.");
                String sName;
#if defined(_WIN32)
                HANDLE hProc = NULL;
                if (!QueryProcessNameById(iProcessId, sName, hProc) && !FindProcessInSnapshot(iProcessId, sName)) {
                    throw ArgumentException("Process with specified ID is not running.");
                }
                return SmartPointer<Process>::NewShared(iProcessId, sName, hProc);
#else
                if (!QueryLinuxProcessNameById(iProcessId, sName) && !FindProcessInSnapshot(iProcessId, sName)) {
                    throw ArgumentException("Process with specified ID is not running.");
                }
                return SmartPointer<Process>::NewShared(iProcessId, sName, nullptr);
#endif
            }

            SmartPointer<Process> Process::GetCurrentProcess() {
                return GetProcessById(GetCurrentProcessId());
            }

            static bool MatchProcessName(const String& sCandidate, const String& sTarget) {
                if (sCandidate.Equals(sTarget)) return true;
                String sCandNorm = sCandidate.EndsWith(".exe", true) ? sCandidate.Substring(0, sCandidate.GetLength() - 4) : sCandidate;
                String sTargNorm = sTarget.EndsWith(".exe", true) ? sTarget.Substring(0, sTarget.GetLength() - 4) : sTarget;
                return sCandNorm.ToLower().Equals(sTargNorm.ToLower());
            }

            Array<SmartPointer<Process>> Process::GetProcessesByName(const String& sProcessName) {
                if (sProcessName.IsEmpty()) return Array<SmartPointer<Process>>(0);
                auto arrAll = GetProcesses();
                Collections::Generic::List<SmartPointer<Process>> lstMatches;
                for (int i = 0; i < arrAll.GetLength(); ++i) {
                    if (MatchProcessName(arrAll[i]->GetProcessName(), sProcessName)) {
                        lstMatches.Add(arrAll[i]);
                    }
                }
                Array<SmartPointer<Process>> arrResult(lstMatches.GetCount());
                for (int i = 0; i < lstMatches.GetCount(); ++i) arrResult[i] = lstMatches[i];
                return arrResult;
            }
        }
    }
}
