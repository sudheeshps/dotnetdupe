#include "pch.h"
#include "System/IO/DriveInfo.h"
#include "System/IO/Path.h"
#include "System/ArgumentException.h"
#include "System/IOException.h"
#include <vector>
#include <cctype>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/statvfs.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace IO {

            DriveInfo::DriveInfo() : m_sName("") {
                /// Step: Initialize default instance.
            }

            static String NormalizeDrivePath(const String& sDriveName) {
                String sName = sDriveName;
#if defined(_WIN32)
                if (sName.GetLength() == 1 && isalpha(static_cast<unsigned char>(sName[0]))) return sName + ":\\";
                if (sName.GetLength() == 2 && sName[1] == ':') return sName + "\\";
                if (!sName.EndsWith('\\', false) && !sName.EndsWith('/', false)) return sName + "\\";
#else
                if (!sName.EndsWith('/', false)) return sName + "/";
#endif
                return sName;
            }

            DriveInfo::DriveInfo(const String& sDriveName) {
                /// Guard: Validate input argument.
                if (sDriveName.IsEmpty()) throw ArgumentException("Drive name cannot be empty.");
                m_sName = NormalizeDrivePath(sDriveName);
            }

            String DriveInfo::GetName() const {
                /// Return: Canonical drive path.
                return m_sName;
            }

            DriveType DriveInfo::GetDriveType() const {
#if defined(_WIN32)
                UINT uType = ::GetDriveTypeA(m_sName.GetRawString());
                if (uType == DRIVE_REMOVABLE) return DriveType::Removable;
                if (uType == DRIVE_FIXED) return DriveType::Fixed;
                if (uType == DRIVE_REMOTE) return DriveType::Network;
                if (uType == DRIVE_CDROM) return DriveType::CDRom;
                if (uType == DRIVE_RAMDISK) return DriveType::Ram;
                if (uType == DRIVE_NO_ROOT_DIR) return DriveType::NoRootDirectory;
                return DriveType::Unknown;
#else
                return DriveType::Fixed;
#endif
            }

            String DriveInfo::GetDriveFormat() const {
#if defined(_WIN32)
                char szFsName[MAX_PATH + 1] = { 0 };
                if (::GetVolumeInformationA(m_sName.GetRawString(), NULL, 0, NULL, NULL, NULL, szFsName, MAX_PATH)) {
                    return String(szFsName);
                }
                return String("NTFS");
#else
                return String("ext4");
#endif
            }

            bool DriveInfo::GetIsReady() const {
#if defined(_WIN32)
                ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
                return ::GetDiskFreeSpaceExA(m_sName.GetRawString(), &freeBytes, &totalBytes, &totalFreeBytes) != 0;
#else
                struct statvfs vfs;
                return statvfs(m_sName.GetRawString(), &vfs) == 0;
#endif
            }

            long long DriveInfo::GetAvailableFreeSpace() const {
#if defined(_WIN32)
                ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
                if (::GetDiskFreeSpaceExA(m_sName.GetRawString(), &freeBytes, &totalBytes, &totalFreeBytes)) {
                    return static_cast<long long>(freeBytes.QuadPart);
                }
                return 0LL;
#else
                struct statvfs vfs;
                if (statvfs(m_sName.GetRawString(), &vfs) == 0) {
                    return static_cast<long long>(vfs.f_bavail) * static_cast<long long>(vfs.f_frsize);
                }
                return 0LL;
#endif
            }

            long long DriveInfo::GetTotalFreeSpace() const {
#if defined(_WIN32)
                ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
                if (::GetDiskFreeSpaceExA(m_sName.GetRawString(), &freeBytes, &totalBytes, &totalFreeBytes)) {
                    return static_cast<long long>(totalFreeBytes.QuadPart);
                }
                return 0LL;
#else
                struct statvfs vfs;
                if (statvfs(m_sName.GetRawString(), &vfs) == 0) {
                    return static_cast<long long>(vfs.f_bfree) * static_cast<long long>(vfs.f_frsize);
                }
                return 0LL;
#endif
            }

            long long DriveInfo::GetTotalSize() const {
#if defined(_WIN32)
                ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
                if (::GetDiskFreeSpaceExA(m_sName.GetRawString(), &freeBytes, &totalBytes, &totalFreeBytes)) {
                    return static_cast<long long>(totalBytes.QuadPart);
                }
                return 0LL;
#else
                struct statvfs vfs;
                if (statvfs(m_sName.GetRawString(), &vfs) == 0) {
                    return static_cast<long long>(vfs.f_blocks) * static_cast<long long>(vfs.f_frsize);
                }
                return 0LL;
#endif
            }

            String DriveInfo::GetVolumeLabel() const {
#if defined(_WIN32)
                char szVolName[MAX_PATH + 1] = { 0 };
                if (::GetVolumeInformationA(m_sName.GetRawString(), szVolName, MAX_PATH, NULL, NULL, NULL, NULL, 0)) {
                    return String(szVolName);
                }
                return String("");
#else
                return String("");
#endif
            }

            void DriveInfo::SetVolumeLabel(const String& sVolumeLabel) {
#if defined(_WIN32)
                if (!::SetVolumeLabelA(m_sName.GetRawString(), sVolumeLabel.GetRawString())) {
                    throw IOException("Failed to set volume label.");
                }
#else
                (void)sVolumeLabel;
#endif
            }

            String DriveInfo::ToString() const {
                /// Return: Drive path string representation.
                return m_sName;
            }

            static void EnumerateNativeDrives(std::vector<DriveInfo>& listDrives) {
#if defined(_WIN32)
                char szBuffer[512] = { 0 };
                DWORD dwLen = ::GetLogicalDriveStringsA(sizeof(szBuffer) - 1, szBuffer);
                for (char* p = szBuffer; p < szBuffer + dwLen && *p; p += strlen(p) + 1) {
                    listDrives.push_back(DriveInfo(String(p)));
                }
#else
                listDrives.push_back(DriveInfo(String("/")));
#endif
            }

            Array<DriveInfo> DriveInfo::GetDrives() {
                std::vector<DriveInfo> listDrives;
                EnumerateNativeDrives(listDrives);
                Array<DriveInfo> arrDrives(static_cast<int>(listDrives.size()));
                for (int i = 0; i < static_cast<int>(listDrives.size()); ++i) arrDrives[i] = listDrives[i];
                return arrDrives;
            }

        }
    }
}
