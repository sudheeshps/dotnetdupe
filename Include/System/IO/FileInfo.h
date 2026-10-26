/// \file FileInfo.h
/// \brief Provides properties and instance methods for the creation, copying, deletion, moving, and opening of files.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/DateTimeOffset.h"
#include "System/IO/File.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

            /// \class FileInfo
            /// \brief Provides properties and instance methods for inspecting and manipulating a single file.
            ///
            /// Caches file metadata upon Refresh() to minimize repeated filesystem stat calls.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.50 (System.IO.FileInfo).
            /// \see File, FileSystemEntry, Directory
            class FileInfo : public Object {
            public:
                /// \brief Initializes an empty FileInfo instance.
                DOTNETDUPE_API FileInfo();

                /// \brief Initializes a new instance of the FileInfo class, which acts as a wrapper for a file path.
                /// \param sFileName The fully qualified name of the new file, or the relative file name.
                /// \throws ArgumentException Thrown when sFileName is empty.
                DOTNETDUPE_API explicit FileInfo(const String& sFileName);

                /// \brief Virtual destructor.
                DOTNETDUPE_API ~FileInfo() override = default;

                /// \brief Refreshes the state of the object from the underlying filesystem.
                DOTNETDUPE_API void Refresh();

                /// \brief Gets a value indicating whether a file exists.
                /// \return True if the file exists; false if the file does not exist or is a directory.
                DOTNETDUPE_API bool GetExists() const noexcept;

                /// \brief Gets the name of the file.
                /// \return The name of the file.
                DOTNETDUPE_API String GetName() const;

                /// \brief Gets the full path of the file.
                /// \return A string containing the full path.
                DOTNETDUPE_API String GetFullName() const;

                /// \brief Gets a string representing the directory's full path.
                /// \return A string representing the parent directory's full path.
                DOTNETDUPE_API String GetDirectoryName() const;

                /// \brief Gets the string representing the extension part of the file.
                /// \return A string containing the FileInfo extension.
                DOTNETDUPE_API String GetExtension() const;

                /// \brief Gets the size, in bytes, of the current file.
                /// \return The 64-bit size of the current file in bytes.
                /// \throws FileNotFoundException Thrown when the file does not exist.
                DOTNETDUPE_API long long GetLength64() const;

                /// \brief Gets a value that determines if the current file is read-only.
                /// \return True if the current file is read-only; otherwise false.
                DOTNETDUPE_API bool GetIsReadOnly() const noexcept;

                /// \brief Gets a value that determines if the current file is a symbolic link or reparse point.
                /// \return True if the current file is a reparse point; otherwise false.
                DOTNETDUPE_API bool GetIsReparsePoint() const noexcept;

                /// \brief Gets the FileAttributes of the current file.
                /// \return FileAttributes of the current FileInfo.
                DOTNETDUPE_API FileAttributes GetAttributes() const noexcept;

                /// \brief Gets the creation time of the current file.
                /// \return The creation DateTimeOffset of the current file.
                DOTNETDUPE_API DateTimeOffset GetCreationTime() const;

                /// \brief Gets the time when the current file was last written to.
                /// \return The DateTimeOffset when the current file was last written.
                DOTNETDUPE_API DateTimeOffset GetLastWriteTime() const;

                /// \brief Copies an existing file to a new file, allowing the overwriting of an existing file.
                /// \param sDestFileName The name of the new file to copy to.
                /// \param bOverwrite True to allow an existing file to be overwritten; otherwise false.
                /// \return A new FileInfo for the destination file.
                DOTNETDUPE_API FileInfo CopyTo(const String& sDestFileName, bool bOverwrite = false) const;

                /// \brief Moves a specified file to a new location, providing the option to specify a new file name.
                /// \param sDestFileName The path to move the file to, which can specify a different file name.
                DOTNETDUPE_API void MoveTo(const String& sDestFileName);

                /// \brief Permanently deletes a file.
                DOTNETDUPE_API void Delete();

            private:
                String m_sOriginalPath;
                String m_sFullPath;
                bool m_bExists;
                long long m_llLength64;
                FileAttributes m_eAttributes;
                DateTimeOffset m_dtCreationTime;
                DateTimeOffset m_dtLastWriteTime;
            };

        }
    }
}
