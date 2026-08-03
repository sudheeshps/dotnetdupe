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

            class DOTNETDUPE_API HttpRequest : public virtual DotNetDupe::System::Object {
            public:
                HttpRequest() = default;
                ~HttpRequest() override = default;

                DotNetDupe::System::String GetMethod() const { return m_sMethod; }
                void SetMethod(const DotNetDupe::System::String& method) { m_sMethod = method; }

                DotNetDupe::System::String GetPath() const { return m_sPath; }
                void SetPath(const DotNetDupe::System::String& path) { m_sPath = path; }

                DotNetDupe::System::String GetBody() const { return m_sBody; }
                void SetBody(const DotNetDupe::System::String& body) { m_sBody = body; }

                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() { return m_headers; }
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() const { return m_headers; }

                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetQuery() { return m_query; }
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetQuery() const { return m_query; }

                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetRouteValues() { return m_routeValues; }
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetRouteValues() const { return m_routeValues; }

            private:
                DotNetDupe::System::String m_sMethod;
                DotNetDupe::System::String m_sPath;
                DotNetDupe::System::String m_sBody;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_headers;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_query;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_routeValues;
            };

            class DOTNETDUPE_API HttpResponse : public virtual DotNetDupe::System::Object {
            public:
                HttpResponse() : m_nStatusCode(200), m_sContentType("text/plain"), m_bHeadersSent(false), m_bChunked(false) {}
                ~HttpResponse() override = default;

                int GetStatusCode() const { return m_nStatusCode; }
                void SetStatusCode(int code) { m_nStatusCode = code; }
                void SetStatusCode(DotNetDupe::System::Net::HttpStatusCode code) { m_nStatusCode = static_cast<int>(code); }
                DotNetDupe::System::Net::HttpStatusCode GetStatusCodeEnum() const { return static_cast<DotNetDupe::System::Net::HttpStatusCode>(m_nStatusCode); }

                DotNetDupe::System::String GetContentType() const { return m_sContentType; }
                void SetContentType(const DotNetDupe::System::String& type) { m_sContentType = type; }

                DotNetDupe::System::String GetBody() const { return m_sBody; }
                void SetBody(const DotNetDupe::System::String& body) { m_sBody = body; }

                void Write(const DotNetDupe::System::String& text) { m_sBody = m_sBody + text; }

                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() { return m_headers; }
                const DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& GetHeaders() const { return m_headers; }

                void BindStream(DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::Sockets::NetworkStream> pStream) { m_pStream = pStream; }
                bool IsHeadersSent() const { return m_bHeadersSent; }
                bool IsChunked() const { return m_bChunked; }
                void SetChunked(bool bChunked) { m_bChunked = bChunked; }

                void FlushHeaders();
                void WriteChunk(const DotNetDupe::System::String& data);
                void Flush();

            private:
                int m_nStatusCode;
                DotNetDupe::System::String m_sContentType;
                DotNetDupe::System::String m_sBody;
                DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String> m_headers;
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Net::Sockets::NetworkStream> m_pStream;
                bool m_bHeadersSent;
                bool m_bChunked;
            };

            class DOTNETDUPE_API HttpContext : public virtual DotNetDupe::System::Object {
            public:
                HttpContext() {
                    m_spRequest = DotNetDupe::System::SmartPointer<HttpRequest>::NewShared();
                    m_spResponse = DotNetDupe::System::SmartPointer<HttpResponse>::NewShared();
                }
                ~HttpContext() override = default;

                DotNetDupe::System::SmartPointer<HttpRequest> GetRequest() const { return m_spRequest; }
                DotNetDupe::System::SmartPointer<HttpResponse> GetResponse() const { return m_spResponse; }

            private:
                DotNetDupe::System::SmartPointer<HttpRequest> m_spRequest;
                DotNetDupe::System::SmartPointer<HttpResponse> m_spResponse;
            };

        }
    }
}
