#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Action.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /**
             * @brief Represents a callback method to be executed by a thread pool thread.
             */
            typedef Action<Object*> WaitCallback;

            /**
             * @brief Provides a pool of threads that can be used to execute tasks, post work items, process asynchronous I/O, wait on behalf of other threads, and process timers.
             */
            class ThreadPool : public Object {
            public:
                /**
                 * @brief Queues a method for execution. The method executes when a thread pool thread becomes available.
                 * @param callback A WaitCallback that represents the method to be executed.
                 * @return true if the method is successfully queued; NotSupportedException is thrown if the work item could not be queued.
                 */
                DOTNETDUPE_API static bool QueueUserWorkItem(WaitCallback callback);

                /**
                 * @brief Queues a method for execution, and specifies an object containing data to be used by the method. The method executes when a thread pool thread becomes available.
                 * @param callback A WaitCallback that represents the method to be executed.
                 * @param pState An object containing data to be used by the method.
                 * @return true if the method is successfully queued; NotSupportedException is thrown if the work item could not be queued.
                 */
                DOTNETDUPE_API static bool QueueUserWorkItem(WaitCallback callback, Object* pState);

            private:
                // Strictly static class
                ThreadPool() = delete;
            };
        }
    }
}
