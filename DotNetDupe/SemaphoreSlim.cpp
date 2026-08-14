#include "pch.h"
#include "System/Threading/SemaphoreSlim.h"
#include "System/Threading/SemaphoreFullException.h"
#include "System/TimeoutException.h"
#include "System/Char.h"
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            struct SemaphoreSlim::Impl {
                std::mutex mutex;
                std::condition_variable cv;
            };

            SemaphoreSlim::SemaphoreSlim(int initialCount)
                : _count(initialCount), _maxCount(2147483647), _pImpl(new Impl()) {}

            SemaphoreSlim::SemaphoreSlim(int initialCount, int maximumCount)
                : _count(initialCount), _maxCount(maximumCount), _pImpl(new Impl()) {}

            SemaphoreSlim::~SemaphoreSlim() {
                if (_pImpl) {
                    delete _pImpl;
                    _pImpl = nullptr;
                }
            }

            bool SemaphoreSlim::WaitOne() {
                if (!_pImpl) return false;
                std::unique_lock<std::mutex> lock(_pImpl->mutex);
                _pImpl->cv.wait(lock, [this]() { return _count > 0; });
                --_count;
                return true;
            }

            bool SemaphoreSlim::WaitOne(int millisecondsTimeout) {
                if (!_pImpl) return false;
                std::unique_lock<std::mutex> lock(_pImpl->mutex);
                bool result = _pImpl->cv.wait_for(lock, std::chrono::milliseconds(millisecondsTimeout), [this]() { return _count > 0; });
                if (result) {
                    --_count;
                } else {
                    throw TimeoutException("The wait operation timed out.");
                }
                return result;
            }

            int SemaphoreSlim::Release(int releaseCount) {
                if (!_pImpl) return 0;
                std::lock_guard<std::mutex> lock(_pImpl->mutex);
                if (_count + releaseCount > _maxCount) {
                    throw SemaphoreFullException("Semaphore count exceeded maximum count.");
                }
                int prev = _count;
                _count += releaseCount;
                for (int i = 0; i < releaseCount; ++i) _pImpl->cv.notify_one();
                return prev;
            }

            int SemaphoreSlim::GetCurrentCount() const {
                if (!_pImpl) return _count;
                std::lock_guard<std::mutex> lock(_pImpl->mutex);
                return _count;
            }
        }
    }
}