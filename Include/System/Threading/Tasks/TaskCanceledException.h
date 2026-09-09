/// \file TaskCanceledException.h
/// \brief Exception thrown to indicate task cancellation in asynchronous operations.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            namespace Tasks {

                /// \class TaskCanceledException
                /// \brief Exception thrown to indicate task cancellation in asynchronous operations.
                ///
                /// Mirrors .NET System.Threading.Tasks.TaskCanceledException per ECMA-335.
                class TaskCanceledException : public SystemException {
                public:
                    /// \brief Initializes a new instance of the TaskCanceledException class with a specified error message.
                    /// \param sMessage The message that describes the error.
                    DOTNETDUPE_API TaskCanceledException(const String& sMessage) : SystemException(sMessage) { }
                };

            }
        }
    }
}
