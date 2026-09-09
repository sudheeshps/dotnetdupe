/// \file HttpClient.h
/// \brief Provides a base class for sending HTTP requests and receiving HTTP responses mirroring .NET HttpClient.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Uri.h"
#include "System/SmartPointer.h"
#include "System/IO/Stream.h"
#include "System/Net/Http/HttpRequestMessage.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpContent.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
	namespace System {
		namespace Net {
			namespace Sockets {
				class TcpClient;
			}
			namespace Http {

				/// \enum HttpCompletionOption
				/// \brief Indicates if HttpClient operations should be considered completed as soon as headers are read, or after buffering the entire response content.
				enum class HttpCompletionOption {
					/// \brief Operation should complete after reading the entire response including content.
					ResponseContentRead,
					/// \brief Operation should complete as soon as a response is available and headers are read.
					ResponseHeadersRead
				};

				/// \class HttpClient
				/// \brief Sends HTTP requests and receives HTTP responses from a resource identified by a URI.
				///
				/// Supports standard HTTP verbs (GET, POST, PUT, DELETE), header manipulation,
				/// Pimpl ABI isolation, and seamless stream and byte array conversions.
				/// Conforms to RFC 9110 (HTTP Semantics) and RFC 9112 (HTTP/1.1).
				class HttpClient : public Object {
				public:
					/// \brief Initializes a new instance of the HttpClient class.
					DOTNETDUPE_API HttpClient();
					/// \brief Releases unmanaged resources and destroys the HttpClient instance.
					DOTNETDUPE_API ~HttpClient();

					/// \brief Sends a GET request to the specified string Uri.
					/// \param requestUri The URI the request is sent to.
					/// \return The HttpResponseMessagePtr received from the server.
					/// \throws HttpRequestException When connection or transmission fails.
					DOTNETDUPE_API HttpResponseMessagePtr Get(const String& requestUri);

					/// \brief Sends a GET request to the specified Uri object.
					/// \param requestUri The Uri the request is sent to.
					/// \return The HttpResponseMessagePtr received from the server.
					/// \throws HttpRequestException When connection or transmission fails.
					DOTNETDUPE_API HttpResponseMessagePtr Get(const Uri& requestUri);

					/// \brief Sends a POST request with content to the specified string Uri.
					/// \param requestUri The target request URI string.
					/// \param content The HttpContentPtr payload to transmit.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Post(const String& requestUri, const HttpContentPtr& content);

					/// \brief Sends a POST request with content to the specified Uri.
					/// \param requestUri The target Uri.
					/// \param content The HttpContentPtr payload to transmit.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Post(const Uri& requestUri, const HttpContentPtr& content);

					/// \brief Sends a PUT request with content to the specified string Uri.
					/// \param requestUri The target URI string.
					/// \param content The HttpContentPtr payload to transmit.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Put(const String& requestUri, const HttpContentPtr& content);

					/// \brief Sends a PUT request with content to the specified Uri.
					/// \param requestUri The target Uri.
					/// \param content The HttpContentPtr payload to transmit.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Put(const Uri& requestUri, const HttpContentPtr& content);

					/// \brief Sends a DELETE request to the specified string Uri.
					/// \param requestUri The URI string to send the DELETE request to.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Delete(const String& requestUri);

					/// \brief Sends a DELETE request to the specified Uri.
					/// \param requestUri The Uri to send the DELETE request to.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Delete(const Uri& requestUri);

					/// \brief Sends an HTTP request as an operation.
					/// \param request The HttpRequestMessagePtr to transmit.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Send(const HttpRequestMessagePtr& request);

					/// \brief Sends an HTTP request with specific completion options.
					/// \param request The HttpRequestMessagePtr to transmit.
					/// \param completionOption Option indicating when the operation completes.
					/// \return The HttpResponseMessagePtr received.
					DOTNETDUPE_API HttpResponseMessagePtr Send(const HttpRequestMessagePtr& request, HttpCompletionOption completionOption);

					/// \brief Sends a GET request to the specified URI and returns the response body as a string.
					/// \param requestUri The URI string to query.
					/// \return The response body string.
					DOTNETDUPE_API String GetString(const String& requestUri);

					/// \brief Sends a GET request to the specified Uri and returns the response body as a string.
					/// \param requestUri The Uri to query.
					/// \return The response body string.
					DOTNETDUPE_API String GetString(const Uri& requestUri);

					/// \brief Sends a GET request to the specified URI and returns the response body as a byte array.
					/// \param requestUri The URI string to query.
					/// \return Array of raw response bytes.
					DOTNETDUPE_API Array<char> GetByteArray(const String& requestUri);

					/// \brief Sends a GET request to the specified Uri and returns the response body as a byte array.
					/// \param requestUri The Uri to query.
					/// \return Array of raw response bytes.
					DOTNETDUPE_API Array<char> GetByteArray(const Uri& requestUri);

					/// \brief Gets the headers which should be sent with each request.
					/// \return Mutable reference to headers Dictionary.
					DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders();

					/// \brief Gets the headers which should be sent with each request as read-only.
					/// \return Const reference to headers Dictionary.
					DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders() const;

				private:
					struct Impl;
					SmartPointer<Impl> m_pImpl;

				};
			}
		}
	}
}


