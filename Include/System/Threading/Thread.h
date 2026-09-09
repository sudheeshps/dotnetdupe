/// \file Thread.h
/// \brief Creates and controls a thread, sets its priority, and gets its status mirroring .NET System.Threading.Thread.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Action.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            typedef Action<> ThreadStart;
            typedef Action<Object*> ParameterizedThreadStart;

            /// \brief Creates and controls execution of operating system threads.
            ///
            /// Encapsulates native execution threads (Win32 Thread / POSIX pthread), supporting
            /// parameterless and parameterized thread starts, thread joining with timeouts,
            /// thread-local current thread tracking, and native thread ID extraction.
            /// Not thread-safe for concurrent configuration; query methods are thread-safe.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.5 (System.Threading.Thread).
            /// \see ThreadPool, Task
            class Thread : public Object {
            public:
                /// \brief Initializes a new instance of the Thread class with a parameterless start delegate.
                /// \param start A delegate representing the method to be invoked when this thread begins executing.
                DOTNETDUPE_API Thread(ThreadStart start);

                /// \brief Initializes a new instance of the Thread class with a parameterized start delegate.
                /// \param start A delegate representing the method to be invoked with an Object parameter.
                DOTNETDUPE_API Thread(ParameterizedThreadStart start);

                DOTNETDUPE_API ~Thread() override;

                /// \brief Causes the operating system to change the state of the current instance to Running.
                DOTNETDUPE_API void Start();

                /// \brief Causes the operating system to change the state of the current instance to Running with an argument.
                /// \param parameter An object containing data to be used by the thread start method.
                DOTNETDUPE_API void Start(Object* parameter);

                /// \brief Blocks the calling thread until the thread represented by this instance terminates.
                DOTNETDUPE_API void Join();

                /// \brief Blocks the calling thread until this instance terminates or the specified time elapses.
                /// \param millisecondsTimeout Number of milliseconds to wait for thread termination.
                /// \return True if the thread terminated; false if timed out.
                /// \throws TimeoutException Thrown when the timeout expires before thread termination.
                DOTNETDUPE_API bool Join(int millisecondsTimeout);
                
                /// \brief Suspends the current thread for the specified number of milliseconds.
                /// \param millisecondsTimeout The number of milliseconds for which the thread is suspended.
                DOTNETDUPE_API static void Sleep(int millisecondsTimeout);
                
                /// \brief Gets a value indicating the execution status of the current thread.
                /// \return True if this thread has been started and has not terminated normally or stopped.
                DOTNETDUPE_API bool IsAlive() const;

                /// \brief Gets the friendly name of the thread.
                /// \return A string containing the thread name.
                DOTNETDUPE_API String GetName() const;

                /// \brief Sets the friendly name of the thread.
                /// \param name The string containing the new thread name.
                DOTNETDUPE_API void SetName(const String& name);
                
                /// \brief Gets the currently running thread.
                /// \return A pointer to the Thread instance representing the calling thread.
                DOTNETDUPE_API static Thread* GetCurrentThread();

                /// \brief Returns an integer identifier for the current managed thread.
                /// \return An integer representing the operating system thread ID.
                DOTNETDUPE_API static int GetCurrentThreadId();

            private:
                Thread(); // For internal use (main thread wrapper)
                static SmartPointer<Thread> CreateCurrentThreadWrapper();

                struct Impl;
                Impl* m_pImpl;

                static thread_local Thread* _currentThread;
                
                void ThreadMain(Object* parameter);
            };
        }
    }
}
