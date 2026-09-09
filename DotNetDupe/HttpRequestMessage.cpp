#include "pch.h"
#include "System/Net/Http/HttpRequestMessage.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                HttpRequestMessage::HttpRequestMessage()
                    : m_method(HttpMethod::Get), m_requestUri("http://localhost"), m_pContent(nullptr) {
                    /// Default initialize with GET verb and localhost URI.
                }

                HttpRequestMessage::HttpRequestMessage(const HttpMethod& method, const Uri& requestUri)
                    : m_method(method), m_requestUri(requestUri), m_pContent(nullptr) {
                    /// Initialize with method and target Uri object.
                }

                HttpRequestMessage::HttpRequestMessage(const HttpMethod& method, const String& requestUri)
                    : m_method(method), m_requestUri(Uri(requestUri)), m_pContent(nullptr) {
                    /// Initialize with method and parsed URI string.
                }

                HttpMethod HttpRequestMessage::GetMethod() const {
                    /// Return assigned HTTP verb.
                    return m_method;
                }

                void HttpRequestMessage::SetMethod(const HttpMethod& method) {
                    /// Mutate request HTTP verb.
                    m_method = method;
                }

                Uri HttpRequestMessage::GetRequestUri() const {
                    /// Return target request Uri.
                    return m_requestUri;
                }

                void HttpRequestMessage::SetRequestUri(const Uri& requestUri) {
                    /// Mutate target request Uri.
                    m_requestUri = requestUri;
                }

                HttpContentPtr HttpRequestMessage::GetContent() const {
                    /// Return message entity body content pointer.
                    return m_pContent;
                }

                void HttpRequestMessage::SetContent(const HttpContentPtr& content) {
                    /// Assign message entity body content pointer.
                    m_pContent = content;
                }

                Collections::Generic::Dictionary<String, String>& HttpRequestMessage::GetHeaders() {
                    /// Return mutable reference to request headers dictionary.
                    return m_headers;
                }

                const Collections::Generic::Dictionary<String, String>& HttpRequestMessage::GetHeaders() const {
                    /// Return read-only const reference to request headers dictionary.
                    return m_headers;
                }

            }
        }
    }
}
