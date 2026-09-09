/// \file UnknownException.h
/// \brief Represents an unknown or unmapped exception encountered during execution.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {

        /// \class UnknownException
        /// \brief Represents an unknown or unmapped exception encountered during execution.
        ///
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        class UnknownException : public SystemException {
        public:
            /// \brief Initializes a new instance of the UnknownException class with a default message.
            DOTNETDUPE_API UnknownException();

            /// \brief Initializes a new instance of the UnknownException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            DOTNETDUPE_API UnknownException(const String& sMessage) : SystemException(sMessage) {}

            /// \brief Initializes a new instance of the UnknownException class with a specified error message and inner exception.
            /// \param sMessage The error message that explains the reason for the exception.
            /// \param innerException The exception that is the cause of the current exception.
            DOTNETDUPE_API UnknownException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
        };

    }
}

