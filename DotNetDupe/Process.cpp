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
                SmartPointer<Process> pProcess = SmartPointer<Process>::New();
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
        }
    }
}
