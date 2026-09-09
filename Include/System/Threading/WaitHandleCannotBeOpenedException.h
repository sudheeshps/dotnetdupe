/// \file WaitHandleCannotBeOpenedException.h
/// \brief Exception thrown when an attempt to open a named system wait handle fails.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \class WaitHandleCannotBeOpenedException
            /// \brief Exception thrown when an attempt to open a system synchronization handle that does not exist fails.
            ///
            /// Mirrors .NET System.Threading.WaitHandleCannotBeOpenedException per ECMA-335.
            class WaitHandleCannotBeOpenedException : public SystemException {
            public:
                /// \brief Initializes a new instance of the WaitHandleCannotBeOpenedException class with a specified error message.
                /// \param sMessage The message that describes the error.
                DOTNETDUPE_API WaitHandleCannotBeOpenedException(const String& sMessage) : SystemException(sMessage) { }
            };

        }
    }
}
