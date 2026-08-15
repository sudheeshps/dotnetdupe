# Queue<T> (`DotNetDupe::System::Collections::Generic`)

Represents a first-in, first-out (FIFO) collection of objects. This implementation utilizes `List<T>` as the underlying storage, providing an ABI-safe implementation suitable for dynamic boundary passage.

## API Summary

```cpp
template <typename T>
class Queue : public Object {
public:
    Queue();

    int GetCount() const;
    
    void Enqueue(const T& item);
    T Dequeue();
    T Peek() const;
    
    bool TryDequeue(T& result);
    bool TryPeek(T& result) const;
    
    void Clear();
    bool Contains(const T& item) const;
    Array<T> ToArray() const;
};
```

## Example Usage

```cpp
#include "System/Collections/Generic/Queue.h"
#include "System/String.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

void QueueDemo() {
    Queue<String> queue;
    
    // Enqueue items
    queue.Enqueue("First in line");
    queue.Enqueue("Second in line");
    queue.Enqueue("Third in line");
    
    Console::WriteLine(String::Format("Count after enqueue: {0}", queue.GetCount()));
    
    // Peek at the first item
    String firstItem = queue.Peek();
    Console::WriteLine(String::Format("First item: {0}", firstItem));
    
    // Dequeue items
    while (queue.GetCount() > 0) {
        String dequeued = queue.Dequeue();
        Console::WriteLine(String::Format("Dequeued: {0}", dequeued));
    }
}
```
