#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Action.h"
#include "System/SmartPointer.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Tasks/TaskStatus.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            namespace Tasks {

                /**
                 * @brief Represents an asynchronous operation.
                 */
                class Task : public Object {
                public:
                    /**
                     * @brief Initializes a new Task with the specified action.
                     * @param objAction The delegate that represents the code to execute in the Task.
                     */
                    DOTNETDUPE_API Task(Action<> objAction);
                    
                    /**
                     * @brief Destructor for Task.
                     */
                    DOTNETDUPE_API virtual ~Task();

                    /**
                     * @brief Starts the Task, scheduling it for execution to the current TaskScheduler.
                     */
                    DOTNETDUPE_API void Start();

                    /**
                     * @brief Waits for the Task to complete execution.
                     */
                    DOTNETDUPE_API void Wait();

                    /**
                     * @brief Waits for the Task to complete execution within a specified time interval.
                     * @param iMillisecondsTimeout The number of milliseconds to wait, or -1 to wait indefinitely.
                     * @return true if the Task completed execution within the allotted time; otherwise, false.
                     */
                    DOTNETDUPE_API bool Wait(int iMillisecondsTimeout);
                    
                    /**
                     * @brief Gets the TaskStatus of this task.
                     */
                    DOTNETDUPE_API TaskStatus GetStatus() const;

                    /**
                     * @brief Gets whether this Task has completed.
                     */
                    DOTNETDUPE_API bool GetIsCompleted() const;

                    /**
                     * @brief Gets whether the Task completed due to an unhandled exception.
                     */
                    DOTNETDUPE_API bool GetIsFaulted() const;

                    /**
                     * @brief Gets whether this Task instance has completed execution due to being canceled.
                     */
                    DOTNETDUPE_API bool GetIsCanceled() const;

                    /**
                     * @brief Queues the specified work to run on the ThreadPool and returns a Task object that represents that work.
                     * @param objAction The work to execute asynchronously.
                     * @return A Task that represents the work queued to execute in the ThreadPool.
                     */
                    DOTNETDUPE_API static SmartPointer<Task> Run(Action<> objAction);

                private:
                    Action<> m_objAction;
                    TaskStatus m_eStatus;
                    SmartPointer<ManualResetEvent> m_pCompletionEvent = nullptr;
                    mutable CriticalSection m_csSync;

                    void Execute();
                    static void ThreadPoolCallback(Object* pState);
                    
                    static void RetainTask(SmartPointer<Task> pTask);
                    static void ReleaseTask(Task* pTask);
                };

            }
        }
    }
}
