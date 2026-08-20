# EventHandler & EventArgs

**Namespace:** `DotNetDupe::System`  
**Headers:** `#include "System/EventArgs.h"`, `#include "System/EventHandler.h"`

Provides the foundational types for event-driven programming in DotNetDupe, replicating the familiar and powerful C# `EventHandler` delegate and `EventArgs` class model.

---

## Overview

In .NET and C#, event communication is modeled using delegates and event argument payloads:
- **`EventArgs`**: The base class for classes that contain event data. It provides the static singleton `EventArgs::Empty()` for events that convey no payload data.
- **`EventHandler<TEventArgs>`**: A multicast delegate representing the method that will handle an event with signature `void(const void* pSender, const TEventArgs& e)`.

`EventHandler` supports:
- **Multicast Subscriptions**: Multiple listeners can register with `operator+=` or `Add()`.
- **Token-Based Unsubscription**: Subscribing returns a unique `size_t` token that can be passed to `operator-=` or `Remove()`.
- **Flexible Target Binding**: Supports lambdas, free functions, and class member functions.
- **Natural Invocation**: Callable via `Invoke(pSender, e)` or `operator()(pSender, e)`.

---

## Syntax

```cpp
namespace DotNetDupe::System {
    // Base class for event data
    class EventArgs : public virtual Object;

    // Multicast delegate template (defaults to EventArgs)
    template <typename TEventArgs = EventArgs>
    class EventHandler;
}
```

---

## EventArgs API Reference

### `EventArgs()`
Default constructor. Constructs an `EventArgs` instance.

### `static const EventArgs& Empty()`
Gets a value that represents an event with no event data.

```cpp
const EventArgs& empty = EventArgs::Empty();
```

---

## EventHandler<TEventArgs> API Reference

### Subscriptions

#### `size_t Add(const Action<const void*, const TEventArgs&>& fnHandler)`
#### `template <typename F> size_t Add(F&& fnHandler)`
Subscribes a callable (lambda, closure, or free function) to the event.

- **Parameters:**
  - `fnHandler`: The callback to invoke when the event is raised.
- **Returns:**
  - `size_t`: A unique subscription token used to unsubscribe later.

#### `template <typename TClass> size_t Add(TClass* pInstance, void (TClass::*pMethod)(const void*, const TEventArgs&))`
Subscribes a class member method to the event.

- **Parameters:**
  - `pInstance`: Pointer to the class instance.
  - `pMethod`: Pointer to the member method.
- **Returns:**
  - `size_t`: A unique subscription token.

#### `size_t operator+=(const Action<const void*, const TEventArgs&>& fnHandler)`
#### `template <typename F> size_t operator+=(F&& fnHandler)`
Operator syntax for subscribing a callback. Equivalent to `Add()`.

```cpp
size_t token = (button.Click += [](const void* pSender, const EventArgs& e) {
    Console::WriteLine("Button clicked!");
});
```

---

### Unsubscription & Management

#### `bool Remove(size_t nToken)`
#### `bool operator-=(size_t nToken)`
Unsubscribes the listener associated with the specified subscription token.

- **Parameters:**
  - `nToken` (`size_t`): The token returned during subscription.
- **Returns:**
  - `bool`: `true` if the subscriber was found and removed; otherwise, `false`.

```cpp
button.Click -= token;
```

#### `void Clear()`
Removes all registered subscribers from the event.

```cpp
button.Click.Clear();
```

---

### Invocation & Diagnostics

#### `void Invoke(const void* pSender, const TEventArgs& e) const`
#### `void operator()(const void* pSender, const TEventArgs& e) const`
Sequentially invokes all subscribed handlers in FIFO order. If no subscribers are registered, this is a safe no-op.

```cpp
button.Click.Invoke(&button, EventArgs::Empty());
// Or using call operator:
button.Click(&button, EventArgs::Empty());
```

#### `bool IsEmpty() const`
Determines whether the event has no active subscribers.

#### `size_t GetSubscriberCount() const`
Returns the total number of registered subscribers.

#### `explicit operator bool() const`
Returns `true` if there is at least one active subscriber (`!IsEmpty()`).

---

## Real-World Example

```cpp
#include "System/Console.h"
#include "System/String.h"
#include "System/EventArgs.h"
#include "System/EventHandler.h"

using namespace DotNetDupe::System;

// 1. Define custom EventArgs
class FileUploadProgressEventArgs : public EventArgs {
public:
    FileUploadProgressEventArgs(const String& sFileName, int iPercent)
        : m_sFileName(sFileName), m_iPercent(iPercent) {}

    String GetFileName() const { return m_sFileName; }
    int GetPercent() const { return m_iPercent; }

private:
    String m_sFileName;
    int m_iPercent;
};

// 2. Define event publisher
class Uploader : public virtual Object {
public:
    EventHandler<FileUploadProgressEventArgs> ProgressChanged;
    EventHandler<> UploadCompleted;

    void Upload(const String& sFileName) {
        Console::WriteLine("Starting upload for: " + sFileName);

        for (int i = 25; i <= 100; i += 25) {
            FileUploadProgressEventArgs args(sFileName, i);
            ProgressChanged.Invoke(this, args);
        }

        UploadCompleted(this, EventArgs::Empty());
    }
};

// 3. Define subscriber class
class UploadMonitor {
public:
    void OnProgress(const void* pSender, const FileUploadProgressEventArgs& e) {
        (void)pSender;
        Console::WriteLine(String::Format("  [Monitor] {0} -> {1}%", e.GetFileName(), e.GetPercent()));
    }
};

int main() {
    Uploader uploader;
    UploadMonitor monitor;

    // Subscribe member method
    uploader.ProgressChanged.Add(&monitor, &UploadMonitor::OnProgress);

    // Subscribe lambda
    size_t token = (uploader.UploadCompleted += [](const void* pSender, const EventArgs& e) {
        (void)pSender; (void)e;
        Console::WriteLine("  [Main] File upload finished successfully!");
    });

    // Run operation
    uploader.Upload("Dataset.csv");

    // Unsubscribe
    uploader.UploadCompleted -= token;

    return 0;
}
```
