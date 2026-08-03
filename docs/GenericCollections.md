# Generic Collections (`DotNetDupe::System::Collections::Generic`)

The `System::Collections::Generic` namespace provides standard strongly-typed data structures matching the .NET Base Class Library collections API.

## Available Classes

| Class | Description |
| :--- | :--- |
| `List<T>` | Represents a strongly typed list of objects that can be accessed by index. |
| `Dictionary<TKey, TValue>` | Represents a collection of keys and values. |
| `Queue<T>` | Represents a First-In-First-Out (FIFO) collection of instances. |
| `Stack<T>` | Represents a Last-In-First-Out (LIFO) collection of instances. |
| `HashSet<T>` | Represents a set of unique values using a hash table. |
| `SortedDictionary<TKey, TValue>` | Represents a collection of key/value pairs that are sorted on the key. |
| `SortedSet<T>` | Represents a collection of objects that is maintained in sorted order. |
| `LinkedList<T>` | Represents a doubly linked list. |
| `PriorityQueue<TElement, TPriority>` | Represents a collection of items that have a value and a priority (min-heap order). |

---

## Usage Examples

### 1. `Queue<T>` & `Stack<T>`
```cpp
#include "System/Collections/Generic/Queue.h"
#include "System/Collections/Generic/Stack.h"

using namespace DotNetDupe::System::Collections::Generic;

void ExampleQueueStack() {
    Queue<int> q;
    q.Enqueue(10);
    q.Enqueue(20);
    int first = q.Dequeue(); // 10

    Stack<int> s;
    s.Push(100);
    s.Push(200);
    int top = s.Pop(); // 200
}
```

### 2. `HashSet<T>` & `SortedSet<T>`
```cpp
#include "System/Collections/Generic/HashSet.h"
#include "System/Collections/Generic/SortedSet.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

void ExampleSets() {
    HashSet<String> hashSet;
    hashSet.Add("Item1");
    hashSet.Add("Item2");

    SortedSet<int> sortedSet;
    sortedSet.Add(30);
    sortedSet.Add(10);
    sortedSet.Add(20); // Retains elements in sorted order: [10, 20, 30]
}
```

### 3. `LinkedList<T>` & `PriorityQueue<TElement, TPriority>`
```cpp
#include "System/Collections/Generic/LinkedList.h"
#include "System/Collections/Generic/PriorityQueue.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

void ExampleLinkedListAndPQ() {
    LinkedList<String> list;
    list.AddLast("Node1");
    list.AddFirst("HeaderNode");

    PriorityQueue<String, int> pq;
    pq.Enqueue("Low Priority Task", 10);
    pq.Enqueue("High Priority Task", 1);
    String topTask = pq.Dequeue(); // "High Priority Task"
}
```
