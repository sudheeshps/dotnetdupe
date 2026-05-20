#include "pch.h"
#include "gtest/gtest.h"
#include "System/Threading/Thread.h"
#include <atomic>
#include <tchar.h>

using namespace DotNetDupe::System::Threading;

namespace SystemTests {
    namespace ThreadingTestCases {

        TEST(ThreadTest, Start_Should_ExecuteThreadStart_When_Invoked) {
            // Given: A flag and a ThreadStart delegate
            std::atomic<bool> executed = false;
            ThreadStart start = [&executed]() {
                executed = true;
            };
            Thread thread(start);

            // When: Starting and joining the thread
            thread.Start();
            thread.Join();

            // Then: The flag should be true
            ASSERT_TRUE(executed);
        }

        TEST(ThreadTest, Start_Should_PassParameter_When_ParameterizedThreadStartInvoked) {
            // Given: A flag and a ParameterizedThreadStart delegate
            struct MockObject : public DotNetDupe::System::Object {
                bool executed = false;
            };
            MockObject mock;
            
            ParameterizedThreadStart start = [](DotNetDupe::System::Object* obj) {
                static_cast<MockObject*>(obj)->executed = true;
            };
            Thread thread(start);

            // When: Starting with parameter and joining
            thread.Start(&mock);
            thread.Join();

            // Then: The flag in the object should be true
            ASSERT_TRUE(mock.executed);
        }

        TEST(ThreadTest, GetCurrentThread_Should_ReturnNonNull_When_Invoked) {
            // Given: The current thread environment
            // When: GetCurrentThread is called
            Thread* current = Thread::GetCurrentThread();
            // Then: It should not be null
            ASSERT_NE(current, nullptr);
        }

        TEST(ThreadTest, IsAlive_Should_ReturnTrue_While_Running) {
            // Given: A thread that waits
            std::atomic<bool> canFinish = false;
            Thread thread([&canFinish]() {
                while (!canFinish) {
                    Thread::Sleep(10);
                }
            });

            // When: Starting the thread
            thread.Start();
            bool wasAlive = thread.IsAlive();
            
            // Cleanup
            canFinish = true;
            thread.Join();

            // Then: IsAlive should have been true
            ASSERT_TRUE(wasAlive);
            ASSERT_FALSE(thread.IsAlive());
        }

        TEST(ThreadTest, NegativeCase_StartTwice_Should_DoNothing_When_AlreadyStarted) {
            // Given: A started thread
            int executionCount = 0;
            Thread thread([&executionCount]() { executionCount++; });
            thread.Start();
            
            // When: Starting again
            thread.Start();
            thread.Join();

            // Then: It should only execute once
            ASSERT_EQ(executionCount, 1);
        }
        
        TEST(ThreadTest, EdgeCase_SleepZero_Should_ReturnImmediately) {
            // Given/When: Sleep(0) is called
            // Then: It should not crash or hang
            Thread::Sleep(0);
        }
    }
}
