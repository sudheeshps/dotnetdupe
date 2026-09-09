/// \file SecurityException.h
/// \brief Exception thrown when a security error is detected.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {

            /// \class SecurityException
            /// \brief Exception thrown when a security error is detected.
            ///
            /// Mirrors .NET System.Security.SecurityException per ECMA-335.
            class SecurityException : public SystemException {
            public:
                /// \brief Initializes a new instance of the SecurityException class with default properties.
                DOTNETDUPE_API SecurityException();

                /// \brief Initializes a new instance of the SecurityException class with a specified error message.
                /// \param sMessage The error message that explains the reason for the exception.
                SecurityException(const String& sMessage) : SystemException(sMessage) {}

                /// \brief Initializes a new instance of the SecurityException class with a specified error message and inner exception.
                /// \param sMessage The error message that explains the reason for the exception.
                /// \param innerException The exception that is the cause of the current exception.
                SecurityException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
            };

        }
    }
}
