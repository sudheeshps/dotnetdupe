#include "pch.h"
#include "gtest/gtest.h"
#include "System/Collections/Concurrent/ConcurrentDictionary.h"
#include "System/Collections/Concurrent/ConcurrentQueue.h"
#include "System/Collections/Concurrent/ConcurrentStack.h"
#include "System/Collections/Concurrent/ConcurrentBag.h"
#include "System/Collections/Concurrent/BlockingCollection.h"
#include "System/Threading/Thread.h"
#include "System/String.h"
#include <vector>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

namespace SystemTests {
    namespace ConcurrentCollectionsTestCases {

        TEST(ConcurrentDictionaryTest, TryAdd_TryGetValue_TryRemove_WorkCorrectly) {
            ConcurrentDictionary<String, int> dict;
            EXPECT_TRUE(dict.TryAdd("Key1", 100));
            EXPECT_FALSE(dict.TryAdd("Key1", 200));

            int iVal = 0;
            EXPECT_TRUE(dict.TryGetValue("Key1", iVal));
            EXPECT_EQ(iVal, 100);

            EXPECT_TRUE(dict.TryRemove("Key1", iVal));
            EXPECT_EQ(iVal, 100);
            EXPECT_FALSE(dict.TryGetValue("Key1", iVal));
        }

        TEST(ConcurrentDictionaryTest, Multithreaded_AddAndGet_IsThreadSafe) {
            ConcurrentDictionary<int, int> dict;
            const int iThreadCount = 4;
            const int iItemsPerThread = 250;

            std::vector<Thread*> vThreads;
            for (int t = 0; t < iThreadCount; t++) {
                vThreads.push_back(new Thread([&dict, t, iItemsPerThread]() {
                    for (int i = 0; i < iItemsPerThread; i++) {
                        dict.TryAdd(t * iItemsPerThread + i, i);
                    }
                }));
            }

            for (auto pThread : vThreads) {
                pThread->Start();
            }
            for (auto pThread : vThreads) {
                pThread->Join();
                delete pThread;
            }

            EXPECT_EQ(dict.GetCount(), iThreadCount * iItemsPerThread);
        }

        TEST(ConcurrentQueueTest, Enqueue_TryDequeue_TryPeek_WorkCorrectly) {
            ConcurrentQueue<int> queue;
            queue.Enqueue(10);
            queue.Enqueue(20);

            int iPeek = 0;
            EXPECT_TRUE(queue.TryPeek(iPeek));
            EXPECT_EQ(iPeek, 10);

            int iVal = 0;
            EXPECT_TRUE(queue.TryDequeue(iVal));
            EXPECT_EQ(iVal, 10);
            EXPECT_TRUE(queue.TryDequeue(iVal));
            EXPECT_EQ(iVal, 20);
            EXPECT_FALSE(queue.TryDequeue(iVal));
        }

        TEST(ConcurrentStackTest, Push_TryPop_TryPeek_WorkCorrectly) {
            ConcurrentStack<int> stack;
            stack.Push(10);
            stack.Push(20);

            int iPeek = 0;
            EXPECT_TRUE(stack.TryPeek(iPeek));
            EXPECT_EQ(iPeek, 20);

            int iVal = 0;
            EXPECT_TRUE(stack.TryPop(iVal));
            EXPECT_EQ(iVal, 20);
            EXPECT_TRUE(stack.TryPop(iVal));
            EXPECT_EQ(iVal, 10);
            EXPECT_FALSE(stack.TryPop(iVal));
        }

        TEST(ConcurrentBagTest, Add_TryTake_WorkCorrectly) {
            ConcurrentBag<int> bag;
            bag.Add(1);
            bag.Add(2);
            EXPECT_EQ(bag.GetCount(), 2);

            int iVal = 0;
            EXPECT_TRUE(bag.TryTake(iVal));
            EXPECT_TRUE(bag.TryTake(iVal));
            EXPECT_FALSE(bag.TryTake(iVal));
        }

        TEST(BlockingCollectionTest, Add_Take_ProducerConsumer_WorksCorrectly) {
            BlockingCollection<int> collection(5);

            Thread producer([&collection]() {
                for (int i = 1; i <= 10; i++) {
                    collection.Add(i);
                }
                collection.CompleteAdding();
            });

            producer.Start();

            int iSum = 0;
            int iVal = 0;
            while (collection.TryTake(iVal, -1)) {
                iSum += iVal;
            }

            producer.Join();

            EXPECT_EQ(iSum, 55);
            EXPECT_TRUE(collection.IsCompleted());
        }

    }
}
