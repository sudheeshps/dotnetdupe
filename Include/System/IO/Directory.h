#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/DateTimeOffset.h"

#if defined(_WIN32)
#undef CreateDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#endif

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class Directory
            /// \brief Exposes static methods for creating, moving, and enumerating through directories and subdirectories.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.48 (System.IO.Directory).
            ///       Cross-platform directory manipulation adhering to Windows Win32 and POSIX filesystem conventions.
            class Directory : public Object {
            public:
                /// \brief Determines whether the given path refers to an existing directory on disk.
                /// \param sPath The path to test.
                /// \return true if sPath refers to an existing directory; otherwise, false.
                DOTNETDUPE_API static bool Exists(const String& sPath);

                /// \brief Creates all directories and subdirectories in the specified path unless they already exist.
                /// \param sPath The directory to create.
                DOTNETDUPE_API static void CreateDirectory(const String& sPath);

                /// \brief Creates all directories in the specified path with explicit recursion flag.
                /// \param sPath The directory to create.
                /// \param bRecursive Whether to create parent directories recursively.
                DOTNETDUPE_API static void CreateDirectory(const String& sPath, bool bRecursive);

                /// \brief Deletes an empty directory from a specified path.
                /// \param sPath The name of the empty directory to remove.
                DOTNETDUPE_API static void Delete(const String& sPath);

                /// \brief Deletes the specified directory and, if indicated, any subdirectories and files in the directory.
                /// \param sPath The name of the directory to remove.
                /// \param bRecursive true to remove directories, subdirectories, and files in sPath; otherwise, false.
                DOTNETDUPE_API static void Delete(const String& sPath, bool bRecursive);

                /// \brief Moves a file or a directory and its contents to a new location.
                /// \param sSourceDirName The path of the file or directory to move.
                /// \param sDestDirName The path to the new location for sSourceDirName.
                DOTNETDUPE_API static void Move(const String& sSourceDirName, const String& sDestDirName);
                
                /// \brief Returns the names of files (including their paths) in the specified directory.
                /// \param sPath The relative or absolute path to the directory to search.
                /// \return An array of the full names (including paths) for the files in the specified directory.
                DOTNETDUPE_API static Array<String> GetFiles(const String& sPath);

                /// \brief Returns the names of files (including their paths) that match the specified search pattern in the specified directory.
                /// \param sPath The relative or absolute path to the directory to search.
                /// \param sSearchPattern The search string to match against the names of files in sPath.
                /// \return An array of the full names (including paths) for the files matching pattern.
                DOTNETDUPE_API static Array<String> GetFiles(const String& sPath, const String& sSearchPattern);
                
                /// \brief Returns the names of subdirectories (including their paths) in the specified directory.
                /// \param sPath The relative or absolute path to the directory to search.
                /// \return An array of the full names (including paths) for the subdirectories.
                DOTNETDUPE_API static Array<String> GetDirectories(const String& sPath);

                /// \brief Returns the names of subdirectories (including their paths) matching pattern.
                DOTNETDUPE_API static Array<String> GetDirectories(const String& sPath, const String& sSearchPattern);
                
                /// \brief Returns the names of all files and subdirectories in the specified directory.
                DOTNETDUPE_API static Array<String> GetFileSystemEntries(const String& sPath);

                /// \brief Returns the names of all files and subdirectories matching pattern.
                DOTNETDUPE_API static Array<String> GetFileSystemEntries(const String& sPath, const String& sSearchPattern);
                
                /// \brief Gets the current working directory of the application.
                /// \return A string that contains the path of the current working directory.
                DOTNETDUPE_API static String GetCurrentDirectory();

                /// \brief Sets the application's current working directory to the specified directory.
                /// \param sPath The path to which the current working directory is set.
                DOTNETDUPE_API static void SetCurrentDirectory(const String& sPath);
                
                /// \brief Returns the volume information, root information, or both for the specified path.
                DOTNETDUPE_API static String GetDirectoryRoot(const String& sPath);

                /// \brief Gets the creation date and time of a directory.
                DOTNETDUPE_API static DateTimeOffset GetCreationTime(const String& sPath);

                /// \brief Returns the date and time the specified file or directory was last written to.
                DOTNETDUPE_API static DateTimeOffset GetLastWriteTime(const String& sPath);

                /// \brief Returns the date and time the specified file or directory was last accessed.
                DOTNETDUPE_API static DateTimeOffset GetLastAccessTime(const String& sPath);
            };
        }
    }
}
