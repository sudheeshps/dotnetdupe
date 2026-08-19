#include "pch.h"
#include "System/Diagnostics/SystemMetrics.h"
#include "System/ArgumentException.h"
#include "System/Utils/StringConvert.h"
#include <algorithm>
#include <vector>
#include <map>

#if !defined(_WIN32)
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <arpa/inet.h>

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            struct SystemMetricsLinuxHelper {
                static int FindPid(const String& sProcessName) {
                    DIR* dir = ::opendir("/proc");
                    if (!dir) return -1;
                    struct dirent* entry = nullptr;
                    std::string target = sProcessName.GetRawString();
                    while ((entry = ::readdir(dir)) != nullptr) {
                        if (entry->d_type == DT_DIR) {
                            std::string dname = entry->d_name;
                            if (std::all_of(dname.begin(), dname.end(), ::isdigit)) {
                                std::ifstream f("/proc/" + dname + "/comm");
                                std::string comm;
                                if (f.is_open() && std::getline(f, comm) && comm == target) {
                                    ::closedir(dir); return std::stoi(dname);
                                }
                            }
                        }
                    }
                    ::closedir(dir); return -1;
                }

                static MemoryInfo ReadMem(int iPid) {
                    MemoryInfo info;
                    if (iPid <= 0) return info;
                    std::ifstream f("/proc/" + std::to_string(iPid) + "/status");
                    std::string line; long long rss = 0, vm = 0;
                    while (f.is_open() && std::getline(f, line)) {
                        if (line.rfind("VmRSS:", 0) == 0) std::istringstream(line.substr(6)) >> rss;
                        else if (line.rfind("VmSize:", 0) == 0) std::istringstream(line.substr(7)) >> vm;
                    }
                    info.lPhysicalMemoryBytes = rss * 1024LL; info.lPrivateBytes = vm * 1024LL;
                    return info;
                }

                static DiskInfo ReadDisk(int iPid) {
                    DiskInfo info;
                    if (iPid <= 0) return info;
                    std::ifstream f("/proc/" + std::to_string(iPid) + "/io");
                    std::string line;
                    while (f.is_open() && std::getline(f, line)) {
                        if (line.rfind("read_bytes:", 0) == 0) std::istringstream(line.substr(11)) >> info.lDiskReadBytes;
                        else if (line.rfind("write_bytes:", 0) == 0) std::istringstream(line.substr(12)) >> info.lDiskWriteBytes;
                    }
                    return info;
                }

                static NetworkUsageInfo ReadNet(int iPid) {
                    NetworkUsageInfo info;
                    if (iPid <= 0) return info;
                    std::ifstream f("/proc/" + std::to_string(iPid) + "/net/dev");
                    std::string line; long long totalRx = 0, totalTx = 0; int cnt = 0;
                    while (f.is_open() && std::getline(f, line)) {
                        if (++cnt <= 2) continue;
                        auto pos = line.find(':');
                        if (pos != std::string::npos) {
                            std::istringstream iss(line.substr(pos + 1));
                            long long rx = 0, tx = 0, dummy = 0;
                            iss >> rx; for (int i = 0; i < 7; ++i) iss >> dummy; iss >> tx;
                            totalRx += rx; totalTx += tx;
                        }
                    }
                    info.lNetworkReadBytes = totalRx; info.lNetworkWriteBytes = totalTx;
                    return info;
                }

                static ProcessNetworkConnectionInfo ReadNetInfo(int iPid) {
                    ProcessNetworkConnectionInfo info;
                    if (iPid <= 0) return info;
                    std::ifstream f("/proc/net/tcp");
                    std::string line; int cnt = 0;
                    while (f.is_open() && std::getline(f, line)) {
                        if (cnt++ == 0) continue;
                        std::istringstream iss(line);
                        std::string sl, lAddr, rAddr, st;
                        if (iss >> sl >> lAddr >> rAddr >> st) {
                            auto c1 = lAddr.find(':'), c2 = rAddr.find(':');
                            if (c1 != std::string::npos && c2 != std::string::npos) {
                                int lp = std::stoi(lAddr.substr(c1 + 1), nullptr, 16);
                                int rp = std::stoi(rAddr.substr(c2 + 1), nullptr, 16);
                                NetworkConnectionInfo conn; conn.iLocalPort = lp; conn.iRemotePort = rp;
                                conn.sState = (st == "01") ? "ESTABLISHED" : "LISTEN";
                                info.lstConnections.Add(conn); info.lstOpenPorts.Add(lp);
                                if (st == "01") info.bHasEstablishedInboundConnection = true;
                            }
                        }
                    }
                    return info;
                }

                static MemoryInfo GetSysMem() {
                    MemoryInfo info;
                    std::ifstream f("/proc/meminfo");
                    std::string line; unsigned long long tot = 0, av = 0;
                    while (f.is_open() && std::getline(f, line)) {
                        if (line.rfind("MemTotal:", 0) == 0) std::istringstream(line.substr(9)) >> tot;
                        else if (line.rfind("MemAvailable:", 0) == 0) std::istringstream(line.substr(13)) >> av;
                    }
                    info.uMemoryTotalBytes = tot * 1024ULL;
                    info.uMemoryUsedBytes = (tot > av) ? (tot - av) * 1024ULL : 0ULL;
                    if (tot > 0) info.dMemoryUsagePercent = (static_cast<double>(info.uMemoryUsedBytes) / static_cast<double>(info.uMemoryTotalBytes)) * 100.0;
                    return info;
                }

                static double GetSysCpu() {
                    std::ifstream f("/proc/stat");
                    std::string line;
                    if (f.is_open() && std::getline(f, line) && line.rfind("cpu ", 0) == 0) {
                        std::istringstream iss(line.substr(4));
                        long long u, n, s, id, io, ir, so, st;
                        if (iss >> u >> n >> s >> id >> io >> ir >> so >> st) {
                            long long idle = id + io, nonIdle = u + n + s + ir + so + st, total = idle + nonIdle;
                            if (total > 0) return (static_cast<double>(nonIdle) / static_cast<double>(total)) * 100.0;
                        }
                    }
                    return 0.0;
                }

                static DiskInfo GetSysDisk() {
                    DiskInfo info; struct statvfs vfs;
                    if (statvfs("/", &vfs) == 0) {
                        info.lDiskReadBytes = static_cast<long long>(vfs.f_blocks * vfs.f_frsize);
                        info.lDiskWriteBytes = static_cast<long long>((vfs.f_blocks - vfs.f_bfree) * vfs.f_frsize);
                    }
                    return info;
                }

                static double GetSysNet() {
                    std::ifstream f("/proc/net/dev");
                    std::string line; long long total = 0; int cnt = 0;
                    while (f.is_open() && std::getline(f, line)) {
                        if (++cnt <= 2) continue;
                        auto pos = line.find(':');
                        if (pos != std::string::npos) {
                            std::istringstream iss(line.substr(pos + 1));
                            long long rx = 0, tx = 0, dummy = 0;
                            iss >> rx; for (int i = 0; i < 7; ++i) iss >> dummy; iss >> tx;
                            total += (rx + tx);
                        }
                    }
                    return static_cast<double>(total);
                }

                static String ReadCmdLine(int pid) {
                    if (pid <= 0) return String("");
                    std::ifstream f("/proc/" + std::to_string(pid) + "/cmdline", std::ios::binary);
                    std::string cmd; char ch;
                    while (f.is_open() && f.get(ch)) cmd += (ch == '\0') ? ' ' : ch;
                    return String(cmd.c_str());
                }
            };

            int SystemMetrics::FindPidByName(const String& sProcessName) { return SystemMetricsLinuxHelper::FindPid(sProcessName); }
            MemoryInfo SystemMetrics::ReadLinuxProcessMemory(int iPid) { return SystemMetricsLinuxHelper::ReadMem(iPid); }
            DiskInfo SystemMetrics::ReadLinuxProcessDisk(int iPid) { return SystemMetricsLinuxHelper::ReadDisk(iPid); }
            NetworkUsageInfo SystemMetrics::ReadLinuxProcessNetwork(int iPid) { return SystemMetricsLinuxHelper::ReadNet(iPid); }
            ProcessNetworkConnectionInfo SystemMetrics::ReadLinuxProcessNetworkInfo(int iPid) { return SystemMetricsLinuxHelper::ReadNetInfo(iPid); }
            Collections::Generic::List<int> SystemMetrics::ReadLinuxProcessPorts(int iPid) { return SystemMetricsLinuxHelper::ReadNetInfo(iPid).lstOpenPorts; }

            MemoryInfo SystemMetrics::GetSystemMemoryUsage() { return SystemMetricsLinuxHelper::GetSysMem(); }
            double SystemMetrics::GetSystemCpuUsage() { return SystemMetricsLinuxHelper::GetSysCpu(); }
            DiskInfo SystemMetrics::GetSystemDiskUsage() { return SystemMetricsLinuxHelper::GetSysDisk(); }
            double SystemMetrics::GetSystemNetworkUsage() { return SystemMetricsLinuxHelper::GetSysNet(); }

            String SystemMetrics::GetProcessCommandLine(const String& sProcessName) { return SystemMetricsLinuxHelper::ReadCmdLine(FindPidByName(sProcessName)); }
            MemoryInfo SystemMetrics::GetProcessMemoryUsage(const String& sProcessName) { return ReadLinuxProcessMemory(FindPidByName(sProcessName)); }
            DiskInfo SystemMetrics::GetProcessDiskUsage(const String& sProcessName) { return ReadLinuxProcessDisk(FindPidByName(sProcessName)); }
            NetworkUsageInfo SystemMetrics::GetProcessNetworkUsage(const String& sProcessName) { return ReadLinuxProcessNetwork(FindPidByName(sProcessName)); }
            Collections::Generic::List<int> SystemMetrics::GetProcessNetworkPort(const String& sProcessName) { return ReadLinuxProcessPorts(FindPidByName(sProcessName)); }
            Collections::Generic::List<int> SystemMetrics::GetProcessNetworkPort(int iProcessId) { return ReadLinuxProcessPorts(iProcessId); }
            ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(const String& sProcessName) { return ReadLinuxProcessNetworkInfo(FindPidByName(sProcessName)); }
            ProcessNetworkConnectionInfo SystemMetrics::GetProcessNetworkInfo(int iProcessId) { return ReadLinuxProcessNetworkInfo(iProcessId); }

            void SystemMetrics::EnrichProcessInfo(ProcessInfo& proc, bool bIncludeNetwork) {
                if (proc.iProcessId <= 0) throw ArgumentException("Process ID must be greater than zero.");
                proc.sCommandLine = GetProcessCommandLine(proc.sName);
                proc.memory = ReadLinuxProcessMemory(proc.iProcessId);
                proc.disk = ReadLinuxProcessDisk(proc.iProcessId);
                proc.network = ReadLinuxProcessNetwork(proc.iProcessId);
                if (bIncludeNetwork) {
                    auto netInfo = ReadLinuxProcessNetworkInfo(proc.iProcessId);
                    proc.lstOpenPorts = netInfo.lstOpenPorts;
                    proc.lstConnections = netInfo.lstConnections;
                    proc.bHasEstablishedConnection = netInfo.bHasEstablishedInboundConnection;
                }
            }



            void SystemMetrics::PopulateLinuxProc(const std::string& dname, ProcessInfo& proc) {
                proc.iProcessId = std::stoi(dname);
                std::ifstream commFile("/proc/" + dname + "/comm");
                std::string procComm;
                if (commFile.is_open() && std::getline(commFile, procComm)) proc.sName = String(procComm.c_str());
                proc.sCommandLine = GetProcessCommandLine(proc.sName);
                proc.memory = ReadLinuxProcessMemory(proc.iProcessId);
                proc.disk = ReadLinuxProcessDisk(proc.iProcessId);
                proc.network = ReadLinuxProcessNetwork(proc.iProcessId);
                auto netInfo = ReadLinuxProcessNetworkInfo(proc.iProcessId);
                proc.lstOpenPorts = netInfo.lstOpenPorts;
                proc.lstConnections = netInfo.lstConnections;
                proc.bHasEstablishedConnection = netInfo.bHasEstablishedInboundConnection;
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
                            ProcessInfo proc; PopulateLinuxProc(dname, proc);
                            if (iSessionId == -1 || proc.iSessionId == iSessionId) lstProcs.Add(proc);
                        }
                    }
                }
                ::closedir(dir);
                return lstProcs;
            }

            Collections::Generic::List<ServiceInfo> SystemMetrics::GetAllServices() {
                return Collections::Generic::List<ServiceInfo>();
            }

        }
    }
}
#endif
