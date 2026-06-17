#include "pch.h"
#include "System/Net/Http/HttpRequestMessage.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                HttpRequestMessage::HttpRequestMessage()
                    : m_method(HttpMethod::Get), m_requestUri("http://localhost"), m_pContent(nullptr) {}

                HttpRequestMessage::HttpRequestMessage(const HttpMethod& method, const Uri& requestUri)
                    : m_method(method), m_requestUri(requestUri), m_pContent(nullptr) {}

                HttpRequestMessage::HttpRequestMessage(const HttpMethod& method, const String& requestUri)
                    : m_method(method), m_requestUri(Uri(requestUri)), m_pContent(nullptr) {}

                HttpMethod HttpRequestMessage::GetMethod() const {
                    return m_method;
                }

                void HttpRequestMessage::SetMethod(const HttpMethod& method) {
                    m_method = method;
                }

                Uri HttpRequestMessage::GetRequestUri() const {
                    return m_requestUri;
                }

                void HttpRequestMessage::SetRequestUri(const Uri& requestUri) {
                    m_requestUri = requestUri;
                }

                SmartPointer<HttpContent> HttpRequestMessage::GetContent() const {
                    return m_pContent;
                }

                void HttpRequestMessage::SetContent(const SmartPointer<HttpContent>& content) {
                    m_pContent = content;
                }

                Collections::Generic::Dictionary<String, String>& HttpRequestMessage::GetHeaders() {
                    return m_headers;
                }

                const Collections::Generic::Dictionary<String, String>& HttpRequestMessage::GetHeaders() const {
                    return m_headers;
                }

            }
        }
    }
}
