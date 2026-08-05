#include "pch.h"
#include "gtest/gtest.h"
#include "System/Collections/Generic/Queue.h"
#include "System/Collections/Generic/Stack.h"
#include "System/Collections/Generic/HashSet.h"
#include "System/Collections/Generic/SortedDictionary.h"
#include "System/Collections/Generic/SortedSet.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/Collections/Generic/PriorityQueue.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

namespace SystemTests {
    namespace GenericCollectionsTestCases {

        TEST(QueueTest, Enqueue_Dequeue_Peek_WorkCorrectly) {
            Queue<int> q;
            q.Enqueue(10);
            q.Enqueue(20);
            EXPECT_EQ(q.GetCount(), 2);
            EXPECT_EQ(q.Peek(), 10);
            EXPECT_EQ(q.Dequeue(), 10);
            EXPECT_EQ(q.Dequeue(), 20);
            EXPECT_EQ(q.GetCount(), 0);
        }

        TEST(StackTest, Push_Pop_Peek_WorkCorrectly) {
            Stack<int> s;
            s.Push(10);
            s.Push(20);
            EXPECT_EQ(s.GetCount(), 2);
            EXPECT_EQ(s.Peek(), 20);
            EXPECT_EQ(s.Pop(), 20);
            EXPECT_EQ(s.Pop(), 10);
            EXPECT_EQ(s.GetCount(), 0);
        }

        TEST(HashSetTest, Add_Contains_Remove_WorkCorrectly) {
            HashSet<String> set;
            EXPECT_TRUE(set.Add("Alpha"));
            EXPECT_FALSE(set.Add("Alpha"));
            EXPECT_TRUE(set.Contains("Alpha"));
            EXPECT_TRUE(set.Remove("Alpha"));
            EXPECT_FALSE(set.Contains("Alpha"));
        }

        TEST(SortedDictionaryTest, Add_OrderedTraversal_WorksCorrectly) {
            SortedDictionary<int, String> dict;
            dict.Add(3, "Three");
            dict.Add(1, "One");
            dict.Add(2, "Two");

            auto keys = dict.GetKeys();
            EXPECT_EQ(keys[0], 1);
            EXPECT_EQ(keys[1], 2);
            EXPECT_EQ(keys[2], 3);
        }

        TEST(SortedSetTest, Add_SetOperations_WorkCorrectly) {
            SortedSet<int> set1;
            set1.Add(1);
            set1.Add(2);

            SortedSet<int> set2;
            set2.Add(2);
            set2.Add(3);

            set1.UnionWith(set2);
            EXPECT_EQ(set1.GetCount(), 3);
            EXPECT_TRUE(set1.Contains(3));
        }

        TEST(LinkedListTest, AddFirst_AddLast_Remove_WorkCorrectly) {
            LinkedList<int> list;
            list.AddLast(10);
            list.AddFirst(5);
            list.AddLast(15);

            EXPECT_EQ(list.GetFirst()->Value, 5);
            EXPECT_EQ(list.GetLast()->Value, 15);

            list.Remove(10);
            EXPECT_EQ(list.GetCount(), 2);
            EXPECT_FALSE(list.Contains(10));
        }

        TEST(PriorityQueueTest, Enqueue_MinPriorityOrder_WorksCorrectly) {
            PriorityQueue<String, int> pq;
            pq.Enqueue("TaskLow", 10);
            pq.Enqueue("TaskHigh", 1);
            pq.Enqueue("TaskMedium", 5);

            EXPECT_EQ(pq.Dequeue(), "TaskHigh");
            EXPECT_EQ(pq.Dequeue(), "TaskMedium");
            EXPECT_EQ(pq.Dequeue(), "TaskLow");
        }

    }
}
