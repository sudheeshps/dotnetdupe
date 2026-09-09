/// \file RestClient.h
/// \brief Strongly typed REST client template with automatic JSON serialization and deserialization.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/List.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Text/Json/JsonSerializer.h"

#include "System/Convert.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                /// \class RestClient
                /// \brief Strongly-typed REST client template with automated C++ structure JSON serialization/deserialization.
                /// \tparam TResource The resource model type conforming to JSON serialization.
                ///
                /// Conforms to RFC 9110 (HTTP Semantics) and RFC 8259 (The JavaScript Object Notation (JSON) Data Interchange Format).
                template <typename TResource>
                class RestClient : public virtual DotNetDupe::System::Object {
                private:
                    DotNetDupe::System::String m_sBaseUrl;
                    HttpClient m_client;

                public:
                    /// \brief Initializes a new instance of RestClient with a base resource endpoint URL.
                    /// \param sBaseUrl The base endpoint URI (e.g. "https://api.example.com/items").
                    RestClient(const DotNetDupe::System::String& sBaseUrl) : m_sBaseUrl(sBaseUrl) {
                        m_client.GetDefaultRequestHeaders().Add("accept", "application/json");
                    }

                    /// \brief Virtual destructor.
                    ~RestClient() override = default;

                    /// \brief Sets a Bearer token for the HTTP Authorization header (RFC 6750).
                    /// \param sToken The JWT or OAuth bearer token string.
                    void SetBearerToken(const DotNetDupe::System::String& sToken) {
                        auto& headers = m_client.GetDefaultRequestHeaders();
                        headers.Remove("Authorization");
                        headers.Remove("authorization");
                        headers.Add("Authorization", DotNetDupe::System::String("Bearer ") + sToken);
                    }

                    /// \brief Sets Basic Authentication (username:password) encoded via Base64 for HTTP Authorization header (RFC 7617).
                    /// \param sUsername The authentication username.
                    /// \param sPassword The authentication user password.
                    void SetBasicAuthentication(const DotNetDupe::System::String& sUsername, const DotNetDupe::System::String& sPassword) {
                        DotNetDupe::System::String credentials = sUsername + ":" + sPassword;
                        int len = credentials.GetLength();
                        DotNetDupe::System::Array<char> arr(len);
                        for (int i = 0; i < len; ++i) {
                            arr[i] = credentials[i];
                        }
                        DotNetDupe::System::String base64 = DotNetDupe::System::Convert::ToBase64String(arr);
                        
                        auto& headers = m_client.GetDefaultRequestHeaders();
                        headers.Remove("Authorization");
                        headers.Remove("authorization");
                        headers.Add("Authorization", DotNetDupe::System::String("Basic ") + base64);
                    }

                    /// \brief Clears custom authentication headers.
                    void ClearAuthentication() {
                        auto& headers = m_client.GetDefaultRequestHeaders();
                        headers.Remove("Authorization");
                        headers.Remove("authorization");
                    }

                    /// \brief Adds a custom default request header sent with all HTTP calls.
                    /// \param sName Header key name.
                    /// \param sValue Header value.
                    void AddDefaultRequestHeader(const DotNetDupe::System::String& sName, const DotNetDupe::System::String& sValue) {
                        auto& headers = m_client.GetDefaultRequestHeaders();
                        headers.Remove(sName);
                        headers.Add(sName, sValue);
                    }

                    /// \brief Removes a specific default request header.
                    /// \param sName Header key name to remove.
                    void RemoveDefaultRequestHeader(const DotNetDupe::System::String& sName) {
                        m_client.GetDefaultRequestHeaders().Remove(sName);
                    }

                    /// \brief Sends GET request to endpoint and deserializes JSON response into a List of models.
                    /// \return List of TResource entities.
                    DotNetDupe::System::Collections::Generic::List<TResource> GetAll() {
                        auto resp = m_client.Get(m_sBaseUrl);
                        resp->EnsureSuccessStatusCode();
                        DotNetDupe::System::String body = resp->GetContent()->ReadAsString();
                        return DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<DotNetDupe::System::Collections::Generic::List<TResource>>(body);
                    }

                    /// \brief Sends GET request for a specific entity ID and deserializes the JSON response.
                    /// \param sId The resource identifier.
                    /// \return The deserialized TResource model.
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

                    /// \brief Sends POST request with serialized resource and returns raw string response body.
                    /// \param resource The TResource model to serialize into JSON body.
                    /// \return Response body as string.
                    DotNetDupe::System::String Post(const TResource& resource) {
                        DotNetDupe::System::String json = DotNetDupe::System::Text::Json::JsonSerializer::Serialize(resource);
                        auto content = DotNetDupe::System::SmartPointer<StringContent>::NewShared(json, "application/json");
                        auto resp = m_client.Post(m_sBaseUrl, content);
                        resp->EnsureSuccessStatusCode();
                        return resp->GetContent()->ReadAsString();
                    }

                    /// \brief Sends POST request with serialized resource and deserializes response into TResult.
                    /// \tparam TResult The expected response entity model type.
                    /// \param resource The TResource payload.
                    /// \return Deserialized TResult instance.
                    template <typename TResult = TResource>
                    TResult PostAndReturn(const TResource& resource) {
                        DotNetDupe::System::String json = DotNetDupe::System::Text::Json::JsonSerializer::Serialize(resource);
                        auto content = DotNetDupe::System::SmartPointer<StringContent>::NewShared(json, "application/json");
                        auto resp = m_client.Post(m_sBaseUrl, content);
                        resp->EnsureSuccessStatusCode();
                        DotNetDupe::System::String body = resp->GetContent()->ReadAsString();
                        return DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResult>(body);
                    }

                    /// \brief Sends PUT request updating a specific resource ID and returns raw response string.
                    /// \param sId The target entity identifier.
                    /// \param resource The updated TResource payload.
                    /// \return Response body as string.
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

                    /// \brief Sends DELETE request targeting a specific resource ID.
                    /// \param sId The resource identifier to delete.
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
