/// \file SystemException.h
/// \brief Serves as the base class for system exceptions across the library.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/Exception.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {

        /// \class SystemException
        /// \brief Serves as the base class for system exceptions namespace.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.24 (System.SystemException).
        class SystemException : public Exception {
        public:
            /// \brief Initializes a new instance of the SystemException class with a default message.
            DOTNETDUPE_API SystemException();

            /// \brief Initializes a new instance of the SystemException class with a specified error message.
            /// \param sMessage The message that describes the error.
            DOTNETDUPE_API SystemException(const String& sMessage);

            /// \brief Initializes a new instance of the SystemException class with a specified error message and inner exception.
            /// \param sMessage The error message that explains the reason for the exception.
            /// \param innerException The exception that is the cause of the current exception.
            DOTNETDUPE_API SystemException(const String& sMessage, const Exception& innerException);
        };
    }
}
