/// \file Environment.h
/// \brief Provides information about, and means to manipulate, the current environment and platform.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/OperatingSystem.h"

#if defined(_WIN32)
#undef GetCurrentDirectory
#undef GetSystemDirectory
#undef GetEnvironmentVariable
#undef SetEnvironmentVariable
#undef GetUserName
#undef GetOSVersion
#endif

#include "System/Char.h"
#include "System/Array.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        /// \brief Provides environment variables, machine info, hardware traits, and platform properties.
        ///
        /// Mirrors .NET System.Environment with cross-platform implementations for Windows and Linux.
        /// Static methods provide access to execution context, memory working set, and command-line arguments.
        /// Thread-safe for const queries; environment mutations affect the entire process.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.10 (System.Environment).
        /// \see OperatingSystem, Process
        class Environment : public Object {
        public:
            /// \brief Specifies enumerated constants used to retrieve directory paths to system special folders.
            enum class SpecialFolder {
                ApplicationData,
                CommonApplicationData,
                CommonProgramFiles,
                Cookies,
                Desktop,
                Favorites,
                History,
                InternetCache,
                LocalApplicationData,
                MyComputer,
                MyDocuments,
                MyMusic,
                MyPictures,
                MyVideos,
                ProgramFiles,
                Programs,
                Recent,
                SendTo,
                StartMenu,
                Startup,
                System,
                Templates,
                UserProfile
            };

            /**
             * @brief Gets the NetBIOS / network name of this local computer.
             */
            DOTNETDUPE_API static String GetMachineName();

            /**
             * @brief Gets the user name of the person who is currently logged on to the OS.
             */
            DOTNETDUPE_API static String GetUserName();

            /**
             * @brief Gets the number of logical processors available on the current machine.
             */
            DOTNETDUPE_API static int GetProcessorCount();

            /**
             * @brief Gets the newline string defined for this environment ("\r\n" on Windows, "\n" on POSIX).
             */
            DOTNETDUPE_API static String GetNewLine();

            /**
             * @brief Gets the fully qualified path of the current working directory.
             */
            DOTNETDUPE_API static String GetCurrentDirectory();

            /**
             * @brief Gets the fully qualified path of the system directory.
             */
            DOTNETDUPE_API static String GetSystemDirectory();

            /**
             * @brief Gets an operating system version string representation.
             */
            DOTNETDUPE_API static String GetOSVersion();

            /**
             * @brief Gets the network domain name associated with the current user.
             */
            DOTNETDUPE_API static String GetUserDomainName();

            /**
             * @brief Gets the amount of physical memory mapped to the process context (Working Set in bytes).
             */
            DOTNETDUPE_API static int64_t GetWorkingSet();

            /**
             * @brief Terminates this process and returns an exit code to the operating system.
             * @param iExitCode Exit code to return.
             */
            DOTNETDUPE_API static void Exit(int iExitCode);

            /**
             * @brief Replaces the name of each environment variable embedded in the specified string with the string equivalent of the value of the variable.
             * @param sName A string containing the names of zero or more environment variables.
             * @return A string with each environment variable replaced by its value.
             */
            DOTNETDUPE_API static String ExpandEnvironmentVariables(const String& sName);

            /**
             * @brief Returns a string array containing the command-line arguments for the current process.
             */
            DOTNETDUPE_API static Array<String> GetCommandLineArgs();

            /**
             * @brief Retrieves the value of an environment variable from the current process.
             * @param sVariable Variable name.
             * @return Variable value or empty string if not found.
             */
            DOTNETDUPE_API static String GetEnvironmentVariable(const String& sVariable);

            /**
             * @brief Retrieves all environment variable names and their values from the current process.
             */
            DOTNETDUPE_API static Collections::Generic::Dictionary<String, String> GetEnvironmentVariables();

            /**
             * @brief Gets the path to the system special folder that is identified by the specified enumeration.
             */
            DOTNETDUPE_API static String GetFolderPath(SpecialFolder eFolder);

            /**
             * @brief Returns an array of string containing the names of the logical drives on the current computer.
             */
            DOTNETDUPE_API static Array<String> GetLogicalDrives();

            /**
             * @brief Creates, modifies, or deletes an environment variable stored in the current process.
             */
            DOTNETDUPE_API static void SetEnvironmentVariable(const String& sVariable, const String& sValue);

            /**
             * @brief Gets an OperatingSystem object that contains the current platform identifier and version.
             */
            DOTNETDUPE_API static OperatingSystem GetOperatingSystem();
        };
    }
}