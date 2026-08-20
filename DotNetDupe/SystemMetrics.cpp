#include "pch.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/Diagnostics/ProcessStreamer.h"
#include "System/Diagnostics/Process.h"
#include "System/ArgumentException.h"
#include "System/SystemException.h"
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

            SmartPointer<ProcessStreamer> SystemMetrics::CreateProcessStreamer(const ProcessStreamOptions& options) {
                return SmartPointer<ProcessStreamer>::NewShared(options);
            }

            void SystemMetrics::EnumerateProcessesAsync(const Action<const ProcessInfo&>& fnOnProcess, const Action<>& fnOnComplete) {
                ProcessStreamOptions options;
                options.eDetailLevel = ProcessMetricsDetail::FastDiscoveryOnly;
                auto pStreamer = CreateProcessStreamer(options);
                pStreamer->ProcessDiscovered += [fnOnProcess](const void*, const ProcessEventArgs& e) {
                    if (fnOnProcess) fnOnProcess(e.GetProcess());
                };
                if (fnOnComplete) {
                    pStreamer->Completed += [fnOnComplete](const void*, const EventArgs&) {
                        fnOnComplete();
                    };
                }
                pStreamer->Start();
            }

#if defined(_WIN32)
            struct ProcessCpuSample {
                uint64_t uProcessTime;
                uint64_t uSystemTime;
            };
            static std::map<int, ProcessCpuSample> s_mapProcessCpuSamples;

            struct SystemMetricsWin32Helper {
                static MemoryInfo GetSystemMemory() {
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

                static double CalculateCpuDelta(uint64_t uIdle, uint64_t uKernel, uint64_t uUser) {
                    static uint64_t s_uPrevIdle = 0, s_uPrevTotal = 0;
                    uint64_t uTotal = uKernel + uUser;
                    double dCpu = 0.0;
                    if (s_uPrevTotal > 0 && uTotal > s_uPrevTotal) {
                        uint64_t uTotalDiff = uTotal - s_uPrevTotal;
                        uint64_t uIdleDiff = uIdle - s_uPrevIdle;
                        dCpu = static_cast<double>((uTotalDiff - uIdleDiff) * 100.0 / uTotalDiff);
                    } else if (uTotal > 0) {
                        dCpu = static_cast<double>((uTotal - uIdle) * 100.0 / uTotal);
                    }
                    s_uPrevIdle = uIdle; s_uPrevTotal = uTotal;
                    return (dCpu < 0.0) ? 0.0 : ((dCpu > 100.0) ? 100.0 : dCpu);
                }

                static double GetSystemCpu() {
                    FILETIME ftIdle, ftKernel, ftUser;
                    if (!::GetSystemTimes(&ftIdle, &ftKernel, &ftUser)) return 0.0;
                    uint64_t uIdle = (static_cast<uint64_t>(ftIdle.dwHighDateTime) << 32) | ftIdle.dwLowDateTime;
                    uint64_t uKernel = (static_cast<uint64_t>(ftKernel.dwHighDateTime) << 32) | ftKernel.dwLowDateTime;
                    uint64_t uUser = (static_cast<uint64_t>(ftUser.dwHighDateTime) << 32) | ftUser.dwLowDateTime;
                    return CalculateCpuDelta(uIdle, uKernel, uUser);
                }

                static void InitDiskQuery(HQUERY& hQuery, HCOUNTER& hRead, HCOUNTER& hWrite) {
                    static bool s_bPdhInitialized = false;
                    if (!s_bPdhInitialized && ::PdhOpenQueryW(NULL, 0, &hQuery) == ERROR_SUCCESS) {
                        ::PdhAddEnglishCounterW(hQuery, L"\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", 0, &hRead);
                        ::PdhAddEnglishCounterW(hQuery, L"\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", 0, &hWrite);
                        ::PdhCollectQueryData(hQuery);
                        s_bPdhInitialized = true;
                    }
                }

                static DiskInfo GetSystemDisk() {
                    DiskInfo info;
                    static HQUERY s_hQuery = NULL; static HCOUNTER s_hRead = NULL, s_hWrite = NULL;
                    InitDiskQuery(s_hQuery, s_hRead, s_hWrite);
                    if (s_hQuery && ::PdhCollectQueryData(s_hQuery) == ERROR_SUCCESS) {
                        PDH_FMT_COUNTERVALUE fmtRead, fmtWrite;
                        if (::PdhGetFormattedCounterValue(s_hRead, PDH_FMT_LARGE, NULL, &fmtRead) == ERROR_SUCCESS) info.lDiskReadBytes = static_cast<long long>(fmtRead.largeValue);
                        if (::PdhGetFormattedCounterValue(s_hWrite, PDH_FMT_LARGE, NULL, &fmtWrite) == ERROR_SUCCESS) info.lDiskWriteBytes = static_cast<long long>(fmtWrite.largeValue);
                    }
                    return info;
                }

                static double CalculateNetRate(uint64_t totalOctets) {
                    static uint64_t s_uPrevOctets = 0; static DWORD s_dwPrevTick = 0;
                    DWORD dwNow = ::GetTickCount(); double dMbps = 0.0;
                    if (s_dwPrevTick > 0 && dwNow > s_dwPrevTick && totalOctets >= s_uPrevOctets) {
                        double dElapsedSec = (dwNow - s_dwPrevTick) / 1000.0;
                        dMbps = ((totalOctets - s_uPrevOctets) / dElapsedSec * 8.0) / 1000000.0;
                    }
                    s_uPrevOctets = totalOctets; s_dwPrevTick = dwNow;
                    return dMbps;
                }

                static double GetSystemNetwork() {
                    DWORD dwSize = 0;
                    if (::GetIfTable(NULL, &dwSize, FALSE) != ERROR_INSUFFICIENT_BUFFER) return 0.0;
                    std::vector<uint8_t> buf(dwSize, 0);
                    MIB_IFTABLE* pTable = reinterpret_cast<MIB_IFTABLE*>(buf.data());
                    if (::GetIfTable(pTable, &dwSize, FALSE) != NO_ERROR) return 0.0;
                    uint64_t totalOctets = 0;
                    for (DWORD i = 0; i < pTable->dwNumEntries; ++i) {
                        if (pTable->table[i].dwType != IF_TYPE_SOFTWARE_LOOPBACK && pTable->table[i].dwOperStatus == IF_OPER_STATUS_OPERATIONAL)
                            totalOctets += pTable->table[i].dwInOctets + pTable->table[i].dwOutOctets;
                    }
                    return CalculateNetRate(totalOctets);
                }

                static void* OpenProcByName(const String& sProcessName, unsigned long dwAccess, int& iOutPid) {
                    iOutPid = -1;
                    auto arrMatches = Process::GetProcessesByName(sProcessName);
                    if (arrMatches.GetLength() == 0) return NULL;
                    iOutPid = arrMatches[0]->GetId();
                    return ::OpenProcess(dwAccess, FALSE, static_cast<DWORD>(iOutPid));
                }

                static String ReadPebCommandLine(HANDLE hProc, PVOID pebBase) {
                    PEB peb; SIZE_T bytesRead = 0;
                    if (!::ReadProcessMemory(hProc, pebBase, &peb, sizeof(peb), &bytesRead)) return String("");
                    RTL_USER_PROCESS_PARAMETERS upp;
                    if (!::ReadProcessMemory(hProc, peb.ProcessParameters, &upp, sizeof(upp), &bytesRead) || !upp.CommandLine.Buffer || upp.CommandLine.Length == 0) return String("");
                    std::vector<wchar_t> wCmd(upp.CommandLine.Length / sizeof(wchar_t) + 1, 0);
                    if (::ReadProcessMemory(hProc, upp.CommandLine.Buffer, wCmd.data(), upp.CommandLine.Length, &bytesRead)) return String(wCmd.data());
                    return String("");
                }

                static String ReadProcCmdLine(void* hProc) {
                    typedef NTSTATUS(NTAPI* pfnNtQuery)(HANDLE, ULONG, PVOID, ULONG, PULONG);
                    HMODULE hNtDll = ::GetModuleHandleW(L"ntdll.dll");
                    pfnNtQuery fnNtQuery = hNtDll ? (pfnNtQuery)::GetProcAddress(hNtDll, "NtQueryInformationProcess") : NULL;
                    if (!fnNtQuery || !hProc) return String("");
                    PROCESS_BASIC_INFORMATION pbi; DWORD dwLen = 0;
                    if (fnNtQuery(static_cast<HANDLE>(hProc), 0, &pbi, sizeof(pbi), &dwLen) == 0 && pbi.PebBaseAddress) {
                        return ReadPebCommandLine(static_cast<HANDLE>(hProc), pbi.PebBaseAddress);
                    }
                    return String("");
                }

                static MemoryInfo ReadProcMemory(void* hProc) {
                    MemoryInfo info;
                    if (hProc) {
                        PROCESS_MEMORY_COUNTERS_EX pmc;
                        if (::GetProcessMemoryInfo(static_cast<HANDLE>(hProc), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
                            info.lPhysicalMemoryBytes = static_cast<long long>(pmc.WorkingSetSize);
                            info.lPrivateBytes = static_cast<long long>(pmc.PrivateUsage);
                        }
                    }
                    return info;
                }

                static void QueryPdhIoCounters(const std::wstring& wProcName, DiskInfo& info) {
                    std::wstring rPath = L"\\Process(" + wProcName + L")\\IO Read Bytes/sec";
                    std::wstring wPath = L"\\Process(" + wProcName + L")\\IO Write Bytes/sec";
                    HQUERY hQuery = NULL; HCOUNTER hRead = NULL, hWrite = NULL;
                    if (::PdhOpenQueryW(NULL, 0, &hQuery) != ERROR_SUCCESS) return;
                    bool rOk = (::PdhAddEnglishCounterW(hQuery, rPath.c_str(), 0, &hRead) == ERROR_SUCCESS);
                    bool wOk = (::PdhAddEnglishCounterW(hQuery, wPath.c_str(), 0, &hWrite) == ERROR_SUCCESS);
                    if (::PdhCollectQueryData(hQuery) == ERROR_SUCCESS) {
                        PDH_FMT_COUNTERVALUE fmtR, fmtW;
                        if (rOk && ::PdhGetFormattedCounterValue(hRead, PDH_FMT_LARGE, NULL, &fmtR) == ERROR_SUCCESS) info.lDiskReadBytes = static_cast<long long>(fmtR.largeValue);
                        if (wOk && ::PdhGetFormattedCounterValue(hWrite, PDH_FMT_LARGE, NULL, &fmtW) == ERROR_SUCCESS) info.lDiskWriteBytes = static_cast<long long>(fmtW.largeValue);
                    }
                    ::PdhCloseQuery(hQuery);
                }

                static DiskInfo ReadProcDisk(void* hProc, const String& sProcessName) {
                    DiskInfo info;
                    std::string sStd(sProcessName.GetRawString() ? sProcessName.GetRawString() : "");
                    std::wstring wProc(sStd.begin(), sStd.end());
                    size_t pos = wProc.rfind(L'.'); if (pos != std::wstring::npos) wProc = wProc.substr(0, pos);
                    QueryPdhIoCounters(wProc, info);
                    if ((info.lDiskReadBytes == -1 || info.lDiskWriteBytes == -1) && hProc) {
                        IO_COUNTERS io;
                        if (::GetProcessIoCounters(static_cast<HANDLE>(hProc), &io)) {
                            if (info.lDiskReadBytes == -1) info.lDiskReadBytes = static_cast<long long>(io.ReadTransferCount);
                            if (info.lDiskWriteBytes == -1) info.lDiskWriteBytes = static_cast<long long>(io.WriteTransferCount);
                        }
                    }
                    return info;
                }

                static NetworkUsageInfo ReadProcNetwork(void* hProc, const String& sProcessName) {
                    NetworkUsageInfo info;
                    std::string sStd(sProcessName.GetRawString() ? sProcessName.GetRawString() : "");
                    std::wstring wProc(sStd.begin(), sStd.end());
                    size_t pos = wProc.rfind(L'.'); if (pos != std::wstring::npos) wProc = wProc.substr(0, pos);
                    DiskInfo di;
                    QueryPdhIoCounters(wProc, di);
                    info.lNetworkReadBytes = di.lDiskReadBytes;
                    info.lNetworkWriteBytes = di.lDiskWriteBytes;
                    return info;
                }

                static void ReadUdpPorts(int iPid, Collections::Generic::List<int>& lst) {
                    if (iPid <= 0) return;
                    DWORD dwSize = 0;
                    if (::GetExtendedUdpTable(NULL, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != ERROR_INSUFFICIENT_BUFFER) return;
                    std::vector<uint8_t> buf(dwSize, 0);
                    MIB_UDPTABLE_OWNER_PID* pTable = reinterpret_cast<MIB_UDPTABLE_OWNER_PID*>(buf.data());
                    if (::GetExtendedUdpTable(pTable, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR) return;
                    for (DWORD i = 0; i < pTable->dwNumEntries; ++i) {
                        if (pTable->table[i].dwOwningPid == static_cast<DWORD>(iPid)) {
                            int p = ntohs(static_cast<u_short>(pTable->table[i].dwLocalPort));
                            if (!lst.Contains(p)) lst.Add(p);
                        }
                    }
                }

                static Collections::Generic::List<int> ReadProcPorts(int iPid) {
                    Collections::Generic::List<int> lst;
                    if (iPid <= 0) return lst;
                    DWORD dwSize = 0;
                    if (::GetExtendedTcpTable(NULL, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER) {
                        std::vector<uint8_t> buf(dwSize, 0);
                        MIB_TCPTABLE_OWNER_PID* pTable = reinterpret_cast<MIB_TCPTABLE_OWNER_PID*>(buf.data());
                        if (::GetExtendedTcpTable(pTable, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
                            for (DWORD i = 0; i < pTable->dwNumEntries; ++i) {
                                if (pTable->table[i].dwOwningPid == static_cast<DWORD>(iPid) && pTable->table[i].dwState == MIB_TCP_STATE_LISTEN) {
                                    int p = ntohs(static_cast<u_short>(pTable->table[i].dwLocalPort));
                                    if (!lst.Contains(p)) lst.Add(p);
                                }
                            }
                        }
                    }
                    ReadUdpPorts(iPid, lst);
                    return lst;
                }

                static void ExtractTcpConnection(MIB_TCPROW_OWNER_PID& row, NetworkConnectionInfo& conn) {
                    in_addr lAddr, rAddr;
                    lAddr.S_un.S_addr = row.dwLocalAddr; rAddr.S_un.S_addr = row.dwRemoteAddr;
                    char szL[INET_ADDRSTRLEN] = { 0 }, szR[INET_ADDRSTRLEN] = { 0 };
                    ::inet_ntop(AF_INET, &lAddr, szL, sizeof(szL));
                    ::inet_ntop(AF_INET, &rAddr, szR, sizeof(szR));
                    conn.sLocalAddress = String(szL); conn.iLocalPort = ntohs(static_cast<u_short>(row.dwLocalPort));
                    conn.sRemoteAddress = String(szR); conn.iRemotePort = ntohs(static_cast<u_short>(row.dwRemotePort));
                    conn.sState = (row.dwState == MIB_TCP_STATE_LISTEN) ? "LISTEN" : ((row.dwState == MIB_TCP_STATE_ESTAB) ? "ESTABLISHED" : "OTHER");
                }

                static void ReadUdpConnections(int iPid, ProcessNetworkConnectionInfo& info) {
                    if (iPid <= 0) return;
                    DWORD dwSize = 0;
                    if (::GetExtendedUdpTable(NULL, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != ERROR_INSUFFICIENT_BUFFER) return;
                    std::vector<uint8_t> buf(dwSize, 0);
                    MIB_UDPTABLE_OWNER_PID* pTable = reinterpret_cast<MIB_UDPTABLE_OWNER_PID*>(buf.data());
                    if (::GetExtendedUdpTable(pTable, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR) return;
                    for (DWORD i = 0; i < pTable->dwNumEntries; ++i) {
                        if (pTable->table[i].dwOwningPid == static_cast<DWORD>(iPid)) {
                            int p = ntohs(static_cast<u_short>(pTable->table[i].dwLocalPort));
                            if (!info.lstOpenPorts.Contains(p)) info.lstOpenPorts.Add(p);
                            NetworkConnectionInfo conn; conn.iLocalPort = p; conn.sState = "UDP";
                            in_addr lAddr; lAddr.S_un.S_addr = pTable->table[i].dwLocalAddr;
                            char szL[INET_ADDRSTRLEN] = { 0 };
                            ::inet_ntop(AF_INET, &lAddr, szL, sizeof(szL));
                            conn.sLocalAddress = String(szL);
                            info.lstConnections.Add(conn);
                        }
                    }
                }

                static ProcessNetworkConnectionInfo ReadProcNetInfo(int iPid) {
                    ProcessNetworkConnectionInfo info;
                    if (iPid <= 0) return info;
                    DWORD dwSize = 0;
                    if (::GetExtendedTcpTable(NULL, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER) {
                        std::vector<uint8_t> buf(dwSize, 0);
                        MIB_TCPTABLE_OWNER_PID* pTable = reinterpret_cast<MIB_TCPTABLE_OWNER_PID*>(buf.data());
                        if (::GetExtendedTcpTable(pTable, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
                            for (DWORD i = 0; i < pTable->dwNumEntries; ++i) {
                                if (pTable->table[i].dwOwningPid == static_cast<DWORD>(iPid)) {
                                    NetworkConnectionInfo conn;
                                    ExtractTcpConnection(pTable->table[i], conn);
                                    info.lstConnections.Add(conn);
                                    if (pTable->table[i].dwState == MIB_TCP_STATE_LISTEN && !info.lstOpenPorts.Contains(conn.iLocalPort)) info.lstOpenPorts.Add(conn.iLocalPort);
                                    else if (pTable->table[i].dwState == MIB_TCP_STATE_ESTAB) info.bHasEstablishedInboundConnection = true;
                                }
                            }
                        }
                    }
                    ReadUdpConnections(iPid, info);
                    return info;
                }

                static void CalculateProcessCpu(HANDLE hProc, int iPid, double& dCpu) {
                    FILETIME ftCreate, ftExit, ftKernel, ftUser, ftSysIdle, ftSysKernel, ftSysUser, ftNow;
                    if (!::GetProcessTimes(hProc, &ftCreate, &ftExit, &ftKernel, &ftUser) || !::GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser)) return;
                    uint64_t uProc = ((static_cast<uint64_t>(ftKernel.dwHighDateTime) << 32) | ftKernel.dwLowDateTime) + ((static_cast<uint64_t>(ftUser.dwHighDateTime) << 32) | ftUser.dwLowDateTime);
                    uint64_t uSys = ((static_cast<uint64_t>(ftSysKernel.dwHighDateTime) << 32) | ftSysKernel.dwLowDateTime) + ((static_cast<uint64_t>(ftSysUser.dwHighDateTime) << 32) | ftSysUser.dwLowDateTime);
                    auto it = s_mapProcessCpuSamples.find(iPid);
                    if (it != s_mapProcessCpuSamples.end() && uSys > it->second.uSystemTime && uProc >= it->second.uProcessTime) {
                        double c = (static_cast<double>(uProc - it->second.uProcessTime) * 100.0 / static_cast<double>(uSys - it->second.uSystemTime));
                        dCpu = (c > 100.0) ? 100.0 : ((c < 0.0) ? 0.0 : c);
                    } else {
                        ::GetSystemTimeAsFileTime(&ftNow); SYSTEM_INFO si; ::GetSystemInfo(&si);
                        uint64_t uNow = (static_cast<uint64_t>(ftNow.dwHighDateTime) << 32) | ftNow.dwLowDateTime; uint64_t uCreate = (static_cast<uint64_t>(ftCreate.dwHighDateTime) << 32) | ftCreate.dwLowDateTime;
                        if (uNow > uCreate && uProc > 0) { double c = (static_cast<double>(uProc) * 100.0) / (static_cast<double>(uNow - uCreate) * static_cast<double>(si.dwNumberOfProcessors > 0 ? si.dwNumberOfProcessors : 1)); dCpu = (c > 100.0) ? 100.0 : ((c < 0.0) ? 0.0 : c); }
                    }
                    s_mapProcessCpuSamples[iPid] = { uProc, uSys };
                }

                static void PopulateProcNetwork(int iPid, ProcessInfo& proc) {
                    auto netInfo = ReadProcNetInfo(iPid);
                    proc.lstOpenPorts = netInfo.lstOpenPorts;
                    proc.lstConnections = netInfo.lstConnections;
                    proc.bHasEstablishedConnection = netInfo.bHasEstablishedInboundConnection;
                }

                static String GetServiceStartType(SC_HANDLE hSCM, LPCWSTR lpServiceName) {
                    SC_HANDLE hService = ::OpenServiceW(hSCM, lpServiceName, SERVICE_QUERY_CONFIG);
                    if (!hService) return "Manual";
                    BYTE buffer[1024]; DWORD dwNeeded = 0;
                    QUERY_SERVICE_CONFIGW* pConfig = reinterpret_cast<QUERY_SERVICE_CONFIGW*>(buffer);
                    String sType = "Manual";
                    if (::QueryServiceConfigW(hService, pConfig, sizeof(buffer), &dwNeeded)) {
                        switch (pConfig->dwStartType) {
                        case SERVICE_AUTO_START: sType = "Automatic"; break;
                        case SERVICE_DEMAND_START: sType = "Manual"; break;
                        case SERVICE_DISABLED: sType = "Disabled"; break;
                        case SERVICE_BOOT_START: sType = "Boot"; break;
                        case SERVICE_SYSTEM_START: sType = "System"; break;
                        default: sType = "Manual"; break;
                        }
                    }
                    ::CloseServiceHandle(hService);
                    return sType;
                }

                static void ParseServiceStatus(ENUM_SERVICE_STATUS_PROCESSW& svc, SC_HANDLE hSCM, ServiceInfo& info) {
                    info.sServiceName = String(svc.lpServiceName); info.sDisplayName = String(svc.lpDisplayName);
                    info.iProcessId = static_cast<int>(svc.ServiceStatusProcess.dwProcessId);
                    switch (svc.ServiceStatusProcess.dwCurrentState) {
                    case SERVICE_RUNNING: info.sStatus = "Running"; break;
                    case SERVICE_STOPPED: info.sStatus = "Stopped"; break;
                    case SERVICE_PAUSED: info.sStatus = "Paused"; break;
                    case SERVICE_START_PENDING: info.sStatus = "StartPending"; break;
                    case SERVICE_STOP_PENDING: info.sStatus = "StopPending"; break;
                    default: info.sStatus = "Unknown"; break;
                    }
                    info.sStartType = GetServiceStartType(hSCM, svc.lpServiceName);
                }
            };

            MemoryInfo SystemMetrics::GetSystemMemoryUsage() { return SystemMetricsWin32Helper::GetSystemMemory(); }
            double SystemMetrics::GetSystemCpuUsage() { return SystemMetricsWin32Helper::GetSystemCpu(); }
            DiskInfo SystemMetrics::GetSystemDiskUsage() { return SystemMetricsWin32Helper::GetSystemDisk(); }
            double SystemMetrics::GetSystemNetworkUsage() { return SystemMetricsWin32Helper::GetSystemNetwork(); }

            void* SystemMetrics::OpenProcessByName(const String& sProcessName, unsigned long dwDesiredAccess, int& iOutProcessId) {
                return SystemMetricsWin32Helper::OpenProcByName(sProcessName, dwDesiredAccess, iOutProcessId);
            }

            String SystemMetrics::ReadProcessCommandLineHandle(void* hProc) {
                return SystemMetricsWin32Helper::ReadProcCmdLine(hProc);
            }

            String SystemMetrics::GetProcessCommandLine(const String& sProcessName) {
                int iPid = -1;
                HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, iPid));
                if (!hProc) return String("");
                String sCmd = ReadProcessCommandLineHandle(hProc);
                if (sCmd.IsEmpty()) {
                    WCHAR szPath[MAX_PATH] = { 0 }; DWORD dwLen = MAX_PATH;
                    if (::QueryFullProcessImageNameW(hProc, 0, szPath, &dwLen)) sCmd = String(szPath);
                }
                ::CloseHandle(hProc);
                return sCmd;
            }

            MemoryInfo SystemMetrics::ReadProcessMemoryHandle(void* hProc) { return SystemMetricsWin32Helper::ReadProcMemory(hProc); }
            MemoryInfo SystemMetrics::GetProcessMemoryUsage(const String& sProcessName) {
                int iPid = -1; HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, iPid));
                if (!hProc) return MemoryInfo();
                MemoryInfo mem = ReadProcessMemoryHandle(hProc);
                ::CloseHandle(hProc);
                return mem;
            }

            DiskInfo SystemMetrics::ReadProcessDiskHandle(void* hProc, const String& sProcessName) { return SystemMetricsWin32Helper::ReadProcDisk(hProc, sProcessName); }
            DiskInfo SystemMetrics::GetProcessDiskUsage(const String& sProcessName) {
                int iPid = -1; HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                DiskInfo di = ReadProcessDiskHandle(hProc, sProcessName);
                if (hProc) ::CloseHandle(hProc);
                return di;
            }

            NetworkUsageInfo SystemMetrics::ReadProcessNetworkHandle(void* hProc, const String& sProcessName) { return SystemMetricsWin32Helper::ReadProcNetwork(hProc, sProcessName); }
            NetworkUsageInfo SystemMetrics::GetProcessNetworkUsage(const String& sProcessName) {
                int iPid = -1; HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                NetworkUsageInfo net = ReadProcessNetworkHandle(hProc, sProcessName);
                if (hProc) ::CloseHandle(hProc);
                return net;
            }

            Collections::Generic::List<int> SystemMetrics::ReadProcessNetworkPortInternal(int iProcessId) { return SystemMetricsWin32Helper::ReadProcPorts(iProcessId); }
            Collections::Generic::List<int> SystemMetrics::GetProcessNetworkPort(const String& sProcessName) {
                int iPid = -1; HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                if (hProc) ::CloseHandle(hProc);
                return ReadProcessNetworkPortInternal(iPid);
            }
            Collections::Generic::List<int> SystemMetrics::GetProcessNetworkPort(int iProcessId) {
                return ReadProcessNetworkPortInternal(iProcessId);
            }

            ProcessNetworkConnectionInfo SystemMetrics::ReadProcessNetworkInfoInternal(int iProcessId) { return SystemMetricsWin32Helper::ReadProcNetInfo(iProcessId); }
            ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(const String& sProcessName) {
                int iPid = -1; HANDLE hProc = static_cast<HANDLE>(OpenProcessByName(sProcessName, PROCESS_QUERY_LIMITED_INFORMATION, iPid));
                if (hProc) ::CloseHandle(hProc);
                return ReadProcessNetworkInfoInternal(iPid);
            }
            ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(int iProcessId) {
                return ReadProcessNetworkInfoInternal(iProcessId);
            }

            void SystemMetrics::EnrichProcessInfo(ProcessInfo& proc, bool bIncludeNetwork) {
                if (proc.iProcessId <= 0) throw ArgumentException("Process ID must be greater than zero.");
                HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, proc.iProcessId);
                if (hProc) {
                    if (proc.sPath.IsEmpty()) {
                        WCHAR szPath[MAX_PATH] = { 0 }; DWORD dwLen = MAX_PATH;
                        if (::QueryFullProcessImageNameW(hProc, 0, szPath, &dwLen)) proc.sPath = String(szPath);
                    }
                    proc.sCommandLine = SystemMetricsWin32Helper::ReadProcCmdLine(hProc);
                    if (proc.sCommandLine.IsEmpty()) proc.sCommandLine = proc.sPath;
                    proc.memory = SystemMetricsWin32Helper::ReadProcMemory(hProc);
                    proc.disk = SystemMetricsWin32Helper::ReadProcDisk(hProc, proc.sName);
                    proc.network = SystemMetricsWin32Helper::ReadProcNetwork(hProc, proc.sName);
                    SystemMetricsWin32Helper::CalculateProcessCpu(hProc, proc.iProcessId, proc.dCpuUsagePercent);
                    ::CloseHandle(hProc);
                }
                if (bIncludeNetwork) SystemMetricsWin32Helper::PopulateProcNetwork(proc.iProcessId, proc);
            }

            Collections::Generic::List<ServiceInfo> SystemMetrics::GetAllServices() {
                Collections::Generic::List<ServiceInfo> lst;
                SC_HANDLE hSCM = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
                if (!hSCM) return lst;
                DWORD dwNeeded = 0, dwReturned = 0, dwResume = 0;
                ::EnumServicesStatusExW(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwNeeded, &dwReturned, &dwResume, NULL);
                if (dwNeeded > 0) {
                    std::vector<uint8_t> buf(dwNeeded, 0);
                    ENUM_SERVICE_STATUS_PROCESSW* pSvcs = reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(buf.data());
                    if (::EnumServicesStatusExW(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, buf.data(), dwNeeded, &dwNeeded, &dwReturned, &dwResume, NULL)) {
                        for (DWORD i = 0; i < dwReturned; ++i) {
                            ServiceInfo svc; SystemMetricsWin32Helper::ParseServiceStatus(pSvcs[i], hSCM, svc);
                            lst.Add(svc);
                        }
                    }
                }
                ::CloseServiceHandle(hSCM);
                return lst;
            }
#endif

        }
    }
}
