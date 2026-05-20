#pragma once
#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            // Forward declaration for friendship
            template <typename T> class Interlocked;

            // Internal helper - NOT exported and NOT accessible outside
            class InterlockedInternal {
            private:
                // Only Interlocked<T> can call these private methods
                template <typename U> friend class Interlocked;

                // These methods are implemented in the .cpp file.
                // Note: To be reachable from the template in client code, these symbols 
                // must be exported even if the class itself is not.
                DOTNETDUPE_API static long Increment32(volatile long* location);
                DOTNETDUPE_API static long Decrement32(volatile long* location);
                DOTNETDUPE_API static long Add32(volatile long* location, long value);
                DOTNETDUPE_API static long Exchange32(volatile long* location, long value);
                DOTNETDUPE_API static long CompareExchange32(volatile long* location, long value, long comparand);

                DOTNETDUPE_API static long long Increment64(volatile long long* location);
                DOTNETDUPE_API static long long Decrement64(volatile long long* location);
                DOTNETDUPE_API static long long Add64(volatile long long* location, long long value);
                DOTNETDUPE_API static long long Exchange64(volatile long long* location, long long value);
                DOTNETDUPE_API static long long CompareExchange64(volatile long long* location, long long value, long long comparand);
            };

            template <typename T>
            class Interlocked : public Object {
            public:
                Interlocked(T initialValue = 0) : _value(initialValue) {}

                // Atomic Operations
                T Increment() {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Increment32((volatile long*)&_value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Increment64((volatile long long*)&_value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                T Decrement() {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Decrement32((volatile long*)&_value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Decrement64((volatile long long*)&_value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                T Add(T value) {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Add32((volatile long*)&_value, (long)value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Add64((volatile long long*)&_value, (long long)value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                T Exchange(T value) {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Exchange32((volatile long*)&_value, (long)value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Exchange64((volatile long long*)&_value, (long long)value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                T CompareExchange(T value, T comparand) {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::CompareExchange32((volatile long*)&_value, (long)value, (long)comparand);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::CompareExchange64((volatile long long*)&_value, (long long)value, (long long)comparand);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                // Operator Overloading
                T operator++() { return Increment(); } // Prefix
                T operator++(int) { // Postfix
                    T current = (T)_value;
                    Increment();
                    return current;
                }

                T operator--() { return Decrement(); } // Prefix
                T operator--(int) { // Postfix
                    T current = (T)_value;
                    Decrement();
                    return current;
                }

                T operator+=(T value) { return Add(value); }

                // Implicit conversion to base type
                operator T() const { return (T)_value; }

                // Assignment
                T operator=(T value) { return (T)Exchange(value); }

            private:
                volatile T _value;
            };
        }
    }
}
