#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include "WebAppCore/Http/HttpContext.h"
#include "System/Text/Json/JsonSerializer.h"

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

                DotNetDupe::System::SmartPointer<Http::HttpRequest> Request() const { 
                    return m_httpContext->GetRequest(); 
                }
                
                DotNetDupe::System::SmartPointer<Http::HttpResponse> Response() const { 
                    return m_httpContext->GetResponse(); 
                }

                template <typename U>
                DotNetDupe::System::String Ok(const U& value) {
                    Response()->SetStatusCode(200);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(value);
                }

                DotNetDupe::System::String Ok(const DotNetDupe::System::String& body = "") {
                    Response()->SetStatusCode(200);
                    return body;
                }

                template <typename U>
                DotNetDupe::System::String Created(const U& value) {
                    Response()->SetStatusCode(201);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(value);
                }

                DotNetDupe::System::String Created(const DotNetDupe::System::String& body = "") {
                    Response()->SetStatusCode(201);
                    return body;
                }

                DotNetDupe::System::String NoContent() {
                    Response()->SetStatusCode(204);
                    return "";
                }

                DotNetDupe::System::String NotFound(const DotNetDupe::System::String& error = "Not Found") {
                    Response()->SetStatusCode(404);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                DotNetDupe::System::String BadRequest(const DotNetDupe::System::String& error = "Bad Request") {
                    Response()->SetStatusCode(400);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }

                DotNetDupe::System::String Unauthorized(const DotNetDupe::System::String& error = "Unauthorized") {
                    Response()->SetStatusCode(401);
                    Response()->SetContentType("application/json");
                    return DotNetDupe::System::String("{\"error\":\"") + error + "\"}";
                }
            };

        }
    }
}
