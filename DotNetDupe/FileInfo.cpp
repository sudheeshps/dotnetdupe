#include "pch.h"
#include "System/IO/FileInfo.h"
#include "System/IO/Path.h"
#include "System/IO/FileNotFoundException.h"
#include "System/ArgumentException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

            FileInfo::FileInfo()
                : m_sOriginalPath("")
                , m_sFullPath("")
                , m_bExists(false)
                , m_llLength64(0)
                , m_eAttributes(FileAttributes::Normal)
                , m_dtCreationTime(0)
                , m_dtLastWriteTime(0) {
                /// Initialize default unpopulated FileInfo instance.
            }

            FileInfo::FileInfo(const String& sFileName)
                : m_sOriginalPath(sFileName)
                , m_sFullPath("")
                , m_bExists(false)
                , m_llLength64(0)
                , m_eAttributes(FileAttributes::Normal)
                , m_dtCreationTime(0)
                , m_dtLastWriteTime(0) {
                /// Guard: Reject empty path string.
                if (sFileName.IsEmpty()) {
                    throw ArgumentException("File name cannot be empty.");
                }

                /// Resolve full path and query initial filesystem metadata.
                m_sFullPath = Path::GetFullPath(sFileName);
                Refresh();
            }

            void FileInfo::Refresh() {
                /// Guard: Handle empty path gracefully.
                if (m_sFullPath.IsEmpty()) {
                    m_bExists = false;
                    return;
                }

                /// Query file attributes and verify regular file existence.
                FileAttributes eAttrs = FileAttributes::Normal;
                bool bHasAttrs = File::GetAttributes(m_sFullPath, eAttrs);
                bool bIsDir = (static_cast<int>(eAttrs) & static_cast<int>(FileAttributes::Directory)) != 0;
                m_bExists = bHasAttrs && !bIsDir && File::Exists(m_sFullPath);
                m_eAttributes = bHasAttrs ? eAttrs : FileAttributes::Normal;
                if (!m_bExists) {
                    m_llLength64 = 0;
                    return;
                }

                /// Populate 64-bit size and timestamps for existing file.
                m_llLength64 = File::GetLength(m_sFullPath);
                m_dtLastWriteTime = File::GetLastWriteTime(m_sFullPath);
                m_dtCreationTime = File::GetCreationTime(m_sFullPath);
            }

            bool FileInfo::GetExists() const noexcept {
                /// Return cached file existence flag.
                return m_bExists;
            }

            String FileInfo::GetName() const {
                /// Extract filename component from full path.
                return Path::GetFileName(m_sFullPath);
            }

            String FileInfo::GetFullName() const {
                /// Return resolved full path.
                return m_sFullPath;
            }

            String FileInfo::GetDirectoryName() const {
                /// Extract parent directory path from full path.
                return Path::GetDirectoryName(m_sFullPath);
            }

            String FileInfo::GetExtension() const {
                /// Extract file extension including leading period.
                return Path::GetExtension(m_sFullPath);
            }

            long long FileInfo::GetLength64() const {
                /// Guard: Ensure file exists before returning byte length.
                if (!m_bExists) {
                    throw FileNotFoundException("Could not find file.");
                }

                /// Return cached 64-bit file size.
                return m_llLength64;
            }

            bool FileInfo::GetIsReadOnly() const noexcept {
                /// Check ReadOnly flag in cached attributes.
                int iFlags = static_cast<int>(m_eAttributes);
                return (iFlags & static_cast<int>(FileAttributes::ReadOnly)) != 0;
            }

            bool FileInfo::GetIsReparsePoint() const noexcept {
                /// Check ReparsePoint flag in cached attributes.
                int iFlags = static_cast<int>(m_eAttributes);
                return (iFlags & static_cast<int>(FileAttributes::ReparsePoint)) != 0;
            }

            FileAttributes FileInfo::GetAttributes() const noexcept {
                /// Return cached attribute flags.
                return m_eAttributes;
            }

            DateTimeOffset FileInfo::GetCreationTime() const {
                /// Return cached creation timestamp.
                return m_dtCreationTime;
            }

            DateTimeOffset FileInfo::GetLastWriteTime() const {
                /// Return cached last write timestamp.
                return m_dtLastWriteTime;
            }

            FileInfo FileInfo::CopyTo(const String& sDestFileName, bool bOverwrite) const {
                /// Guard: Validate destination path.
                if (sDestFileName.IsEmpty()) {
                    throw ArgumentException("Destination file name cannot be empty.");
                }

                /// Copy file preserving timestamps and return target FileInfo.
                File::Copy(m_sFullPath, sDestFileName, bOverwrite, true);
                return FileInfo(sDestFileName);
            }

            void FileInfo::MoveTo(const String& sDestFileName) {
                /// Guard: Validate destination path.
                if (sDestFileName.IsEmpty()) {
                    throw ArgumentException("Destination file name cannot be empty.");
                }

                /// Move file on disk and refresh this instance to point to new path.
                File::Move(m_sFullPath, sDestFileName);
                m_sOriginalPath = sDestFileName;
                m_sFullPath = Path::GetFullPath(sDestFileName);
                Refresh();
            }

            void FileInfo::Delete() {
                /// Delete file if present and update cached state.
                if (m_bExists) {
                    File::Delete(m_sFullPath);
                }
                m_bExists = false;
                m_llLength64 = 0;
            }

        }
    }
}
