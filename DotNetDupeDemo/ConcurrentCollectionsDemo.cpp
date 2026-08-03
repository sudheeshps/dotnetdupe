#include "System/Console.h"
#include "System/String.h"
#include "System/Collections/Concurrent/ConcurrentDictionary.h"
#include "System/Collections/Concurrent/ConcurrentQueue.h"
#include "System/Collections/Concurrent/ConcurrentStack.h"
#include "System/Collections/Concurrent/ConcurrentBag.h"
#include "System/Collections/Concurrent/BlockingCollection.h"
#include "System/Threading/Thread.h"
#include <vector>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

void RunConcurrentCollectionsDemo() {
    Console::WriteLine("=== System.Collections.Concurrent Demo ===");

    // 1. ConcurrentDictionary
    ConcurrentDictionary<String, int> dict;
    dict.TryAdd("Apple", 5);
    dict.TryAdd("Banana", 12);
    dict.GetOrAdd("Cherry", 20);

    Console::WriteLine("ConcurrentDictionary items count: {0}", dict.GetCount());
    int iAppleCount = 0;
    if (dict.TryGetValue("Apple", iAppleCount)) {
        Console::WriteLine("Apple count: {0}", iAppleCount);
    }

    // 2. ConcurrentQueue
    ConcurrentQueue<String> queue;
    queue.Enqueue("Job1");
    queue.Enqueue("Job2");

    String sJob;
    if (queue.TryDequeue(sJob)) {
        Console::WriteLine("Dequeued from ConcurrentQueue: {0}", sJob);
    }

    // 3. ConcurrentStack
    ConcurrentStack<String> stack;
    stack.Push("Undo1");
    stack.Push("Undo2");

    String sUndo;
    if (stack.TryPop(sUndo)) {
        Console::WriteLine("Popped from ConcurrentStack: {0}", sUndo);
    }

    // 4. ConcurrentBag
    ConcurrentBag<int> bag;
    bag.Add(100);
    bag.Add(200);

    int iBagItem = 0;
    if (bag.TryTake(iBagItem)) {
        Console::WriteLine("Took from ConcurrentBag: {0}", iBagItem);
    }

    // 5. BlockingCollection (Producer-Consumer)
    BlockingCollection<int> boundedCol(3);
    Console::WriteLine("Starting BlockingCollection Producer/Consumer...");

    Thread producer([&boundedCol]() {
        for (int i = 1; i <= 5; i++) {
            boundedCol.Add(i);
        }
        boundedCol.CompleteAdding();
    });

    producer.Start();

    int item = 0;
    while (boundedCol.TryTake(item, -1)) {
        Console::WriteLine("Consumed item from BlockingCollection: {0}", item);
    }

    producer.Join();
    Console::WriteLine("Concurrent collections demo completed successfully!");
}
