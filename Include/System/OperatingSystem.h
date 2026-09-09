#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Version.h"

namespace DotNetDupe
{
    namespace System
    {
        /// \enum PlatformID
        /// \brief Identifies the operating system, or platform, supported by an assembly.
        enum class PlatformID
        {
            Win32S = 0,       ///< Win32s operating system.
            Win32Windows = 1, ///< Windows 95, Windows 98, or Windows Me.
            Win32NT = 2,      ///< Windows NT, 2000, XP, Vista, 7, 8, 10, 11, or Windows Server.
            WinCE = 3,        ///< Windows CE.
            Unix = 4,         ///< Unix or Linux platform.
            Xbox = 5          ///< Xbox platform.
        };

        /// \class OperatingSystem
        /// \brief Represents information about an operating system, such as the version and platform identifier.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.18 (System.OperatingSystem).
        class OperatingSystem : public Object
        {
        public:
            /// \brief Initializes a new instance of the OperatingSystem class, using the specified platform identifier and version object.
            /// \param platform One of the PlatformID values that indicates the operating system platform.
            /// \param version A Version object that indicates the version of the operating system.
            DOTNETDUPE_API OperatingSystem(PlatformID platform, const Version& version);

            /// \brief Gets a PlatformID value that identifies the operating system platform.
            /// \return One of the PlatformID values.
            DOTNETDUPE_API PlatformID GetPlatform() const;

            /// \brief Gets a Version object that identifies the operating system.
            /// \return A Version object that describes the operating system version.
            DOTNETDUPE_API const Version& GetVersion() const;

            /// \brief Gets the concatenated string representation of the platform identifier and version.
            /// \return The string representation of the values returned by GetPlatform and GetVersion.
            DOTNETDUPE_API String GetVersionString() const;

            /// \brief Gets the service pack version represented by this OperatingSystem object.
            /// \return The service pack version, if service packs are supported; otherwise, an empty string.
            DOTNETDUPE_API String GetServicePack() const;

            /// \brief Indicates whether the current application is running on Windows.
            /// \return true if the current application is running on Windows; otherwise, false.
            DOTNETDUPE_API static bool IsWindows();

            /// \brief Indicates whether the current application is running on Linux.
            /// \return true if the current application is running on Linux; otherwise, false.
            DOTNETDUPE_API static bool IsLinux();

        private:
            PlatformID m_ePlatform;
            Version m_vVersion;
        };
    }
}
