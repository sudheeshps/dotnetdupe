/// \file HttpContext.h
/// \brief HTTP request, response, and context abstractions for WebAppCore processing pipelines.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Net/HttpStatusCode.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Http {

            /// \brief Encapsulates an incoming HTTP request within the WebAppCore pipeline.
            ///
            /// Provides access to the HTTP method, URI path, headers, query parameters, route values, and body content.
            ///
            /// \note Thread Safety: Not thread-safe; processed by a single worker thread per connection.
            /// \note Conforms to RFC 9110 (HTTP Semantics) and RFC 9112 (HTTP/1.1).
            /// \see HttpResponse, HttpContext
            class HttpRequest : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Default constructor.
                HttpRequest() = default;

                /// \brief Virtual destructor.
                ~HttpRequest() override = default;

                /// \brief Gets the HTTP request method verb (e.g., "GET", "POST").
                /// \return Method verb string.
                DotNetDupe::System::String GetMethod() const { return m_sMethod; }

                /// \brief Sets the HTTP request method verb.
                /// \param method Method verb string.
                void SetMethod(const DotNetDupe::System::String& method) { m_sMethod = method; }

                /// \brief Gets the request URI path.
                /// \return Request path string.
                DotNetDupe::System::String GetPath() const { return m_sPath; }

                /// \brief Sets the request URI path.
                /// \param path Request path string.
                void SetPath(const DotNetDupe::System::String& path) { m_sPath = path; }

                /// \brief Gets the raw HTTP request body string.
                /// \return Request body payload.
                DotNetDupe::System::String GetBody() const { return m_sBody; }

                /// \brief Sets the raw HTTP request body string.
                /// \param body Request body payload.
                void SetBody(const DotNetDupe::System::String& body) { m_sBody = body; }

                /// \brief Gets the mutable dictionary of HTTP request headers.
                /// \return Reference to headers dictionary.
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() { return m_headers; }

                /// \brief Gets the read-only dictionary of HTTP request headers.
                /// \return Const reference to headers dictionary.
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() const { return m_headers; }

                /// \brief Gets the mutable dictionary of decoded URL query parameters.
                /// \return Reference to query parameters dictionary.
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetQuery() { return m_query; }

                /// \brief Gets the read-only dictionary of decoded URL query parameters.
                /// \return Const reference to query parameters dictionary.
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetQuery() const { return m_query; }

                /// \brief Gets the mutable dictionary of matched route parameter values.
                /// \return Reference to route values dictionary.
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetRouteValues() { return m_routeValues; }

                /// \brief Gets the read-only dictionary of matched route parameter values.
                /// \return Const reference to route values dictionary.
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetRouteValues() const { return m_routeValues; }

            private:
                DotNetDupe::System::String m_sMethod;
                DotNetDupe::System::String m_sPath;
                DotNetDupe::System::String m_sBody;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_headers;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_query;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_routeValues;
            };

            /// \brief Encapsulates an outgoing HTTP response within the WebAppCore pipeline.
            ///
            /// Provides status code, content-type, header collection, chunked transfer encoding, and body manipulation.
            ///
            /// \note Thread Safety: Not thread-safe; written sequentially by connection handlers.
            /// \note Conforms to RFC 9110 (HTTP Semantics) and RFC 9112 (HTTP/1.1 chunked transfer-encoding).
            /// \see HttpRequest, HttpContext
            class HttpResponse : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Default constructor initializing status code 200 OK and text/plain content type.
                HttpResponse() : m_nStatusCode(200), m_sContentType("text/plain"), m_bHeadersSent(false), m_bChunked(false) {}

                /// \brief Virtual destructor.
                ~HttpResponse() override = default;

                /// \brief Gets the integer HTTP response status code.
                /// \return Status code (e.g., 200, 404).
                int GetStatusCode() const { return m_nStatusCode; }

                /// \brief Sets the integer HTTP response status code.
                /// \param code Status code integer.
                void SetStatusCode(int code) { m_nStatusCode = code; }

                /// \brief Sets the HTTP response status code via HttpStatusCode enum.
                /// \param code Standard HttpStatusCode value.
                void SetStatusCode(DotNetDupe::System::Net::HttpStatusCode code) { m_nStatusCode = static_cast<int>(code); }

                /// \brief Gets the HTTP response status code as a strongly-typed HttpStatusCode enum.
                /// \return Strongly-typed HttpStatusCode value.
                DotNetDupe::System::Net::HttpStatusCode GetStatusCodeEnum() const { return static_cast<DotNetDupe::System::Net::HttpStatusCode>(m_nStatusCode); }

                /// \brief Gets the Content-Type header string.
                /// \return MIME content type string.
                DotNetDupe::System::String GetContentType() const { return m_sContentType; }

                /// \brief Sets the Content-Type header string.
                /// \param type MIME content type string (e.g., "application/json").
                void SetContentType(const DotNetDupe::System::String& type) { m_sContentType = type; }

                /// \brief Gets the accumulated response body string.
                /// \return Response body content.
                DotNetDupe::System::String GetBody() const { return m_sBody; }

                /// \brief Sets the response body content.
                /// \param body Response body content.
                void SetBody(const DotNetDupe::System::String& body) { m_sBody = body; }

                /// \brief Appends text to the response body buffer.
                /// \param text Text string to append.
                void Write(const DotNetDupe::System::String& text) { m_sBody = m_sBody + text; }

                /// \brief Gets the mutable dictionary of HTTP response headers.
                /// \return Reference to response headers dictionary.
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() { return m_headers; }

                /// \brief Gets the read-only dictionary of HTTP response headers.
                /// \return Const reference to response headers dictionary.
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() const { return m_headers; }

                /// \brief Binds the underlying network stream to this response for direct writing or streaming.
                /// \param pStream SmartPointer to the active NetworkStream.
                void BindStream(DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::Sockets::NetworkStream> pStream) { m_pStream = pStream; }

                /// \brief Indicates whether the HTTP response status line and headers have already been transmitted.
                /// \return True if headers have been written to the network stream; otherwise, false.
                bool IsHeadersSent() const { return m_bHeadersSent; }

                /// \brief Indicates whether chunked transfer-encoding is enabled for this response.
                /// \return True if chunked encoding is active; otherwise, false.
                bool IsChunked() const { return m_bChunked; }

                /// \brief Sets whether chunked transfer-encoding should be used.
                /// \param bChunked True to enable chunked transfer encoding; otherwise, false.
                void SetChunked(bool bChunked) { m_bChunked = bChunked; }

                /// \brief Serializes and transmits the HTTP status line and response headers to the network stream.
                DOTNETDUPE_API void FlushHeaders();

                /// \brief Writes a chunk of data using HTTP/1.1 chunked transfer encoding.
                /// \param data Data chunk to transmit.
                DOTNETDUPE_API void WriteChunk(const DotNetDupe::System::String& data);

                /// \brief Flushes any remaining response body or terminal chunk marker to the client stream.
                DOTNETDUPE_API void Flush();

            private:
                int m_nStatusCode;
                DotNetDupe::System::String m_sContentType;
                DotNetDupe::System::String m_sBody;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_headers;
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::Sockets::NetworkStream> m_pStream;
                bool m_bHeadersSent;
                bool m_bChunked;
            };

            /// \brief Encapsulates all HTTP-specific information about an individual HTTP request and response pair.
            ///
            /// Serves as the central execution context passed throughout the WebAppCore pipeline, controller actions,
            /// and middleware handlers.
            ///
            /// \note Thread Safety: Not thread-safe; lifetime is scoped to a single request-response cycle on a worker thread.
            /// \note Models ASP.NET Core HttpContext per RFC 9110 specifications.
            /// \see HttpRequest, HttpResponse
            class HttpContext : public virtual DotNetDupe::System::Object {
            public:
                /// \brief Default constructor instantiating paired HttpRequest and HttpResponse objects.
                HttpContext() {
                    m_spRequest = DotNetDupe::System::SmartPointer<HttpRequest>::NewShared();
                    m_spResponse = DotNetDupe::System::SmartPointer<HttpResponse>::NewShared();
                }

                /// \brief Virtual destructor.
                ~HttpContext() override = default;

                /// \brief Gets the HttpRequest representation for this execution context.
                /// \return SmartPointer to HttpRequest.
                DotNetDupe::System::SmartPointer<HttpRequest> GetRequest() const { return m_spRequest; }

                /// \brief Gets the HttpResponse representation for this execution context.
                /// \return SmartPointer to HttpResponse.
                DotNetDupe::System::SmartPointer<HttpResponse> GetResponse() const { return m_spResponse; }

            private:
                DotNetDupe::System::SmartPointer<HttpRequest> m_spRequest;
                DotNetDupe::System::SmartPointer<HttpResponse> m_spResponse;
            };

        }
    }
}
