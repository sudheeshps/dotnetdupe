/// \file ConditionVariable.h
/// \brief Provides condition variable synchronization primitive for thread coordination.
///
/// Standard Citation: POSIX.1-2001 pthread_cond_t, ISO/IEC 14882 C++11 std::condition_variable.

#pragma once
#include "System/Object.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class CriticalSection;
            
            /// \class ConditionVariable
            /// \brief Provides a synchronization primitive that can be used to block a thread, or multiple threads, until another thread modifies a shared variable and notifies the ConditionVariable.
            ///
            /// \details Complies with POSIX pthread_cond_t and ISO C++11 condition variable specifications.
            /// Integrates with CriticalSection via custom lock adapters, providing Pulse (notify_one)
            /// and PulseAll (notify_all) semantics.
            class ConditionVariable : public Object {
            public:
                /// \brief Initializes a new instance of the ConditionVariable class.
                DOTNETDUPE_API ConditionVariable();

                /// \brief Virtual destructor releasing condition variable resources.
                DOTNETDUPE_API virtual ~ConditionVariable();
                
                ConditionVariable(const ConditionVariable&) = delete;
                ConditionVariable& operator=(const ConditionVariable&) = delete;
                DOTNETDUPE_API ConditionVariable(ConditionVariable&& other) noexcept;
                DOTNETDUPE_API ConditionVariable& operator=(ConditionVariable&& other) noexcept;
                
                /// \brief Releases the associated CriticalSection lock and blocks the current thread until signaled.
                /// \param cs The CriticalSection locked by the calling thread.
                DOTNETDUPE_API void Wait(CriticalSection& cs);

                /// \brief Releases the associated CriticalSection lock and blocks the current thread until signaled or timeout expires.
                /// \param cs The CriticalSection locked by the calling thread.
                /// \param millisecondsTimeout The maximum time to wait in milliseconds.
                /// \return true if signaled before timeout; otherwise, false.
                DOTNETDUPE_API bool Wait(CriticalSection& cs, int millisecondsTimeout);
                
                /// \brief Notifies one waiting thread.
                DOTNETDUPE_API void Pulse();

                /// \brief Notifies all waiting threads.
                DOTNETDUPE_API void PulseAll();

            private:
                struct Impl;
                SmartPointer<Impl> m_pImpl;
            };
        }
    }
}
