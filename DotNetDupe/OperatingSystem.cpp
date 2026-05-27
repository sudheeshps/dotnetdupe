#include "pch.h"
#include "System/OperatingSystem.h"

#if defined(_WIN32)
#include <windows.h>
#include "Win32Internal.h"
typedef void (WINAPI* RtlGetVersion_FUNC) (OSVERSIONINFOEXW*);
using namespace DotNetDupe::System::Internal;
#else
#include <sys/utsname.h>
#endif

namespace DotNetDupe {
    namespace System {
        OperatingSystem::OperatingSystem(PlatformID platform, const Version& version)
            : _platform(platform), _version(version) { }

        PlatformID OperatingSystem::GetPlatform() const {
            return _platform;
        }

        const Version& OperatingSystem::GetVersion() const {
            return _version;
        }

        String OperatingSystem::GetServicePack() const {
#if defined(_WIN32)
            OSVERSIONINFOEXW info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEXW));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

            HMODULE hModule = GetModuleHandleW(L"ntdll.dll");
            if (hModule) {
                RtlGetVersion_FUNC rtlGetVersion = (RtlGetVersion_FUNC)GetProcAddress(hModule, "RtlGetVersion");
                if (rtlGetVersion) {
                    rtlGetVersion(&info);
                }
            }

            return String(WCharToUtf8(info.szCSDVersion).c_str());
#else
            return String("");
#endif
        }

        String OperatingSystem::GetVersionString() const {
            return _version.ToString();
        }

        bool OperatingSystem::IsWindows() {
#if defined(_WIN32)
            return true;
#else
            return false;
#endif
        }

        bool OperatingSystem::IsLinux() {
#if defined(__linux__)
            return true;
#else
            return false;
#endif
        }

        bool OperatingSystem::IsMacOS() {
#if defined(__APPLE__)
            return true;
#else
            return false;
#endif
        }

    }
}
