#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Text/Json/JsonSerializer.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                template <typename TResource>
                class RestClient : public virtual DotNetDupe::System::Object {
                private:
                    DotNetDupe::System::String m_sBaseUrl;
                    HttpClient m_client;

                public:
                    RestClient(const DotNetDupe::System::String& sBaseUrl) : m_sBaseUrl(sBaseUrl) {
                        m_client.GetDefaultRequestHeaders().Add("accept", "application/json");
                    }

                    ~RestClient() override = default;

                    // Gets all resources: GET /api/resource
                    DotNetDupe::System::Collections::Generic::List<TResource> GetAll() {
                        auto resp = m_client.Get(m_sBaseUrl);
                        resp->EnsureSuccessStatusCode();
                        DotNetDupe::System::String body = resp->GetContent()->ReadAsString();
                        return DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<DotNetDupe::System::Collections::Generic::List<TResource>>(body);
                    }

                    // Gets a resource by ID: GET /api/resource/{id}
                    TResource Get(const DotNetDupe::System::String& sId) {
                        DotNetDupe::System::String url = m_sBaseUrl;
                        if (!url.EndsWith('/', false)) {
                            url = url + "/";
                        }
                        url = url + sId;
                        auto resp = m_client.Get(url);
                        resp->EnsureSuccessStatusCode();
                        DotNetDupe::System::String body = resp->GetContent()->ReadAsString();
                        return DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                    }

                    // POST: POST /api/resource (Sends resource, returns raw string response e.g., Created status)
                    DotNetDupe::System::String Post(const TResource& resource) {
                        DotNetDupe::System::String json = DotNetDupe::System::Text::Json::JsonSerializer::Serialize(resource);
                        auto content = DotNetDupe::System::SmartPointer<StringContent>::NewShared(json, "application/json");
                        auto resp = m_client.Post(m_sBaseUrl, content);
                        resp->EnsureSuccessStatusCode();
                        return resp->GetContent()->ReadAsString();
                    }

                    // POST and returns deserialized response: POST /api/resource
                    template <typename TResult = TResource>
                    TResult PostAndReturn(const TResource& resource) {
                        DotNetDupe::System::String json = DotNetDupe::System::Text::Json::JsonSerializer::Serialize(resource);
                        auto content = DotNetDupe::System::SmartPointer<StringContent>::NewShared(json, "application/json");
                        auto resp = m_client.Post(m_sBaseUrl, content);
                        resp->EnsureSuccessStatusCode();
                        DotNetDupe::System::String body = resp->GetContent()->ReadAsString();
                        return DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResult>(body);
                    }

                    // PUT: PUT /api/resource/{id} (Sends resource, returns raw string response)
                    DotNetDupe::System::String Put(const DotNetDupe::System::String& sId, const TResource& resource) {
                        DotNetDupe::System::String url = m_sBaseUrl;
                        if (!url.EndsWith('/', false)) {
                            url = url + "/";
                        }
                        url = url + sId;
                        DotNetDupe::System::String json = DotNetDupe::System::Text::Json::JsonSerializer::Serialize(resource);
                        auto content = DotNetDupe::System::SmartPointer<StringContent>::NewShared(json, "application/json");
                        auto resp = m_client.Put(url, content);
                        resp->EnsureSuccessStatusCode();
                        return resp->GetContent()->ReadAsString();
                    }

                    // DELETE: DELETE /api/resource/{id}
                    void Delete(const DotNetDupe::System::String& sId) {
                        DotNetDupe::System::String url = m_sBaseUrl;
                        if (!url.EndsWith('/', false)) {
                            url = url + "/";
                        }
                        url = url + sId;
                        auto resp = m_client.Delete(url);
                        resp->EnsureSuccessStatusCode();
                    }
                };

            }
        }
    }
}
