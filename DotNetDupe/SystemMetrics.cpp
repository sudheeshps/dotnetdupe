#include "pch.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Utils/StringConvert.h"
#include <algorithm>
#include <vector>
#include <map>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iphlpapi.h>
#include <pdh.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "pdh.lib")
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            SystemMetrics::SystemMetrics() {}
            SystemMetrics::~SystemMetrics() {}

#if defined(_WIN32)
            struct ProcessCpuSample {
                uint64_t uProcessTime;
                uint64_t uSystemTime;
            };
            static std::map<int, ProcessCpuSample> s_mapProcessCpuSamples;

            void SystemMetrics::ReadWin32Memory(RealTimeSystemInfo& data) {
                MEMORYSTATUSEX memStatus;
                memStatus.dwLength = sizeof(MEMORYSTATUSEX);

                if (::GlobalMemoryStatusEx(&memStatus)) {
                    data.dMemoryUsagePercent = static_cast<double>(memStatus.dwMemoryLoad);
                    data.uMemoryTotalBytes = static_cast<unsigned long long>(memStatus.ullTotalPhys);
                    data.uMemoryUsedBytes = static_cast<unsigned long long>(memStatus.ullTotalPhys - memStatus.ullAvailPhys);
                }
            }

            void SystemMetrics::ReadWin32Cpu(RealTimeSystemInfo& data) {
                static uint64_t s_uPrevIdle = 0, s_uPrevTotal = 0;
                FILETIME ftIdle, ftKernel, ftUser;

                if (::GetSystemTimes(&ftIdle, &ftKernel, &ftUser)) {
                    uint64_t uIdle = (static_cast<uint64_t>(ftIdle.dwHighDateTime) << 32) | ftIdle.dwLowDateTime;
                    uint64_t uKernel = (static_cast<uint64_t>(ftKernel.dwHighDateTime) << 32) | ftKernel.dwLowDateTime;
                    uint64_t uUser = (static_cast<uint64_t>(ftUser.dwHighDateTime) << 32) | ftUser.dwLowDateTime;
                    uint64_t uTotal = uKernel + uUser;

                    if (s_uPrevTotal > 0 && uTotal > s_uPrevTotal) {
                        uint64_t uTotalDiff = uTotal - s_uPrevTotal;
                        uint64_t uIdleDiff = uIdle - s_uPrevIdle;
                        data.dCpuUsagePercent = static_cast<double>((uTotalDiff - uIdleDiff) * 100.0 / uTotalDiff);
                    } else if (uTotal > 0) {
                        data.dCpuUsagePercent = static_cast<double>((uTotal - uIdle) * 100.0 / uTotal);
                    }

                    if (data.dCpuUsagePercent < 0.0) data.dCpuUsagePercent = 0.0;
                    if (data.dCpuUsagePercent > 100.0) data.dCpuUsagePercent = 100.0;

                    s_uPrevIdle = uIdle;
                    s_uPrevTotal = uTotal;
                }
            }

            void SystemMetrics::ReadWin32Disk(RealTimeSystemInfo& data) {
                data.uDiskReadBytes = 0;
                data.uDiskWriteBytes = 0;
                static HQUERY s_hQuery = NULL;
                static HCOUNTER s_hCounterRead = NULL;
                static HCOUNTER s_hCounterWrite = NULL;
                static bool s_bPdhInitialized = false;

                if (!s_bPdhInitialized) {
                    if (::PdhOpenQueryW(NULL, 0, &s_hQuery) == ERROR_SUCCESS) {
                        ::PdhAddEnglishCounterW(s_hQuery, L"\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", 0, &s_hCounterRead);
                        ::PdhAddEnglishCounterW(s_hQuery, L"\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", 0, &s_hCounterWrite);
                        ::PdhCollectQueryData(s_hQuery);
                        s_bPdhInitialized = true;
                    }
                }

                if (s_bPdhInitialized && s_hQuery != NULL) {
                    if (::PdhCollectQueryData(s_hQuery) == ERROR_SUCCESS) {
                        PDH_FMT_COUNTERVALUE fmtRead, fmtWrite;

                        if (::PdhGetFormattedCounterValue(s_hCounterRead, PDH_FMT_LARGE, NULL, &fmtRead) == ERROR_SUCCESS)
                            data.uDiskReadBytes = static_cast<unsigned long long>(fmtRead.largeValue);

                        if (::PdhGetFormattedCounterValue(s_hCounterWrite, PDH_FMT_LARGE, NULL, &fmtWrite) == ERROR_SUCCESS)
                            data.uDiskWriteBytes = static_cast<unsigned long long>(fmtWrite.largeValue);
                    }
                }
            }

            void SystemMetrics::ReadWin32Network(RealTimeSystemInfo& data) {
                static uint64_t s_uPrevOctets = 0;
                static DWORD s_dwPrevTick = 0;
                DWORD dwSize = 0;

                if (::GetIfTable(NULL, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
                    std::vector<uint8_t> ifTableBuffer(dwSize, 0);
                    MIB_IFTABLE* pIfTable = reinterpret_cast<MIB_IFTABLE*>(ifTableBuffer.data());

                    if (::GetIfTable(pIfTable, &dwSize, FALSE) == NO_ERROR) {
                        uint64_t totalOctets = 0;

                        for (DWORD i = 0; i < pIfTable->dwNumEntries; ++i) {
                            if (pIfTable->table[i].dwType != IF_TYPE_SOFTWARE_LOOPBACK && pIfTable->table[i].dwOperStatus == IF_OPER_STATUS_OPERATIONAL)
                                totalOctets += pIfTable->table[i].dwInOctets + pIfTable->table[i].dwOutOctets;
                        }

                        DWORD dwNow = ::GetTickCount();

                        if (s_dwPrevTick > 0 && dwNow > s_dwPrevTick && totalOctets >= s_uPrevOctets) {
                            double dElapsedSec = (dwNow - s_dwPrevTick) / 1000.0;
                            double dBytesPerSec = (totalOctets - s_uPrevOctets) / dElapsedSec;
                            data.dNetworkUsageMbps = (dBytesPerSec * 8.0) / 1000000.0;
                        }

                        s_uPrevOctets = totalOctets;
                        s_dwPrevTick = dwNow;
                    }
                }
            }

            void SystemMetrics::ReadProcessCommandLine(void* hProc, void* pPebBaseAddress, ProcessResourceInfo& proc, bool& bCmdRead) {
                typedef NTSTATUS(NTAPI* pfnNtQueryInformationProcess)(HANDLE, ULONG, PVOID, ULONG, PULONG);
                HMODULE hNtDll = ::GetModuleHandleW(L"ntdll.dll");
                pfnNtQueryInformationProcess fnNtQuery = hNtDll ? (pfnNtQueryInformationProcess)::GetProcAddress(hNtDll, "NtQueryInformationProcess") : NULL;

                if (fnNtQuery) {
                    PROCESS_BASIC_INFORMATION pbi;
                    DWORD dwLen = 0;

                    if (fnNtQuery(static_cast<HANDLE>(hProc), 0, &pbi, sizeof(pbi), &dwLen) == 0 && pbi.PebBaseAddress) {
                        PEB peb;
                        SIZE_T bytesRead = 0;

                        if (::ReadProcessMemory(static_cast<HANDLE>(hProc), pbi.PebBaseAddress, &peb, sizeof(peb), &bytesRead)) {
                            RTL_USER_PROCESS_PARAMETERS upp;

                            if (::ReadProcessMemory(static_cast<HANDLE>(hProc), peb.ProcessParameters, &upp, sizeof(upp), &bytesRead) && upp.CommandLine.Buffer && upp.CommandLine.Length > 0) {
                                std::vector<wchar_t> wCmd(upp.CommandLine.Length / sizeof(wchar_t) + 1, 0);

                                if (::ReadProcessMemory(static_cast<HANDLE>(hProc), upp.CommandLine.Buffer, wCmd.data(), upp.CommandLine.Length, &bytesRead)) {
                                    proc.sCommandLine = String(wCmd.data());
                                    bCmdRead = true;
                                }
                            }
                        }
                    }
                }
            }

            void SystemMetrics::ReadProcessIoAndTimes(void* hProc, ProcessResourceInfo& proc) {
                std::string sStdProc(proc.sName.GetRawString() ? proc.sName.GetRawString() : "");
                std::wstring wProcName(sStdProc.begin(), sStdProc.end());
                size_t nExtPos = wProcName.rfind(L'.');

                if (nExtPos != std::wstring::npos) wProcName = wProcName.substr(0, nExtPos);

                std::wstring wReadCounterPath = L"\\Process(" + wProcName + L")\\IO Read Bytes/sec";
                std::wstring wWriteCounterPath = L"\\Process(" + wProcName + L")\\IO Write Bytes/sec";

                HQUERY hProcQuery = NULL;
                HCOUNTER hProcRead = NULL, hProcWrite = NULL;

                if (::PdhOpenQueryW(NULL, 0, &hProcQuery) == ERROR_SUCCESS) {
                    bool bReadOk = (::PdhAddEnglishCounterW(hProcQuery, wReadCounterPath.c_str(), 0, &hProcRead) == ERROR_SUCCESS);
                    bool bWriteOk = (::PdhAddEnglishCounterW(hProcQuery, wWriteCounterPath.c_str(), 0, &hProcWrite) == ERROR_SUCCESS);

                    if (::PdhCollectQueryData(hProcQuery) == ERROR_SUCCESS) {
                        PDH_FMT_COUNTERVALUE fmtRead, fmtWrite;

                        if (bReadOk && ::PdhGetFormattedCounterValue(hProcRead, PDH_FMT_LARGE, NULL, &fmtRead) == ERROR_SUCCESS)
                            proc.lDiskReadBytes = static_cast<long long>(fmtRead.largeValue);

                        if (bWriteOk && ::PdhGetFormattedCounterValue(hProcWrite, PDH_FMT_LARGE, NULL, &fmtWrite) == ERROR_SUCCESS)
                            proc.lDiskWriteBytes = static_cast<long long>(fmtWrite.largeValue);
                    }

                    ::PdhCloseQuery(hProcQuery);
                }

                if (proc.lDiskReadBytes == -1 || proc.lDiskWriteBytes == -1) {
                    IO_COUNTERS io;

                    if (::GetProcessIoCounters(static_cast<HANDLE>(hProc), &io)) {
                        if (proc.lDiskReadBytes == -1) proc.lDiskReadBytes = static_cast<long long>(io.ReadTransferCount);
                        if (proc.lDiskWriteBytes == -1) proc.lDiskWriteBytes = static_cast<long long>(io.WriteTransferCount);
                    }
                }

                FILETIME ftCreate, ftExit, ftKernel, ftUser, ftSysIdle, ftSysKernel, ftSysUser;

                if (::GetProcessTimes(static_cast<HANDLE>(hProc), &ftCreate, &ftExit, &ftKernel, &ftUser) && ::GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser)) {
                    uint64_t uProcTime = ((static_cast<uint64_t>(ftKernel.dwHighDateTime) << 32) | ftKernel.dwLowDateTime) +
                                         ((static_cast<uint64_t>(ftUser.dwHighDateTime) << 32) | ftUser.dwLowDateTime);
                    uint64_t uSysTime = ((static_cast<uint64_t>(ftSysKernel.dwHighDateTime) << 32) | ftSysKernel.dwLowDateTime) +
                                        ((static_cast<uint64_t>(ftSysUser.dwHighDateTime) << 32) | ftSysUser.dwLowDateTime);

                    auto it = s_mapProcessCpuSamples.find(proc.iProcessId);

                    if (it != s_mapProcessCpuSamples.end() && uSysTime > it->second.uSystemTime && uProcTime >= it->second.uProcessTime) {
                        uint64_t uProcDiff = uProcTime - it->second.uProcessTime;
                        uint64_t uSysDiff = uSysTime - it->second.uSystemTime;
                        double dCpu = (static_cast<double>(uProcDiff) * 100.0 / static_cast<double>(uSysDiff));

                        if (dCpu > 100.0) dCpu = 100.0;
                        if (dCpu < 0.0) dCpu = 0.0;

                        proc.dCpuUsagePercent = dCpu;
                    } else {
                        proc.dCpuUsagePercent = 0.0;
                    }

                    s_mapProcessCpuSamples[proc.iProcessId] = { uProcTime, uSysTime };
                }
            }

            void SystemMetrics::ReadProcessNetwork(void* hProc, ProcessResourceInfo& proc) {
                proc.lNetworkReadBytes = -1;
                proc.lNetworkWriteBytes = -1;

                std::string sStdProc(proc.sName.GetRawString() ? proc.sName.GetRawString() : "");
                std::wstring wProcName(sStdProc.begin(), sStdProc.end());
                size_t nExtPos = wProcName.rfind(L'.');

                if (nExtPos != std::wstring::npos) wProcName = wProcName.substr(0, nExtPos);

                std::wstring wReadPath = L"\\Process(" + wProcName + L")\\IO Read Bytes/sec";
                std::wstring wWritePath = L"\\Process(" + wProcName + L")\\IO Write Bytes/sec";

                HQUERY hQuery = NULL;
                HCOUNTER hCounterRead = NULL, hCounterWrite = NULL;

                if (::PdhOpenQueryW(NULL, 0, &hQuery) == ERROR_SUCCESS) {
                    bool bReadOk = (::PdhAddEnglishCounterW(hQuery, wReadPath.c_str(), 0, &hCounterRead) == ERROR_SUCCESS);
                    bool bWriteOk = (::PdhAddEnglishCounterW(hQuery, wWritePath.c_str(), 0, &hCounterWrite) == ERROR_SUCCESS);

                    if (::PdhCollectQueryData(hQuery) == ERROR_SUCCESS) {
                        PDH_FMT_COUNTERVALUE fmtRead, fmtWrite;

                        if (bReadOk && ::PdhGetFormattedCounterValue(hCounterRead, PDH_FMT_LARGE, NULL, &fmtRead) == ERROR_SUCCESS)
                            proc.lNetworkReadBytes = static_cast<long long>(fmtRead.largeValue);

                        if (bWriteOk && ::PdhGetFormattedCounterValue(hCounterWrite, PDH_FMT_LARGE, NULL, &fmtWrite) == ERROR_SUCCESS)
                            proc.lNetworkWriteBytes = static_cast<long long>(fmtWrite.largeValue);
                    }

                    ::PdhCloseQuery(hQuery);
                }
            }

            void SystemMetrics::ReadProcessPortsAndConnections(int iProcessId, ProcessResourceInfo& proc) {
                proc.bHasEstablishedInboundConnection = false;
                DWORD dwSize = 0;

                if (::GetExtendedTcpTable(NULL, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER) {
                    std::vector<uint8_t> tcpTableBuffer(dwSize, 0);
                    MIB_TCPTABLE_OWNER_PID* pTcpTable = reinterpret_cast<MIB_TCPTABLE_OWNER_PID*>(tcpTableBuffer.data());

                    if (::GetExtendedTcpTable(pTcpTable, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
                        for (DWORD i = 0; i < pTcpTable->dwNumEntries; ++i) {
                            if (pTcpTable->table[i].dwOwningPid == static_cast<DWORD>(iProcessId)) {
                                if (pTcpTable->table[i].dwState == MIB_TCP_STATE_LISTEN) {
                                    int iPort = ntohs(static_cast<u_short>(pTcpTable->table[i].dwLocalPort));
                                    if (!proc.lstOpenPorts.Contains(iPort)) {
                                        proc.lstOpenPorts.Add(iPort);
                                    }
                                } else if (pTcpTable->table[i].dwState == MIB_TCP_STATE_ESTAB) {
                                    proc.bHasEstablishedInboundConnection = true;
                                }
                            }
                        }
                    }
                }
            }

            void SystemMetrics::PopulateProcessInfo(void* pEntry32, ProcessResourceInfo& proc) {
                PROCESSENTRY32W* pe32 = static_cast<PROCESSENTRY32W*>(pEntry32);
                proc.iProcessId = pe32->th32ProcessID;
                proc.dCpuUsagePercent = -1.0;
                proc.lMemoryUsageBytes = -1;
                proc.lDiskReadBytes = -1;
                proc.lDiskWriteBytes = -1;
                proc.lNetworkReadBytes = -1;
                proc.lNetworkWriteBytes = -1;
                proc.bHasEstablishedInboundConnection = false;
                proc.sName = String(pe32->szExeFile);

                HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pe32->th32ProcessID);

                if (hProc != NULL) {
                    WCHAR szPath[MAX_PATH] = { 0 };
                    DWORD dwPathLen = MAX_PATH;

                    if (::QueryFullProcessImageNameW(hProc, 0, szPath, &dwPathLen)) {
                        proc.sPath = String(szPath);
                    }

                    bool bCmdRead = false;
                    ReadProcessCommandLine(hProc, NULL, proc, bCmdRead);

                    if (!bCmdRead) proc.sCommandLine = proc.sPath;

                    PROCESS_MEMORY_COUNTERS pmc;

                    if (::GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc)))
                        proc.lMemoryUsageBytes = static_cast<long long>(pmc.WorkingSetSize);

                    ReadProcessIoAndTimes(hProc, proc);
                    ReadProcessNetwork(hProc, proc);

                    ::CloseHandle(hProc);
                }

                ReadProcessPortsAndConnections(proc.iProcessId, proc);
            }

            void SystemMetrics::ReadWin32TopProcesses(RealTimeSystemInfo& data) {
                HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

                if (hSnapshot == INVALID_HANDLE_VALUE) return;

                PROCESSENTRY32W pe32;
                pe32.dwSize = sizeof(PROCESSENTRY32W);

                if (::Process32FirstW(hSnapshot, &pe32)) {
                    std::vector<ProcessResourceInfo> tempProcList;

                    do {
                        ProcessResourceInfo proc;
                        PopulateProcessInfo(&pe32, proc);
                        tempProcList.push_back(proc);
                    } while (::Process32NextW(hSnapshot, &pe32));

                    std::sort(tempProcList.begin(), tempProcList.end(), [](const ProcessResourceInfo& a, const ProcessResourceInfo& b) {
                        return a.dCpuUsagePercent > b.dCpuUsagePercent;
                    });

                    for (size_t i = 0; i < tempProcList.size() && i < 8; ++i)
                        data.lstTopProcesses.Add(tempProcList[i]);
                }

                ::CloseHandle(hSnapshot);
            }
#endif

            RealTimeSystemInfo SystemMetrics::GetSystemMetrics() {
                RealTimeSystemInfo data;
#if defined(_WIN32)
                ReadWin32Memory(data);
                ReadWin32Cpu(data);
                ReadWin32Disk(data);
                ReadWin32Network(data);
                ReadWin32TopProcesses(data);
#endif
                return data;
            }

        }
    }
}
