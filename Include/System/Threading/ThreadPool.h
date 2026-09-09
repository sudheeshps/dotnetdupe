/// \file ThreadPool.h
/// \brief Provides a pool of threads that can be used to execute tasks and work items.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Action.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \brief Represents a callback method to be executed by a thread pool thread.
            typedef Action<Object*> WaitCallback;

            /// \brief Provides a pool of threads that can be used to execute tasks, post work items, and process timers.
            ///
            /// Manages worker threads across an internal work queue, automatically scheduling tasks
            /// as worker threads become idle. Thread-safe for all concurrent operations.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.15 (System.Threading.ThreadPool).
            /// \see Thread, Task
            class ThreadPool : public Object {
            public:
                /// \brief Queues a method for execution. The method executes when a thread pool thread becomes available.
                /// \param callback A WaitCallback that represents the method to be executed.
                /// \return True if the method is successfully queued.
                DOTNETDUPE_API static bool QueueUserWorkItem(WaitCallback callback);

                /// \brief Queues a method for execution with a state parameter.
                /// \param callback A WaitCallback that represents the method to be executed.
                /// \param pState An object containing data to be used by the method.
                /// \return True if the method is successfully queued.
                DOTNETDUPE_API static bool QueueUserWorkItem(WaitCallback callback, Object* pState);

                /// \brief Sets the minimum number of threads the thread pool creates on demand as new requests are made.
                /// \param iMinThreads The minimum number of worker threads.
                /// \return True if the minimum thread count was successfully set.
                DOTNETDUPE_API static bool SetMinThreads(int iMinThreads);

            private:
                // Strictly static class
                ThreadPool() = delete;
            };
        }
    }
}
