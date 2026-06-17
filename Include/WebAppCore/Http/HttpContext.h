#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Http {

            class HttpRequest : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API HttpRequest() = default;
                DOTNETDUPE_API ~HttpRequest() override = default;

                DOTNETDUPE_API DotNetDupe::System::String GetMethod() const { return m_sMethod; }
                DOTNETDUPE_API void SetMethod(const DotNetDupe::System::String& method) { m_sMethod = method; }

                DOTNETDUPE_API DotNetDupe::System::String GetPath() const { return m_sPath; }
                DOTNETDUPE_API void SetPath(const DotNetDupe::System::String& path) { m_sPath = path; }

                DOTNETDUPE_API DotNetDupe::System::String GetBody() const { return m_sBody; }
                DOTNETDUPE_API void SetBody(const DotNetDupe::System::String& body) { m_sBody = body; }

                DOTNETDUPE_API DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() { return m_headers; }
                DOTNETDUPE_API const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() const { return m_headers; }

                DOTNETDUPE_API DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetQuery() { return m_query; }
                DOTNETDUPE_API const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetQuery() const { return m_query; }

            private:
                DotNetDupe::System::String m_sMethod;
                DotNetDupe::System::String m_sPath;
                DotNetDupe::System::String m_sBody;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_headers;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_query;
            };

            class HttpResponse : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API HttpResponse() : m_nStatusCode(200), m_sContentType("text/plain") {}
                DOTNETDUPE_API ~HttpResponse() override = default;

                DOTNETDUPE_API int GetStatusCode() const { return m_nStatusCode; }
                DOTNETDUPE_API void SetStatusCode(int code) { m_nStatusCode = code; }

                DOTNETDUPE_API DotNetDupe::System::String GetContentType() const { return m_sContentType; }
                DOTNETDUPE_API void SetContentType(const DotNetDupe::System::String& type) { m_sContentType = type; }

                DOTNETDUPE_API DotNetDupe::System::String GetBody() const { return m_sBody; }
                DOTNETDUPE_API void SetBody(const DotNetDupe::System::String& body) { m_sBody = body; }

                DOTNETDUPE_API void Write(const DotNetDupe::System::String& text) { m_sBody = m_sBody + text; }

                DOTNETDUPE_API DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() { return m_headers; }
                DOTNETDUPE_API const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() const { return m_headers; }

            private:
                int m_nStatusCode;
                DotNetDupe::System::String m_sContentType;
                DotNetDupe::System::String m_sBody;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_headers;
            };

            class HttpContext : public virtual DotNetDupe::System::Object {
            public:
                DOTNETDUPE_API HttpContext() {
                    m_spRequest = DotNetDupe::System::SmartPointer<HttpRequest>::NewShared();
                    m_spResponse = DotNetDupe::System::SmartPointer<HttpResponse>::NewShared();
                }
                DOTNETDUPE_API ~HttpContext() override = default;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<HttpRequest> GetRequest() const { return m_spRequest; }
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<HttpResponse> GetResponse() const { return m_spResponse; }

            private:
                DotNetDupe::System::SmartPointer<HttpRequest> m_spRequest;
                DotNetDupe::System::SmartPointer<HttpResponse> m_spResponse;
            };

        }
    }
}
