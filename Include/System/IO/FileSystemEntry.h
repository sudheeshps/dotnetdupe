/// \file FileSystemEntry.h
/// \brief Represents lightweight metadata for a file or directory discovered during single-pass filesystem enumeration.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/DateTimeOffset.h"
#include "System/IO/File.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

            /// \class FileSystemEntry
            /// \brief Encapsulates path, 64-bit size, timestamps, and attributes for a filesystem entry.
            ///
            /// Populated directly from OS directory enumeration structures (WIN32_FIND_DATAW on Windows,
            /// stat/dirent on POSIX) to eliminate redundant per-file open/stat system calls during
            /// parallel disk scanning.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5 (System.IO).
            /// \see Directory, FileInfo, FileAttributes
            class FileSystemEntry : public Object {
            public:
                /// \brief Initializes an empty FileSystemEntry instance.
                DOTNETDUPE_API FileSystemEntry();

                /// \brief Initializes a FileSystemEntry with full metadata properties.
                /// \param sFullPath Full path to the file or directory.
                /// \param sName Entry filename or directory name.
                /// \param llLength64 Size in bytes (0 for directories).
                /// \param dtCreationTime Creation timestamp.
                /// \param dtLastWriteTime Last modification timestamp.
                /// \param eAttributes Filesystem attribute flags.
                DOTNETDUPE_API FileSystemEntry(
                    const String& sFullPath,
                    const String& sName,
                    long long llLength64,
                    const DateTimeOffset& dtCreationTime,
                    const DateTimeOffset& dtLastWriteTime,
                    FileAttributes eAttributes);

                /// \brief Virtual destructor.
                DOTNETDUPE_API ~FileSystemEntry() override = default;

                /// \brief Gets the full path of the filesystem entry.
                /// \return Full path string.
                DOTNETDUPE_API String GetFullPath() const;

                /// \brief Sets the full path of the filesystem entry.
                /// \param sFullPath Full path string.
                DOTNETDUPE_API void SetFullPath(const String& sFullPath);

                /// \brief Gets the file or directory name component.
                /// \return Name string.
                DOTNETDUPE_API String GetName() const;

                /// \brief Sets the file or directory name component.
                /// \param sName Name string.
                DOTNETDUPE_API void SetName(const String& sName);

                /// \brief Gets the 64-bit file size in bytes.
                /// \return File size in bytes, or 0 for directories.
                DOTNETDUPE_API long long GetLength64() const noexcept;

                /// \brief Sets the 64-bit file size in bytes.
                /// \param llLength64 File size in bytes.
                DOTNETDUPE_API void SetLength64(long long llLength64) noexcept;

                /// \brief Gets the creation timestamp of the entry.
                /// \return Creation DateTimeOffset.
                DOTNETDUPE_API DateTimeOffset GetCreationTime() const;

                /// \brief Sets the creation timestamp of the entry.
                /// \param dtCreationTime Creation DateTimeOffset.
                DOTNETDUPE_API void SetCreationTime(const DateTimeOffset& dtCreationTime);

                /// \brief Gets the last write timestamp of the entry.
                /// \return Last write DateTimeOffset.
                DOTNETDUPE_API DateTimeOffset GetLastWriteTime() const;

                /// \brief Sets the last write timestamp of the entry.
                /// \param dtLastWriteTime Last write DateTimeOffset.
                DOTNETDUPE_API void SetLastWriteTime(const DateTimeOffset& dtLastWriteTime);

                /// \brief Gets the filesystem attribute flags.
                /// \return FileAttributes enumeration value.
                DOTNETDUPE_API FileAttributes GetAttributes() const noexcept;

                /// \brief Sets the filesystem attribute flags.
                /// \param eAttributes FileAttributes enumeration value.
                DOTNETDUPE_API void SetAttributes(FileAttributes eAttributes) noexcept;

                /// \brief Gets whether this entry represents a directory.
                /// \return True if the Directory attribute flag is set; otherwise false.
                DOTNETDUPE_API bool GetIsDirectory() const noexcept;

                /// \brief Gets whether this entry is a symbolic link, junction, or reparse point.
                /// \return True if the ReparsePoint attribute flag is set; otherwise false.
                DOTNETDUPE_API bool GetIsReparsePoint() const noexcept;

                /// \brief Gets whether this entry is marked read-only.
                /// \return True if the ReadOnly attribute flag is set; otherwise false.
                DOTNETDUPE_API bool GetIsReadOnly() const noexcept;

                /// \brief Gets whether this entry is marked hidden.
                /// \return True if the Hidden attribute flag is set; otherwise false.
                DOTNETDUPE_API bool GetIsHidden() const noexcept;

            private:
                String m_sFullPath;
                String m_sName;
                long long m_llLength64;
                DateTimeOffset m_dtCreationTime;
                DateTimeOffset m_dtLastWriteTime;
                FileAttributes m_eAttributes;
            };

        }
    }
}
