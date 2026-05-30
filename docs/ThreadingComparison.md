# Threading Comparison: STL vs. DotNetDupe

This guide provides a side-by-side comparison of common threading tasks using the standard C++ library (STL) and DotNetDupe.

---

## 1. Thread Management

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <thread>` | `#include "System/Threading/Thread.h"` |
| **Creation** | `std::thread t(workerFunc);` | `Thread t(workerFunc); t.Start();` |
| **Joining** | `t.join();` | `t.Join();` |
| **Join w/ Timeout**| Requires manual `std::condition_variable` signaling. | `t.Join(1000); // Throws TimeoutException` |
| **Sleep** | `std::this_thread::sleep_for(std::chrono::milliseconds(100));` | `Thread::Sleep(100);` |
| **Current Thread**| N/A (direct access) | `Thread* current = Thread::GetCurrentThread();` |

### Example
**STL:**
```cpp
#include <thread>

void Worker() { /* ... */ }

int main() {
    std::thread t(Worker);
    t.join();
}
```

**DotNetDupe:**
```cpp
#include "System/Threading/Thread.h"

void Worker() { /* ... */ }

int main() {
    Thread t(Worker);
    t.Start();
    t.Join();
}
```

---

## 2. Mutex & Exclusive Locking

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <mutex>` | `#include "System/Threading/Mutex.h"` |
| **Class** | `std::mutex` | `Mutex` |
| **Lock** | `m.lock();` | `m.WaitOne();` |
| **Unlock** | `m.unlock();` | `m.Release();` |
| **RAII Wrapper** | `std::lock_guard<std::mutex> lock(m);` | `MutexLock lock(m);` |

---

## 7. RAII Scoped Locking

| Standard C++ (STL) | DotNetDupe |
| :--- | :--- |
| `std::lock_guard<std::mutex> lock(m);` | `MutexLock lock(m);` |
| `std::unique_lock<std::mutex> lock(m, std::defer_lock);` | `MutexLock lock(m, 100); // with timeout` |

### Example
**STL:**
```cpp
#include <mutex>
std::mutex m;

void Work() {
    std::lock_guard<std::mutex> lock(m);
    // safe
}
```

**DotNetDupe:**
```cpp
#include "System/Threading/Lock.h"
Mutex m;

void Work() {
    MutexLock lock(m);
    // safe
}
```

### Example
**STL:**
```cpp
std::mutex m;
m.lock();
// shared resource
m.unlock();
```

**DotNetDupe:**
```cpp
Mutex m;
m.WaitOne();
// shared resource
m.Release();
```

---

## 3. Critical Section (User Mode)

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <mutex>` | `#include "System/Threading/CriticalSection.h"` |
| **Class** | `std::recursive_mutex` | `CriticalSection` |
| **Enter** | `m.lock();` | `cs.Enter();` |
| **Leave** | `m.unlock();` | `cs.Leave();` |

### Example
**STL:**
```cpp
std::recursive_mutex rm;
rm.lock();
// work
rm.unlock();
```

**DotNetDupe:**
```cpp
CriticalSection cs;
cs.Enter();
// work
cs.Leave();
```

---

## 4. Signaling Events (Manual & Auto Reset)

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <condition_variable>` | `#include "System/Threading/ManualResetEvent.h"` |
| **Wait** | `cv.wait(lk, []{ return signaled; });` | `mre.WaitOne();` |
| **Wait w/ Timeout**| `cv.wait_for(...)` | `mre.WaitOne(1000); // Throws TimeoutException` |
| **Signal** | `cv.notify_all();` | `mre.Set();` |
| **Reset** | `signaled = false;` | `mre.Reset();` |

### Example
**STL:**
```cpp
std::mutex m;
std::condition_variable cv;
bool signaled = false;

// Waiter
std::unique_lock<std::mutex> lk(m);
cv.wait(lk, [&]{ return signaled; });

// Signaler
{
    std::lock_guard<std::mutex> lk(m);
    signaled = true;
}
cv.notify_all();
```

**DotNetDupe:**
```cpp
ManualResetEvent mre(false);

// Waiter
mre.WaitOne();

// Signaler
mre.Set();
```

---

## 5. Semaphores

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <semaphore>` (C++20) | `#include "System/Threading/Semaphore.h"` |
| **Wait** | `s.acquire();` | `s.WaitOne();` |
| **Release** | `s.release();` | `s.Release();` |

### Example
**STL:**
```cpp
std::counting_semaphore<10> s(2);
s.acquire();
// ...
s.release();
```

**DotNetDupe:**
```cpp
Semaphore s(2, 10);
s.WaitOne();
// ...
s.Release();
```

---

## 6. Atomic Operations (Interlocked)

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <atomic>` | `#include "System/Threading/Interlocked.h"` |
| **Variable** | `std::atomic<int> counter;` | `Interlocked<int> counter;` |
| **Increment** | `++counter;` | `counter++;` |
| **Add** | `counter += 5;` | `counter += 5;` |
| **Exchange** | `counter.exchange(val);` | `counter.Exchange(val);` |

### Example
**STL:**
```cpp
#include <atomic>

std::atomic<int> counter = 0;
counter++;
counter += 10;
```

**DotNetDupe:**
```cpp
#include "System/Threading/Interlocked.h"

Interlocked<int> counter = 0;
counter++;  // Atomic increment via operator++
counter += 10; // Atomic add via operator+=
```

