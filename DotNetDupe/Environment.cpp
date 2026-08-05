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
#include <fstream>
#include <iostream>

extern char** environ;
#endif

namespace DotNetDupe {
    namespace System {
        String Environment::GetMachineName() {
#if defined(_WIN32)
            wchar_t buffer [MAX_COMPUTERNAME_LENGTH + 1];
            DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1;
            GetComputerNameW(buffer, &nSize);
            return String(WCharToUtf8(buffer).c_str());
#else
            char buffer[HOST_NAME_MAX + 1];
            if (gethostname(buffer, sizeof(buffer)) == 0) {
                return buffer;
            }
            return "";
#endif
        }

        String Environment::GetUserName() {
#if defined(_WIN32)
            wchar_t buffer [256];
            DWORD nSize = 256;
            ::GetUserNameW(buffer, &nSize);
            return String(buffer);
#else
            struct passwd *pPw = getpwuid(getuid());
            if (pPw) {
                return pPw->pw_name;
            }
            const char* pUser = getenv("USER");
            return pUser ? pUser : "";
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
            return "\r\n";
#else
            return "\n";
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
            return String("/usr/lib"); // Better POSIX default
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

        int64_t Environment::GetWorkingSet() {
#if defined(_WIN32)
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
                return (int64_t)pmc.WorkingSetSize;
            }
#else
            std::ifstream statusFile("/proc/self/status");
            std::string sLine;
            while (std::getline(statusFile, sLine)) {
                if (sLine.compare(0, 6, "VmRSS:") == 0) {
                    size_t nStart = sLine.find_first_of("0123456789");
                    size_t nEnd = sLine.find_first_not_of("0123456789", nStart);
                    if (nStart != std::string::npos) {
                        return std::stoll(sLine.substr(nStart, nEnd - nStart)) * 1024;
                    }
                }
            }
#endif
            return 0;
        }

        void Environment::Exit(int iExitCode) {
            ::exit(iExitCode);
        }

        String Environment::ExpandEnvironmentVariables(const String& sName) {
#if defined(_WIN32)
            std::wstring sWName = Utf8ToWChar(sName.GetRawString());
            DWORD nSize = ::ExpandEnvironmentStringsW(sWName.c_str(), NULL, 0);
            if (nSize == 0) return sName;
            std::vector<wchar_t> buffer(nSize);
            ::ExpandEnvironmentStringsW(sWName.c_str(), buffer.data(), nSize);
            return String(WCharToUtf8(buffer.data()).c_str());
#else
            // Simple implementation: check if name is exactly a variable
            if (sName.StartsWith("%", false) && sName.EndsWith("%", false)) {
                const char* pVal = getenv(sName.Substring(1, sName.GetLength() - 2).GetRawString());
                return pVal ? String(pVal) : sName;
            }
            return sName;
#endif
        }

        Array<String> Environment::GetCommandLineArgs() {
            std::vector<String> tempArgs;
#if defined(_WIN32)
            int nArgs;
            LPWSTR* pSzArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
            if (pSzArglist != NULL) {
                for (int iIndex = 0; iIndex < nArgs; iIndex++) {
                    tempArgs.push_back(String(WCharToUtf8(pSzArglist[iIndex]).c_str()));
                }
                LocalFree(pSzArglist);
            }
#else
            std::ifstream cmdline("/proc/self/cmdline", std::ios::binary);
            std::string sArg;
            while (std::getline(cmdline, sArg, '\0')) {
                tempArgs.push_back(String(sArg.c_str()));
            }
#endif
            Array<String> result((int)tempArgs.size());
            for (int iIndex = 0; iIndex < (int)tempArgs.size(); iIndex++) result[iIndex] = tempArgs[iIndex];
            return result;
        }

        String Environment::GetEnvironmentVariable(const String& sVariable) {
#if defined(_WIN32)
            std::wstring sWVar = Utf8ToWChar(sVariable.GetRawString());
            DWORD nSize = ::GetEnvironmentVariableW(sWVar.c_str(), NULL, 0);
            if (nSize == 0) return String("");
            std::vector<wchar_t> buffer(nSize);
            ::GetEnvironmentVariableW(sWVar.c_str(), buffer.data(), nSize);
            return String(WCharToUtf8(buffer.data()).c_str());
#else
            const char* pVal = getenv(sVariable.GetRawString());
            return pVal ? String(pVal) : String("");
#endif
        }

        Collections::Generic::Dictionary<String, String> Environment::GetEnvironmentVariables() {
            Collections::Generic::Dictionary<String, String> result;
#if defined(_WIN32)
            wchar_t* pLpvEnv = GetEnvironmentStringsW();
            for (wchar_t* pLpszVariable = pLpvEnv; *pLpszVariable; ) {
                std::string sLineUtf8 = WCharToUtf8(pLpszVariable);
                String sLine(sLineUtf8.c_str());
                int iEqIdx = sLine.IndexOf("=");
                if (iEqIdx != -1) {
                    result.Add(sLine.Substring(0, iEqIdx), sLine.Substring(iEqIdx + 1, sLine.GetLength() - iEqIdx - 1));
                }
                pLpszVariable += wcslen(pLpszVariable) + 1;
            }
            FreeEnvironmentStringsW(pLpvEnv);
#else
            for (char** ppEnv = environ; *ppEnv; ++ppEnv) {
                std::string sLine(*ppEnv);
                size_t nEqPos = sLine.find('=');
                if (nEqPos != std::string::npos) {
                    result.Add(String(sLine.substr(0, nEqPos).c_str()), String(sLine.substr(nEqPos + 1).c_str()));
                }
            }
#endif
            return result;
        }

