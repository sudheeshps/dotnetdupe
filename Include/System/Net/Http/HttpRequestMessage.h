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

                class HttpRequestMessage : public Object {
                public:
                    DOTNETDUPE_API HttpRequestMessage();
                    DOTNETDUPE_API HttpRequestMessage(const HttpMethod& method, const Uri& requestUri);
                    DOTNETDUPE_API HttpRequestMessage(const HttpMethod& method, const String& requestUri);

                    DOTNETDUPE_API HttpMethod GetMethod() const;
                    DOTNETDUPE_API void SetMethod(const HttpMethod& method);

                    DOTNETDUPE_API Uri GetRequestUri() const;
                    DOTNETDUPE_API void SetRequestUri(const Uri& requestUri);

                    DOTNETDUPE_API HttpContentPtr GetContent() const;
                    DOTNETDUPE_API void SetContent(const HttpContentPtr& content);

                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeaders();
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeaders() const;

                private:
                    HttpMethod m_method;
                    Uri m_requestUri;
                    HttpContentPtr m_pContent;
                    Collections::Generic::Dictionary<String, String> m_headers;
                };

                using HttpRequestMessagePtr = SmartPointer<HttpRequestMessage>;

            }
        }
    }
}
