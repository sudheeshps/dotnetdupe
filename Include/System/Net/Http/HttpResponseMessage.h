/// \file HttpResponseMessage.h
/// \brief Represents an HTTP response message adhering to RFC 9110 and RFC 9112.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Net/HttpStatusCode.h"
#include "System/Net/Http/HttpContent.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                /// \class HttpResponseMessage
                /// \brief Represents an HTTP response message received from an HTTP request.
                ///
                /// Contains status code, reason phrase, headers, and body content payload.
                /// Standards Conformance: RFC 9110 (HTTP Semantics), RFC 9112 (HTTP/1.1).
                class HttpResponseMessage : public Object {
                public:
                    /// \brief Initializes a new instance of the HttpResponseMessage class with OK (200) status.
                    DOTNETDUPE_API HttpResponseMessage();

                    /// \brief Initializes a new instance of the HttpResponseMessage class with a specific status code.
                    /// \param statusCode The HTTP status code of the response.
                    DOTNETDUPE_API explicit HttpResponseMessage(HttpStatusCode statusCode);

                    /// \brief Gets the status code of the HTTP response.
                    /// \return The HttpStatusCode enum value.
                    DOTNETDUPE_API HttpStatusCode GetStatusCode() const;

                    /// \brief Sets the status code of the HTTP response.
                    /// \param statusCode The HttpStatusCode to assign.
                    DOTNETDUPE_API void SetStatusCode(HttpStatusCode statusCode);

                    /// \brief Gets the reason phrase which typically accompanies the status code.
                    /// \return The reason phrase string.
                    DOTNETDUPE_API String GetReasonPhrase() const;

                    /// \brief Sets the reason phrase which typically accompanies the status code.
                    /// \param reasonPhrase The reason phrase to assign.
                    DOTNETDUPE_API void SetReasonPhrase(const String& reasonPhrase);

                    /// \brief Gets a value that indicates if the HTTP response was successful (status in range 200-299).
                    /// \return true if StatusCode was in the 200-299 range; otherwise, false.
                    DOTNETDUPE_API bool IsSuccessStatusCode() const;

                    /// \brief Gets the content of the HTTP response message.
                    /// \return The HttpContentPtr payload, or null if no body is present.
                    DOTNETDUPE_API HttpContentPtr GetContent() const;

                    /// \brief Sets the content of the HTTP response message.
                    /// \param content The HttpContentPtr payload to associate with the response.
                    DOTNETDUPE_API void SetContent(const HttpContentPtr& content);

                    /// \brief Gets the collection of HTTP response headers.
                    /// \return A mutable reference to the headers Dictionary.
                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeaders();

                    /// \brief Gets the collection of HTTP response headers as read-only.
                    /// \return A const reference to the headers Dictionary.
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeaders() const;

                    /// \brief Throws an HttpRequestException if IsSuccessStatusCode() is false.
                    /// \throws HttpRequestException When status code is outside the 200-299 range.
                    DOTNETDUPE_API void EnsureSuccessStatusCode();

                private:
                    HttpStatusCode m_statusCode;
                    String m_sReasonPhrase;
                    HttpContentPtr m_pContent;
                    Collections::Generic::Dictionary<String, String> m_headers;
                };

                /// \brief Type alias for reference-counted SmartPointer to HttpResponseMessage.
                using HttpResponseMessagePtr = SmartPointer<HttpResponseMessage>;

            }
        }
    }
}
