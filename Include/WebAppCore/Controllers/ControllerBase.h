/// \file ControllerBase.h
/// \brief Base controller class providing REST action helpers, JWT authorization, and HTTP context accessors.

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

            /// \brief Base class for MVC/REST API controllers mirroring ASP.NET Core `ControllerBase`.
            ///
            /// Provides core facilities for extracting HTTP request headers and parameters, serializing JSON responses,
            /// evaluating RFC 7519 JSON Web Token (JWT) bearer authentication, and formatting standard HTTP status responses.
            ///
            /// \note Thread Safety: Controller instances are transiently instantiated or initialized per request and are not shared across threads.
            /// \note Conforms to RFC 9110 (HTTP Semantics) and RFC 7519 (JSON Web Tokens).
            /// \see HttpContext, ControllerRouteBuilder, JWTToken
            class ControllerBase : public virtual DotNetDupe::System::Object {
            protected:
                /// \brief Active HTTP context encapsulating request and response streams for this execution.
                DotNetDupe::System::SmartPointer<Http::HttpContext> m_httpContext;

            public:
                /// \brief Default constructor.
                ControllerBase() = default;

                /// \brief Virtual destructor.
                ~ControllerBase() override = default;

                /// \brief Binds the active HTTP context to this controller instance.
                /// \param context SmartPointer to HttpContext.
                void Initialize(const DotNetDupe::System::SmartPointer<Http::HttpContext>& context) {
                    m_httpContext = context;
                }

                /// \brief Validates incoming Bearer JWT authentication token from the Authorization header against the secret key.
                /// \param sSecretKey HMAC SHA-256 secret key used to verify the JWT signature.
                /// \param claims Output dictionary populated with the decoded JWT claims if verification succeeds.
                /// \return True if the token is valid, unexpired, and verified; otherwise, false (sets 401 Unauthorized response).
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
                    } catch (const DotNetDupe::System::Exception&) {
                        Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                        Response()->SetContentType("application/json");
                        Response()->SetBody("{\"error\":\"Unauthorized - Token parsing failed\"}");
                        return false;
                    } catch (const std::exception&) {
                        Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                        Response()->SetContentType("application/json");
                        Response()->SetBody("{\"error\":\"Unauthorized - Token parsing failed\"}");
                        return false;
                    }
                }

                /// \brief Validates incoming Bearer JWT authentication and ensures a specific claim and value match.
                /// \param sSecretKey HMAC SHA-256 secret key used to verify the JWT signature.
                /// \param sRequiredClaim Required claim key name (e.g., "role").
                /// \param sRequiredValue Expected claim value (e.g., "admin").
                /// \param claims Output dictionary populated with the decoded JWT claims if verification succeeds.
                /// \return True if authorized and claim matches; otherwise, false (sets 401 or 403 response).
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

                /// \brief Sets a 403 Forbidden HTTP status and returns a JSON error payload.
                /// \param error Error message string (default "Forbidden").
                /// \return JSON formatted error string.
                DotNetDupe::System::String Forbidden(const DotNetDupe::System::String& error = "Forbidden") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Forbidden);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                /// \brief Gets the active HTTP request abstraction.
                /// \return SmartPointer to HttpRequest.
                DotNetDupe::System::SmartPointer<Http::HttpRequest> Request() const { 
                    return m_httpContext->GetRequest(); 
                }
                
                /// \brief Gets the active HTTP response abstraction.
                /// \return SmartPointer to HttpResponse.
                DotNetDupe::System::SmartPointer<Http::HttpResponse> Response() const { 
                    return m_httpContext->GetResponse(); 
                }

                /// \brief Sets a 200 OK HTTP status and serializes the given object model as JSON.
                /// \tparam U Type of the value to serialize.
                /// \param value Object instance to serialize.
                /// \return JSON serialized string representation.
                template <typename U>
                DotNetDupe::System::String Ok(const U& value) {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::OK);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(value);
                }

                /// \brief Sets a 200 OK HTTP status and returns raw string response content.
                /// \param body Response body payload (default empty).
                /// \return The response body string.
                DotNetDupe::System::String Ok(const DotNetDupe::System::String& body = "") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::OK);
                    return body;
                }

                /// \brief Sets a 201 Created HTTP status and serializes the given resource model as JSON.
                /// \tparam U Type of the resource to serialize.
                /// \param value Resource instance to serialize.
                /// \return JSON serialized string representation.
                template <typename U>
                DotNetDupe::System::String Created(const U& value) {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Created);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(value);
                }

                /// \brief Sets a 201 Created HTTP status and returns raw string response content.
                /// \param body Response body payload (default empty).
                /// \return The response body string.
                DotNetDupe::System::String Created(const DotNetDupe::System::String& body = "") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Created);
                    return body;
                }

                /// \brief Sets a 204 No Content HTTP status and returns an empty body.
                /// \return Empty string.
                DotNetDupe::System::String NoContent() {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::NoContent);
                    return "";
                }

                /// \brief Sets a 404 Not Found HTTP status and returns a JSON error payload.
                /// \param error Error message string (default "Not Found").
                /// \return JSON formatted error string.
                DotNetDupe::System::String NotFound(const DotNetDupe::System::String& error = "Not Found") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::NotFound);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                /// \brief Sets a 400 Bad Request HTTP status and returns a JSON error payload.
                /// \param error Error message string (default "Bad Request").
                /// \return JSON formatted error string.
                DotNetDupe::System::String BadRequest(const DotNetDupe::System::String& error = "Bad Request") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::BadRequest);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                /// \brief Sets a 401 Unauthorized HTTP status and returns a JSON error payload.
                /// \param error Error message string (default "Unauthorized").
                /// \return JSON formatted error string.
                DotNetDupe::System::String Unauthorized(const DotNetDupe::System::String& error = "Unauthorized") {
                    Response()->SetStatusCode(DotNetDupe::System::Net::HttpStatusCode::Unauthorized);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }
            };

        }
    }
}
