# Stack<T> (`DotNetDupe::System::Collections::Generic`)

Represents a variable size last-in-first-out (LIFO) collection of instances of the same specified type. This implementation utilizes `List<T>` as the underlying storage, providing fast and ABI-safe operations without exposing STL internals.

## API Summary

```cpp
template <typename T>
class Stack : public Object {
public:
    Stack();

    int GetCount() const;
    
    void Push(const T& item);
    T Pop();
    T Peek() const;
    
    bool TryPop(T& result);
    bool TryPeek(T& result) const;
    
    void Clear();
    bool Contains(const T& item) const;
    Array<T> ToArray() const;
};
```

## Example Usage

```cpp
#include "System/Collections/Generic/Stack.h"
#include "System/String.h"
#include "System/Console.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

void StackDemo() {
    Stack<String> stack;
    
    // Push items onto the stack
    stack.Push("First");
    stack.Push("Second");
    stack.Push("Third");
    
    Console::WriteLine(String::Format("Count after push: {0}", stack.GetCount()));
    
    // Peek at the top item
    String topItem = stack.Peek();
    Console::WriteLine(String::Format("Top item: {0}", topItem));
    
    // Pop items
    while (stack.GetCount() > 0) {
        String popped = stack.Pop();
        Console::WriteLine(String::Format("Popped: {0}", popped));
    }
}
```
