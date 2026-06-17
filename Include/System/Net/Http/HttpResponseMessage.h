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

                class HttpResponseMessage : public Object {
                public:
                    DOTNETDUPE_API HttpResponseMessage();
                    DOTNETDUPE_API explicit HttpResponseMessage(HttpStatusCode statusCode);

                    DOTNETDUPE_API HttpStatusCode GetStatusCode() const;
                    DOTNETDUPE_API void SetStatusCode(HttpStatusCode statusCode);

                    DOTNETDUPE_API String GetReasonPhrase() const;
                    DOTNETDUPE_API void SetReasonPhrase(const String& reasonPhrase);

                    DOTNETDUPE_API bool IsSuccessStatusCode() const;

                    DOTNETDUPE_API SmartPointer<HttpContent> GetContent() const;
                    DOTNETDUPE_API void SetContent(const SmartPointer<HttpContent>& content);

                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeaders();
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeaders() const;

                    DOTNETDUPE_API void EnsureSuccessStatusCode();

                private:
                    HttpStatusCode m_statusCode;
                    String m_sReasonPhrase;
                    SmartPointer<HttpContent> m_pContent;
                    Collections::Generic::Dictionary<String, String> m_headers;
                };

            }
        }
    }
}
