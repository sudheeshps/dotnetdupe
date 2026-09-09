#include "pch.h"
#include "WebAppCore/Server/WebAppServer.h"
#include "WebAppCore/Http/HttpContext.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/ArgumentException.h"
#include "System/Console.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Server {

            WebAppServer::WebAppServer(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::String& webRoot)
                : m_spApp(app), m_sWebRoot(webRoot), m_sDefaultDocument("index.html"), m_bStaticFilesEnabled(true) {
                /// Guard: Ensure WebApplication host instance is not null.
                if (app.IsNull()) {
                    throw DotNetDupe::System::ArgumentException("WebApplication cannot be null");
                }
            }

            WebAppServer::WebAppServer(WebAppServer&& other) noexcept
                : m_spApp(std::move(other.m_spApp)),
                  m_sWebRoot(std::move(other.m_sWebRoot)),
                  m_sDefaultDocument(std::move(other.m_sDefaultDocument)),
                  m_bStaticFilesEnabled(other.m_bStaticFilesEnabled) {
            }

            WebAppServer& WebAppServer::operator=(WebAppServer&& other) noexcept {
                /// Guard: Check for self-assignment.
                if (this != &other) {
                    /// Move: Transfer application host pointer, root path, and configuration settings.
                    m_spApp = std::move(other.m_spApp);
                    m_sWebRoot = std::move(other.m_sWebRoot);
                    m_sDefaultDocument = std::move(other.m_sDefaultDocument);
                    m_bStaticFilesEnabled = other.m_bStaticFilesEnabled;
                }
                /// Return result: Return self reference.
                return *this;
            }

            void WebAppServer::EnableStaticFiles(const DotNetDupe::System::String& defaultDocument) {
                /// Configure: Enable static file serving flag and update default fallback document.
                m_bStaticFilesEnabled = true;
                m_sDefaultDocument = defaultDocument;
            }

            DotNetDupe::System::String WebAppServer::GetMimeType(const DotNetDupe::System::String& filePath) {
                /// Parse: Extract and lowercase file extension.
                DotNetDupe::System::String ext = DotNetDupe::System::IO::Path::GetExtension(filePath);
                std::string s = ext.GetRawString();
                std::transform(s.begin(), s.end(), s.begin(), ::tolower);
                if (s.rfind('.', 0) == 0) s = s.substr(1);

                /// Lookup: Match against standard MIME types table.
                static const std::unordered_map<std::string, const char*> mimeMap = {
                    {"html", "text/html; charset=utf-8"}, {"htm", "text/html; charset=utf-8"},
                    {"css", "text/css; charset=utf-8"}, {"js", "application/javascript; charset=utf-8"},
                    {"json", "application/json; charset=utf-8"}, {"png", "image/png"},
                    {"jpg", "image/jpeg"}, {"jpeg", "image/jpeg"}, {"gif", "image/gif"},
                    {"svg", "image/svg+xml"}, {"ico", "image/x-icon"}, {"txt", "text/plain; charset=utf-8"},
                    {"pdf", "application/pdf"}, {"woff", "font/woff"}, {"woff2", "font/woff2"}
                };
                auto it = mimeMap.find(s);
                /// Return result: Return resolved MIME type string or fallback octet-stream.
                return (it != mimeMap.end()) ? it->second : "application/octet-stream";
            }

            static DotNetDupe::System::String ResolveStaticPath(const DotNetDupe::System::String& reqPath, const DotNetDupe::System::String& webRoot, const DotNetDupe::System::String& defaultDoc) {
                /// Sanitize: Trim leading slash and check for directory traversal attempts.
                std::string rel = reqPath.GetRawString();
                if (!rel.empty() && (rel[0] == '/' || rel[0] == '\\')) rel = rel.substr(1);
                if (rel.empty()) rel = defaultDoc.GetRawString();
                if (rel.find("..") != std::string::npos) return "";

                /// Combine: Form target filesystem path.
                DotNetDupe::System::String target = DotNetDupe::System::IO::Path::Combine({webRoot, DotNetDupe::System::String(rel.c_str())});
                DotNetDupe::System::IO::FileAttributes attr;
                if (DotNetDupe::System::IO::File::GetAttributes(target, attr) && (static_cast<int>(attr) & static_cast<int>(DotNetDupe::System::IO::FileAttributes::Directory)) != 0) {
                    target = DotNetDupe::System::IO::Path::Combine({target, defaultDoc});
                }
                /// Return result: Return verified filesystem target path.
                return target;
            }

            static DotNetDupe::System::String ServeStaticFile(DotNetDupe::System::SmartPointer<Http::HttpContext> spContext, const DotNetDupe::System::String& webRoot, const DotNetDupe::System::String& defaultDoc) {
                /// Resolve: Map request URI to filesystem path.
                DotNetDupe::System::String target = ResolveStaticPath(spContext->GetRequest()->GetPath(), webRoot, defaultDoc);
                if (target.IsEmpty()) {
                    spContext->GetResponse()->SetStatusCode(403); spContext->GetResponse()->SetContentType("text/plain");
                    return "403 Forbidden: Invalid file path";
                }
                /// Verify: Check that the requested file exists on disk.
                if (!DotNetDupe::System::IO::File::Exists(target)) {
                    spContext->GetResponse()->SetStatusCode(404); spContext->GetResponse()->SetContentType("text/plain");
                    return "404 Not Found";
                }
                /// Read: Read file content and set appropriate Content-Type headers.
                try {
                    DotNetDupe::System::String content = DotNetDupe::System::IO::File::ReadAllText(target);
                    spContext->GetResponse()->SetStatusCode(200);
                    spContext->GetResponse()->SetContentType(WebAppServer::GetMimeType(target));
                    spContext->GetResponse()->GetHeaders()["Content-Disposition"] = "inline";
                    return content;
                } catch (...) {
                    spContext->GetResponse()->SetStatusCode(500); spContext->GetResponse()->SetContentType("text/plain");
                    return "500 Internal Server Error: Unable to open file";
                }
            }

            static DotNetDupe::System::String HandleStaticRouteOrWebSocket(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& spApp, const DotNetDupe::System::SmartPointer<Http::HttpContext>& ctx, const DotNetDupe::System::String& webRoot, const DotNetDupe::System::String& defaultDoc) {
                /// Check: Check if path matches an active WebSocket endpoint.
                if (spApp->HasWebSocketRoute(ctx->GetRequest()->GetPath())) {
                    ctx->GetResponse()->SetStatusCode(426);
                    ctx->GetResponse()->SetContentType("text/plain");
                    ctx->GetResponse()->GetHeaders()["Upgrade"] = "websocket";
                    ctx->GetResponse()->GetHeaders()["Connection"] = "Upgrade";
                    return "426 Upgrade Required";
                }
                /// Fallback: Serve matching static file asset from web root.
                return ServeStaticFile(ctx, webRoot, defaultDoc);
            }

            void WebAppServer::Run(const DotNetDupe::System::String& url, int threadCount) {
                /// Configure: Register catch-all static file handler route if enabled.
                if (m_bStaticFilesEnabled) {
                    m_spApp->MapGet("/{*filepath}", [this](DotNetDupe::System::SmartPointer<Http::HttpContext> ctx) {
                        return HandleStaticRouteOrWebSocket(m_spApp, ctx, m_sWebRoot, m_sDefaultDocument);
                    });
                }
                /// Execute: Start listening for HTTP requests on the application host.
                m_spApp->Run(url, threadCount);
            }

            void WebAppServer::Stop() {
                /// Shutdown: Stop underlying WebApplication server.
                m_spApp->Stop();
            }

        }
    }
}
