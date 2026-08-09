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
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {
                class TcpClient;
            }
            namespace Http {

                enum class HttpCompletionOption {
                    ResponseContentRead,
                    ResponseHeadersRead
                };

                class HttpClient : public Object {
                public:
                    DOTNETDUPE_API HttpClient();
                    DOTNETDUPE_API ~HttpClient() = default;

                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Get(const String& requestUri);
                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Get(const Uri& requestUri);

                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Post(const String& requestUri, const SmartPointer<HttpContent>& content);
                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Post(const Uri& requestUri, const SmartPointer<HttpContent>& content);

                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Put(const String& requestUri, const SmartPointer<HttpContent>& content);
                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Put(const Uri& requestUri, const SmartPointer<HttpContent>& content);

                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Delete(const String& requestUri);
                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Delete(const Uri& requestUri);

                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Send(const SmartPointer<HttpRequestMessage>& request);
                    DOTNETDUPE_API SmartPointer<HttpResponseMessage> Send(const SmartPointer<HttpRequestMessage>& request, HttpCompletionOption completionOption);

                    DOTNETDUPE_API String GetString(const String& requestUri);
                    DOTNETDUPE_API String GetString(const Uri& requestUri);

                    DOTNETDUPE_API Array<char> GetByteArray(const String& requestUri);
                    DOTNETDUPE_API Array<char> GetByteArray(const Uri& requestUri);

                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders();
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders() const;

                private:
                    Collections::Generic::Dictionary<String, String> m_defaultRequestHeaders;
                    SmartPointer<Sockets::TcpClient> m_pLastTcpClient;

                    String ResolveHost(const Uri& uri, int& riPort);
                    std::string PrepareHeaders(const SmartPointer<HttpRequestMessage>& spRequest, const Uri& uri);
                    void SendRequest(const SmartPointer<IO::Stream>& spStream, const std::string& sHeaders, const SmartPointer<HttpContent>& spContent);
                    SmartPointer<HttpResponseMessage> PrepareResponse(const SmartPointer<IO::Stream>& spStream);
                    SmartPointer<HttpResponseMessage> ParseStatusLine(const SmartPointer<IO::Stream>& spStream);
                    void ParseHeaders(const SmartPointer<IO::Stream>& spStream, const SmartPointer<HttpResponseMessage>& spResponse, bool& rbChunked, long& rlContentLength, String& rsContentType);
                    Array<char> ReadResponseBody(const SmartPointer<IO::Stream>& spStream, bool bChunked, long lContentLength);
                };

            }
        }
    }
}


