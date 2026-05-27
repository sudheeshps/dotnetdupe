#include "pch.h"
#include "System/Threading/EventWaitHandle.h"
#include "System/TimeoutException.h"
#include <chrono>
#include "System/Char.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            EventWaitHandle::EventWaitHandle(bool initialState, bool manualReset)
                : _state(initialState), _manualReset(manualReset) {}

            EventWaitHandle::~EventWaitHandle() {}

            bool EventWaitHandle::Set() {
                std::lock_guard<std::mutex> lock(_mutex);
                _state = true;
                if (_manualReset) {
                    _cv.notify_all();
                } else {
                    _cv.notify_one();
                }
                return true;
            }

            bool EventWaitHandle::Reset() {
                std::lock_guard<std::mutex> lock(_mutex);
                _state = false;
                return true;
            }

            bool EventWaitHandle::WaitOne() {
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]() { return _state; });
                if (!_manualReset) {
                    _state = false;
                }
                return true;
            }

            bool EventWaitHandle::WaitOne(int millisecondsTimeout) {
                std::unique_lock<std::mutex> lock(_mutex);
                bool result = _cv.wait_for(lock, std::chrono::milliseconds(millisecondsTimeout), [this]() { return _state; });
                if (result) {
                    if (!_manualReset) {
                        _state = false;
                    }
                    return true;
                }
                
                throw TimeoutException("The wait operation timed out.");
            }
        }
    }
}