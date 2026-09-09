/// \file Process.h
/// \brief Provides access to local and remote processes and enables you to start and stop local system processes.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            /// \brief Specifies a set of values that are used when you start a process.
            class ProcessStartInfo : public Object {
            public:
                DOTNETDUPE_API ProcessStartInfo();
                DOTNETDUPE_API ProcessStartInfo(const String& sFileName);
                DOTNETDUPE_API ProcessStartInfo(const String& sFileName, const String& sArguments);

                String FileName;
                String Arguments;
                String WorkingDirectory;
                bool CreateNoWindow;
                bool UseShellExecute;
            };

            /// \brief Provides access to local processes, starting/stopping processes, and querying process snapshots.
            ///
            /// Encapsulates native operating system process resources (Win32 Process / POSIX fork-exec).
            /// Offers high-performance (<5ms) system-wide process discovery via Toolhelp32 on Windows and /proc on Linux.
            /// Thread-safe for const queries; process control methods require synchronization.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.8 (System.Diagnostics.Process).
            /// \see Thread, ProcessStreamer
            class Process : public Object {
            public:
                DOTNETDUPE_API Process();
                DOTNETDUPE_API Process(int iId, const String& sProcessName, void* pProcessHandle = nullptr);
                DOTNETDUPE_API virtual ~Process();

                DOTNETDUPE_API ProcessStartInfo GetStartInfo() const { return m_objStartInfo; }
                DOTNETDUPE_API void SetStartInfo(const ProcessStartInfo& objStartInfo) { m_objStartInfo = objStartInfo; }

                /// \brief Starts (or reuses) the process resource that is specified by the StartInfo property.
                /// \return True if a process resource is started; false if no new process is started.
                DOTNETDUPE_API bool Start();

                /// \brief Starts a process resource by specifying the name of an application.
                /// \param sFileName The name of an application file to run.
                /// \return A SmartPointer to the started Process.
                DOTNETDUPE_API static SmartPointer<Process> Start(const String& sFileName);

                /// \brief Starts a process resource by specifying an application and command-line arguments.
                /// \param sFileName The name of an application file to run.
                /// \param sArguments Command-line arguments to pass when starting the process.
                /// \return A SmartPointer to the started Process.
                DOTNETDUPE_API static SmartPointer<Process> Start(const String& sFileName, const String& sArguments);

                /// \brief Starts a process resource by specifying process start information.
                /// \param objStartInfo The ProcessStartInfo containing launch parameters.
                /// \return A SmartPointer to the started Process.
                DOTNETDUPE_API static SmartPointer<Process> Start(const ProcessStartInfo& objStartInfo);

                /// \brief Instructs the Process component to wait indefinitely for the associated process to exit.
                DOTNETDUPE_API void WaitForExit();

                /// \brief Instructs the Process component to wait the specified number of milliseconds for the associated process to exit.
                /// \param iMilliseconds The amount of time, in milliseconds, to wait for the process to exit.
                /// \return True if the associated process has exited; otherwise, false.
                DOTNETDUPE_API bool WaitForExit(int iMilliseconds);

                /// \brief Gets the value that the associated process specified when it terminated.
                /// \return The code that the associated process specified when it terminated.
                DOTNETDUPE_API int GetExitCode() const { return m_iExitCode; }

                /// \brief Gets a value indicating whether the associated process has been terminated.
                /// \return True if the operating system process has exited; otherwise, false.
                DOTNETDUPE_API bool GetHasExited() const;

                /// \brief Gets the unique identifier for the associated process.
                /// \return The system-generated unique identifier of the process.
                DOTNETDUPE_API int GetId() const { return m_iId; }

                /// \brief Gets the name of the process.
                /// \return The name that the system uses to identify the process to the user.
                DOTNETDUPE_API String GetProcessName() const { return m_sProcessName; }

                /// \brief Gets the process identifier of the calling process.
                /// \return The process identifier of the calling process.
                DOTNETDUPE_API static int GetCurrentProcessId();

                /// \brief Gets a new Process component and associates it with the currently active process.
                /// \return A SmartPointer to a Process component associated with the current process.
                DOTNETDUPE_API static SmartPointer<Process> GetCurrentProcess();

                /// \brief Creates an array of new Process components and associates them with all active system processes.
                /// \return An array of Process components.
                DOTNETDUPE_API static Array<SmartPointer<Process>> GetProcesses();

                /// \brief Returns a Process component given the identifier of a process on the local computer.
                /// \param iProcessId The system-unique identifier of a process resource.
                /// \return A Process component associated with the specified process identifier.
                DOTNETDUPE_API static SmartPointer<Process> GetProcessById(int iProcessId);

                /// \brief Creates an array of new Process components and associates them with all processes sharing the specified name.
                /// \param sProcessName The friendly name of the process.
                /// \return An array of Process components.
                DOTNETDUPE_API static Array<SmartPointer<Process>> GetProcessesByName(const String& sProcessName);

                /// \brief Immediately stops the associated process.
                DOTNETDUPE_API void Kill();

            private:
                ProcessStartInfo m_objStartInfo;
                int m_iId;
                String m_sProcessName;
                mutable int m_iExitCode;
                mutable bool m_bHasExited;
                void* m_pProcessHandle;

                void Refresh() const;
            };
        }
    }
}
