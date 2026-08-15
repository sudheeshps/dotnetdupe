#include "pch.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpRequestException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                HttpResponseMessage::HttpResponseMessage()
                    : m_statusCode(HttpStatusCode::OK), m_pContent(nullptr) {}

                HttpResponseMessage::HttpResponseMessage(HttpStatusCode statusCode)
                    : m_statusCode(statusCode), m_pContent(nullptr) {}

                HttpStatusCode HttpResponseMessage::GetStatusCode() const {
                    return m_statusCode;
                }

                void HttpResponseMessage::SetStatusCode(HttpStatusCode statusCode) {
                    m_statusCode = statusCode;
                }

                String HttpResponseMessage::GetReasonPhrase() const {
                    return m_sReasonPhrase;
                }

                void HttpResponseMessage::SetReasonPhrase(const String& reasonPhrase) {
                    m_sReasonPhrase = reasonPhrase;
                }

                bool HttpResponseMessage::IsSuccessStatusCode() const {
                    int code = static_cast<int>(m_statusCode);
                    return code >= 200 && code <= 299;
                }

                HttpContentPtr HttpResponseMessage::GetContent() const {
                    return m_pContent;
                }

                void HttpResponseMessage::SetContent(const HttpContentPtr& content) {
                    m_pContent = content;
                }

                Collections::Generic::Dictionary<String, String>& HttpResponseMessage::GetHeaders() {
                    return m_headers;
                }

                const Collections::Generic::Dictionary<String, String>& HttpResponseMessage::GetHeaders() const {
                    return m_headers;
                }

                void HttpResponseMessage::EnsureSuccessStatusCode() {
                    if (!IsSuccessStatusCode()) {
                        std::string msg = "Response status code does not indicate success: ";
                        msg += std::to_string(static_cast<int>(m_statusCode));
                        if (!m_sReasonPhrase.IsEmpty()) {
                            msg += " (";
                            msg += m_sReasonPhrase.GetRawString();
                            msg += ")";
                        }
                        msg += ".";
                        throw HttpRequestException(msg.c_str());
                    }
                }

            }
        }
    }
}
