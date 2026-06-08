### class `ThreadPool`

Provides a pool of threads that can be used to execute tasks, post work items, process asynchronous I/O, wait on behalf of other threads, and process timers.

#### Methods

##### `static bool QueueUserWorkItem(WaitCallback callback)`

Queues a method for execution. The method executes when a thread pool thread becomes available.

**Usage:**
```cpp
ThreadPool::QueueUserWorkItem(WaitCallback([](Object* state) {
    Console::WriteLine("Running on thread pool...");
}));
```

##### `static bool QueueUserWorkItem(WaitCallback callback, Object* pState)`

Queues a method for execution, and specifies an object containing data to be used by the method. The method executes when a thread pool thread becomes available.

**Usage:**
```cpp
Object* pData = reinterpret_cast<Object*>(42);
ThreadPool::QueueUserWorkItem(WaitCallback([](Object* state) {
    int iVal = (int)(intptr_t)state;
    Console::Write("Received value: ");
    Console::WriteLine(iVal);
}), pData);
```

#### Related Types

##### `typedef Action<Object*> WaitCallback`

Represents a callback method to be executed by a thread pool thread.
