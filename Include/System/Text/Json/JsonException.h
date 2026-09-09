/// \file JsonException.h
/// \brief Exception thrown when invalid JSON payload or token is encountered per RFC 8259.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {

                /// \class JsonException
                /// \brief Exception thrown during JSON serialization or deserialization per RFC 8259.
                ///
                /// Mirrors .NET System.Text.Json.JsonException per ECMA-335 and RFC 8259.
                class JsonException : public SystemException {
                public:
                    /// \brief Initializes a new instance of the JsonException class with a default message.
                    DOTNETDUPE_API JsonException() : SystemException(String("A JSON parse error occurred.")) {}

                    /// \brief Initializes a new instance of the JsonException class with a specified error message.
                    /// \param sMessage The error message that explains the reason for the exception.
                    DOTNETDUPE_API JsonException(const String& sMessage) : SystemException(sMessage) {}

                    /// \brief Initializes a new instance of the JsonException class with a specified error message and inner exception.
                    /// \param sMessage The error message that explains the reason for the exception.
                    /// \param innerException The exception that is the cause of the current exception.
                    DOTNETDUPE_API JsonException(const String& sMessage, const Exception& innerException) : SystemException(sMessage, innerException) {}
                };

            }
        }
    }
}
