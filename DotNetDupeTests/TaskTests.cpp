#include "pch.h"
#include "System/Threading/Tasks/Task.h"
#include "System/Threading/Interlocked.h"
#include "System/Threading/Thread.h"
#include "System/Exception.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::Threading::Tasks;

namespace SystemTests {
    namespace TasksTests {

        /**
         * @brief Tests that a Task correctly executes an action and transitions to RanToCompletion.
         */
        TEST(Task, Run_WhenValidAction_ExecutesSuccessfully) {
            // Given
            Interlocked<int> iValue = 0;
            Action<> objAction([&iValue]() {
                iValue++;
            });

            // When
            auto pTask = Task::Run(objAction);
            pTask->Wait();

            // Then
            ASSERT_EQ((int)iValue, 1);
            ASSERT_TRUE(pTask->GetIsCompleted());
            ASSERT_FALSE(pTask->GetIsFaulted());
            ASSERT_EQ(pTask->GetStatus(), TaskStatus::RanToCompletion);
        }

        /**
         * @brief Tests that waiting with a short timeout correctly returns false if the task is still running.
         */
        TEST(Task, Wait_WithTimeout_ReturnsFalseIfTimeoutExceeded) {
            // Given
            Action<> objAction([]() {
                Thread::Sleep(2000); // Sleep for 2 seconds
            });

            // When
            auto pTask = Task::Run(objAction);
            bool bFinished = pTask->Wait(100); // Wait for only 100ms

            // Then
            ASSERT_FALSE(bFinished);
            ASSERT_FALSE(pTask->GetIsCompleted());
            
            // Cleanup: ensure test runner doesn't exit while task accesses captured variables (none here, but good practice)
            pTask->Wait();
        }

        /**
         * @brief Tests that exceptions thrown inside a task are caught and the task transitions to Faulted.
         */
        TEST(Task, Execute_WhenExceptionThrown_SetsFaultedState) {
            // Given
            Action<> objAction([]() {
                throw Exception("Test Exception");
            });

            // When
            auto pTask = Task::Run(objAction);
            pTask->Wait();

            // Then
            ASSERT_TRUE(pTask->GetIsCompleted());
            ASSERT_TRUE(pTask->GetIsFaulted());
            ASSERT_EQ(pTask->GetStatus(), TaskStatus::Faulted);
        }
    }
}
