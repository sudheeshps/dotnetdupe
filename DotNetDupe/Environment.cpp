#include "pch.h"
#include "System/Environment.h"
#include <vector>
#include <map>
#include <Psapi.h>
#include <shellapi.h>
#include <shlobj.h>

#pragma comment(lib, "psapi.lib")
#pragma warning(disable : 4996)

namespace DotNetDupe {
    namespace System {
        String Environment::GetMachineName() {
            TCHAR buffer [MAX_COMPUTERNAME_LENGTH + 1];
            DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
            GetComputerName(buffer, &size);
            return String(buffer);
        }

        String Environment::GetUserName() {
            TCHAR buffer [256];
            DWORD size = 256;
            ::GetUserName(buffer, &size);
            return String(buffer);
        }

        int Environment::GetProcessorCount() {
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return sysInfo.dwNumberOfProcessors;
        }

        String Environment::GetNewLine() {
            return String(_T("\r\n"));
        }

        String Environment::GetCurrentDirectory() {
            TCHAR buffer [MAX_PATH];
            ::GetCurrentDirectory(MAX_PATH, buffer);
            return String(buffer);
        }

        String Environment::GetSystemDirectory() {
            TCHAR buffer [MAX_PATH];
            ::GetSystemDirectory(buffer, MAX_PATH);
            return String(buffer);
        }

        String Environment::GetOSVersion() {
            OSVERSIONINFOEX info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
            GetVersionEx((LPOSVERSIONINFO)&info);

            std::wstringstream ss;
            ss << info.dwMajorVersion << L"." << info.dwMinorVersion << L"." << info.dwBuildNumber;
            return String(ss.str().c_str());
        }

        String Environment::GetUserDomainName() {
            TCHAR buffer [256];
            DWORD size = 256;
            ::GetUserName(buffer, &size);
            return String(buffer);
        }

        String Environment::GetVersion() {
            return String(_T("4.8.0"));
        }

        int64_t Environment::GetWorkingSet() {
            PROCESS_MEMORY_COUNTERS pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
            return pmc.WorkingSetSize;
        }

        void Environment::Exit(int exitCode) {
            ::ExitProcess(exitCode);
        }

        String Environment::ExpandEnvironmentVariables(const String& name) {
            TCHAR buffer [1024];
            ::ExpandEnvironmentStrings(name.GetRawString(), buffer, 1024);
            return String(buffer);
        }

        std::vector<String> Environment::GetCommandLineArgs() {
            int nArgs;
            LPWSTR* szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
            if (szArglist == NULL) {
                return std::vector<String>();
            }

            std::vector<String> result;
            for (int i = 0; i < nArgs; i++) {
                result.push_back(String(szArglist [i]));
            }
            LocalFree(szArglist);
            return result;
        }

        String Environment::GetEnvironmentVariable(const String& variable) {
            TCHAR buffer [1024];
            ::GetEnvironmentVariable(variable.GetRawString(), buffer, 1024);
            return String(buffer);
        }

        std::map<String, String> Environment::GetEnvironmentVariables() {
            std::map<String, String> result;
            LPTCH lpszVariable;
            LPTSTR lpvEnv;

            lpvEnv = GetEnvironmentStrings();

            for (lpszVariable = lpvEnv; *lpszVariable; lpszVariable++) {
                String line(lpszVariable);
                auto parts = line.Split(_T('='));
                if (parts.size() == 2) {
                    result [parts [0]] = parts [1];
                }
                lpszVariable += lstrlen(lpszVariable);
            }
            FreeEnvironmentStrings(lpvEnv);

            return result;
        }

        String Environment::GetFolderPath(SpecialFolder folder) {
            int csidl = 0;
            switch (folder) {
            case SpecialFolder::ApplicationData:
                csidl = CSIDL_APPDATA;
                break;
            case SpecialFolder::CommonApplicationData:
                csidl = CSIDL_COMMON_APPDATA;
                break;
            case SpecialFolder::CommonProgramFiles:
                csidl = CSIDL_PROGRAM_FILES_COMMON;
                break;
            case SpecialFolder::Cookies:
                csidl = CSIDL_COOKIES;
                break;
            case SpecialFolder::Desktop:
                csidl = CSIDL_DESKTOP;
                break;
            case SpecialFolder::Favorites:
                csidl = CSIDL_FAVORITES;
                break;
            case SpecialFolder::History:
                csidl = CSIDL_HISTORY;
                break;
            case SpecialFolder::InternetCache:
                csidl = CSIDL_INTERNET_CACHE;
                break;
            case SpecialFolder::LocalApplicationData:
                csidl = CSIDL_LOCAL_APPDATA;
                break;
            case SpecialFolder::MyComputer:
                csidl = CSIDL_DRIVES;
                break;
            case SpecialFolder::MyDocuments:
                csidl = CSIDL_MYDOCUMENTS;
                break;
            case SpecialFolder::MyMusic:
                csidl = CSIDL_MYMUSIC;
                break;
            case SpecialFolder::MyPictures:
                csidl = CSIDL_MYPICTURES;
                break;
            case SpecialFolder::MyVideos:
                csidl = CSIDL_MYVIDEO;
                break;
            case SpecialFolder::ProgramFiles:
                csidl = CSIDL_PROGRAM_FILES;
                break;
            case SpecialFolder::Programs:
                csidl = CSIDL_PROGRAMS;
                break;
            case SpecialFolder::Recent:
                csidl = CSIDL_RECENT;
                break;
            case SpecialFolder::SendTo:
                csidl = CSIDL_SENDTO;
                break;
            case SpecialFolder::StartMenu:
                csidl = CSIDL_STARTMENU;
                break;
            case SpecialFolder::Startup:
                csidl = CSIDL_STARTUP;
                break;
            case SpecialFolder::System:
                csidl = CSIDL_SYSTEM;
                break;
            case SpecialFolder::Templates:
                csidl = CSIDL_TEMPLATES;
                break;
            case SpecialFolder::UserProfile:
                csidl = CSIDL_PROFILE;
                break;
            }

            TCHAR buffer [MAX_PATH];
            SHGetFolderPath(NULL, csidl, NULL, 0, buffer);
            return String(buffer);
        }

        std::vector<String> Environment::GetLogicalDrives() {
            std::vector<String> result;
            DWORD drives = ::GetLogicalDrives();
            for (int i = 0; i < 26; i++) {
                if ((drives >> i) & 1) {
                    TCHAR driveName [4] = { (TCHAR)('A' + i), ':', '\\', 0 };
                    result.push_back(String(driveName));
                }
            }
            return result;
        }

        void Environment::SetEnvironmentVariable(const String& variable, const String& value) {
            ::SetEnvironmentVariable(variable.GetRawString(), value.GetRawString());
        }

        OperatingSystem Environment::GetOperatingSystem() {
            OSVERSIONINFOEX info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
            GetVersionEx((LPOSVERSIONINFO)&info);

            Version version(info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber, 0);

            PlatformID platform = PlatformID::Win32NT;

            return OperatingSystem(platform, version);
        }
    }
}
