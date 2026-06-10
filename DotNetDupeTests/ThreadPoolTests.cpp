#include "pch.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/Interlocked.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;

namespace SystemTests {
    namespace ThreadingTests {

        /**
         * @brief Tests that a simple work item is successfully queued and executed.
         */
        TEST(ThreadPool, QueueUserWorkItem_WhenValidCallback_ExecutesTask) {
            // Given
            ManualResetEvent objMre(false);
            bool bTaskExecuted = false;
            WaitCallback objCallback([&bTaskExecuted, &objMre](Object* pState) {
                bTaskExecuted = true;
                objMre.Set();
            });

            // When
            bool bQueued = ThreadPool::QueueUserWorkItem(objCallback);

            // Then
            ASSERT_TRUE(bQueued);
            bool bSignaled = objMre.WaitOne(5000); // Wait up to 5 seconds
            ASSERT_TRUE(bSignaled);
            ASSERT_TRUE(bTaskExecuted);
        }

        /**
         * @brief Tests that the state object passed to QueueUserWorkItem is correctly received by the callback.
         */
        TEST(ThreadPool, QueueUserWorkItem_WhenPassingState_StateIsCorrect) {
            // Given
            ManualResetEvent objMre(false);
            Object* pSentState = reinterpret_cast<Object*>(0x1234);
            Object* pReceivedState = nullptr;
            WaitCallback objCallback([&pReceivedState, &objMre](Object* pState) {
                pReceivedState = pState;
                objMre.Set();
            });

            // When
            bool bQueued = ThreadPool::QueueUserWorkItem(objCallback, pSentState);

            // Then
            ASSERT_TRUE(bQueued);
            bool bSignaled = objMre.WaitOne(5000);
            ASSERT_TRUE(bSignaled);
            ASSERT_EQ(pReceivedState, pSentState);
        }

        /**
         * @brief Tests that the thread pool can handle multiple tasks being queued rapidly.
         */
        TEST(ThreadPool, QueueUserWorkItem_WhenManyTasks_AllAreExecuted) {
            // Given
            const int iTaskCount = 100;
            Interlocked<int> iCompletedCount = 0;
            ManualResetEvent objMre(false);
            
            WaitCallback objCallback([&iCompletedCount, &objMre, iTaskCount](Object* pState) {
                int iCurrent = ++iCompletedCount;
                if (iCurrent == iTaskCount) {
                    objMre.Set();
                }
            });

            // When
            for (int i = 0; i < iTaskCount; ++i) {
                ThreadPool::QueueUserWorkItem(objCallback);
            }

            // Then
            bool bFinished = objMre.WaitOne(10000); // Higher timeout for many tasks
            ASSERT_TRUE(bFinished);
            ASSERT_EQ((int)iCompletedCount, iTaskCount);
        }

        /**
         * @brief Tests that passing a null callback does not cause a crash.
         */
        TEST(ThreadPool, QueueUserWorkItem_WhenCallbackIsNull_ReturnsTrueGracefully) {
            // Given
            WaitCallback objNullCallback(nullptr);

            // When
            bool bQueued = ThreadPool::QueueUserWorkItem(objNullCallback);

            // Then
            ASSERT_TRUE(bQueued);
            // No crash expected; internal worker should handle null gracefully
        }
    }
}
