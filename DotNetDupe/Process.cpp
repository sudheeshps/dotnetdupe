#include "pch.h"
#include "System/Diagnostics/Process.h"
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
#include <errno.h>
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

            bool Process::Start() {
                if (m_objStartInfo.FileName.GetLength() == 0) return false;

#if defined(_WIN32)
                STARTUPINFOW si;
                PROCESS_INFORMATION pi;
                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));

                std::wstring sWFileName = DotNetDupe::System::Internal::StringConvertInternal::Utf8ToWChar(m_objStartInfo.FileName.GetRawString());
                std::wstring sWArgs = DotNetDupe::System::Internal::StringConvertInternal::Utf8ToWChar(m_objStartInfo.Arguments.GetRawString());
                std::wstring sWCommandLine = L"\"" + sWFileName + L"\"";
                if (m_objStartInfo.Arguments.GetLength() > 0) {
                    sWCommandLine += L" " + sWArgs;
                }

                DWORD dwCreationFlags = 0;
                if (m_objStartInfo.CreateNoWindow) {
                    dwCreationFlags |= CREATE_NO_WINDOW;
                }

                if (CreateProcessW(NULL, (LPWSTR)sWCommandLine.c_str(), NULL, NULL, FALSE, 
                    dwCreationFlags, NULL, NULL, &si, &pi)) {
                    m_iId = (int)pi.dwProcessId;
                    m_pProcessHandle = (void*)pi.hProcess;
                    CloseHandle(pi.hThread);
                    m_bHasExited = false;
                    return true;
                }
                return false;
#else
                pid_t pid;
                std::vector<std::string> argStrings;
                std::string sFileName = m_objStartInfo.FileName.GetRawString();
                argStrings.push_back(sFileName);

                std::string sArgs = m_objStartInfo.Arguments.GetRawString();
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
                if (!sCurrentArg.empty()) {
                    argStrings.push_back(sCurrentArg);
                }

                std::vector<char*> argv;
                for (auto& s : argStrings) {
                    argv.push_back((char*)s.c_str());
                }
                argv.push_back(NULL);

                if (posix_spawn(&pid, sFileName.c_str(), NULL, NULL, argv.data(), environ) == 0) {
                    m_iId = (int)pid;
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
                if (pProcess->Start()) {
                    return pProcess;
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

            bool Process::GetHasExited() {
                Refresh();
                return m_bHasExited;
            }

            void Process::Kill() {
                if (m_bHasExited) return;
                if (m_pProcessHandle == nullptr) return;

#if defined(_WIN32)
                TerminateProcess((HANDLE)m_pProcessHandle, 1);
                Refresh();
#else
                pid_t pid = (pid_t)(intptr_t)m_pProcessHandle;
                kill(pid, SIGKILL);
                
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

            void Process::Refresh() {
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
