#pragma once

#include "Common.h"
#include "Object.h"
#include "String.h"
#include "Version.h"
#include <Windows.h>

namespace DotNetDupe
{
    namespace System
    {
        enum class PlatformID
        {
            Win32S = 0,
            Win32Windows = 1,
            Win32NT = 2,
            WinCE = 3,
            Unix = 4,
            Xbox = 5,
            MacOSX = 6
        };

        class OperatingSystem : public Object
        {
        public:
            DOTNETDUPE_API OperatingSystem(PlatformID platform, const Version& version);

            DOTNETDUPE_API PlatformID GetPlatform() const;
            DOTNETDUPE_API const Version& GetVersion() const;
            DOTNETDUPE_API String GetVersionString() const;
            DOTNETDUPE_API String GetServicePack() const;

            DOTNETDUPE_API static bool IsWindows();
            DOTNETDUPE_API static bool IsLinux();
            DOTNETDUPE_API static bool IsMacOS();

        private:
            PlatformID _platform;
            Version _version;
        };
    }
}
