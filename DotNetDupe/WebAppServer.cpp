#include <filesystem>
#include "pch.h"
#include "WebAppCore/Server/WebAppServer.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/ArgumentException.h"
#include "System/Console.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Server {

            WebAppServer::WebAppServer(const DotNetDupe::System::SmartPointer<Builder::WebApplication>& app, const DotNetDupe::System::String& webRoot)
                : m_spApp(app), m_sWebRoot(webRoot), m_sDefaultDocument("index.html"), m_bStaticFilesEnabled(true) {
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
                if (this != &other) {
                    m_spApp = std::move(other.m_spApp);
                    m_sWebRoot = std::move(other.m_sWebRoot);
                    m_sDefaultDocument = std::move(other.m_sDefaultDocument);
                    m_bStaticFilesEnabled = other.m_bStaticFilesEnabled;
                }
                return *this;
            }

            void WebAppServer::EnableStaticFiles(const DotNetDupe::System::String& defaultDocument) {
                m_bStaticFilesEnabled = true;
                m_sDefaultDocument = defaultDocument;
            }

            DotNetDupe::System::String WebAppServer::GetMimeType(const DotNetDupe::System::String& filePath) {
                DotNetDupe::System::String ext = DotNetDupe::System::IO::Path::GetExtension(filePath);
                std::string extLower = ext.GetRawString();
                std::transform(extLower.begin(), extLower.end(), extLower.begin(), ::tolower);

                if (extLower == ".html" || extLower == "html" || extLower == ".htm" || extLower == "htm") return "text/html; charset=utf-8";
                if (extLower == ".css" || extLower == "css") return "text/css; charset=utf-8";
                if (extLower == ".js" || extLower == "js") return "application/javascript; charset=utf-8";
                if (extLower == ".json" || extLower == "json") return "application/json; charset=utf-8";
                if (extLower == ".png" || extLower == "png") return "image/png";
                if (extLower == ".jpg" || extLower == "jpg" || extLower == ".jpeg" || extLower == "jpeg") return "image/jpeg";
                if (extLower == ".gif" || extLower == "gif") return "image/gif";
                if (extLower == ".svg" || extLower == "svg") return "image/svg+xml";
                if (extLower == ".ico" || extLower == "ico") return "image/x-icon";
                if (extLower == ".txt" || extLower == "txt") return "text/plain; charset=utf-8";
                if (extLower == ".pdf" || extLower == "pdf") return "application/pdf";
                if (extLower == ".woff" || extLower == "woff") return "font/woff";
                if (extLower == ".woff2" || extLower == "woff2") return "font/woff2";

                return "application/octet-stream";
            }

            void WebAppServer::Run(const DotNetDupe::System::String& url) {
                if (m_bStaticFilesEnabled) {
                    // Register fallback handler for GET requests to serve static files
                    m_spApp->MapGet("/{*filepath}", [this](DotNetDupe::System::SmartPointer<Http::HttpContext> ctx) -> DotNetDupe::System::String {
                        DotNetDupe::System::String reqPath = ctx->GetRequest()->GetPath();
                        DotNetDupe::System::Console::WriteLine(DotNetDupe::System::String("[WebAppServer] Incoming static file request URL: ") + reqPath);

                        std::string relPathStr = reqPath.GetRawString();

                        // Clean leading slash
                        if (!relPathStr.empty() && (relPathStr[0] == '/' || relPathStr[0] == '\\')) {
                            relPathStr = relPathStr.substr(1);
                        }

                        // If path is empty, default to index.html
                        if (relPathStr.empty()) {
                            relPathStr = m_sDefaultDocument.GetRawString();
                        }

                        // Security check: Prevent directory traversal attack (../)
                        if (relPathStr.find("..") != std::string::npos) {
                            DotNetDupe::System::Console::WriteLine(DotNetDupe::System::String("[WebAppServer] Directory traversal blocked for path: ") + reqPath);
                            ctx->GetResponse()->SetStatusCode(403);
                            ctx->GetResponse()->SetContentType("text/plain");
                            return "403 Forbidden: Invalid file path";
                        }

                        DotNetDupe::System::String targetFilePath = DotNetDupe::System::IO::Path::Combine({m_sWebRoot, DotNetDupe::System::String(relPathStr.c_str())});

                        std::error_code ec;
                        if (std::filesystem::is_directory(targetFilePath.GetRawString(), ec)) {
                            targetFilePath = DotNetDupe::System::IO::Path::Combine({targetFilePath, m_sDefaultDocument});
                        }

                        DotNetDupe::System::Console::WriteLine(DotNetDupe::System::String("[WebAppServer] Resolved file path: ") + targetFilePath);

                        if (!DotNetDupe::System::IO::File::Exists(targetFilePath)) {
                            DotNetDupe::System::Console::WriteLine(DotNetDupe::System::String("[WebAppServer] Path not found (404): ") + targetFilePath);
                            ctx->GetResponse()->SetStatusCode(404);
                            ctx->GetResponse()->SetContentType("text/plain");
                            return "404 Not Found";
                        }

                        DotNetDupe::System::String contentStr;
                        try {
                            contentStr = DotNetDupe::System::IO::File::ReadAllText(targetFilePath);
                        } catch (...) {
                            ctx->GetResponse()->SetStatusCode(500);
                            ctx->GetResponse()->SetContentType("text/plain");
                            return "500 Internal Server Error: Unable to open file";
                        }

                        // Set Content-Type, Content-Disposition (inline rendering), and Status 200
                        ctx->GetResponse()->SetStatusCode(200);
                        ctx->GetResponse()->SetContentType(GetMimeType(targetFilePath));
                        ctx->GetResponse()->GetHeaders()[DotNetDupe::System::String("Content-Disposition")] = DotNetDupe::System::String("inline");

                        return contentStr;
                    });
                }

                m_spApp->Run(url);
            }

            void WebAppServer::Stop() {
                m_spApp->Stop();
            }

        }
    }
}
