/// \file PlatformNotSupportedException.h
/// \brief Defines the exception thrown when a feature does not run on a particular platform.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/NotSupportedException.h"

namespace DotNetDupe {
    namespace System {

        /// \class PlatformNotSupportedException
        /// \brief The exception that is thrown when a feature does not run on a particular platform.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.36 (System.PlatformNotSupportedException).
        class PlatformNotSupportedException : public NotSupportedException {
        public:
            /// \brief Initializes a new instance of the PlatformNotSupportedException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            PlatformNotSupportedException(const String& sMessage) : NotSupportedException(sMessage) {}
        };

    }
}
