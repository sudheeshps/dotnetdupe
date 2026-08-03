#include "System/Console.h"
#include "System/String.h"
#include "System/Collections/Generic/Queue.h"
#include "System/Collections/Generic/Stack.h"
#include "System/Collections/Generic/HashSet.h"
#include "System/Collections/Generic/SortedDictionary.h"
#include "System/Collections/Generic/SortedSet.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/Collections/Generic/PriorityQueue.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

void RunGenericCollectionsDemo() {
    Console::WriteLine("=== System.Collections.Generic Expanded Demo ===");

    // Queue
    Queue<String> q;
    q.Enqueue("First");
    q.Enqueue("Second");
    Console::WriteLine("Queue Dequeued: {0}", q.Dequeue());

    // Stack
    Stack<String> s;
    s.Push("Page1");
    s.Push("Page2");
    Console::WriteLine("Stack Popped: {0}", s.Pop());

    // HashSet
    HashSet<String> set;
    set.Add("User1");
    set.Add("User2");
    Console::WriteLine("HashSet Contains 'User1': {0}", set.Contains("User1") ? "True" : "False");

    // SortedDictionary
    SortedDictionary<int, String> sortedDict;
    sortedDict.Add(102, "Bob");
    sortedDict.Add(101, "Alice");
    Console::WriteLine("SortedDictionary First Key: {0} ({1})", sortedDict.GetKeys()[0], sortedDict[sortedDict.GetKeys()[0]]);

    // SortedSet
    SortedSet<int> sortedSet;
    sortedSet.Add(50);
    sortedSet.Add(10);
    sortedSet.Add(30);
    Console::WriteLine("SortedSet First Element: {0}", sortedSet.ToArray()[0]);

    // LinkedList
    LinkedList<String> list;
    list.AddLast("Node1");
    list.AddLast("Node2");
    Console::WriteLine("LinkedList First: {0}", list.GetFirst()->Value);

    // PriorityQueue
    PriorityQueue<String, int> pq;
    pq.Enqueue("Critical Bug", 1);
    pq.Enqueue("Feature Request", 10);
    Console::WriteLine("PriorityQueue Top Priority Task: {0}", pq.Dequeue());

    Console::WriteLine("Generic collections demo completed successfully!");
}
