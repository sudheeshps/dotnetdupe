/// \file Interlocked.h
/// \brief Provides atomic operations for variables that are shared by multiple threads.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            // Forward declaration for friendship
            template <typename T> class Interlocked;

            /// \class InterlockedInternal
            /// \brief Internal helper exposing platform-specific atomic intrinsic primitives.
            class InterlockedInternal {
            private:
                template <typename U> friend class Interlocked;

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

            /// \class Interlocked
            /// \brief Provides atomic operations for variables that are shared by multiple threads.
            ///
            /// \details Complies with ECMA-335 Partition I Section 12.6.5 and Partition IV Section 5.66.
            /// Uses hardware-accelerated atomic CPU primitives (_Interlocked intrinsics on Windows,
            /// __sync / __atomic intrinsics on Linux) to prevent race conditions without lock overhead.
            template <typename T>
            class Interlocked : public Object {
            public:
                /// \brief Initializes a new instance of the Interlocked class with an initial value.
                /// \param initialValue The starting value for the atomic variable.
                Interlocked(T initialValue = 0) : _value(initialValue) {}

                /// \brief Increments a specified variable and stores the result, as an atomic operation.
                /// \return The incremented value.
                T Increment() {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Increment32((volatile long*)&_value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Increment64((volatile long long*)&_value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                /// \brief Decrements a specified variable and stores the result, as an atomic operation.
                /// \return The decremented value.
                T Decrement() {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Decrement32((volatile long*)&_value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Decrement64((volatile long long*)&_value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                /// \brief Adds two integers and replaces the first integer with the sum, as an atomic operation.
                /// \param value The value to add.
                /// \return The new value.
                T Add(T value) {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Add32((volatile long*)&_value, (long)value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Add64((volatile long long*)&_value, (long long)value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                /// \brief Sets a variable to a specified value and returns the original value, as an atomic operation.
                /// \param value The value to which the parameter is set.
                /// \return The original value.
                T Exchange(T value) {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::Exchange32((volatile long*)&_value, (long)value);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::Exchange64((volatile long long*)&_value, (long long)value);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                /// \brief Compares two values for equality and, if they are equal, replaces the first value.
                /// \param value The value that replaces the destination value if comparison results in equality.
                /// \param comparand The value that is compared to the value at destination.
                /// \return The original value.
                T CompareExchange(T value, T comparand) {
                    if constexpr (sizeof(T) == 4) return (T)InterlockedInternal::CompareExchange32((volatile long*)&_value, (long)value, (long)comparand);
                    else if constexpr (sizeof(T) == 8) return (T)InterlockedInternal::CompareExchange64((volatile long long*)&_value, (long long)value, (long long)comparand);
                    else static_assert(sizeof(T) == 4 || sizeof(T) == 8, "Unsupported type size for Interlocked");
                }

                /// \brief Atomic pre-increment operator.
                T operator++() { return Increment(); }

                /// \brief Atomic post-increment operator.
                T operator++(int) {
                    T current = (T)_value;
                    Increment();
                    return current;
                }

                /// \brief Atomic pre-decrement operator.
                T operator--() { return Decrement(); }

                /// \brief Atomic post-decrement operator.
                T operator--(int) {
                    T current = (T)_value;
                    Decrement();
                    return current;
                }

                /// \brief Atomic addition assignment operator.
                T operator+=(T value) { return Add(value); }

                /// \brief Implicit conversion to base type.
                operator T() const { return (T)_value; }

                /// \brief Atomic assignment operator.
                T operator=(T value) { return (T)Exchange(value); }

            private:
                volatile T _value;
            };
        }
    }
}
