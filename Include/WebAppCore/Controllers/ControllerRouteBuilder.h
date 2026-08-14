#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include "System/Func.h"
#include "System/Convert.h"
#include "System/Collections/Generic/List.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Http/HttpContext.h"
#include "System/Text/Json/JsonSerializer.h"
#include <type_traits>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Controllers {

            class IControllerRouteRegistrar : public virtual DotNetDupe::System::Object {
            public:
                virtual ~IControllerRouteRegistrar() = default;
                virtual void Register(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app) = 0;
            };

            template <typename TController>
            class ControllerRouteBuilder : public IControllerRouteRegistrar {
            private:
                DotNetDupe::System::String m_routePrefix;

                struct RouteInfo {
                    DotNetDupe::System::String Method;
                    DotNetDupe::System::String SubPath;
                    DotNetDupe::System::Func<DotNetDupe::System::String, const DotNetDupe::System::SmartPointer<Builder::WebApplication>&, const DotNetDupe::System::SmartPointer<Http::HttpContext>&> Handler;
                };
                DotNetDupe::System::Collections::Generic::List<RouteInfo> m_routes;

                static DotNetDupe::System::SmartPointer<TController> ResolveController(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& context) {
                    auto controller = app->GetServices()->GetRequiredService<TController>();
                    controller->Initialize(context);
                    return controller;
                }

                static int HexCharToDecimal(char c) {
                    if (c >= '0' && c <= '9') return c - '0';
                    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                    return 0;
                }

                static DotNetDupe::System::String UrlDecode(const DotNetDupe::System::String& input) {
                    const char* pStr = input.GetRawString();
                    if (!pStr) return DotNetDupe::System::String("");
                    DotNetDupe::System::String decoded("");
                    for (int i = 0; pStr[i] != '\0'; ++i) {
                        if (pStr[i] == '%') {
                            if (pStr[i + 1] != '\0' && pStr[i + 2] != '\0') {
                                char ch = static_cast<char>((HexCharToDecimal(pStr[i + 1]) << 4) | HexCharToDecimal(pStr[i + 2]));
                                decoded += ch;
                                i += 2;
                                continue;
                            }
                        } else if (pStr[i] == '+') {
                            decoded += ' ';
                            continue;
                        }
                        decoded += pStr[i];
                    }
                    return decoded;
                }

                static DotNetDupe::System::String ExtractRouteOrQueryParam(const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx, const DotNetDupe::System::String& subPath) {
                    DotNetDupe::System::String val;
                    // 1. Try route parameter "id" first
                    if (ctx->GetRequest()->GetRouteValues().TryGetValue("id", val) && !val.IsEmpty()) {
                        return UrlDecode(val);
                    }
                    // 2. Try extracting parameter name from subPath placeholder pattern, e.g., "/{channelName}"
                    int openBrace = subPath.IndexOf("{");
                    int closeBrace = subPath.IndexOf("}");
                    if (openBrace >= 0 && closeBrace > openBrace + 1) {
                        DotNetDupe::System::String paramName = subPath.Substring(openBrace + 1, closeBrace - openBrace - 1);
                        if (ctx->GetRequest()->GetRouteValues().TryGetValue(paramName, val) && !val.IsEmpty()) {
                            return UrlDecode(val);
                        }
                        if (ctx->GetRequest()->GetQuery().TryGetValue(paramName, val) && !val.IsEmpty()) {
                            return UrlDecode(val);
                        }
                    }
                    // 3. Fallback to query parameter "id"
                    if (ctx->GetRequest()->GetQuery().TryGetValue("id", val) && !val.IsEmpty()) {
                        return UrlDecode(val);
                    }
                    // 4. Fallback to first query parameter value if present
                    auto queryKeys = ctx->GetRequest()->GetQuery().GetKeys();
                    if (queryKeys.GetLength() > 0) {
                        ctx->GetRequest()->GetQuery().TryGetValue(queryKeys[0], val);
                        return UrlDecode(val);
                    }
                    return DotNetDupe::System::String("");
                }

            public:
                ControllerRouteBuilder(const DotNetDupe::System::String& prefix)
                    : m_routePrefix(prefix) {}

                ~ControllerRouteBuilder() override = default;

                // === GET (No params) ===

                // 1. Returns String
                ControllerRouteBuilder& MapGet(const DotNetDupe::System::String& subPath, DotNetDupe::System::String (TController::*action)()) {
                    m_routes.Add({"GET", subPath, [action](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        auto controller = ResolveController(app, ctx);
                        return (controller.Get()->*action)();
                    }});
                    return *this;
                }

                // 2. Returns TResult (not String)
                template <typename TResult, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String>>>
                ControllerRouteBuilder& MapGet(const DotNetDupe::System::String& subPath, TResult (TController::*action)()) {
                    m_routes.Add({"GET", subPath, [action](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        auto controller = ResolveController(app, ctx);
                        auto result = (controller.Get()->*action)();
                        ctx->GetResponse()->SetContentType("application/json");
                        return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                    }});
                    return *this;
                }

                // === GET (With string ID param) ===

                // 3. Returns String
                ControllerRouteBuilder& MapGet(const DotNetDupe::System::String& subPath, DotNetDupe::System::String (TController::*action)(const DotNetDupe::System::String&)) {
                    m_routes.Add({"GET", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        auto controller = ResolveController(app, ctx);
                        DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                        return (controller.Get()->*action)(id);
                    }});
                    return *this;
                }

                // 4. Returns TResult (not String)
                template <typename TResult, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String>>>
                ControllerRouteBuilder& MapGet(const DotNetDupe::System::String& subPath, TResult (TController::*action)(const DotNetDupe::System::String&)) {
                    m_routes.Add({"GET", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        auto controller = ResolveController(app, ctx);
                        DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                        auto result = (controller.Get()->*action)(id);
                        ctx->GetResponse()->SetContentType("application/json");
                        return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                    }});
                    return *this;
                }

                // 4b. Returns TResult (action with page & pageSize parameters)
                template <typename TResult, typename TString, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String> && std::is_same_v<std::decay_t<TString>, DotNetDupe::System::String>>>
                ControllerRouteBuilder& MapGet(const DotNetDupe::System::String& subPath, TResult (TController::*action)(const TString&, size_t, size_t)) {
                    m_routes.Add({"GET", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        auto controller = ResolveController(app, ctx);
                        DotNetDupe::System::String channelName = ExtractRouteOrQueryParam(ctx, subPath);
                        
                        size_t page = 1;
                        size_t pageSize = 20;
                        
                        DotNetDupe::System::String sPage, sPageSize;
                        if (ctx->GetRequest()->GetQuery().TryGetValue("page", sPage) || ctx->GetRequest()->GetRouteValues().TryGetValue("page", sPage)) {
                            page = static_cast<size_t>(DotNetDupe::System::Convert::ToInt32(sPage));
                        }
                        if (ctx->GetRequest()->GetQuery().TryGetValue("pageSize", sPageSize) || ctx->GetRequest()->GetRouteValues().TryGetValue("pageSize", sPageSize)) {
                            pageSize = static_cast<size_t>(DotNetDupe::System::Convert::ToInt32(sPageSize));
                        }
                        
                        auto result = (controller.Get()->*action)(channelName, page, pageSize);
                        ctx->GetResponse()->SetContentType("application/json");
                        return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                    }});
                    return *this;
                }

                // === POST ===

                // 5. Returns String, parameter const TResource&
                template <typename TResource>
                ControllerRouteBuilder& MapPost(const DotNetDupe::System::String& subPath, DotNetDupe::System::String (TController::*action)(const TResource&)) {
                    m_routes.Add({"POST", subPath, [action](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            return (controller.Get()->*action)(payload);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // 6. Returns String, parameter TResource (by value)
                template <typename TResource, typename = std::enable_if_t<!std::is_reference_v<TResource>>>
                ControllerRouteBuilder& MapPost(const DotNetDupe::System::String& subPath, DotNetDupe::System::String (TController::*action)(TResource)) {
                    m_routes.Add({"POST", subPath, [action](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            return (controller.Get()->*action)(payload);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // 7. Returns TResult (not String), parameter const TResource&
                template <typename TResult, typename TResource, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String>>>
                ControllerRouteBuilder& MapPost(const DotNetDupe::System::String& subPath, TResult (TController::*action)(const TResource&)) {
                    m_routes.Add({"POST", subPath, [action](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            auto result = (controller.Get()->*action)(payload);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // 8. Returns TResult (not String), parameter TResource (by value)
                template <typename TResult, typename TResource, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String> && !std::is_reference_v<TResource>>>
                ControllerRouteBuilder& MapPost(const DotNetDupe::System::String& subPath, TResult (TController::*action)(TResource)) {
                    m_routes.Add({"POST", subPath, [action](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            auto result = (controller.Get()->*action)(payload);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // === PUT ===

                // 9. Returns String, parameter const TResource&
                template <typename TResource>
                ControllerRouteBuilder& MapPut(const DotNetDupe::System::String& subPath, DotNetDupe::System::String (TController::*action)(const DotNetDupe::System::String&, const TResource&)) {
                    m_routes.Add({"PUT", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            return (controller.Get()->*action)(id, payload);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // 10. Returns String, parameter TResource (by value)
                template <typename TResource, typename = std::enable_if_t<!std::is_reference_v<TResource>>>
                ControllerRouteBuilder& MapPut(const DotNetDupe::System::String& subPath, DotNetDupe::System::String (TController::*action)(const DotNetDupe::System::String&, TResource)) {
                    m_routes.Add({"PUT", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            return (controller.Get()->*action)(id, payload);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // 11. Returns TResult (not String), parameter const TResource&
                template <typename TResult, typename TResource, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String>>>
                ControllerRouteBuilder& MapPut(const DotNetDupe::System::String& subPath, TResult (TController::*action)(const DotNetDupe::System::String&, const TResource&)) {
                    m_routes.Add({"PUT", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            auto result = (controller.Get()->*action)(id, payload);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // 12. Returns TResult (not String), parameter TResource (by value)
                template <typename TResult, typename TResource, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String> && !std::is_reference_v<TResource>>>
                ControllerRouteBuilder& MapPut(const DotNetDupe::System::String& subPath, TResult (TController::*action)(const DotNetDupe::System::String&, TResource)) {
                    m_routes.Add({"PUT", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        try {
                            auto controller = ResolveController(app, ctx);
                            DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                            auto body = ctx->GetRequest()->GetBody();
                            TResource payload = DotNetDupe::System::Text::Json::JsonSerializer::template Deserialize<TResource>(body);
                            auto result = (controller.Get()->*action)(id, payload);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                        } catch (const DotNetDupe::System::Exception& ex) {
                            ctx->GetResponse()->SetStatusCode(400);
                            ctx->GetResponse()->SetContentType("application/json");
                            return DotNetDupe::System::String("{\"error\":\"Invalid JSON request body: ") + ex.What() + "\"}";
                        }
                    }});
                    return *this;
                }

                // === DELETE ===

                // 13. Returns String
                ControllerRouteBuilder& MapDelete(const DotNetDupe::System::String& subPath, DotNetDupe::System::String (TController::*action)(const DotNetDupe::System::String&)) {
                    m_routes.Add({"DELETE", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        auto controller = ResolveController(app, ctx);
                        DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                        return (controller.Get()->*action)(id);
                    }});
                    return *this;
                }

                // 14. Returns TResult (not String)
                template <typename TResult, typename = std::enable_if_t<!std::is_same_v<TResult, DotNetDupe::System::String>>>
                ControllerRouteBuilder& MapDelete(const DotNetDupe::System::String& subPath, TResult (TController::*action)(const DotNetDupe::System::String&)) {
                    m_routes.Add({"DELETE", subPath, [action, subPath](const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx) {
                        auto controller = ResolveController(app, ctx);
                        DotNetDupe::System::String id = ExtractRouteOrQueryParam(ctx, subPath);
                        auto result = (controller.Get()->*action)(id);
                        ctx->GetResponse()->SetContentType("application/json");
                        return DotNetDupe::System::Text::Json::JsonSerializer::Serialize(result);
                    }});
                    return *this;
                }

                // Registers recorded routes onto WebApplication
                void Register(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app) override {
                    for (int i = 0; i < m_routes.GetCount(); ++i) {
                        const auto& r = m_routes[i];
                        if (r.Method == "GET") {
                            app->MapGet(m_routePrefix + r.SubPath, [r, app](DotNetDupe::System::SmartPointer<Http::HttpContext> ctx) {
                                return r.Handler(app, ctx);
                            });
                        } else if (r.Method == "POST") {
                            app->MapPost(m_routePrefix + r.SubPath, [r, app](DotNetDupe::System::SmartPointer<Http::HttpContext> ctx) {
                                return r.Handler(app, ctx);
                            });
                        } else if (r.Method == "PUT") {
                            app->MapPut(m_routePrefix + r.SubPath, [r, app](DotNetDupe::System::SmartPointer<Http::HttpContext> ctx) {
                                return r.Handler(app, ctx);
                            });
                        } else if (r.Method == "DELETE") {
                            app->MapDelete(m_routePrefix + r.SubPath, [r, app](DotNetDupe::System::SmartPointer<Http::HttpContext> ctx) {
                                return r.Handler(app, ctx);
                            });
                        }
                    }
                }
            };

        }
    }
}
