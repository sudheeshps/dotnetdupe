# Task &amp; TaskStatus

**Namespace:** `DotNetDupe::System::Threading::Tasks`  
**Header:** `#include "System/Threading/Tasks/Task.h"`

Represents an asynchronous operation that can be scheduled, queried for lifecycle state, and waited on for completion.

---

## `TaskStatus` Enum

```cpp
enum class TaskStatus {
    Created,
    WaitingToRun,
    Running,
    RanToCompletion,
    Canceled,
    Faulted
};
```

---

## `Task` Class

### Syntax
```cpp
class Task : public Object;
```

---

## Constructors

### `Task(Action<> objAction)`
Initializes a new `Task` with the specified action delegate.

---

## Member Functions

### `void Start()`
Starts the `Task`, scheduling it for execution on the `ThreadPool`.

### `void Wait()`
Waits for the `Task` to complete execution synchronously.

### `bool Wait(int iMillisecondsTimeout)`
Waits for the `Task` to complete execution within a specified time interval.
- **Returns:**
  - `bool`: `true` if the `Task` completed execution within the allotted time; otherwise, `false`.

### `TaskStatus GetStatus() const`
Gets the `TaskStatus` lifecycle state of this task.

### `bool GetIsCompleted() const`
Gets whether this `Task` has completed (RanToCompletion, Faulted, or Canceled).

### `bool GetIsFaulted() const`
Gets whether the `Task` completed due to an unhandled exception.

### `bool GetIsCanceled() const`
Gets whether this `Task` completed execution due to being canceled.

---

## Static Methods

### `static SmartPointer<Task> Run(Action<> objAction)`
Queues the specified work to run on the `ThreadPool` and returns a `SmartPointer<Task>` representing that work.

```cpp
auto spTask = Task::Run([]() {
    Console::WriteLine("Running background computation");
});
spTask->Wait();
```

---

## Example

```cpp
#include "System/Console.h"
#include "System/Threading/Tasks/Task.h"
#include "System/Threading/Thread.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::Threading::Tasks;

int main() {
    Console::WriteLine("Dispatching async task...");

    auto spTask = Task::Run([]() {
        Console::WriteLine("Task is executing on background thread pool.");
        Thread::Sleep(100);
        Console::WriteLine("Task execution finished.");
    });

    Console::WriteLine("Is Completed before wait: {0}", spTask->GetIsCompleted());

    spTask->Wait();

    Console::WriteLine("Is Completed after wait: {0}", spTask->GetIsCompleted());
    Console::WriteLine("Task Status: {0}", (spTask->GetStatus() == TaskStatus::RanToCompletion ? "RanToCompletion" : "Other"));

    return 0;
}
```
