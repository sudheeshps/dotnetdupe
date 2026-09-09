#include "pch.h"
#include "System/Net/Http/HttpMethod.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                /// Static singleton instances for RFC 9110 standard HTTP verbs.
                const HttpMethod HttpMethod::Get("GET");
                const HttpMethod HttpMethod::Post("POST");
                const HttpMethod HttpMethod::Put("PUT");
                const HttpMethod HttpMethod::Delete("DELETE");
                const HttpMethod HttpMethod::Head("HEAD");
                const HttpMethod HttpMethod::Options("OPTIONS");
                const HttpMethod HttpMethod::Trace("TRACE");
                const HttpMethod HttpMethod::Patch("PATCH");

                HttpMethod::HttpMethod(const String& method) : m_sMethod(method) {
                    /// Initialize method string representation.
                }

                String HttpMethod::GetMethod() const {
                    /// Return underlying HTTP method string.
                    return m_sMethod;
                }

                String HttpMethod::ToString() const {
                    /// Return string representation.
                    return m_sMethod;
                }

                bool HttpMethod::Equals(const HttpMethod& other) const {
                    /// Case-insensitive comparison per RFC 9110.
                    return m_sMethod.ToUpper() == other.m_sMethod.ToUpper();
                }

                bool HttpMethod::operator==(const HttpMethod& other) const {
                    /// Delegate to Equals().
                    return Equals(other);
                }

                bool HttpMethod::operator!=(const HttpMethod& other) const {
                    /// Invert equality comparison.
                    return !Equals(other);
                }

            }
        }
    }
}
