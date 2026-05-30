#include "pch.h"
#include "System/Threading/Interlocked.h"

#if defined(_WIN32)
#include <windows.h>
#include <intrin.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            long InterlockedInternal::Increment32(volatile long* location) {
#if defined(_WIN32)
                return _InterlockedIncrement(location);
#else
                return __sync_add_and_fetch(location, 1);
#endif
            }
            long InterlockedInternal::Decrement32(volatile long* location) {
#if defined(_WIN32)
                return _InterlockedDecrement(location);
#else
                return __sync_sub_and_fetch(location, 1);
#endif
            }
            long InterlockedInternal::Add32(volatile long* location, long value) {
#if defined(_WIN32)
                return _InterlockedExchangeAdd(location, value) + value;
#else
                return __sync_add_and_fetch(location, value);
#endif
            }
            long InterlockedInternal::Exchange32(volatile long* location, long value) {
#if defined(_WIN32)
                return _InterlockedExchange(location, value);
#else
                return __sync_lock_test_and_set(location, value);
#endif
            }
            long InterlockedInternal::CompareExchange32(volatile long* location, long value, long comparand) {
#if defined(_WIN32)
                return _InterlockedCompareExchange(location, value, comparand);
#else
                return __sync_val_compare_and_swap(location, comparand, value);
#endif
            }

            long long InterlockedInternal::Increment64(volatile long long* location) {
#if defined(_WIN32)
                return _InterlockedIncrement64(location);
#else
                return __sync_add_and_fetch(location, 1);
#endif
            }
            long long InterlockedInternal::Decrement64(volatile long long* location) {
#if defined(_WIN32)
                return _InterlockedDecrement64(location);
#else
                return __sync_sub_and_fetch(location, 1);
#endif
            }
            long long InterlockedInternal::Add64(volatile long long* location, long long value) {
#if defined(_WIN32)
                return _InterlockedExchangeAdd64(location, value) + value;
#else
                return __sync_add_and_fetch(location, value);
#endif
            }
            long long InterlockedInternal::Exchange64(volatile long long* location, long long value) {
#if defined(_WIN32)
                return _InterlockedExchange64(location, value);
#else
                return __sync_lock_test_and_set(location, value);
#endif
            }
            long long InterlockedInternal::CompareExchange64(volatile long long* location, long long value, long long comparand) {
#if defined(_WIN32)
                return _InterlockedCompareExchange64(location, value, comparand);
#else
                return __sync_val_compare_and_swap(location, comparand, value);
#endif
            }
        }
    }
}