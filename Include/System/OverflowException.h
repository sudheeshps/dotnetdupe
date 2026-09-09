/// \file OverflowException.h
/// \brief Defines the exception thrown when an arithmetic, casting, or conversion operation results in an overflow.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        /// \class OverflowException
        /// \brief The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.30 (System.OverflowException).
        class OverflowException : public SystemException {
        public:
            /// \brief Initializes a new instance of the OverflowException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            OverflowException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}
