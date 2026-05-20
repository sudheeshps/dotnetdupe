#include "pch.h"
#include "gtest/gtest.h"
#include "System/Threading/Thread.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreSlim.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Interlocked.h"
#include "System/Threading/Lock.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/TimeoutException.h"
#include <atomic>
#include <vector>

using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System;

namespace SystemTests {
    namespace ThreadingTestCases {

        // --- RAII Lock Tests ---
        TEST(LockTest, MutexLock_Should_AcquireAndRelease) {
            // Given
            Mutex m;
            bool threadAcquired = false;

            // When
            {
                MutexLock lock(m);
                Thread t([&]() {
                    try {
                        m.WaitOne(50);
                        threadAcquired = true;
                    } catch (...) {}
                });
                t.Start();
                t.Join();
            }

            // Then: thread should NOT have acquired it while lock was in scope
            ASSERT_FALSE(threadAcquired);

            // When: Acquire again after lock destroyed
            bool canAcquireNow = m.WaitOne(50);
            if(canAcquireNow) m.Release();

            // Then
            ASSERT_TRUE(canAcquireNow);
        }

        TEST(LockTest, CriticalSectionLock_Should_AcquireAndRelease) {
            // Given
            CriticalSection cs;
            int counter = 0;

            // When
            {
                CriticalSectionLock lock(cs);
                counter++;
            }

            // Then: Should be able to enter again
            bool result = cs.TryEnter();
            if(result) cs.Leave();
            ASSERT_TRUE(result);
            ASSERT_EQ(counter, 1);
        }

        TEST(LockTest, SemaphoreLock_Should_ReleaseSpecifiedCount) {
            // Given
            Semaphore s(1, 5); 

            // When
            {
                SemaphoreLock lock(s, -1, 2); // Acquire 1, Release 2 in dtor
            }

            // Then: Count should now be 2 (1 -> acquire -> 0 -> release 2 -> 2)
            bool r1 = s.WaitOne(0);
            bool r2 = s.WaitOne(0);
            
            ASSERT_TRUE(r1);
            ASSERT_TRUE(r2);
            ASSERT_THROW(s.WaitOne(10), TimeoutException);
        }

        // --- ManualResetEvent Tests ---
        TEST(ManualResetEventTest, WaitOne_Should_Block_When_InitialStateIsFalse) {
            // Given: A ManualResetEvent in non-signaled state
            ManualResetEvent mre(false);
            std::atomic<bool> threadStarted = false;
            std::atomic<bool> operationComplete = false;

            Thread t([&]() {
                threadStarted = true;
                mre.WaitOne();
                operationComplete = true;
            });

            // When: Starting thread and waiting a bit
            t.Start();
            while(!threadStarted) Thread::Sleep(10);
            Thread::Sleep(50);

            // Then: Operation should not be complete
            ASSERT_FALSE(operationComplete);

            // When: Signaling the event
            mre.Set();
            t.Join();

            // Then: Operation should be complete
            ASSERT_TRUE(operationComplete);
        }

        TEST(ManualResetEventTest, WaitOne_Should_NotBlock_When_InitialStateIsTrue) {
            // Given: A ManualResetEvent in signaled state
            ManualResetEvent mre(true);

            // When: Waiting
            bool result = mre.WaitOne(100);

            // Then: It should return immediately
            ASSERT_TRUE(result);
        }

        TEST(ManualResetEventTest, WaitOne_Should_ThrowTimeoutException_When_TimeoutExpires) {
            // Given: A ManualResetEvent in non-signaled state
            ManualResetEvent mre(false);

            // When / Then: Waiting with timeout should throw
            ASSERT_THROW(mre.WaitOne(10), TimeoutException);
        }

        // --- AutoResetEvent Tests ---
        TEST(AutoResetEventTest, WaitOne_Should_ResetAutomatically_After_OneThreadReleased) {
            // Given: An AutoResetEvent in signaled state
            AutoResetEvent are(true);

            // When: Waiting once
            bool firstWait = are.WaitOne(100);
            // When: Waiting again without setting
            
            // Then: First should succeed, second should throw TimeoutException
            ASSERT_TRUE(firstWait);
            ASSERT_THROW(are.WaitOne(10), TimeoutException);
        }

        TEST(MutexTest, WaitOne_Should_AllowOnlyOneThread_AtATime) {
            // Given: A Mutex and a shared resource
            Mutex m;
            int sharedValue = 0;
            const int count = 100;
            std::vector<Thread*> threads;

            // When: Multiple threads increment shared value under mutex
            for(int i=0; i<10; ++i) {
                threads.push_back(new Thread([&]() {
                    for(int j=0; j<count; ++j) {
                        m.WaitOne();
                        sharedValue++;
                        m.Release();
                    }
                }));
            }

            for(auto t : threads) t->Start();
            for(auto t : threads) {
                t->Join();
                delete t;
            }

            // Then: Shared value should be exactly total increments
            ASSERT_EQ(sharedValue, 1000);
        }

        // --- Semaphore Tests ---
        TEST(SemaphoreTest, Release_Should_ThrowException_When_MaxCountExceeded) {
            // Given: A semaphore with max count 1, already at 1
            Semaphore s(1, 1);

            // When / Then: Releasing again should throw
            ASSERT_THROW(s.Release(1), SemaphoreFullException);
        }

        // --- SemaphoreSlim Tests ---
        TEST(SemaphoreSlimTest, Wait_Should_LimitConcurrency) {
            // Given: A SemaphoreSlim with count 2
            SemaphoreSlim ss(2);
            std::atomic<int> concurrentCount = 0;
            std::atomic<int> maxObserved = 0;

            auto work = [&]() {
                ss.WaitOne();
                int current = ++concurrentCount;
                if (current > maxObserved) maxObserved = current;
                Thread::Sleep(50);
                --concurrentCount;
                ss.Release(1);
            };

            std::vector<Thread*> threads;
            for(int i=0; i<5; ++i) threads.push_back(new Thread(work));

            for(auto t : threads) t->Start();
            for(auto t : threads) {
                t->Join();
                delete t;
            }

            // Then: maxObserved should be 2
            ASSERT_EQ(maxObserved, 2);
        }

        // --- CriticalSection Tests ---
        TEST(CriticalSectionTest, Enter_Should_BeRecursive) {
            // Given: A CriticalSection
            CriticalSection cs;

            // When: Entering twice on same thread
            cs.Enter();
            bool result = cs.TryEnter();
            if(result) cs.Leave();
            cs.Leave();

            // Then: Should not deadlock
            ASSERT_TRUE(result);
        }

        // --- Interlocked Tests ---
        TEST(InterlockedBatchTest, Increment_Should_BeAtomic) {
            // Given: An Interlocked object
            Interlocked<int> val = 0;
            std::vector<Thread*> threads;

            // When: 10 threads increment 1000 times each
            for(int i=0; i<10; ++i) {
                threads.push_back(new Thread([&]() {
                    for(int j=0; j<1000; ++j) val++;
                }));
            }

            for(auto t : threads) t->Start();
            for(auto t : threads) {
                t->Join();
                delete t;
            }

            // Then: Result should be 10000
            ASSERT_EQ((int)val, 10000);
        }
    }
}
