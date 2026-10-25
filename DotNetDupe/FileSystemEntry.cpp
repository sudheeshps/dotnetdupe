#include "pch.h"
#include "System/IO/FileSystemEntry.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

            FileSystemEntry::FileSystemEntry()
                : m_sFullPath("")
                , m_sName("")
                , m_llLength64(0)
                , m_dtCreationTime(0)
                , m_dtLastWriteTime(0)
                , m_eAttributes(FileAttributes::Normal) {
                /// Initialize default empty filesystem entry state.
            }

            FileSystemEntry::FileSystemEntry(
                const String& sFullPath,
                const String& sName,
                long long llLength64,
                const DateTimeOffset& dtCreationTime,
                const DateTimeOffset& dtLastWriteTime,
                FileAttributes eAttributes)
                : m_sFullPath(sFullPath)
                , m_sName(sName)
                , m_llLength64(llLength64)
                , m_dtCreationTime(dtCreationTime)
                , m_dtLastWriteTime(dtLastWriteTime)
                , m_eAttributes(eAttributes) {
                /// Initialize filesystem entry with populated OS metadata fields.
            }

            String FileSystemEntry::GetFullPath() const {
                /// Return full path of entry.
                return m_sFullPath;
            }

            void FileSystemEntry::SetFullPath(const String& sFullPath) {
                /// Update full path of entry.
                m_sFullPath = sFullPath;
            }

            String FileSystemEntry::GetName() const {
                /// Return filename or directory name.
                return m_sName;
            }

            void FileSystemEntry::SetName(const String& sName) {
                /// Update filename or directory name.
                m_sName = sName;
            }

            long long FileSystemEntry::GetLength64() const noexcept {
                /// Return 64-bit byte size.
                return m_llLength64;
            }

            void FileSystemEntry::SetLength64(long long llLength64) noexcept {
                /// Update 64-bit byte size.
                m_llLength64 = llLength64;
            }

            DateTimeOffset FileSystemEntry::GetCreationTime() const {
                /// Return creation timestamp.
                return m_dtCreationTime;
            }

            void FileSystemEntry::SetCreationTime(const DateTimeOffset& dtCreationTime) {
                /// Update creation timestamp.
                m_dtCreationTime = dtCreationTime;
            }

            DateTimeOffset FileSystemEntry::GetLastWriteTime() const {
                /// Return last write timestamp.
                return m_dtLastWriteTime;
            }

            void FileSystemEntry::SetLastWriteTime(const DateTimeOffset& dtLastWriteTime) {
                /// Update last write timestamp.
                m_dtLastWriteTime = dtLastWriteTime;
            }

            FileAttributes FileSystemEntry::GetAttributes() const noexcept {
                /// Return attribute flags.
                return m_eAttributes;
            }

            void FileSystemEntry::SetAttributes(FileAttributes eAttributes) noexcept {
                /// Update attribute flags.
                m_eAttributes = eAttributes;
            }

            bool FileSystemEntry::GetIsDirectory() const noexcept {
                /// Check whether Directory bit flag is present.
                int iFlags = static_cast<int>(m_eAttributes);
                return (iFlags & static_cast<int>(FileAttributes::Directory)) != 0;
            }

            bool FileSystemEntry::GetIsReparsePoint() const noexcept {
                /// Check whether ReparsePoint bit flag is present.
                int iFlags = static_cast<int>(m_eAttributes);
                return (iFlags & static_cast<int>(FileAttributes::ReparsePoint)) != 0;
            }

            bool FileSystemEntry::GetIsReadOnly() const noexcept {
                /// Check whether ReadOnly bit flag is present.
                int iFlags = static_cast<int>(m_eAttributes);
                return (iFlags & static_cast<int>(FileAttributes::ReadOnly)) != 0;
            }

            bool FileSystemEntry::GetIsHidden() const noexcept {
                /// Check whether Hidden bit flag is present.
                int iFlags = static_cast<int>(m_eAttributes);
                return (iFlags & static_cast<int>(FileAttributes::Hidden)) != 0;
            }

        }
    }
}
