#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Uri.h"
#include "System/SmartPointer.h"
#include "System/Net/Http/HttpRequestMessage.h"
#include "System/Net/Http/HttpResponseMessage.h"
#include "System/Net/Http/HttpContent.h"
#include "System/Collections/Generic/Dictionary.h"
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {
                class NetworkStream;
            }

            namespace Http {

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

                    DOTNETDUPE_API String GetString(const String& requestUri);
                    DOTNETDUPE_API String GetString(const Uri& requestUri);

                    DOTNETDUPE_API Array<char> GetByteArray(const String& requestUri);
                    DOTNETDUPE_API Array<char> GetByteArray(const Uri& requestUri);

                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders();
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetDefaultRequestHeaders() const;

                private:
                    Collections::Generic::Dictionary<String, String> m_defaultRequestHeaders;

                    String ResolveHost(const Uri& uri, int& riPort);
                    std::string PrepareHeaders(const SmartPointer<HttpRequestMessage>& spRequest, const Uri& uri);
                    void SendRequest(const SmartPointer<Sockets::NetworkStream>& spStream, const std::string& sHeaders, const SmartPointer<HttpContent>& spContent);
                    SmartPointer<HttpResponseMessage> PrepareResponse(const SmartPointer<Sockets::NetworkStream>& spStream);
                };

            }
        }
    }
}