        String Environment::GetFolderPath(SpecialFolder eFolder) {
#if defined(_WIN32)
            int iCsidl = 0;
            switch (eFolder) {
            case SpecialFolder::ApplicationData: iCsidl = CSIDL_APPDATA; break;
            case SpecialFolder::CommonApplicationData: iCsidl = CSIDL_COMMON_APPDATA; break;
            case SpecialFolder::CommonProgramFiles: iCsidl = CSIDL_PROGRAM_FILES_COMMON; break;
            case SpecialFolder::Cookies: iCsidl = CSIDL_COOKIES; break;
            case SpecialFolder::Desktop: iCsidl = CSIDL_DESKTOP; break;
            case SpecialFolder::Favorites: iCsidl = CSIDL_FAVORITES; break;
            case SpecialFolder::History: iCsidl = CSIDL_HISTORY; break;
            case SpecialFolder::InternetCache: iCsidl = CSIDL_INTERNET_CACHE; break;
            case SpecialFolder::LocalApplicationData: iCsidl = CSIDL_LOCAL_APPDATA; break;
            case SpecialFolder::MyComputer: iCsidl = CSIDL_DRIVES; break;
            case SpecialFolder::MyDocuments: iCsidl = CSIDL_MYDOCUMENTS; break;
            case SpecialFolder::MyMusic: iCsidl = CSIDL_MYMUSIC; break;
            case SpecialFolder::MyPictures: iCsidl = CSIDL_MYPICTURES; break;
            case SpecialFolder::MyVideos: iCsidl = CSIDL_MYVIDEO; break;
            case SpecialFolder::ProgramFiles: iCsidl = CSIDL_PROGRAM_FILES; break;
            case SpecialFolder::Programs: iCsidl = CSIDL_PROGRAMS; break;
            case SpecialFolder::Recent: iCsidl = CSIDL_RECENT; break;
            case SpecialFolder::SendTo: iCsidl = CSIDL_SENDTO; break;
            case SpecialFolder::StartMenu: iCsidl = CSIDL_STARTMENU; break;
            case SpecialFolder::Startup: iCsidl = CSIDL_STARTUP; break;
            case SpecialFolder::System: iCsidl = CSIDL_SYSTEM; break;
            case SpecialFolder::Templates: iCsidl = CSIDL_TEMPLATES; break;
            case SpecialFolder::UserProfile: iCsidl = CSIDL_PROFILE; break;
            }

            wchar_t buffer [MAX_PATH];
            if (SHGetFolderPathW(NULL, iCsidl, NULL, 0, buffer) == S_OK) {
                return String(WCharToUtf8(buffer).c_str());
            }
#else
            // Simple POSIX mapping
            switch (eFolder) {
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

        Array<String> Environment::GetLogicalDrives() {
            std::vector<String> tempDrives;
#if defined(_WIN32)
            DWORD nDrives = ::GetLogicalDrives();
            for (int iIndex = 0; iIndex < 26; iIndex++) {
                if ((nDrives >> iIndex) & 1) {
                    char chDriveName [4] = { (char)('A' + iIndex), ':', '\\', 0 };
                    tempDrives.push_back(String(chDriveName));
                }
            }
#else
            tempDrives.push_back(String("/"));
#endif
            Array<String> result((int)tempDrives.size());
            for (int iIndex = 0; iIndex < (int)tempDrives.size(); iIndex++) result[iIndex] = tempDrives[iIndex];
            return result;
        }

        void Environment::SetEnvironmentVariable(const String& sVariable, const String& sValue) {
#if defined(_WIN32)
            std::wstring sWVar = Utf8ToWChar(sVariable.GetRawString());
            std::wstring sWVal = Utf8ToWChar(sValue.GetRawString());
            ::SetEnvironmentVariableW(sWVar.c_str(), sWVal.c_str());
#else
            setenv(sVariable.GetRawString(), sValue.GetRawString(), 1);
#endif
        }

        OperatingSystem Environment::GetOperatingSystem() {
#if defined(_WIN32)
            OSVERSIONINFOEXW info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEXW));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
            GetVersionExW((LPOSVERSIONINFOW)&info);

            Version version(info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber, 0);
            PlatformID ePlatform = PlatformID::Win32NT;
            return OperatingSystem(ePlatform, version);
#else
            struct utsname name;
            uname(&name);
            Version version(0, 0, 0, 0); // Need better parsing
            PlatformID ePlatform = PlatformID::Unix;
            return OperatingSystem(ePlatform, version);
#endif
        }
    }
}
