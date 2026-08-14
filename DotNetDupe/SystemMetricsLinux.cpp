#include "pch.h"
#include "System/Diagnostics/SystemMetrics.h"
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
                    if (++lineCount <= 2) continue;
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
                    if (lineNum++ == 0) continue;
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

        }
    }
}
#endif
