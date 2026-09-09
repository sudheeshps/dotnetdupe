/// \file OutOfMemoryException.h
/// \brief The exception that is thrown when there is not enough memory to continue the execution of a program.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"
#include "Common.h"

namespace DotNetDupe {
    namespace System {

        /// \class OutOfMemoryException
        /// \brief The exception that is thrown when there is not enough memory to continue execution.
        ///
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        class OutOfMemoryException : public SystemException {
        public:
            /// \brief Initializes a new instance of the OutOfMemoryException class with a default message.
            DOTNETDUPE_API OutOfMemoryException();

            /// \brief Initializes a new instance of the OutOfMemoryException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            OutOfMemoryException(const String& sMessage) : SystemException(sMessage) {}

            /// \brief Initializes a new instance of the OutOfMemoryException class with a specified error message and inner exception.
            /// \param sMessage The error message that explains the reason for the exception.
            /// \param innerException The exception that is the cause of the current exception.
            OutOfMemoryException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
        };

    }
}

