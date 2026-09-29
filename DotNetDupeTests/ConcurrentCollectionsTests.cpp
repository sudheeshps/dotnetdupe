#include "pch.h"
#include "gtest/gtest.h"
#include "System/Collections/Concurrent/ConcurrentDictionary.h"
#include "System/Collections/Concurrent/ConcurrentQueue.h"
#include "System/Collections/Concurrent/ConcurrentStack.h"
#include "System/Collections/Concurrent/ConcurrentBag.h"
#include "System/Collections/Concurrent/BlockingCollection.h"
#include "System/Collections/Concurrent/IProducerConsumerCollection.h"
#include "System/Collections/Generic/IEnumerable.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"
#include "System/Collections/Generic/IDictionary.h"
#include "System/Linq/Enumerable.h"
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

        TEST(ConcurrentQueueTest, PolymorphicInterfaceAndLinq_WorkCorrectly) {
            ConcurrentQueue<int> queue;
            queue.Enqueue(10);
            queue.Enqueue(20);
            queue.Enqueue(30);

            IProducerConsumerCollection<int>* pColl = &queue;
            EXPECT_EQ(pColl->GetCount(), 3);
            EXPECT_TRUE(pColl->TryAdd(40));

            int iSum = 0;
            for (int x : queue) {
                iSum += x;
            }
            EXPECT_EQ(iSum, 100);

            auto filtered = queue.Where([](int x) { return x > 20; }).ToList();
            EXPECT_EQ(filtered.GetCount(), 2);
            EXPECT_EQ(filtered[0], 30);
            EXPECT_EQ(filtered[1], 40);
        }

        TEST(ConcurrentQueueTest, SnapshotEnumeratorIsThreadSafe) {
            ConcurrentQueue<int> queue;
            queue.Enqueue(1);
            queue.Enqueue(2);

            auto spEnum = queue.GetEnumerator();
            queue.Enqueue(3);

            EXPECT_TRUE(spEnum->MoveNext());
            EXPECT_EQ(spEnum->Current(), 1);
            EXPECT_TRUE(spEnum->MoveNext());
            EXPECT_EQ(spEnum->Current(), 2);
            EXPECT_FALSE(spEnum->MoveNext());
        }

        TEST(ConcurrentStackTest, PolymorphicInterfaceAndLinq_WorkCorrectly) {
            ConcurrentStack<int> stack;
            stack.Push(1);
            stack.Push(2);
            stack.Push(3);

            IProducerConsumerCollection<int>* pColl = &stack;
            EXPECT_EQ(pColl->GetCount(), 3);

            int iFirst = stack.First();
            EXPECT_EQ(iFirst, 3);

            auto evens = stack.Where([](int x) { return x % 2 == 0; }).ToArray();
            EXPECT_EQ(evens.GetLength(), 1);
            EXPECT_EQ(evens[0], 2);
        }

        TEST(ConcurrentBagTest, PolymorphicInterfaceAndLinq_WorkCorrectly) {
            ConcurrentBag<int> bag;
            bag.Add(10);
            bag.Add(20);
            bag.Add(30);

            IProducerConsumerCollection<int>* pColl = &bag;
            EXPECT_EQ(pColl->GetCount(), 3);

            int iCount = bag.Count([](int x) { return x >= 20; });
            EXPECT_EQ(iCount, 2);
        }

        TEST(BlockingCollectionTest, PolymorphicInterfaceAndLinq_WorkCorrectly) {
            BlockingCollection<int> bColl;
            bColl.Add(10);
            bColl.Add(20);
            bColl.Add(30);

            Collections::Generic::IReadOnlyCollection<int>* pRo = &bColl;
            EXPECT_EQ(pRo->GetCount(), 3);

            int iSum = 0;
            for (int x : bColl) {
                iSum += x;
            }
            EXPECT_EQ(iSum, 60);

            int iMax = bColl.Max();
            EXPECT_EQ(iMax, 30);
        }

        TEST(ConcurrentDictionaryTest, IDictionaryPolymorphismAndLinq_WorkCorrectly) {
            ConcurrentDictionary<String, int> dict;
            Collections::Generic::IDictionary<String, int>* pDict = &dict;

            pDict->Add("Apple", 1);
            pDict->Add("Banana", 2);
            pDict->Add("Cherry", 3);

            EXPECT_EQ(pDict->GetCount(), 3);
            EXPECT_TRUE(pDict->ContainsKey("Banana"));
            EXPECT_EQ((*pDict)["Cherry"], 3);

            int iSum = 0;
            for (auto kvp : dict) {
                iSum += kvp.Value;
            }
            EXPECT_EQ(iSum, 6);

            auto filtered = dict.Where([](auto kvp) { return kvp.Value > 1; }).ToList();
            EXPECT_EQ(filtered.GetCount(), 2);
        }

    }
}

