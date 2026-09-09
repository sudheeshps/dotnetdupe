#pragma once
#include "Common.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            /// \class CriticalSection
            /// \brief Provides a re-entrant mutual exclusion primitive for thread synchronization.
            ///
            /// \details Encapsulates recursive mutex semantics modeled after Win32 CRITICAL_SECTION
            /// and POSIX recursive mutexes. Supports recursive entry from the owning thread and
            /// non-blocking acquisition attempts via TryEnter.
            class CriticalSection : public Object {
            public:
                /// \brief Initializes a new instance of the CriticalSection class.
                DOTNETDUPE_API CriticalSection();

                /// \brief Destructor releasing critical section resources.
                DOTNETDUPE_API ~CriticalSection() override;

                /// \brief Waits for ownership of the specified critical section object.
                DOTNETDUPE_API void Enter();

                /// \brief Releases ownership of the critical section object.
                DOTNETDUPE_API void Leave();

                /// \brief Attempts to enter a critical section without blocking.
                /// \return true if the current thread successfully entered; otherwise, false.
                DOTNETDUPE_API bool TryEnter();

            private:
                struct Impl;
                Impl* m_pImpl;
            };
        }
    }
}