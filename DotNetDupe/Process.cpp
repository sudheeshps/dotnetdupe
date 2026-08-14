#include "pch.h"
#include "System/Diagnostics/Process.h"
#include "System/UnauthorizedAccessException.h"
#include "System/IO/FileNotFoundException.h"
#include "System/InvalidOperationException.h"
#include "System/Utils/StringConvert.h"

#if defined(_WIN32)
#include <windows.h>
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#else
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <spawn.h>
#include <vector>
#include <string>
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

            bool Process::Start() {
                if (m_objStartInfo.FileName.GetLength() == 0) return false;

#if defined(_WIN32)
                PROCESS_INFORMATION pi;
                if (CreateWin32Process(m_objStartInfo, pi)) {
                    m_iId = static_cast<int>(pi.dwProcessId);
                    m_pProcessHandle = (void*)pi.hProcess;
                    ::CloseHandle(pi.hThread);
                    m_bHasExited = false;
                    return true;
                }
                return false;
#else
                pid_t pid;
                if (SpawnPosixProcess(m_objStartInfo, pid)) {
                    m_iId = static_cast<int>(pid);
                    m_pProcessHandle = (void*)(intptr_t)pid;
                    m_bHasExited = false;
                    return true;
                }
                return false;
#endif
            }

            SmartPointer<Process> Process::Start(const String& sFileName) {
                return Start(ProcessStartInfo(sFileName));
            }

            SmartPointer<Process> Process::Start(const String& sFileName, const String& sArguments) {
                return Start(ProcessStartInfo(sFileName, sArguments));
            }

            SmartPointer<Process> Process::Start(const ProcessStartInfo& objStartInfo) {
                SmartPointer<Process> pProcess = SmartPointer<Process>::New();
                pProcess->SetStartInfo(objStartInfo);
                try {
                    if (pProcess->Start()) {
                        return pProcess;
                    }
                } catch (const IO::FileNotFoundException&) {
                    return SmartPointer<Process>(nullptr);
                }
                return SmartPointer<Process>(nullptr);
            }

            void Process::WaitForExit() {
                WaitForExit(-1);
            }

            bool Process::WaitForExit(int iMilliseconds) {
                if (m_bHasExited) return true;
                if (m_pProcessHandle == nullptr) return true;

#if defined(_WIN32)
                DWORD dwTimeout = (iMilliseconds == -1) ? INFINITE : (DWORD)iMilliseconds;
                DWORD dwResult = WaitForSingleObject((HANDLE)m_pProcessHandle, dwTimeout);
                if (dwResult == WAIT_OBJECT_0) {
                    Refresh();
                    return true;
                }
                return false;
#else
                int iStatus;
                pid_t pid = (pid_t)(intptr_t)m_pProcessHandle;
                if (iMilliseconds == -1) {
                    if (waitpid(pid, &iStatus, 0) == pid) {
                        m_iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                        m_bHasExited = true;
                        return true;
                    }
                    return false;
                } else {
                    int iElapsed = 0;
                    while (iElapsed < iMilliseconds) {
                        pid_t res = waitpid(pid, &iStatus, WNOHANG);
                        if (res == pid) {
                            m_iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                            m_bHasExited = true;
                            return true;
                        } else if (res == 0) {
                            usleep(10000); // 10ms
                            iElapsed += 10;
                        } else {
                            if (errno == ECHILD) {
                                m_bHasExited = true;
                            }
                            break;
                        }
                    }
                    return m_bHasExited;
                }
#endif
            }

            bool Process::GetHasExited() const {
                Refresh();
                return m_bHasExited;
            }

            void Process::Kill() {
                if (m_bHasExited || m_pProcessHandle == nullptr) return;

#if defined(_WIN32)
                if (!::TerminateProcess((HANDLE)m_pProcessHandle, 1)) {
                    DWORD dwErr = ::GetLastError();
                    if (dwErr == ERROR_ACCESS_DENIED) {
                        throw UnauthorizedAccessException("Cannot terminate target process: access denied.");
                    }
                    if (dwErr == ERROR_INVALID_HANDLE) {
                        throw InvalidOperationException("Process has already exited.");
                    }
                }
                Refresh();
#else
                pid_t pid = (pid_t)(intptr_t)m_pProcessHandle;
                if (kill(pid, SIGKILL) != 0) {
                    if (errno == EPERM) throw UnauthorizedAccessException("Cannot terminate target process: access denied.");
                    if (errno == ESRCH) throw InvalidOperationException("Process has already exited.");
                }
                
                // Wait for the process to actually be reaped to ensure GetHasExited() 
                // returns true immediately after Kill()
                int iStatus;
                if (waitpid(pid, &iStatus, 0) == pid) {
                    m_iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                    m_bHasExited = true;
                } else if (errno == ECHILD) {
                    m_bHasExited = true;
                }
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
                if (m_bHasExited) return;
                if (m_pProcessHandle == nullptr) return;

#if defined(_WIN32)
                DWORD dwExitCode;
                if (GetExitCodeProcess((HANDLE)m_pProcessHandle, &dwExitCode)) {
                    if (dwExitCode != STILL_ACTIVE) {
                        m_iExitCode = (int)dwExitCode;
                        m_bHasExited = true;
                    }
                }
#else
                int iStatus;
                pid_t pid = (pid_t)(intptr_t)m_pProcessHandle;
                pid_t res = waitpid(pid, &iStatus, WNOHANG);
                if (res == pid) {
                    m_iExitCode = WIFEXITED(iStatus) ? WEXITSTATUS(iStatus) : (WIFSIGNALED(iStatus) ? -WTERMSIG(iStatus) : 0);
                    m_bHasExited = true;
                } else if (res == -1 && errno == ECHILD) {
                    m_bHasExited = true;
                }
#endif
            }
        }
    }
}
