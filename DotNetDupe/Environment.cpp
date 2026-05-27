#include "pch.h"
#include "System/Environment.h"
#include <vector>
#include <map>
#include <sstream>

#if defined(_WIN32)
#include <windows.h>
#include <Psapi.h>
#include <shellapi.h>
#include <shlobj.h>
#include "Win32Internal.h"
#pragma comment(lib, "psapi.lib")
#pragma warning(disable : 4996)

using namespace DotNetDupe::System::Internal;
#else
#include <unistd.h>
#include <pwd.h>
#include <limits.h>
#include <sys/utsname.h>
#endif

namespace DotNetDupe {
    namespace System {
        String Environment::GetMachineName() {
#if defined(_WIN32)
            wchar_t buffer [MAX_COMPUTERNAME_LENGTH + 1];
            DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
            GetComputerNameW(buffer, &size);
            return String(WCharToUtf8(buffer).c_str());
#else
            char buffer[HOST_NAME_MAX + 1];
            if (gethostname(buffer, sizeof(buffer)) == 0) {
                return String(buffer);
            }
            return String("");
#endif
        }

        String Environment::GetUserName() {
#if defined(_WIN32)
            wchar_t buffer [256];
            DWORD size = 256;
            ::GetUserNameW(buffer, &size);
            return String(WCharToUtf8(buffer).c_str());
#else
            struct passwd *pw = getpwuid(getuid());
            if (pw) {
                return String(pw->pw_name);
            }
            const char* user = getenv("USER");
            return user ? String(user) : String("");
#endif
        }

        int Environment::GetProcessorCount() {
#if defined(_WIN32)
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return (int)sysInfo.dwNumberOfProcessors;
#else
            return (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
        }

        String Environment::GetNewLine() {
#if defined(_WIN32)
            return String("\r\n");
#else
            return String("\n");
#endif
        }

        String Environment::GetCurrentDirectory() {
#if defined(_WIN32)
            wchar_t buffer [MAX_PATH];
            ::GetCurrentDirectoryW(MAX_PATH, buffer);
            return String(WCharToUtf8(buffer).c_str());
#else
            char buffer[PATH_MAX];
            if (getcwd(buffer, sizeof(buffer))) {
                return String(buffer);
            }
            return String("");
#endif
        }

        String Environment::GetSystemDirectory() {
#if defined(_WIN32)
            wchar_t buffer [MAX_PATH];
            ::GetSystemDirectoryW(buffer, MAX_PATH);
            return String(WCharToUtf8(buffer).c_str());
#else
            return String("/system"); // Or appropriate POSIX default
#endif
        }

        String Environment::GetOSVersion() {
#if defined(_WIN32)
            OSVERSIONINFOEXW info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEXW));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
            GetVersionExW((LPOSVERSIONINFOW)&info);

            std::stringstream ss;
            ss << info.dwMajorVersion << "." << info.dwMinorVersion << "." << info.dwBuildNumber;
            return String(ss.str().c_str());
#else
            struct utsname name;
            if (uname(&name) == 0) {
                return String(name.release);
            }
            return String("Unknown");
#endif
        }

        String Environment::GetUserDomainName() {
            return GetMachineName();
        }

        String Environment::GetVersion() {
            return String("4.8.0");
        }

