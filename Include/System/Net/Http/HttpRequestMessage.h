/// \file HttpRequestMessage.h
/// \brief Represents an HTTP request message adhering to RFC 9110 and RFC 9112.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Uri.h"
#include "System/SmartPointer.h"
#include "System/Net/Http/HttpMethod.h"
#include "System/Net/Http/HttpContent.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                /// \class HttpRequestMessage
                /// \brief Represents an HTTP request message sent by an HttpClient instance.
                ///
                /// Encapsulates the HTTP verb, target URI, outgoing headers dictionary, and optional payload body.
                /// Standards Conformance: RFC 9110 (HTTP Semantics), RFC 9112 (HTTP/1.1).
                class HttpRequestMessage : public Object {
                public:
                    /// \brief Initializes a new instance of HttpRequestMessage with default GET method and localhost URI.
                    DOTNETDUPE_API HttpRequestMessage();

                    /// \brief Initializes a new instance of HttpRequestMessage with a specified HTTP method and Uri.
                    /// \param method The HTTP method verb.
                    /// \param requestUri The target request Uri.
                    DOTNETDUPE_API HttpRequestMessage(const HttpMethod& method, const Uri& requestUri);

                    /// \brief Initializes a new instance of HttpRequestMessage with a specified HTTP method and string URI.
                    /// \param method The HTTP method verb.
                    /// \param requestUri The target request URI string.
                    DOTNETDUPE_API HttpRequestMessage(const HttpMethod& method, const String& requestUri);

                    /// \brief Gets the HTTP method used by the HTTP request message.
                    /// \return The HttpMethod instance.
                    DOTNETDUPE_API HttpMethod GetMethod() const;

                    /// \brief Sets the HTTP method used by the HTTP request message.
                    /// \param method The HttpMethod to assign.
                    DOTNETDUPE_API void SetMethod(const HttpMethod& method);

                    /// \brief Gets the Uri used for the HTTP request.
                    /// \return The Uri instance.
                    DOTNETDUPE_API Uri GetRequestUri() const;

                    /// \brief Sets the Uri used for the HTTP request.
                    /// \param requestUri The target request Uri.
                    DOTNETDUPE_API void SetRequestUri(const Uri& requestUri);

                    /// \brief Gets the contents of the HTTP message.
                    /// \return A SmartPointer to the HttpContent object, or null if no body is set.
                    DOTNETDUPE_API HttpContentPtr GetContent() const;

                    /// \brief Sets the contents of the HTTP message.
                    /// \param content The HttpContentPtr payload to associate with the request.
                    DOTNETDUPE_API void SetContent(const HttpContentPtr& content);

                    /// \brief Gets the collection of HTTP request headers.
                    /// \return A mutable reference to the headers Dictionary.
                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeaders();

                    /// \brief Gets the collection of HTTP request headers as read-only.
                    /// \return A const reference to the headers Dictionary.
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeaders() const;

                private:
                    HttpMethod m_method;
                    Uri m_requestUri;
                    HttpContentPtr m_pContent;
                    Collections::Generic::Dictionary<String, String> m_headers;
                };

                /// \brief Type alias for reference-counted SmartPointer to HttpRequestMessage.
                using HttpRequestMessagePtr = SmartPointer<HttpRequestMessage>;

            }
        }
    }
}
