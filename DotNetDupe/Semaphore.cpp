#include "pch.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/TimeoutException.h"
#include <chrono>
#include <tchar.h>

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            Semaphore::Semaphore(int initialCount, int maximumCount) : _count(initialCount), _maxCount(maximumCount) {}
            Semaphore::~Semaphore() {}
            bool Semaphore::WaitOne() {
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]() { return _count > 0; });
                --_count;
                return true;
            }
            bool Semaphore::WaitOne(int millisecondsTimeout) {
                std::unique_lock<std::mutex> lock(_mutex);
                bool result = _cv.wait_for(lock, std::chrono::milliseconds(millisecondsTimeout), [this]() { return _count > 0; });
                if (result) {
                    --_count;
                } else {
                    throw TimeoutException(_T("The wait operation timed out."));
                }
                return result;
            }

            int Semaphore::Release(int releaseCount) {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_count + releaseCount > _maxCount) {
                    throw SemaphoreFullException(_T("Semaphore count exceeded maximum count."));
                }
                int prev = _count;
                _count += releaseCount;
                for (int i = 0; i < releaseCount; ++i) _cv.notify_one();
                return prev;
            }
        }
    }
}