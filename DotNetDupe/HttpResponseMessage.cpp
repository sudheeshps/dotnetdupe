#include "pch.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpRequestException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                HttpResponseMessage::HttpResponseMessage()
                    : m_statusCode(HttpStatusCode::OK), m_pContent(nullptr) {
                    /// Default initialize with HTTP 200 OK status.
                }

                HttpResponseMessage::HttpResponseMessage(HttpStatusCode statusCode)
                    : m_statusCode(statusCode), m_pContent(nullptr) {
                    /// Initialize with given HTTP status code.
                }

                HttpStatusCode HttpResponseMessage::GetStatusCode() const {
                    /// Return HTTP status code.
                    return m_statusCode;
                }

                void HttpResponseMessage::SetStatusCode(HttpStatusCode statusCode) {
                    /// Mutate HTTP status code.
                    m_statusCode = statusCode;
                }

                String HttpResponseMessage::GetReasonPhrase() const {
                    /// Return HTTP status reason phrase.
                    return m_sReasonPhrase;
                }

                void HttpResponseMessage::SetReasonPhrase(const String& reasonPhrase) {
                    /// Mutate HTTP status reason phrase.
                    m_sReasonPhrase = reasonPhrase;
                }

                bool HttpResponseMessage::IsSuccessStatusCode() const {
                    /// Successful HTTP status codes are in the range [200, 299].
                    int code = static_cast<int>(m_statusCode);
                    return code >= 200 && code <= 299;
                }

                HttpContentPtr HttpResponseMessage::GetContent() const {
                    /// Return payload content pointer.
                    return m_pContent;
                }

                void HttpResponseMessage::SetContent(const HttpContentPtr& content) {
                    /// Assign payload content pointer.
                    m_pContent = content;
                }

                Collections::Generic::Dictionary<String, String>& HttpResponseMessage::GetHeaders() {
                    /// Return mutable reference to response headers dictionary.
                    return m_headers;
                }

                const Collections::Generic::Dictionary<String, String>& HttpResponseMessage::GetHeaders() const {
                    /// Return read-only const reference to response headers dictionary.
                    return m_headers;
                }

                void HttpResponseMessage::EnsureSuccessStatusCode() {
                    /// Guard: Verify status is within successful range [200, 299].
                    if (IsSuccessStatusCode()) {
                        return;
                    }
                    /// Format failure message with code and optional reason phrase.
                    std::string msg = "Response status code does not indicate success: ";
                    msg += std::to_string(static_cast<int>(m_statusCode));
                    if (!m_sReasonPhrase.IsEmpty()) {
                        msg += " (";
                        msg += m_sReasonPhrase.GetRawString();
                        msg += ")";
                    }
                    msg += ".";
                    /// Throw library HttpRequestException.
                    throw HttpRequestException(msg.c_str());
                }

            }
        }
    }
}
