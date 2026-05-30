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
        OperatingSystem::OperatingSystem(PlatformID ePlatform, const Version& vVersion)
            : m_ePlatform(ePlatform), m_vVersion(vVersion) { }

        PlatformID OperatingSystem::GetPlatform() const {
            return m_ePlatform;
        }

        const Version& OperatingSystem::GetVersion() const {
            return m_vVersion;
        }

        String OperatingSystem::GetServicePack() const {
#if defined(_WIN32)
            OSVERSIONINFOEXW info;
            ZeroMemory(&info, sizeof(OSVERSIONINFOEXW));
            info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

            HMODULE pModule = GetModuleHandleW(L"ntdll.dll");
            if (pModule) {
                RtlGetVersion_FUNC pRtlGetVersion = (RtlGetVersion_FUNC)GetProcAddress(pModule, "RtlGetVersion");
                if (pRtlGetVersion) {
                    pRtlGetVersion(&info);
                }
            }

            return String(WCharToUtf8(info.szCSDVersion).c_str());
#else
            return String("");
#endif
        }

        String OperatingSystem::GetVersionString() const {
            return m_vVersion.ToString();
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

    }
}
