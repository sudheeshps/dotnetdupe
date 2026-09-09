/// \file HttpMethod.h
/// \brief Represents standard HTTP method verbs defined in RFC 9110 and RFC 5789.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                /// \class HttpMethod
                /// \brief A helper class for retrieving and comparing standard HTTP methods.
                ///
                /// Conforms to RFC 9110 (HTTP Semantics) and RFC 5789 (PATCH Method for HTTP).
                /// Method comparisons are case-insensitive.
                class HttpMethod : public Object {
                public:
                    /// \brief Initializes a new instance of the HttpMethod class with a specific HTTP method name.
                    /// \param method The HTTP method verb (e.g., "GET", "POST").
                    DOTNETDUPE_API explicit HttpMethod(const String& method);

                    /// \brief Gets the HTTP method verb as a string.
                    /// \return The string representation of the HTTP method.
                    DOTNETDUPE_API String GetMethod() const;

                    /// \brief Returns a string that represents the current HttpMethod object.
                    /// \return A string representing the HTTP method.
                    DOTNETDUPE_API String ToString() const;

                    /// \brief Determines whether the specified HttpMethod is equal to the current HttpMethod (case-insensitive).
                    /// \param other The HttpMethod to compare with the current object.
                    /// \return true if the specified object is equal to the current object; otherwise, false.
                    DOTNETDUPE_API bool Equals(const HttpMethod& other) const;

                    /// \brief Equality operator for HttpMethod instances.
                    /// \param other The HttpMethod to compare.
                    /// \return true if equal; otherwise, false.
                    DOTNETDUPE_API bool operator==(const HttpMethod& other) const;

                    /// \brief Inequality operator for HttpMethod instances.
                    /// \param other The HttpMethod to compare.
                    /// \return true if not equal; otherwise, false.
                    DOTNETDUPE_API bool operator!=(const HttpMethod& other) const;

                    /// \brief Represents an HTTP GET protocol method.
                    DOTNETDUPE_API static const HttpMethod Get;
                    /// \brief Represents an HTTP POST protocol method.
                    DOTNETDUPE_API static const HttpMethod Post;
                    /// \brief Represents an HTTP PUT protocol method.
                    DOTNETDUPE_API static const HttpMethod Put;
                    /// \brief Represents an HTTP DELETE protocol method.
                    DOTNETDUPE_API static const HttpMethod Delete;
                    /// \brief Represents an HTTP HEAD protocol method.
                    DOTNETDUPE_API static const HttpMethod Head;
                    /// \brief Represents an HTTP OPTIONS protocol method.
                    DOTNETDUPE_API static const HttpMethod Options;
                    /// \brief Represents an HTTP TRACE protocol method.
                    DOTNETDUPE_API static const HttpMethod Trace;
                    /// \brief Represents an HTTP PATCH protocol method.
                    DOTNETDUPE_API static const HttpMethod Patch;

                private:
                    String m_sMethod;
                };
            }
        }
    }
}
