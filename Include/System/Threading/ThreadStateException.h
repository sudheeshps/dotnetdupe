/// \file ThreadStateException.h
/// \brief Exception thrown when a Thread is in an invalid state for the method call.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class ThreadStateException
            /// \brief Exception thrown when a thread is in an invalid ThreadState for the requested operation.
            ///
            /// Mirrors .NET System.Threading.ThreadStateException per ECMA-335.
            class ThreadStateException : public SystemException {
            public:
                /// \brief Initializes a new instance of the ThreadStateException class with a specified error message.
                /// \param sMessage The message that describes the error.
                DOTNETDUPE_API ThreadStateException(const String& sMessage) : SystemException(sMessage) { }
            };

        }
    }
}
