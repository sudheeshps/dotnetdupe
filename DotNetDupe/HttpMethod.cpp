#include "pch.h"
#include "System/Net/Http/HttpMethod.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                const HttpMethod HttpMethod::Get("GET");
                const HttpMethod HttpMethod::Post("POST");
                const HttpMethod HttpMethod::Put("PUT");
                const HttpMethod HttpMethod::Delete("DELETE");
                const HttpMethod HttpMethod::Head("HEAD");
                const HttpMethod HttpMethod::Options("OPTIONS");
                const HttpMethod HttpMethod::Trace("TRACE");
                const HttpMethod HttpMethod::Patch("PATCH");

                HttpMethod::HttpMethod(const String& method) : m_sMethod(method) {}

                String HttpMethod::GetMethod() const {
                    return m_sMethod;
                }

                String HttpMethod::ToString() const {
                    return m_sMethod;
                }

                bool HttpMethod::Equals(const HttpMethod& other) const {
                    return m_sMethod.ToUpper() == other.m_sMethod.ToUpper();
                }

                bool HttpMethod::operator==(const HttpMethod& other) const {
                    return Equals(other);
                }

                bool HttpMethod::operator!=(const HttpMethod& other) const {
                    return !Equals(other);
                }

            }
        }
    }
}
