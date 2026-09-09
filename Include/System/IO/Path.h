#pragma once
#include "Common.h"
#include "System/Array.h"
#include "System/String.h"

#if defined(_WIN32)
#undef GetTempFileName
#undef GetTempPath
#endif

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class Path
            /// \brief Performs operations on String instances that contain file or directory path information.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.49 (System.IO.Path).
            ///       Normalizes Windows and POSIX path separators and enforces cross-platform pathname invariants.
            class Path {
            public:
                /// \brief Changes the extension of a path string.
                /// \param sFilePath The path information to modify.
                /// \param sExtension The new extension (with or without a leading period).
                /// \return The modified path information.
                DOTNETDUPE_API static String ChangeExtension(const String& sFilePath, const String& sExtension);

                /// \brief Combines an array of strings into a path.
                /// \param sPaths An array of parts of the path.
                /// \return The combined paths.
                DOTNETDUPE_API static String Combine(const std::initializer_list<String> sPaths);

                /// \brief Returns a value that indicates whether the specified path ends in a directory separator.
                /// \param sFilePath The path to analyze.
                /// \return true if the path ends in a directory separator; otherwise, false.
                DOTNETDUPE_API static bool EndsInDirectorySeparator(const String& sFilePath);

                /// \brief Determines whether the given path refers to an existing file or directory on disk.
                /// \param sFilePath The path to test.
                /// \return true if the path exists; otherwise, false.
                DOTNETDUPE_API static bool Exists(const String& sFilePath);

                /// \brief Returns the directory information for the specified path string.
                /// \param sFilePath The path of a file or directory.
                /// \return Directory information for path, or an empty string if path denotes a root directory.
                DOTNETDUPE_API static String GetDirectoryName(const String& sFilePath);

                /// \brief Returns the file name and extension of the specified path string.
                /// \param sFilePath The path string from which to obtain the file name and extension.
                /// \return The characters after the last directory character in path.
                DOTNETDUPE_API static String GetFileName(const String& sFilePath);

                /// \brief Returns the extension of the specified path string.
                /// \param sFilePath The path string from which to get the extension.
                /// \return The extension of the specified path (including the period ".").
                DOTNETDUPE_API static String GetExtension(const String& sFilePath);

                /// \brief Returns the file name of the specified path string without the extension.
                /// \param sFilePath The path of the file.
                /// \return The string returned by GetFileName, minus the last period and all characters following it.
                DOTNETDUPE_API static String GetFileNameWithoutExtension(const String& sFilePath);

                /// \brief Returns the absolute path for the specified path string.
                /// \param sPath The file or directory for which to obtain absolute path information.
                /// \return The fully qualified location of path.
                DOTNETDUPE_API static String GetFullPath(const String& sPath);

                /// \brief Gets an array of characters that cannot be present in file names.
                /// \return An array of invalid file name characters.
                DOTNETDUPE_API static Array<char> GetInvalidFileNameChars();

                /// \brief Gets an array of characters that cannot be present in path names.
                /// \return An array of invalid path characters.
                DOTNETDUPE_API static Array<char> GetInvalidPathChars();

                /// \brief Gets the root directory information from the path contained in the specified string.
                /// \param sPath A string containing path information.
                /// \return The root directory of path, or an empty string if path does not contain root directory information.
                DOTNETDUPE_API static String GetPathRoot(const String& sPath);

                /// \brief Returns a random folder name or file name.
                /// \return A random folder name or file name.
                DOTNETDUPE_API static String GetRandomFileName();

                /// \brief Returns a relative path from one path to another.
                /// \param sRelativeTo The source path the result should be relative to.
                /// \param sPath The destination path.
                /// \return The relative path from sRelativeTo to sPath.
                DOTNETDUPE_API static String GetRelativePath(const String& sRelativeTo, const String& sPath);

                /// \brief Creates a uniquely named, zero-byte temporary file on disk and returns the full path of that file.
                /// \return The full path of the temporary file.
                DOTNETDUPE_API static String GetTempFileName();

                /// \brief Returns the path of the current user's temporary folder.
                /// \return The path to the temporary folder.
                DOTNETDUPE_API static String GetTempPath();

                /// \brief Determines whether a path includes a file name extension.
                /// \param sPath The path to search for an extension.
                /// \return true if the characters that follow the last directory separator or volume separator include a period; otherwise, false.
                DOTNETDUPE_API static bool HasExtension(const String& sPath);

                /// \brief Returns a value that indicates whether the specified path is fully qualified.
                /// \param sPath The path to test.
                /// \return true if the path is fully qualified; otherwise, false.
                DOTNETDUPE_API static bool IsPathFullyQualified(const String& sPath);

                /// \brief Returns a value that indicates whether the specified path string contains a root.
                /// \param sPath The path to test.
                /// \return true if path contains a root; otherwise, false.
                DOTNETDUPE_API static bool IsPathRooted(const String& sPath);

                /// \brief Concatenates an array of paths into a single path.
                DOTNETDUPE_API static String Join(const std::initializer_list<String> sPaths);

                /// \brief Attempts to concatenate an array of paths into a single path.
                DOTNETDUPE_API static bool TryJoin(const std::initializer_list<String> sPaths, String& sResult);

                /// \brief Trims one trailing directory separator beyond the root of the specified path.
                DOTNETDUPE_API static String TrimEndingDirectorySeparator(const String& sPath);

                /// \brief Provides a platform-specific character used to separate directory levels in a path.
                DOTNETDUPE_API static char GetDirectorySeparatorChar();

                /// \brief Provides a platform-specific alternate character used to separate directory levels.
                DOTNETDUPE_API static char GetAltDirectorySeparatorChar();

                /// \brief Provides a platform-specific volume separator character.
                DOTNETDUPE_API static char GetVolumeSeparatorChar();

                /// \brief A platform-specific separator character used to separate path strings in environment variables.
                DOTNETDUPE_API static char GetPathSeparator();
            };
        }
    }
}
