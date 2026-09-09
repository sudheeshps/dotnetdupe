/**
 * @file HttpClient.h
 * @brief Provides a base class for sending HTTP requests and receiving HTTP responses mirroring .NET HttpClient.
 */

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

				/**
				 * @enum HttpCompletionOption
				 * @brief Indicates if HttpClient operations should be considered completed as soon as a response is available, or after reading the entire response content.
				 */
				enum class HttpCompletionOption {
					ResponseContentRead,
					ResponseHeadersRead
				};

				/**
				 * @class HttpClient
				 * @brief Sends HTTP requests and receives HTTP responses from a resource identified by a URI.
				 * 
				 * Supports standard HTTP verbs (GET, POST, PUT, DELETE), header manipulation,
				 * Pimpl ABI isolation, and seamless stream and byte array conversions.
				 */
				class HttpClient : public Object {
				public:
					DOTNETDUPE_API HttpClient();
					DOTNETDUPE_API ~HttpClient();

					/**
					 * @brief Sends a GET request to the specified Uri as an asynchronous operation.
					 */
					DOTNETDUPE_API HttpResponseMessagePtr Get(const String& requestUri);
					DOTNETDUPE_API HttpResponseMessagePtr Get(const Uri& requestUri);

					/**
					 * @brief Sends a POST request to the specified Uri as an asynchronous operation.
					 */
					DOTNETDUPE_API HttpResponseMessagePtr Post(const String& requestUri, const HttpContentPtr& content);
					DOTNETDUPE_API HttpResponseMessagePtr Post(const Uri& requestUri, const HttpContentPtr& content);

					DOTNETDUPE_API HttpResponseMessagePtr Put(const String& requestUri, const HttpContentPtr& content);
					DOTNETDUPE_API HttpResponseMessagePtr Put(const Uri& requestUri, const HttpContentPtr& content);

					DOTNETDUPE_API HttpResponseMessagePtr Delete(const String& requestUri);
					DOTNETDUPE_API HttpResponseMessagePtr Delete(const Uri& requestUri);

					DOTNETDUPE_API HttpResponseMessagePtr Send(const HttpRequestMessagePtr& request);
					DOTNETDUPE_API HttpResponseMessagePtr Send(const HttpRequestMessagePtr& request, HttpCompletionOption completionOption);

					DOTNETDUPE_API String GetString(const String& requestUri);
					DOTNETDUPE_API String GetString(const Uri& requestUri);

					DOTNETDUPE_API Array<char> GetByteArray(const String& requestUri);
					DOTNETDUPE_API Array<char> GetByteArray(const Uri& requestUri);

					DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders();
					DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders() const;

				private:
					struct Impl;
					SmartPointer<Impl> m_pImpl;

				};
			}
		}
	}
}


