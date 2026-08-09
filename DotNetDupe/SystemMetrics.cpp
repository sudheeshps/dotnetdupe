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
#include <winsvc.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "advapi32.lib")
#else
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <arpa/inet.h>
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

            MemoryInfo SystemMetrics::GetSystemMemoryUsage() {
                MemoryInfo info;
                MEMORYSTATUSEX memStatus;
                memStatus.dwLength = sizeof(MEMORYSTATUSEX);

                if (::GlobalMemoryStatusEx(&memStatus)) {
                    info.dMemoryUsagePercent = static_cast<double>(memStatus.dwMemoryLoad);
                    info.uMemoryTotalBytes = static_cast<unsigned long long>(memStatus.ullTotalPhys);
                    info.uMemoryUsedBytes = static_cast<unsigned long long>(memStatus.ullTotalPhys - memStatus.ullAvailPhys);
                }
                return info;
            }

            double SystemMetrics::GetSystemCpuUsage() {
                static uint64_t s_uPrevIdle = 0, s_uPrevTotal = 0;
                FILETIME ftIdle, ftKernel, ftUser;
                double dCpuUsagePercent = 0.0;

                if (::GetSystemTimes(&ftIdle, &ftKernel, &ftUser)) {
                    uint64_t uIdle = (static_cast<uint64_t>(ftIdle.dwHighDateTime) << 32) | ftIdle.dwLowDateTime;
                    uint64_t uKernel = (static_cast<uint64_t>(ftKernel.dwHighDateTime) << 32) | ftKernel.dwLowDateTime;
                    uint64_t uUser = (static_cast<uint64_t>(ftUser.dwHighDateTime) << 32) | ftUser.dwLowDateTime;
                    uint64_t uTotal = uKernel + uUser;

                    if (s_uPrevTotal > 0 && uTotal > s_uPrevTotal) {
                        uint64_t uTotalDiff = uTotal - s_uPrevTotal;
                        uint64_t uIdleDiff = uIdle - s_uPrevIdle;
                        dCpuUsagePercent = static_cast<double>((uTotalDiff - uIdleDiff) * 100.0 / uTotalDiff);
                    } else if (uTotal > 0) {
                        dCpuUsagePercent = static_cast<double>((uTotal - uIdle) * 100.0 / uTotal);
                    }

                    if (dCpuUsagePercent < 0.0) dCpuUsagePercent = 0.0;
                    if (dCpuUsagePercent > 100.0) dCpuUsagePercent = 100.0;

                    s_uPrevIdle = uIdle;
                    s_uPrevTotal = uTotal;
                }
                return dCpuUsagePercent;
            }

            DiskInfo SystemMetrics::GetSystemDiskUsage() {
                DiskInfo info;
                info.lDiskReadBytes = 0;
                info.lDiskWriteBytes = 0;
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
                            info.lDiskReadBytes = static_cast<long long>(fmtRead.largeValue);

                        if (::PdhGetFormattedCounterValue(s_hCounterWrite, PDH_FMT_LARGE, NULL, &fmtWrite) == ERROR_SUCCESS)
                            info.lDiskWriteBytes = static_cast<long long>(fmtWrite.largeValue);
                    }
                }
                return info;
            }

            double SystemMetrics::GetSystemNetworkUsage() {
                static uint64_t s_uPrevOctets = 0;
                static DWORD s_dwPrevTick = 0;
                DWORD dwSize = 0;
                double dNetworkUsageMbps = 0.0;

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
                            dNetworkUsageMbps = (dBytesPerSec * 8.0) / 1000000.0;
                        }

                        s_uPrevOctets = totalOctets;
                        s_dwPrevTick = dwNow;
                    }
                }
                return dNetworkUsageMbps;
            }

            void* SystemMetrics::OpenProcessByName(const String& sProcessName, unsigned long dwDesiredAccess, int& iOutProcessId) {
                iOutProcessId = -1;
                HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnapshot == INVALID_HANDLE_VALUE) return NULL;

                PROCESSENTRY32W pe32;
                pe32.dwSize = sizeof(PROCESSENTRY32W);

                std::string sTargetProc = sProcessName.GetRawString() ? sProcessName.GetRawString() : "";
                std::wstring wTargetProc(sTargetProc.begin(), sTargetProc.end());
                std::transform(wTargetProc.begin(), wTargetProc.end(), wTargetProc.begin(), ::tolower);

                HANDLE hProcResult = NULL;

                if (::Process32FirstW(hSnapshot, &pe32)) {
                    do {
                        std::wstring wExeFile(pe32.szExeFile);
                        std::transform(wExeFile.begin(), wExeFile.end(), wExeFile.begin(), ::tolower);

                        if (wExeFile == wTargetProc || wExeFile == wTargetProc + L".exe") {
                            iOutProcessId = pe32.th32ProcessID;
                            hProcResult = ::OpenProcess(dwDesiredAccess, FALSE, pe32.th32ProcessID);
                            break;
                        }
                    } while (::Process32NextW(hSnapshot, &pe32));
                }

                ::CloseHandle(hSnapshot);
                return hProcResult;
            }

            String SystemMetrics::ReadProcessCommandLineHandle(void* hProc) {
                typedef NTSTATUS(NTAPI* pfnNtQueryInformationProcess)(HANDLE, ULONG, PVOID, ULONG, PULONG);
                HMODULE hNtDll = ::GetModuleHandleW(L"ntdll.dll");
                pfnNtQueryInformationProcess fnNtQuery = hNtDll ? (pfnNtQueryInformationProcess)::GetProcAddress(hNtDll, "NtQueryInformationProcess") : NULL;

                if (fnNtQuery && hProc != NULL) {
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
                                    return String(wCmd.data());
                                }
                            }
                        }
                    }
                }
                return String("");
            }

            String SystemMetrics::GetProcessCommandLine(const String& sProcessName) {
                int iPid = -1;
                HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, iPid));
                if (hProc == NULL) return String("");

                String sCmd = ReadProcessCommandLineHandle(hProc);
                if (sCmd.GetLength() == 0) {
                    WCHAR szPath[MAX_PATH] = { 0 };
                    DWORD dwPathLen = MAX_PATH;
                    if (::QueryFullProcessImageNameW(hProc, 0, szPath, &dwPathLen)) {
                        sCmd = String(szPath);
                    }
                }
                ::CloseHandle(hProc);
                return sCmd;
            }

            MemoryInfo SystemMetrics::ReadProcessMemoryHandle(void* hProc) {
                MemoryInfo info;
                if (hProc != NULL) {
                    PROCESS_MEMORY_COUNTERS_EX pmcEx;
                    if (::GetProcessMemoryInfo(static_cast<HANDLE>(hProc), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmcEx), sizeof(pmcEx))) {
                        info.lPhysicalMemoryBytes = static_cast<long long>(pmcEx.WorkingSetSize);
                        info.lPrivateBytes = static_cast<long long>(pmcEx.PrivateUsage);
                    }
                }
                return info;
            }

            MemoryInfo SystemMetrics::GetProcessMemoryUsage(const String& sProcessName) {
                int iPid = -1;
                HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, iPid));
                if (hProc == NULL) return MemoryInfo();

                MemoryInfo memInfo = ReadProcessMemoryHandle(hProc);
                ::CloseHandle(hProc);
                return memInfo;
            }

            DiskInfo SystemMetrics::ReadProcessDiskHandle(void* hProc, const String& sProcessName) {
                DiskInfo info;
                std::string sStdProc(sProcessName.GetRawString() ? sProcessName.GetRawString() : "");
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
                            info.lDiskReadBytes = static_cast<long long>(fmtRead.largeValue);

                        if (bWriteOk && ::PdhGetFormattedCounterValue(hProcWrite, PDH_FMT_LARGE, NULL, &fmtWrite) == ERROR_SUCCESS)
                            info.lDiskWriteBytes = static_cast<long long>(fmtWrite.largeValue);
                    }
                    ::PdhCloseQuery(hProcQuery);
                }

                if ((info.lDiskReadBytes == -1 || info.lDiskWriteBytes == -1) && hProc != NULL) {
                    IO_COUNTERS io;
                    if (::GetProcessIoCounters(static_cast<HANDLE>(hProc), &io)) {
                        if (info.lDiskReadBytes == -1) info.lDiskReadBytes = static_cast<long long>(io.ReadTransferCount);
                        if (info.lDiskWriteBytes == -1) info.lDiskWriteBytes = static_cast<long long>(io.WriteTransferCount);
                    }
                }
                return info;
            }

            DiskInfo SystemMetrics::GetProcessDiskUsage(const String& sProcessName) {
                int iPid = -1;
                HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                DiskInfo info = ReadProcessDiskHandle(hProc, sProcessName);
                if (hProc != NULL) ::CloseHandle(hProc);
                return info;
            }

            NetworkUsageInfo SystemMetrics::ReadProcessNetworkHandle(void* hProc, const String& sProcessName) {
                NetworkUsageInfo info;
                std::string sStdProc(sProcessName.GetRawString() ? sProcessName.GetRawString() : "");
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
                            info.lNetworkReadBytes = static_cast<long long>(fmtRead.largeValue);

                        if (bWriteOk && ::PdhGetFormattedCounterValue(hCounterWrite, PDH_FMT_LARGE, NULL, &fmtWrite) == ERROR_SUCCESS)
                            info.lNetworkWriteBytes = static_cast<long long>(fmtWrite.largeValue);
                    }
                    ::PdhCloseQuery(hQuery);
                }
                return info;
            }

            NetworkUsageInfo SystemMetrics::GetProcessNetworkUsage(const String& sProcessName) {
                int iPid = -1;
                HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                NetworkUsageInfo info = ReadProcessNetworkHandle(hProc, sProcessName);
                if (hProc != NULL) ::CloseHandle(hProc);
                return info;
            }

            Collections::Generic::List<int> SystemMetrics::ReadProcessNetworkPortInternal(int iProcessId) {
                Collections::Generic::List<int> lstPorts;
                if (iProcessId <= 0) return lstPorts;

                DWORD dwSize = 0;
                if (::GetExtendedTcpTable(NULL, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER) {
                    std::vector<uint8_t> tcpTableBuffer(dwSize, 0);
                    MIB_TCPTABLE_OWNER_PID* pTcpTable = reinterpret_cast<MIB_TCPTABLE_OWNER_PID*>(tcpTableBuffer.data());

                    if (::GetExtendedTcpTable(pTcpTable, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
                        for (DWORD i = 0; i < pTcpTable->dwNumEntries; ++i) {
                            if (pTcpTable->table[i].dwOwningPid == static_cast<DWORD>(iProcessId)) {
                                if (pTcpTable->table[i].dwState == MIB_TCP_STATE_LISTEN) {
                                    int iPort = ntohs(static_cast<u_short>(pTcpTable->table[i].dwLocalPort));
                                    if (!lstPorts.Contains(iPort)) {
                                        lstPorts.Add(iPort);
                                    }
                                }
                            }
                        }
                    }
                }
                return lstPorts;
            }

            Collections::Generic::List<int> SystemMetrics::GetProcessNetworkPort(const String& sProcessName) {
                int iPid = -1;
                HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                if (hProc != NULL) ::CloseHandle(hProc);
                return ReadProcessNetworkPortInternal(iPid);
            }

            static String TcpStateToString(DWORD dwState) {
                switch (dwState) {
                case MIB_TCP_STATE_CLOSED: return String("CLOSED");
                case MIB_TCP_STATE_LISTEN: return String("LISTEN");
                case MIB_TCP_STATE_SYN_SENT: return String("SYN_SENT");
                case MIB_TCP_STATE_SYN_RCVD: return String("SYN_RCVD");
                case MIB_TCP_STATE_ESTAB: return String("ESTABLISHED");
                case MIB_TCP_STATE_FIN_WAIT1: return String("FIN_WAIT1");
                case MIB_TCP_STATE_FIN_WAIT2: return String("FIN_WAIT2");
                case MIB_TCP_STATE_CLOSE_WAIT: return String("CLOSE_WAIT");
                case MIB_TCP_STATE_CLOSING: return String("CLOSING");
                case MIB_TCP_STATE_LAST_ACK: return String("LAST_ACK");
                case MIB_TCP_STATE_TIME_WAIT: return String("TIME_WAIT");
                case MIB_TCP_STATE_DELETE_TCB: return String("DELETE_TCB");
                default: return String("UNKNOWN");
                }
            }

            ProcessNetworkConnectionInfo SystemMetrics::ReadProcessNetworkInfoInternal(int iProcessId) {
                ProcessNetworkConnectionInfo info;
                if (iProcessId <= 0) return info;

                DWORD dwSize = 0;
                if (::GetExtendedTcpTable(NULL, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER) {
                    std::vector<uint8_t> tcpTableBuffer(dwSize, 0);
                    MIB_TCPTABLE_OWNER_PID* pTcpTable = reinterpret_cast<MIB_TCPTABLE_OWNER_PID*>(tcpTableBuffer.data());

                    if (::GetExtendedTcpTable(pTcpTable, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
                        for (DWORD i = 0; i < pTcpTable->dwNumEntries; ++i) {
                            if (pTcpTable->table[i].dwOwningPid == static_cast<DWORD>(iProcessId)) {
                                NetworkConnectionInfo conn;
                                in_addr localAddr;
                                localAddr.S_un.S_addr = pTcpTable->table[i].dwLocalAddr;
                                char szLocal[INET_ADDRSTRLEN] = { 0 };
                                ::inet_ntop(AF_INET, &localAddr, szLocal, sizeof(szLocal));
                                conn.sLocalAddress = String(szLocal);
                                conn.iLocalPort = ntohs(static_cast<u_short>(pTcpTable->table[i].dwLocalPort));

                                in_addr remoteAddr;
                                remoteAddr.S_un.S_addr = pTcpTable->table[i].dwRemoteAddr;
                                char szRemote[INET_ADDRSTRLEN] = { 0 };
                                ::inet_ntop(AF_INET, &remoteAddr, szRemote, sizeof(szRemote));
                                conn.sRemoteAddress = String(szRemote);
                                conn.iRemotePort = ntohs(static_cast<u_short>(pTcpTable->table[i].dwRemotePort));

                                conn.sState = TcpStateToString(pTcpTable->table[i].dwState);

                                info.lstConnections.Add(conn);

                                if (pTcpTable->table[i].dwState == MIB_TCP_STATE_LISTEN) {
                                    if (!info.lstOpenPorts.Contains(conn.iLocalPort)) {
                                        info.lstOpenPorts.Add(conn.iLocalPort);
                                    }
                                } else if (pTcpTable->table[i].dwState == MIB_TCP_STATE_ESTAB) {
                                    info.bHasEstablishedInboundConnection = true;
                                }
                            }
                        }
                    }
                }
                return info;
            }

            ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(const String& sProcessName) {
                int iPid = -1;
                HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                if (hProc != NULL) ::CloseHandle(hProc);
                return ReadProcessNetworkInfoInternal(iPid);
            }

            void SystemMetrics::PopulateProcessInfo(void* pEntry32, ProcessInfo& proc) {
                PROCESSENTRY32W* pe32 = static_cast<PROCESSENTRY32W*>(pEntry32);
                proc.iProcessId = pe32->th32ProcessID;
                proc.sName = String(pe32->szExeFile);
                proc.dCpuUsagePercent = 0.0;

                DWORD dwSessionId = 0;
                if (::ProcessIdToSessionId(pe32->th32ProcessID, &dwSessionId)) {
                    proc.iSessionId = static_cast<int>(dwSessionId);
                }

                HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pe32->th32ProcessID);
                if (hProc != NULL) {
                    WCHAR szPath[MAX_PATH] = { 0 };
                    DWORD dwPathLen = MAX_PATH;
                    if (::QueryFullProcessImageNameW(hProc, 0, szPath, &dwPathLen)) {
                        proc.sPath = String(szPath);
                    }

                    proc.sCommandLine = ReadProcessCommandLineHandle(hProc);
                    if (proc.sCommandLine.GetLength() == 0) proc.sCommandLine = proc.sPath;

                    proc.memory = ReadProcessMemoryHandle(hProc);
                    proc.disk = ReadProcessDiskHandle(hProc, proc.sName);
                    proc.network = ReadProcessNetworkHandle(hProc, proc.sName);

                    FILETIME ftCreate, ftExit, ftKernel, ftUser, ftSysIdle, ftSysKernel, ftSysUser;
                    if (::GetProcessTimes(hProc, &ftCreate, &ftExit, &ftKernel, &ftUser) && ::GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser)) {
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
                        }
                        s_mapProcessCpuSamples[proc.iProcessId] = { uProcTime, uSysTime };
                    }
                    ::CloseHandle(hProc);
                }
            }

            Collections::Generic::List<ProcessInfo> SystemMetrics::GetAllProcesses(int iSessionId) {
                Collections::Generic::List<ProcessInfo> lstProcesses;
                HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnapshot == INVALID_HANDLE_VALUE) return lstProcesses;

                PROCESSENTRY32W pe32;
                pe32.dwSize = sizeof(PROCESSENTRY32W);

                if (::Process32FirstW(hSnapshot, &pe32)) {
                    do {
                        ProcessInfo proc;
                        PopulateProcessInfo(&pe32, proc);
                        if (iSessionId == -1 || proc.iSessionId == iSessionId) {
                            lstProcesses.Add(proc);
                        }
                    } while (::Process32NextW(hSnapshot, &pe32));
                }

                ::CloseHandle(hSnapshot);
                return lstProcesses;
            }

            Collections::Generic::List<ProcessInfo> SystemMetrics::GetTopProcesses(SystemResource eResource, int iCount) {
                Collections::Generic::List<ProcessInfo> lstAll = GetAllProcesses(-1);
                std::vector<ProcessInfo> tempVector;

                for (int i = 0; i < lstAll.GetCount(); ++i) {
                    tempVector.push_back(lstAll[i]);
                }

                std::sort(tempVector.begin(), tempVector.end(), [eResource](const ProcessInfo& a, const ProcessInfo& b) {
                    switch (eResource) {
                    case SystemResource::Cpu:
                        return a.dCpuUsagePercent > b.dCpuUsagePercent;
                    case SystemResource::Memory:
                        return a.memory.lPhysicalMemoryBytes > b.memory.lPhysicalMemoryBytes;
                    case SystemResource::Disk:
                        return (a.disk.lDiskReadBytes + a.disk.lDiskWriteBytes) > (b.disk.lDiskReadBytes + b.disk.lDiskWriteBytes);
                    case SystemResource::Network:
                        return (a.network.lNetworkReadBytes + a.network.lNetworkWriteBytes) > (b.network.lNetworkReadBytes + b.network.lNetworkWriteBytes);
                    default:
                        return a.dCpuUsagePercent > b.dCpuUsagePercent;
                    }
                });

                Collections::Generic::List<ProcessInfo> lstResult;
                for (size_t i = 0; i < tempVector.size() && static_cast<int>(i) < iCount; ++i) {
                    lstResult.Add(tempVector[i]);
                }
                return lstResult;
            }

            static String GetServiceStartType(SC_HANDLE hSCM, LPCWSTR lpServiceName) {
                SC_HANDLE hService = ::OpenServiceW(hSCM, lpServiceName, SERVICE_QUERY_CONFIG);
                if (hService == NULL) return String("Manual");
                BYTE buffer[1024];
                DWORD dwBytesNeeded = 0;
                QUERY_SERVICE_CONFIGW* pConfig = reinterpret_cast<QUERY_SERVICE_CONFIGW*>(buffer);
                String sStartType = String("Manual");
                if (::QueryServiceConfigW(hService, pConfig, sizeof(buffer), &dwBytesNeeded)) {
                    switch (pConfig->dwStartType) {
                    case SERVICE_AUTO_START: sStartType = String("Automatic"); break;
                    case SERVICE_DEMAND_START: sStartType = String("Manual"); break;
                    case SERVICE_DISABLED: sStartType = String("Disabled"); break;
                    case SERVICE_BOOT_START: sStartType = String("Boot"); break;
                    case SERVICE_SYSTEM_START: sStartType = String("System"); break;
                    default: sStartType = String("Manual"); break;
                    }
                }
                ::CloseServiceHandle(hService);
                return sStartType;
            }

            Collections::Generic::List<ServiceInfo> SystemMetrics::GetAllServices() {
                Collections::Generic::List<ServiceInfo> lstServices;
                SC_HANDLE hSCM = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
                if (hSCM == NULL) return lstServices;

                DWORD dwBytesNeeded = 0;
                DWORD dwServicesReturned = 0;
                DWORD dwResumeHandle = 0;
                ::EnumServicesStatusExW(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
                                        NULL, 0, &dwBytesNeeded, &dwServicesReturned, &dwResumeHandle, NULL);

                if (dwBytesNeeded > 0) {
                    std::vector<uint8_t> buffer(dwBytesNeeded, 0);
                    ENUM_SERVICE_STATUS_PROCESSW* pServices = reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(buffer.data());

                    if (::EnumServicesStatusExW(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
                                                buffer.data(), dwBytesNeeded, &dwBytesNeeded, &dwServicesReturned, &dwResumeHandle, NULL)) {
                        for (DWORD i = 0; i < dwServicesReturned; ++i) {
                            ServiceInfo svc;
                            svc.sServiceName = String(pServices[i].lpServiceName);
                            svc.sDisplayName = String(pServices[i].lpDisplayName);
                            svc.iProcessId = static_cast<int>(pServices[i].ServiceStatusProcess.dwProcessId);

                            switch (pServices[i].ServiceStatusProcess.dwCurrentState) {
                            case SERVICE_RUNNING: svc.sStatus = String("Running"); break;
                            case SERVICE_STOPPED: svc.sStatus = String("Stopped"); break;
                            case SERVICE_PAUSED: svc.sStatus = String("Paused"); break;
                            case SERVICE_START_PENDING: svc.sStatus = String("StartPending"); break;
                            case SERVICE_STOP_PENDING: svc.sStatus = String("StopPending"); break;
                            case SERVICE_CONTINUE_PENDING: svc.sStatus = String("ContinuePending"); break;
                            case SERVICE_PAUSE_PENDING: svc.sStatus = String("PausePending"); break;
                            default: svc.sStatus = String("Unknown"); break;
                            }

                            svc.sStartType = GetServiceStartType(hSCM, pServices[i].lpServiceName);
                            lstServices.Add(svc);
                        }
                    }
                }
                ::CloseServiceHandle(hSCM);
                return lstServices;
            }
