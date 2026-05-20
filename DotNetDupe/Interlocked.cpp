#include "pch.h"
#include "System/Threading/Interlocked.h"
#include <windows.h>
#include <intrin.h>

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            long InterlockedInternal::Increment32(volatile long* location) {
                return _InterlockedIncrement(location);
            }
            long InterlockedInternal::Decrement32(volatile long* location) {
                return _InterlockedDecrement(location);
            }
            long InterlockedInternal::Add32(volatile long* location, long value) {
                return _InterlockedExchangeAdd(location, value) + value;
            }
            long InterlockedInternal::Exchange32(volatile long* location, long value) {
                return _InterlockedExchange(location, value);
            }
            long InterlockedInternal::CompareExchange32(volatile long* location, long value, long comparand) {
                return _InterlockedCompareExchange(location, value, comparand);
            }

            long long InterlockedInternal::Increment64(volatile long long* location) {
                return _InterlockedIncrement64(location);
            }
            long long InterlockedInternal::Decrement64(volatile long long* location) {
                return _InterlockedDecrement64(location);
            }
            long long InterlockedInternal::Add64(volatile long long* location, long long value) {
                return _InterlockedExchangeAdd64(location, value) + value;
            }
            long long InterlockedInternal::Exchange64(volatile long long* location, long long value) {
                return _InterlockedExchange64(location, value);
            }
            long long InterlockedInternal::CompareExchange64(volatile long long* location, long long value, long long comparand) {
                return _InterlockedCompareExchange64(location, value, comparand);
            }
        }
    }
}