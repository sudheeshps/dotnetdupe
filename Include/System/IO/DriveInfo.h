/// \file DriveInfo.h
/// \brief Provides access to information on a drive.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \enum DriveType
            /// \brief Defines constants for drive types.
            enum class DriveType {
                Unknown = 0,
                NoRootDirectory = 1,
                Removable = 2,
                Fixed = 3,
                Network = 4,
                CDRom = 5,
                Ram = 6
            };

            /// \class DriveInfo
            /// \brief Provides access to information on a drive.
            ///
            /// Conforms to ECMA-335 Partition IV Section 5.49 (System.IO.DriveInfo).
            class DriveInfo : public Object {
            public:
                /// \brief Initializes a new instance of the DriveInfo class with the specified drive name.
                /// \param sDriveName A valid drive path or drive letter.
                DOTNETDUPE_API DriveInfo();

                /// \brief Initializes a new instance of the DriveInfo class with the specified drive name.
                DOTNETDUPE_API explicit DriveInfo(const String& sDriveName);

                /// \brief Destructor for DriveInfo.
                DOTNETDUPE_API virtual ~DriveInfo() = default;

                /// \brief Gets the name of a drive, such as "C:\\".
                DOTNETDUPE_API String GetName() const;

                /// \brief Gets the drive type, such as Fixed, CDRom, Removable, or Network.
                DOTNETDUPE_API DriveType GetDriveType() const;

                /// \brief Gets the name of the file system, such as NTFS or FAT32.
                DOTNETDUPE_API String GetDriveFormat() const;

                /// \brief Gets a value that indicates whether a drive is ready.
                DOTNETDUPE_API bool GetIsReady() const;

                /// \brief Indicates the amount of available free space on a drive, in bytes.
                DOTNETDUPE_API long long GetAvailableFreeSpace() const;

                /// \brief Gets the total amount of free space available on a drive, in bytes.
                DOTNETDUPE_API long long GetTotalFreeSpace() const;

                /// \brief Gets the total size of storage space on a drive, in bytes.
                DOTNETDUPE_API long long GetTotalSize() const;

                /// \brief Gets the volume label of a drive.
                DOTNETDUPE_API String GetVolumeLabel() const;

                /// \brief Sets the volume label of a drive.
                DOTNETDUPE_API void SetVolumeLabel(const String& sVolumeLabel);

                /// \brief Returns a drive name as a string.
                DOTNETDUPE_API String ToString() const;

                /// \brief Retrieves the drive names of all logical drives on a computer.
                DOTNETDUPE_API static Array<DriveInfo> GetDrives();

            private:
                String m_sName;
            };
        }
    }
}
