#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include "WebAppCore/Http/HttpContext.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Controllers {

            class ControllerBase : public virtual DotNetDupe::System::Object {
            protected:
                DotNetDupe::System::SmartPointer<Http::HttpContext> m_httpContext;

            public:
                ControllerBase() = default;
                ~ControllerBase() override = default;

                void Initialize(const DotNetDupe::System::SmartPointer<Http::HttpContext>& context) {
                    m_httpContext = context;
                }

                // Server-side Authorization check
                bool Authorize(const DotNetDupe::System::String& sSecretKey, DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& claims) {
                    DotNetDupe::System::String sAuthHeader;
                    if (!Request()->GetHeaders().TryGetValue("authorization", sAuthHeader)) {
                        Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                        Response()->SetContentType("application/json");
                        Response()->SetBody("{\"error\":\"Unauthorized - Missing Authorization header\"}");
                        return false;
                    }

                    if (!sAuthHeader.StartsWith("Bearer ", false)) {
                        Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                        Response()->SetContentType("application/json");
                        Response()->SetBody("{\"error\":\"Unauthorized - Invalid authorization scheme\"}");
                        return false;
                    }

                    DotNetDupe::System::String sToken = sAuthHeader.Substring(7);
                    try {
                        auto spToken = DotNetDupe::System::IdentityModel::Tokens::Jwt::JWTToken::Parse(sToken);
                        if (spToken.IsNull() || !spToken->Verify(sSecretKey)) {
                            Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                            Response()->SetContentType("application/json");
                            Response()->SetBody("{\"error\":\"Unauthorized - Invalid token signature\"}");
                            return false;
                        }

                        // Copy claims
                        auto& payload = spToken->GetPayload();
                        auto keys = payload.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            claims.Add(keys[i], payload[keys[i]]);
                        }
                        return true;
                    } catch (...) {
                        Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                        Response()->SetContentType("application/json");
                        Response()->SetBody("{\"error\":\"Unauthorized - Token parsing failed\"}");
                        return false;
                    }
                }

                // Server-side Authorization check for a specific claim
                bool Authorize(const DotNetDupe::System::String& sSecretKey, const DotNetDupe::System::String& sRequiredClaim, const DotNetDupe::System::String& sRequiredValue, DotNetDupe::System::Collections::Generic::Dictionary<DotNetDupe::System::String, DotNetDupe::System::String>& claims) {
                    if (!Authorize(sSecretKey, claims)) {
                        return false;
                    }

                    DotNetDupe::System::String sClaimValue;
                    if (!claims.TryGetValue(sRequiredClaim, sClaimValue) || sClaimValue != sRequiredValue) {
                        Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Forbidden);
                        Response()->SetContentType("application/json");
                        Response()->SetBody("{\"error\":\"Forbidden - Missing required claim value\"}");
                        return false;
                    }
                    return true;
                }

                DotNetDupe::System::String Forbidden(const DotNetDupe::System::String& error = "Forbidden") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Forbidden);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                DotNetDupe::System::SmartPointer<Http::HttpRequest> Request() const { 
                    return m_httpContext->GetRequest(); 
                }
                
                DotNetDupe::System::SmartPointer<Http::HttpResponse> Response() const { 
                    return m_httpContext->GetResponse(); 
                }

                template <typename U>
                DotNetDupe::System::String Ok(const U& value) {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::OK);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(value);
                }

                DotNetDupe::System::String Ok(const DotNetDupe::System::String& body = "") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::OK);
                    return body;
                }

                template <typename U>
                DotNetDupe::System::String Created(const U& value) {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Created);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(value);
                }

                DotNetDupe::System::String Created(const DotNetDupe::System::String& body = "") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Created);
                    return body;
                }

                DotNetDupe::System::String NoContent() {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::NoContent);
                    return "";
                }

                DotNetDupe::System::String NotFound(const DotNetDupe::System::String& error = "Not Found") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::NotFound);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                DotNetDupe::System::String BadRequest(const DotNetDupe::System::String& error = "Bad Request") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::BadRequest);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                DotNetDupe::System::String Unauthorized(const DotNetDupe::System::String& error = "Unauthorized") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }
            };

        }
    }
}
