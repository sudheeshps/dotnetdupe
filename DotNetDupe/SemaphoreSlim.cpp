#include "pch.h"
#include "System/Threading/SemaphoreSlim.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/TimeoutException.h"
#include <chrono>
#include "System/Char.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            SemaphoreSlim::SemaphoreSlim(int initialCount) : _count(initialCount), _maxCount(2147483647) {}
            SemaphoreSlim::SemaphoreSlim(int initialCount, int maximumCount) : _count(initialCount), _maxCount(maximumCount) {}
            SemaphoreSlim::~SemaphoreSlim() {}

            bool SemaphoreSlim::WaitOne() {
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]() { return _count > 0; });
                --_count;
                return true;
            }

            bool SemaphoreSlim::WaitOne(int millisecondsTimeout) {
                std::unique_lock<std::mutex> lock(_mutex);
                bool result = _cv.wait_for(lock, std::chrono::milliseconds(millisecondsTimeout), [this]() { return _count > 0; });
                if (result) {
                    --_count;
                } else {
                    throw TimeoutException("The wait operation timed out.");
                }
                return result;
            }

            int SemaphoreSlim::Release(int releaseCount) {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_count + releaseCount > _maxCount) {
                    throw SemaphoreFullException("Semaphore count exceeded maximum count.");
                }
                int prev = _count;
                _count += releaseCount;
                for (int i = 0; i < releaseCount; ++i) _cv.notify_one();
                return prev;
            }
            int SemaphoreSlim::GetCurrentCount() const {
                std::lock_guard<std::mutex> lock(_mutex);
                return _count;
            }
        }
    }
}