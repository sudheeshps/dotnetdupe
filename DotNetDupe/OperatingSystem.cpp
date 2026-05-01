#include "pch.h"
#include "System/OperatingSystem.h"

typedef void (WINAPI* RtlGetVersion_FUNC) (OSVERSIONINFOEXW*);

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
            OSVERSIONINFOEXW info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEXW));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

            HMODULE hModule = GetModuleHandle(TEXT("ntdll.dll"));
            if (hModule) {
                RtlGetVersion_FUNC rtlGetVersion = (RtlGetVersion_FUNC)GetProcAddress(hModule, "RtlGetVersion");
                if (rtlGetVersion) {
                    rtlGetVersion(&info);
                }
            }

            return String(info.szCSDVersion);
        }

        String OperatingSystem::GetVersionString() const {
            return _version.ToString();
        }

        bool OperatingSystem::IsWindows() {
            return true;
        }

        bool OperatingSystem::IsLinux() {
            return false;
        }

        bool OperatingSystem::IsMacOS() {
            return false;
        }

    }
}
