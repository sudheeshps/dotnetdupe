### class `Task`

Represents an asynchronous operation.

#### Methods

##### `static SmartPointer<Task> Run(Action<> objAction)`
Queues the specified work to run on the ThreadPool and returns a Task object that represents that work.

**Usage:**
```cpp
auto pTask = Task::Run(Action<>([]() {
    Console::WriteLine("Task is running in background.");
}));
pTask->Wait();
```

##### `void Start()`
Starts the Task, scheduling it for execution to the current TaskScheduler (mapped to ThreadPool).

##### `void Wait()`
Waits for the Task to complete execution.

##### `bool Wait(int iMillisecondsTimeout)`
Waits for the Task to complete execution within a specified time interval.

##### `TaskStatus GetStatus()`
Gets the `TaskStatus` of this task.

##### `bool GetIsCompleted()`
Gets whether the task has completed (successfully, faulted, or canceled).

##### `bool GetIsFaulted()`
Gets whether the task completed due to an unhandled exception.

##### `bool GetIsCanceled()`
Gets whether this Task instance has completed execution due to being canceled.

---

### enum class `TaskStatus`
Represents the current stage in the lifecycle of a Task.

- `Created`: The task has been initialized but has not yet been scheduled.
- `WaitingToRun`: The task has been scheduled for execution but has not yet begun executing.
- `Running`: The task is running but has not yet completed.
- `RanToCompletion`: The task completed execution successfully.
- `Canceled`: The task acknowledged cancellation by throwing OperationCanceledException.
- `Faulted`: The task completed due to an unhandled exception.
