/// \file File.h
/// \brief Provides static methods for the creation, copying, deletion, moving, and opening of a single file.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Text/TextEncoding.h"
#include "System/Array.h"
#include "System/DateTimeOffset.h"
#include "System/Action.h"

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
            /// atomic verified copying, and UTF-8 text serialization. All static methods are thread-safe.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.9 (System.IO.File).
            /// \see Directory, FileStream, Path, FileInfo
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

                /// \brief Gets the 64-bit size in bytes of the specified file without opening a persistent stream.
                /// \param sPath The file to query.
                /// \return The 64-bit length of the file in bytes.
                /// \throws IOException Thrown when the file does not exist or cannot be queried.
                DOTNETDUPE_API static long long GetLength(const String& sPath);

                /// \brief Gets the creation date and time of the specified file.
                /// \param sPath The file for which to obtain creation date and time information.
                /// \return A DateTimeOffset set to the creation date and time of the specified file.
                DOTNETDUPE_API static DateTimeOffset GetCreationTime(const String& sPath);

                /// \brief Returns the date and time the specified file was last written to.
                /// \param sPath The file for which to obtain write date and time information.
                /// \return A DateTimeOffset set to the date and time that the specified file was last written to.
                DOTNETDUPE_API static DateTimeOffset GetLastWriteTime(const String& sPath);

                /// \brief Sets the date and time that the specified file was last written to.
                /// \param sPath The file for which to set the date and time information.
                /// \param dtLastWriteTime A DateTimeOffset containing the value to set for the last write date and time of sPath.
                DOTNETDUPE_API static void SetLastWriteTime(const String& sPath, const DateTimeOffset& dtLastWriteTime);

                /// \brief Opens a binary file, reads the contents of the file into a byte array, and then closes the file.
                /// \param sPath The file to open for reading.
                /// \return A byte array containing the contents of the file.
                DOTNETDUPE_API static Array<byte> ReadAllBytes(const String& sPath);

                /// \brief Creates a new file, writes the specified byte array to the file, and then closes the file.
                /// \param sPath The file to write to.
                /// \param arrBytes The bytes to write to the file.
                DOTNETDUPE_API static void WriteAllBytes(const String& sPath, const Array<byte>& arrBytes);

                /// \brief Performs a strict size and byte-by-byte stream comparison of two files to guarantee 100% content identity.
                /// \param sPathA Path to the first file.
                /// \param sPathB Path to the second file.
                /// \return True if both files exist, have identical 64-bit byte length, and match byte-for-byte; otherwise false.
                DOTNETDUPE_API static bool AreFilesByteIdentical(const String& sPathA, const String& sPathB);

                /// \brief Copies an existing file to a new file, allowing overwriting and optional timestamp preservation.
                /// \param sSourceFileName The file to copy.
                /// \param sDestFileName The name of the destination file.
                /// \param bOverwrite True if the destination file can be overwritten; otherwise false.
                /// \param bPreserveTimestamps True to preserve the source file's last write timestamp on the destination file.
                DOTNETDUPE_API static void Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite, bool bPreserveTimestamps);

                /// \brief Atomically copies a file via a staging .part file, flushes to disk, verifies target SHA-256 and size, and renames to destination.
                /// \param sSourceFileName The source file to copy.
                /// \param sDestFileName The target file path.
                /// \param bOverwrite True to overwrite an existing destination file.
                /// \param bPreserveTimestamps True to preserve the source file's last write timestamp.
                /// \param fnProgress Optional progress callback invoked with (bytesCopied, totalBytes).
                /// \return True if the file was copied, verified by read-back SHA-256, and atomically committed.
                DOTNETDUPE_API static bool CopyVerified(
                    const String& sSourceFileName,
                    const String& sDestFileName,
                    bool bOverwrite = true,
                    bool bPreserveTimestamps = true,
                    Action<long long, long long> fnProgress = Action<long long, long long>());
            };
        }
    }
}
