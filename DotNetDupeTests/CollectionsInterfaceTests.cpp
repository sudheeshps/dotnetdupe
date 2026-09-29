#include "pch.h"
#include "gtest/gtest.h"
#include "System/Linq.h"
#include "System/Array.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/Collections/Generic/HashSet.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Collections/Generic/Queue.h"
#include "System/Collections/Generic/Stack.h"
#include "System/Collections/Generic/SortedSet.h"
#include "System/Collections/Generic/SortedDictionary.h"
#include "System/Collections/Generic/IEnumerable.h"
#include "System/Collections/Generic/ICollection.h"
#include "System/Collections/Generic/IList.h"
#include "System/Collections/Generic/ISet.h"
#include "System/Collections/Generic/IDictionary.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

namespace DotNetDupeTests {

    static int SumSequence(const IEnumerable<int>& seq) {
        int total = 0;
        for (int val : seq) {
            total += val;
        }
        return total;
    }

    static int GetCollectionSize(const IReadOnlyCollection<int>& col) {
        return col.GetCount();
    }

    static void MutateList(IList<int>& list) {
        list.Add(99);
    }

    // 1. Polymorphic IEnumerable Passing
    TEST(CollectionsInterfaceTests, GivenVariousCollections_WhenPassedAsIEnumerable_ThenIteratesPolymorphically) {
        List<int> list = { 1, 2, 3 };
        EXPECT_EQ(SumSequence(list), 6);

        Array<int> arr(3);
        arr[0] = 4; arr[1] = 5; arr[2] = 6;
        EXPECT_EQ(SumSequence(arr), 15);

        LinkedList<int> linkedList;
        linkedList.AddLast(10);
        linkedList.AddLast(20);
        EXPECT_EQ(SumSequence(linkedList), 30);

        HashSet<int> hashSet;
        hashSet.Add(100);
        hashSet.Add(200);
        EXPECT_EQ(SumSequence(hashSet), 300);

        Queue<int> queue;
        queue.Enqueue(7);
        queue.Enqueue(8);
        EXPECT_EQ(SumSequence(queue), 15);

        Stack<int> stack;
        stack.Push(30);
        stack.Push(70);
        EXPECT_EQ(SumSequence(stack), 100);

        SortedSet<int> sortedSet;
        sortedSet.Add(2);
        sortedSet.Add(8);
        EXPECT_EQ(SumSequence(sortedSet), 10);
    }

    // 2. Polymorphic IReadOnlyCollection Passing
    TEST(CollectionsInterfaceTests, GivenVariousCollections_WhenPassedAsIReadOnlyCollection_ThenReportsCount) {
        List<int> list = { 10, 20, 30, 40 };
        EXPECT_EQ(GetCollectionSize(list), 4);

        Array<int> arr(2);
        EXPECT_EQ(GetCollectionSize(arr), 2);

        LinkedList<int> linkedList;
        linkedList.AddLast(1);
        EXPECT_EQ(GetCollectionSize(linkedList), 1);

        HashSet<int> hashSet;
        hashSet.Add(1); hashSet.Add(2); hashSet.Add(3);
        EXPECT_EQ(GetCollectionSize(hashSet), 3);

        Queue<int> queue;
        queue.Enqueue(100);
        EXPECT_EQ(GetCollectionSize(queue), 1);

        Stack<int> stack;
        stack.Push(10); stack.Push(20);
        EXPECT_EQ(GetCollectionSize(stack), 2);
    }

    // 3. Polymorphic IList Passing
    TEST(CollectionsInterfaceTests, GivenListAndArray_WhenPassedAsIList_ThenAccessesByIndex) {
        List<int> list = { 10, 20, 30 };
        IList<int>& listRef = list;
        EXPECT_EQ(listRef[1], 20);
        EXPECT_EQ(listRef.IndexOf(30), 2);
        MutateList(listRef);
        EXPECT_EQ(list.GetCount(), 4);
        EXPECT_EQ(list[3], 99);

        Array<int> arr(3);
        arr[0] = 5; arr[1] = 10; arr[2] = 15;
        const IList<int>& arrRef = arr;
        EXPECT_EQ(arrRef[0], 5);
        EXPECT_EQ(arrRef.IndexOf(15), 2);
    }