#else
            int SystemMetrics::FindPidByName(const String& sProcessName) {
                DIR* dir = ::opendir("/proc");
                if (!dir) return -1;

                struct dirent* entry = nullptr;
                std::string targetName = sProcessName.GetRawString();

                while ((entry = ::readdir(dir)) != nullptr) {
                    if (entry->d_type == DT_DIR) {
                        std::string dname = entry->d_name;
                        if (std::all_of(dname.begin(), dname.end(), ::isdigit)) {
                            std::string commPath = "/proc/" + dname + "/comm";
                            std::ifstream commFile(commPath);
                            if (commFile.is_open()) {
                                std::string procComm;
                                std::getline(commFile, procComm);
                                if (procComm == targetName) {
                                    ::closedir(dir);
                                    return std::stoi(dname);
                                }
                            }
                        }
                    }
                }
                ::closedir(dir);
                return -1;
            }

            MemoryInfo SystemMetrics::ReadLinuxProcessMemory(int iPid) {
                MemoryInfo info;
                if (iPid <= 0) return info;

                std::string statusPath = "/proc/" + std::to_string(iPid) + "/status";
                std::ifstream statusFile(statusPath);
                if (!statusFile.is_open()) return info;

                std::string line;
                long long lRssKb = 0;
                long long lVmKb = 0;

                while (std::getline(statusFile, line)) {
                    if (line.rfind("VmRSS:", 0) == 0) {
                        std::istringstream iss(line.substr(6));
                        iss >> lRssKb;
                    } else if (line.rfind("VmSize:", 0) == 0) {
                        std::istringstream iss(line.substr(7));
                        iss >> lVmKb;
                    }
                }

                info.lPhysicalMemoryBytes = lRssKb * 1024LL;
                info.lPrivateBytes = lVmKb * 1024LL;
                return info;
            }

            DiskInfo SystemMetrics::ReadLinuxProcessDisk(int iPid) {
                DiskInfo info;
                if (iPid <= 0) return info;

                std::string ioPath = "/proc/" + std::to_string(iPid) + "/io";
                std::ifstream ioFile(ioPath);
                if (!ioFile.is_open()) return info;

                std::string line;
                while (std::getline(ioFile, line)) {
                    if (line.rfind("read_bytes:", 0) == 0) {
                        std::istringstream iss(line.substr(11));
                        iss >> info.lDiskReadBytes;
                    } else if (line.rfind("write_bytes:", 0) == 0) {
                        std::istringstream iss(line.substr(12));
                        iss >> info.lDiskWriteBytes;
                    }
                }
                return info;
            }

            NetworkUsageInfo SystemMetrics::ReadLinuxProcessNetwork(int iPid) {
                NetworkUsageInfo info;
                if (iPid <= 0) return info;

                std::string devPath = "/proc/" + std::to_string(iPid) + "/net/dev";
                std::ifstream devFile(devPath);
                if (!devFile.is_open()) return info;

                std::string line;
                long long totalRx = 0, totalTx = 0;
                int lineCount = 0;

                while (std::getline(devFile, line)) {
                    if (++lineCount <= 2) continue; // Skip header lines
                    auto colonPos = line.find(':');
                    if (colonPos != std::string::npos) {
                        std::istringstream iss(line.substr(colonPos + 1));
                        long long rx = 0, tx = 0, dummy = 0;
                        iss >> rx;
                        for (int i = 0; i < 7; ++i) iss >> dummy;
                        iss >> tx;
                        totalRx += rx;
                        totalTx += tx;
                    }
                }
                info.lNetworkReadBytes = totalRx;
                info.lNetworkWriteBytes = totalTx;
                return info;
            }

            Collections::Generic::List<int> SystemMetrics::ReadLinuxProcessPorts(int iPid) {
                Collections::Generic::List<int> lstPorts;
                ProcessNetworkConnectionInfo connInfo = ReadLinuxProcessNetworkInfo(iPid);
                return connInfo.lstOpenPorts;
            }

            ProcessNetworkConnectionInfo SystemMetrics::ReadLinuxProcessNetworkInfo(int iPid) {
                ProcessNetworkConnectionInfo connInfo;
                if (iPid <= 0) return connInfo;

                std::string tcpPath = "/proc/net/tcp";
                std::ifstream tcpFile(tcpPath);
                if (!tcpFile.is_open()) return connInfo;

                std::string line;
                int lineNum = 0;

                while (std::getline(tcpFile, line)) {
                    if (lineNum++ == 0) continue; // Skip header
                    std::istringstream iss(line);
                    std::string sl, localAddr, remAddr, st;
                    if (iss >> sl >> localAddr >> remAddr >> st) {
                        auto colonLocal = localAddr.find(':');
                        auto colonRem = remAddr.find(':');
                        if (colonLocal != std::string::npos && colonRem != std::string::npos) {
                            int localPort = std::stoi(localAddr.substr(colonLocal + 1), nullptr, 16);
                            int remotePort = std::stoi(remAddr.substr(colonRem + 1), nullptr, 16);

                            NetworkConnectionInfo conn;
                            conn.iLocalPort = localPort;
                            conn.iRemotePort = remotePort;
                            conn.sState = (st == "01") ? String("ESTABLISHED") : String("LISTEN");

                            connInfo.lstConnections.Add(conn);
                            connInfo.lstOpenPorts.Add(localPort);

                            if (st == "01") {
                                connInfo.bHasEstablishedInboundConnection = true;
                            }
                        }
                    }
                }
                return connInfo;
            }

            MemoryInfo SystemMetrics::GetSystemMemoryUsage() {
                MemoryInfo info;
                std::ifstream memFile("/proc/meminfo");
                if (!memFile.is_open()) return info;

                std::string line;
                unsigned long long totalKb = 0, availKb = 0;

                while (std::getline(memFile, line)) {
                    if (line.rfind("MemTotal:", 0) == 0) {
                        std::istringstream iss(line.substr(9));
                        iss >> totalKb;
                    } else if (line.rfind("MemAvailable:", 0) == 0) {
                        std::istringstream iss(line.substr(13));
                        iss >> availKb;
                    }
                }

                info.uMemoryTotalBytes = totalKb * 1024ULL;
                info.uMemoryUsedBytes = (totalKb > availKb) ? (totalKb - availKb) * 1024ULL : 0ULL;
                if (totalKb > 0) {
                    info.dMemoryUsagePercent = (static_cast<double>(info.uMemoryUsedBytes) / static_cast<double>(info.uMemoryTotalBytes)) * 100.0;
                }
                return info;
            }

            double SystemMetrics::GetSystemCpuUsage() {
                std::ifstream statFile("/proc/stat");
                if (!statFile.is_open()) return 0.0;

                std::string line;
                if (std::getline(statFile, line) && line.rfind("cpu ", 0) == 0) {
                    std::istringstream iss(line.substr(4));
                    long long user, nice, system, idle, iowait, irq, softirq, steal;
                    if (iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
                        long long totalIdle = idle + iowait;
                        long long totalNonIdle = user + nice + system + irq + softirq + steal;
                        long long total = totalIdle + totalNonIdle;
                        if (total > 0) {
                            return (static_cast<double>(totalNonIdle) / static_cast<double>(total)) * 100.0;
                        }
                    }
                }
                return 0.0;
            }

            DiskInfo SystemMetrics::GetSystemDiskUsage() {
                DiskInfo info;
                struct statvfs vfs;
                if (statvfs("/", &vfs) == 0) {
                    info.lDiskReadBytes = static_cast<long long>(vfs.f_blocks * vfs.f_frsize);
                    info.lDiskWriteBytes = static_cast<long long>((vfs.f_blocks - vfs.f_bfree) * vfs.f_frsize);
                }
                return info;
            }

            double SystemMetrics::GetSystemNetworkUsage() {
                std::ifstream devFile("/proc/net/dev");
                if (!devFile.is_open()) return 0.0;

                std::string line;
                long long totalBytes = 0;
                int lineCount = 0;

                while (std::getline(devFile, line)) {
                    if (++lineCount <= 2) continue;
                    auto colonPos = line.find(':');
                    if (colonPos != std::string::npos) {
                        std::istringstream iss(line.substr(colonPos + 1));
                        long long rx = 0, tx = 0, dummy = 0;
                        iss >> rx;
                        for (int i = 0; i < 7; ++i) iss >> dummy;
                        iss >> tx;
                        totalBytes += (rx + tx);
                    }
                }
                return static_cast<double>(totalBytes);
            }

            String SystemMetrics::GetProcessCommandLine(const String& sProcessName) {
                int pid = FindPidByName(sProcessName);
                if (pid <= 0) return String("");

                std::string cmdPath = "/proc/" + std::to_string(pid) + "/cmdline";
                std::ifstream cmdFile(cmdPath, std::ios::binary);
                if (!cmdFile.is_open()) return String("");

                std::string cmdLine;
                char ch;
                while (cmdFile.get(ch)) {
                    cmdLine += (ch == '\0') ? ' ' : ch;
                }
                return String(cmdLine.c_str());
            }

            MemoryInfo SystemMetrics::GetProcessMemoryUsage(const String& sProcessName) {
                int pid = FindPidByName(sProcessName);
                return ReadLinuxProcessMemory(pid);
            }

            DiskInfo SystemMetrics::GetProcessDiskUsage(const String& sProcessName) {
                int pid = FindPidByName(sProcessName);
                return ReadLinuxProcessDisk(pid);
            }

            NetworkUsageInfo SystemMetrics::GetProcessNetworkUsage(const String& sProcessName) {
                int pid = FindPidByName(sProcessName);
                return ReadLinuxProcessNetwork(pid);
            }

            Collections::Generic::List<int> SystemMetrics::GetProcessNetworkPort(const String& sProcessName) {
                int pid = FindPidByName(sProcessName);
                return ReadLinuxProcessPorts(pid);
            }

            ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(const String& sProcessName) {
                int pid = FindPidByName(sProcessName);
                return ReadLinuxProcessNetworkInfo(pid);
            }

            Collections::Generic::List<ProcessInfo> SystemMetrics::GetTopProcesses(SystemResource eResource, int iCount) {
                Collections::Generic::List<ProcessInfo> lstAll = GetAllProcesses(-1);
                std::vector<ProcessInfo> tempVector;

                for (int i = 0; i < lstAll.GetCount(); ++i) {
                    tempVector.push_back(lstAll[i]);
                }

                std::sort(tempVector.begin(), tempVector.end(), [eResource](const ProcessInfo& a, const ProcessInfo& b) {
                    switch (eResource) {
                    case SystemResource::Cpu: return a.dCpuUsagePercent > b.dCpuUsagePercent;
                    case SystemResource::Memory: return a.memory.lPhysicalMemoryBytes > b.memory.lPhysicalMemoryBytes;
                    case SystemResource::Disk: return (a.disk.lDiskReadBytes + a.disk.lDiskWriteBytes) > (b.disk.lDiskReadBytes + b.disk.lDiskWriteBytes);
                    case SystemResource::Network: return (a.network.lNetworkReadBytes + a.network.lNetworkWriteBytes) > (b.network.lNetworkReadBytes + b.network.lNetworkWriteBytes);
                    default: return a.dCpuUsagePercent > b.dCpuUsagePercent;
                    }
                });

                Collections::Generic::List<ProcessInfo> lstResult;
                for (size_t i = 0; i < tempVector.size() && static_cast<int>(i) < iCount; ++i) {
                    lstResult.Add(tempVector[i]);
                }
                return lstResult;
            }

            Collections::Generic::List<ProcessInfo> SystemMetrics::GetAllProcesses(int iSessionId) {
                Collections::Generic::List<ProcessInfo> lstProcs;
                DIR* dir = ::opendir("/proc");
                if (!dir) return lstProcs;

                struct dirent* entry = nullptr;
                while ((entry = ::readdir(dir)) != nullptr) {
                    if (entry->d_type == DT_DIR) {
                        std::string dname = entry->d_name;
                        if (std::all_of(dname.begin(), dname.end(), ::isdigit)) {
                            int pid = std::stoi(dname);
                            ProcessInfo proc;
                            proc.iProcessId = pid;

                            std::string commPath = "/proc/" + dname + "/comm";
                            std::ifstream commFile(commPath);
                            if (commFile.is_open()) {
                                std::string procComm;
                                std::getline(commFile, procComm);
                                proc.sName = String(procComm.c_str());
                            }

                            proc.sCommandLine = GetProcessCommandLine(proc.sName);
                            proc.memory = ReadLinuxProcessMemory(pid);
                            proc.disk = ReadLinuxProcessDisk(pid);
                            proc.network = ReadLinuxProcessNetwork(pid);

                            if (iSessionId == -1 || proc.iSessionId == iSessionId) {
                                lstProcs.Add(proc);
                            }
                        }
                    }
                }
                ::closedir(dir);
                return lstProcs;
            }

            Collections::Generic::List<ServiceInfo> SystemMetrics::GetAllServices() {
                Collections::Generic::List<ServiceInfo> lstServices;
                return lstServices;
            }
#endif

        }
    }
}
