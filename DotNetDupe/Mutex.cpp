#include "pch.h"
#include "System/Threading/Mutex.h"
#include "System/TimeoutException.h"
#include <chrono>
#include "System/Char.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            Mutex::Mutex() {}
            Mutex::~Mutex() {}
            bool Mutex::WaitOne() {
                _mutex.lock();
                return true;
            }
            bool Mutex::WaitOne(int millisecondsTimeout) {
                if (!_mutex.try_lock_for(std::chrono::milliseconds(millisecondsTimeout))) {
                    throw TimeoutException("The wait operation timed out.");
                }
                return true;
            }

            int Mutex::Release(int releaseCount) {
                // Mutex ignores count and unlocks once.
                _mutex.unlock();
                return 0;
            }
        }
    }
}