        int64_t Environment::GetWorkingSet() {
#if defined(_WIN32)
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
                return (int64_t)pmc.WorkingSetSize;
            }
#endif
            return 0;
        }

        void Environment::Exit(int exitCode) {
            ::exit(exitCode);
        }

        String Environment::ExpandEnvironmentVariables(const String& name) {
#if defined(_WIN32)
            std::wstring wname = Utf8ToWChar(name.GetRawString());
            DWORD size = ::ExpandEnvironmentStringsW(wname.c_str(), NULL, 0);
            if (size == 0) return name;
            std::vector<wchar_t> buffer(size);
            ::ExpandEnvironmentStringsW(wname.c_str(), buffer.data(), size);
            return String(WCharToUtf8(buffer.data()).c_str());
#else
            // Simple implementation: check if name is exactly a variable
            if (name.StartsWith("%", false) && name.EndsWith("%", false)) {
                const char* val = getenv(name.Substring(1, name.GetLength() - 2).GetRawString());
                return val ? String(val) : name;
            }
            return name;
#endif
        }

        std::vector<String> Environment::GetCommandLineArgs() {
            std::vector<String> result;
#if defined(_WIN32)
            int nArgs;
            LPWSTR* szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
            if (szArglist != NULL) {
                for (int i = 0; i < nArgs; i++) {
                    result.push_back(String(WCharToUtf8(szArglist[i]).c_str()));
                }
                LocalFree(szArglist);
            }
#endif
            return result;
        }

        String Environment::GetEnvironmentVariable(const String& variable) {
#if defined(_WIN32)
            std::wstring wvar = Utf8ToWChar(variable.GetRawString());
            DWORD size = ::GetEnvironmentVariableW(wvar.c_str(), NULL, 0);
            if (size == 0) return String("");
            std::vector<wchar_t> buffer(size);
            ::GetEnvironmentVariableW(wvar.c_str(), buffer.data(), size);
            return String(WCharToUtf8(buffer.data()).c_str());
#else
            const char* val = getenv(variable.GetRawString());
            return val ? String(val) : String("");
#endif
        }

        std::map<String, String> Environment::GetEnvironmentVariables() {
            std::map<String, String> result;
#if defined(_WIN32)
            wchar_t* lpvEnv = GetEnvironmentStringsW();
            for (wchar_t* lpszVariable = lpvEnv; *lpszVariable; ) {
                std::string lineUtf8 = WCharToUtf8(lpszVariable);
                String line(lineUtf8.c_str());
                int eqIdx = line.IndexOf("=");
                if (eqIdx != -1) {
                    result[line.Substring(0, eqIdx)] = line.Substring(eqIdx + 1, line.GetLength() - eqIdx - 1);
                }
                lpszVariable += wcslen(lpszVariable) + 1;
            }
            FreeEnvironmentStringsW(lpvEnv);
#endif
            return result;
        }

        String Environment::GetFolderPath(SpecialFolder folder) {
#if defined(_WIN32)
            int csidl = 0;
            switch (folder) {
            case SpecialFolder::ApplicationData: csidl = CSIDL_APPDATA; break;
            case SpecialFolder::CommonApplicationData: csidl = CSIDL_COMMON_APPDATA; break;
            case SpecialFolder::CommonProgramFiles: csidl = CSIDL_PROGRAM_FILES_COMMON; break;
            case SpecialFolder::Cookies: csidl = CSIDL_COOKIES; break;
            case SpecialFolder::Desktop: csidl = CSIDL_DESKTOP; break;
            case SpecialFolder::Favorites: csidl = CSIDL_FAVORITES; break;
            case SpecialFolder::History: csidl = CSIDL_HISTORY; break;
            case SpecialFolder::InternetCache: csidl = CSIDL_INTERNET_CACHE; break;
            case SpecialFolder::LocalApplicationData: csidl = CSIDL_LOCAL_APPDATA; break;
            case SpecialFolder::MyComputer: csidl = CSIDL_DRIVES; break;
            case SpecialFolder::MyDocuments: csidl = CSIDL_MYDOCUMENTS; break;
            case SpecialFolder::MyMusic: csidl = CSIDL_MYMUSIC; break;
            case SpecialFolder::MyPictures: csidl = CSIDL_MYPICTURES; break;
            case SpecialFolder::MyVideos: csidl = CSIDL_MYVIDEO; break;
            case SpecialFolder::ProgramFiles: csidl = CSIDL_PROGRAM_FILES; break;
            case SpecialFolder::Programs: csidl = CSIDL_PROGRAMS; break;
            case SpecialFolder::Recent: csidl = CSIDL_RECENT; break;
            case SpecialFolder::SendTo: csidl = CSIDL_SENDTO; break;
            case SpecialFolder::StartMenu: csidl = CSIDL_STARTMENU; break;
            case SpecialFolder::Startup: csidl = CSIDL_STARTUP; break;
            case SpecialFolder::System: csidl = CSIDL_SYSTEM; break;
            case SpecialFolder::Templates: csidl = CSIDL_TEMPLATES; break;
            case SpecialFolder::UserProfile: csidl = CSIDL_PROFILE; break;
            }

            wchar_t buffer [MAX_PATH];
            if (SHGetFolderPathW(NULL, csidl, NULL, 0, buffer) == S_OK) {
                return String(WCharToUtf8(buffer).c_str());
            }
#else
            // Simple POSIX mapping
            switch (folder) {
            case SpecialFolder::UserProfile:
            case SpecialFolder::MyDocuments:
                return String(getenv("HOME"));
            case SpecialFolder::LocalApplicationData:
                return String(getenv("HOME")) + "/.local/share";
            default:
                break;
            }
#endif
            return String("");
        }

        std::vector<String> Environment::GetLogicalDrives() {
            std::vector<String> result;
#if defined(_WIN32)
            DWORD drives = ::GetLogicalDrives();
            for (int i = 0; i < 26; i++) {
                if ((drives >> i) & 1) {
                    char driveName [4] = { (char)('A' + i), ':', '\\', 0 };
                    result.push_back(String(driveName));
                }
            }
#else
            result.push_back(String("/"));
#endif
            return result;
        }

        void Environment::SetEnvironmentVariable(const String& variable, const String& value) {
#if defined(_WIN32)
            std::wstring wvar = Utf8ToWChar(variable.GetRawString());
            std::wstring wval = Utf8ToWChar(value.GetRawString());
            ::SetEnvironmentVariableW(wvar.c_str(), wval.c_str());
#else
            setenv(variable.GetRawString(), value.GetRawString(), 1);
#endif
        }

        OperatingSystem Environment::GetOperatingSystem() {
#if defined(_WIN32)
            OSVERSIONINFOEXW info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEXW));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
            GetVersionExW((LPOSVERSIONINFOW)&info);

            Version version(info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber, 0);
            PlatformID platform = PlatformID::Win32NT;
            return OperatingSystem(platform, version);
#else
            struct utsname name;
            uname(&name);
            Version version(0, 0, 0, 0); // Need better parsing
            PlatformID platform = PlatformID::Unix;
            return OperatingSystem(platform, version);
#endif
        }
    }
}
