/// \file File.h
/// \brief Provides static methods for the creation, copying, deletion, moving, and opening of a single file.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Text/TextEncoding.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \brief Provides attributes for files and directories.
            enum class FileAttributes {
                Normal = 0,
                ReadOnly = 1,
                Hidden = 2,
                System = 4,
                Directory = 16,
                Archive = 32,
                Device = 64,
                Temporary = 128,
                SparseFile = 256,
                ReparsePoint = 512,
                Compressed = 1024,
                Offline = 2048,
                NotContentIndexed = 4096,
                Encrypted = 8192,
                IntegrityStream = 16384,
                NoScrubData = 32768
            };

            /// \brief Provides static methods for the creation, copying, deletion, moving, and opening of files.
            ///
            /// Mirrors .NET System.IO.File with cross-platform filesystem handling, binary streaming,
            /// and UTF-8 text serialization. All static methods are thread-safe.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.9 (System.IO.File).
            /// \see Directory, FileStream, Path
            class File : public Object {
            public:
                /// \brief Determines whether the specified file exists.
                /// \param sPath The file to check.
                /// \return True if path contains the name of an existing file; otherwise false.
                DOTNETDUPE_API static bool Exists(const String& sPath);

                /// \brief Opens a text file, reads all the text in the file, and then closes the file.
                /// \param sPath The file to open for reading.
                /// \return A string containing all the text in the file.
                /// \throws IOException Thrown when the file cannot be opened for reading.
                DOTNETDUPE_API static String ReadAllText(const String& sPath);

                /// \brief Creates a new file, writes the specified string to the file, and then closes the file.
                /// \param sPath The file to write to.
                /// \param sContents The string to write to the file.
                DOTNETDUPE_API static void WriteAllText(const String& sPath, const String& sContents);

                /// \brief Copies an existing file to a new file, allowing overwriting of an existing file.
                /// \param sSourceFileName The file to copy.
                /// \param sDestFileName The name of the destination file.
                /// \param bOverwrite True if the destination file can be overwritten; otherwise, false.
                DOTNETDUPE_API static void Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite);

                /// \brief Moves a specified file to a new location.
                /// \param sSourceFileName The name of the file to move.
                /// \param sDestFileName The new path and name for the file.
                DOTNETDUPE_API static void Move(const String& sSourceFileName, const String& sDestFileName);

                /// \brief Deletes the specified file.
                /// \param sPath The name of the file to be deleted. Wildcard characters are not supported.
                DOTNETDUPE_API static void Delete(const String& sPath);
                DOTNETDUPE_API static void AppendAllText(const String& sPath, const String& sContents);
                DOTNETDUPE_API static void AppendAllLines(const String& sPath, const Array<String>& sContents);
                DOTNETDUPE_API static Array<String> ReadAllLines(const String& sPath);
                DOTNETDUPE_API static void WriteAllLines(const String& sPath, const Array<String>& sContents);
                DOTNETDUPE_API static void Create(const String& sPath);
                DOTNETDUPE_API static bool GetAttributes(const String& sPath, FileAttributes& attrAttributes);
                DOTNETDUPE_API static bool SetAttributes(const String& sPath, FileAttributes attrAttributes);
            };
        }
    }
}