    // 4. Polymorphic ISet Passing
    TEST(CollectionsInterfaceTests, GivenHashSetAndSortedSet_WhenPassedAsISet_ThenPerformsSetOperations) {
        HashSet<int> hashSet;
        ISet<int>& setRef = hashSet;
        EXPECT_TRUE(setRef.Add(1));
        EXPECT_FALSE(setRef.Add(1));
        EXPECT_TRUE(setRef.Contains(1));

        SortedSet<int> sortedSet;
        ISet<int>& sortedRef = sortedSet;
        EXPECT_TRUE(sortedRef.Add(5));
        EXPECT_TRUE(sortedRef.Add(2));
        EXPECT_EQ(sortedRef.GetCount(), 2);
    }

    // 5. Polymorphic IDictionary Passing
    TEST(CollectionsInterfaceTests, GivenDictionaryAndSortedDictionary_WhenPassedAsIDictionary_ThenAccessesKeyValuePairs) {
        Dictionary<int, String> dict;
        dict.Add(1, "One");
        dict.Add(2, "Two");

        IDictionary<int, String>& dictRef = dict;
        EXPECT_TRUE(dictRef.ContainsKey(1));
        EXPECT_TRUE(dictRef[2] == "Two");
        EXPECT_EQ(dictRef.GetCount(), 2);

        SortedDictionary<int, String> sortedDict;
        sortedDict.Add(10, "Ten");
        sortedDict.Add(5, "Five");

        const IReadOnlyDictionary<int, String>& readOnlyRef = sortedDict;
        EXPECT_TRUE(readOnlyRef.ContainsKey(5));
        EXPECT_TRUE(readOnlyRef[10] == "Ten");
        EXPECT_EQ(readOnlyRef.GetCount(), 2);
    }

    // 6. Direct Native LINQ Queries Across Diverse Containers Without AsEnumerable
    TEST(CollectionsInterfaceTests, GivenDiverseContainers_WhenLinqCalledDirectly_ThenExecutesSeamlessly) {
        // List LINQ
        List<int> list = { 1, 2, 3, 4, 5 };
        auto listEvens = list.Where([](int n) { return n % 2 == 0; }).ToList();
        EXPECT_EQ(listEvens.GetCount(), 2);

        // Array LINQ
        Array<int> arr(4);
        arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40;
        auto arrDoubled = arr.Select([](int n) { return n * 2; }).ToArray();
        EXPECT_EQ(arrDoubled.GetLength(), 4);
        EXPECT_EQ(arrDoubled[0], 20);

        // LinkedList LINQ
        LinkedList<int> linkedList;
        linkedList.AddLast(5); linkedList.AddLast(15); linkedList.AddLast(25);
        auto linkedFiltered = linkedList.Where([](int n) { return n > 10; }).ToList();
        EXPECT_EQ(linkedFiltered.GetCount(), 2);

        // HashSet LINQ
        HashSet<int> hashSet;
        hashSet.Add(100); hashSet.Add(200); hashSet.Add(300);
        EXPECT_EQ(hashSet.Sum(), 600);

        // Queue LINQ
        Queue<int> queue;
        queue.Enqueue(3); queue.Enqueue(1); queue.Enqueue(2);
        auto sortedQueue = queue.OrderBy([](int n) { return n; }).ToList();
        EXPECT_EQ(sortedQueue[0], 1);
        EXPECT_EQ(sortedQueue[2], 3);

        // Stack LINQ
        Stack<int> stack;
        stack.Push(10); stack.Push(20); stack.Push(30);
        auto stackTaken = stack.Take(2).ToList();
        EXPECT_EQ(stackTaken.GetCount(), 2);
        EXPECT_EQ(stackTaken[0], 30); // LIFO order
        EXPECT_EQ(stackTaken[1], 20);

        // Dictionary LINQ
        Dictionary<int, String> dict;
        dict.Add(1, "Alpha"); dict.Add(2, "Beta"); dict.Add(3, "Gamma");
        auto matchingKeys = dict
            .Where([](const auto& pair) { return pair.Key >= 2; })
            .Select([](const auto& pair) { return pair.Value; })
            .ToList();
        EXPECT_EQ(matchingKeys.GetCount(), 2);
    }

} // namespace DotNetDupeTests
