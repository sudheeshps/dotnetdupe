/// \file ThreadInterruptedException.h
/// \brief Exception thrown when a Thread is interrupted while in a waiting state.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class ThreadInterruptedException
            /// \brief Exception thrown when a thread is interrupted while it is in a waiting state.
            ///
            /// Mirrors .NET System.Threading.ThreadInterruptedException per ECMA-335.
            class ThreadInterruptedException : public SystemException {
            public:
                /// \brief Initializes a new instance of the ThreadInterruptedException class with a specified error message.
                /// \param sMessage The message that describes the error.
                DOTNETDUPE_API ThreadInterruptedException(const String& sMessage) : SystemException(sMessage) {}
            };

        }
    }
}
