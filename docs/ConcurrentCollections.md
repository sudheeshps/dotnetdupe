# Concurrent Collections (`DotNetDupe::System::Collections::Concurrent`)

The `System::Collections::Concurrent` namespace provides thread-safe collection classes for multi-threaded C++ applications, matching .NET Base Class Library concurrency primitives.

## Classes Overview

| Class | Description |
| :--- | :--- |
| `ConcurrentDictionary<TKey, TValue>` | Thread-safe dictionary supporting atomic lookup, insertion, removal, and factory update methods. |
| `ConcurrentQueue<T>` | Thread-safe First-In-First-Out (FIFO) collection. |
| `ConcurrentStack<T>` | Thread-safe Last-In-First-Out (LIFO) collection. |
| `ConcurrentBag<T>` | Thread-safe unordered collection of objects. |
| `BlockingCollection<T>` | Thread-safe bounded/unbounded collection supporting blocking producer-consumer patterns. |

---

## 1. `ConcurrentDictionary<TKey, TValue>`

### Methods
- `bool TryAdd(const TKey& key, const TValue& value)`
- `bool TryGetValue(const TKey& key, TValue& value) const`
- `bool TryRemove(const TKey& key, TValue& value)`
- `TValue GetOrAdd(const TKey& key, const TValue& value)`
- `TValue AddOrUpdate(const TKey& key, const TValue& addValue, const TValue& updateValue)`

### Usage Example
```cpp
#include "System/Collections/Concurrent/ConcurrentDictionary.h"
#include "System/String.h"
#include <iostream>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;

void Example() {
    ConcurrentDictionary<String, int> dict;
    dict.TryAdd("Key1", 100);

    int iVal = 0;
    if (dict.TryGetValue("Key1", iVal)) {
        std::cout << "Value: " << iVal << std::endl;
    }
}
```

---

## 2. `ConcurrentQueue<T>`

### Methods
- `void Enqueue(const T& item)`
- `bool TryDequeue(T& result)`
- `bool TryPeek(T& result) const`

### Usage Example
```cpp
#include "System/Collections/Concurrent/ConcurrentQueue.h"

using namespace DotNetDupe::System::Collections::Concurrent;

void Example() {
    ConcurrentQueue<int> queue;
    queue.Enqueue(42);

    int val = 0;
    if (queue.TryDequeue(val)) {
        // Process val
    }
}
```

---

## 3. `ConcurrentStack<T>`

### Methods
- `void Push(const T& item)`
- `bool TryPop(T& result)`
- `bool TryPeek(T& result) const`

### Usage Example
```cpp
#include "System/Collections/Concurrent/ConcurrentStack.h"

using namespace DotNetDupe::System::Collections::Concurrent;

void Example() {
    ConcurrentStack<int> stack;
    stack.Push(42);

    int val = 0;
    if (stack.TryPop(val)) {
        // Process val
    }
}
```

---

## 4. `ConcurrentBag<T>`

### Methods
- `void Add(const T& item)`
- `bool TryTake(T& result)`
- `bool TryPeek(T& result) const`

---

## 5. `BlockingCollection<T>`

### Methods
- `void Add(const T& item)`
- `bool TryAdd(const T& item, int timeoutMs = 0)`
- `T Take()`
- `bool TryTake(T& item, int timeoutMs = 0)`
- `void CompleteAdding()`
- `bool IsCompleted() const`

### Producer-Consumer Example
```cpp
#include "System/Collections/Concurrent/BlockingCollection.h"
#include "System/Threading/Thread.h"

using namespace DotNetDupe::System::Collections::Concurrent;
using namespace DotNetDupe::System::Threading;

void ProducerConsumerExample() {
    BlockingCollection<int> col(10); // Bounded capacity

    Thread producer([&col]() {
        for (int i = 1; i <= 5; i++) {
            col.Add(i);
        }
        col.CompleteAdding();
    });

    producer.Start();

    int item = 0;
    while (col.TryTake(item, -1)) {
        // Process item
    }

    producer.Join();
}
```
