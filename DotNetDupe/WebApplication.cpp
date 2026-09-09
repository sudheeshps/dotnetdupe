#include "pch.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "System/Net/WebSockets/WebSocket.h"
#include "WebAppCore/WebSockets/WebSocketContext.h"
#include "System/Console.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Threading/ThreadPool.h"
#include "System/Convert.h"
#include "System/Net/HttpStatusCode.h"
#include "System/UnknownException.h"
#include <string>
#include <vector>
#include <algorithm>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {

            namespace Internal {
                std::vector<std::string> GetPathSegments(const std::string& path);
                bool MatchRoute(const std::vector<std::string>& patternSegs, const std::vector<std::string>& pathSegs, std::vector<std::pair<std::string, std::string>>& extractedParams);
                void ParseServerUrl(const std::string& sUrl, std::string& host, int& port);
            }

            WebApplication::WebApplication(const System::SmartPointer<System::IServiceProvider>& spServices)
                : m_spServices(spServices), m_bRunning(false), m_nPort(0) {}

            WebApplication::~WebApplication() {
                /// Shutdown: Stop server listener if currently active.
                Stop();
            }

            WebApplication::WebApplication(WebApplication&& other) noexcept
                : m_spServices(std::move(other.m_spServices)),
                  m_getHandlers(std::move(other.m_getHandlers)),
                  m_postHandlers(std::move(other.m_postHandlers)),
                  m_putHandlers(std::move(other.m_putHandlers)),
                  m_deleteHandlers(std::move(other.m_deleteHandlers)),
                  m_pListener(std::move(other.m_pListener)),
                  m_bRunning(other.m_bRunning),
                  m_sHost(std::move(other.m_sHost)),
                  m_nPort(other.m_nPort),
                  m_controllerRegistrars(std::move(other.m_controllerRegistrars)) {}

            WebApplication& WebApplication::operator=(WebApplication&& other) noexcept {
                /// Guard: Check for self-assignment.
                if (this != &other) {
                    /// Move: Transfer state, handler dictionaries, and socket listener.
                    m_spServices = std::move(other.m_spServices);
                    m_getHandlers = std::move(other.m_getHandlers);
                    m_postHandlers = std::move(other.m_postHandlers);
                    m_putHandlers = std::move(other.m_putHandlers);
                    m_deleteHandlers = std::move(other.m_deleteHandlers);
                    m_pListener = std::move(other.m_pListener);
                    m_bRunning = other.m_bRunning;
                    m_sHost = std::move(other.m_sHost);
                    m_nPort = other.m_nPort;
                    m_controllerRegistrars = std::move(other.m_controllerRegistrars);
                }
                /// Return result: Return self reference.
                return *this;
            }

            System::SmartPointer<WebApplicationBuilder> WebApplication::CreateBuilder() {
                /// Return result: Construct and return fresh WebApplicationBuilder instance.
                return System::SmartPointer<WebApplicationBuilder>::NewShared();
            }

            void WebApplication::MapGet(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                /// Register: Map HTTP GET endpoint pattern to the provided delegate.
                m_getHandlers.Add(pattern, handler);
            }

            void WebApplication::MapPost(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                /// Register: Map HTTP POST endpoint pattern to the provided delegate.
                m_postHandlers.Add(pattern, handler);
            }

            void WebApplication::MapPut(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                /// Register: Map HTTP PUT endpoint pattern to the provided delegate.
                m_putHandlers.Add(pattern, handler);
            }

            void WebApplication::MapDelete(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                /// Register: Map HTTP DELETE endpoint pattern to the provided delegate.
                m_deleteHandlers.Add(pattern, handler);
            }

            void WebApplication::MapWebSocket(const System::String& pattern, System::SmartPointer<WebSockets::IWebSocketHandler> handler) {
                /// Register: Map WebSocket endpoint pattern to the provided lifecycle handler.
                m_wsHandlers.Add(pattern, handler);
            }

            DotNetDupe::System::Collections::Generic::List<DotNetDupe::System::String> WebApplication::GetWebSocketRoutes() const {
                /// Initialize: Collect registered WebSocket routes.
                DotNetDupe::System::Collections::Generic::List<DotNetDupe::System::String> list;
                auto keys = m_wsHandlers.GetKeys();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    list.Add(keys[i]);
                }
                /// Return result: Return list of active route keys.
                return list;
            }

            bool WebApplication::HasWebSocketRoute(const DotNetDupe::System::String& path) const {
                /// Iterate: Verify if path matches any registered WebSocket endpoints.
                auto keys = m_wsHandlers.GetKeys();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    if (keys[i] == path) return true;
                }
                return false;
            }

            void WebApplication::MapControllers() {
                /// Dispatch: Invoke all queued controller registration delegates.
                for (int i = 0; i < m_controllerRegistrars.GetCount(); ++i) {
                    m_controllerRegistrars[i](m_spSelf);
                }
            }

            struct ConnectionContext : public System::Object {
                System::SmartPointer<System::Net::Sockets::TcpClient> pClient;
                ConnectionContext(System::SmartPointer<System::Net::Sockets::TcpClient> pC) : pClient(std::move(pC)) {}
            };

            void WebApplication::Run(const System::String& url, int threadCount) {
                /// Configure: Adjust thread pool concurrency limits.
                if (threadCount > 0) System::Threading::ThreadPool::SetMinThreads(threadCount);
                std::string host; int port = 5000;
                /// Parse: Extract host and port from URL string.
                Internal::ParseServerUrl(url.GetRawString(), host, port);
                /// Execute: Start main socket listener loop.
                StartServerLoop(System::String(host.c_str()), port);
            }

            void WebApplication::QueueAcceptedClient(System::SmartPointer<System::Net::Sockets::TcpClient> pClient) {
                /// Dispatch: Enqueue incoming client connection to ThreadPool worker.
                auto spCtx = System::SmartPointer<ConnectionContext>::NewShared(std::move(pClient));
                System::Threading::ThreadPool::QueueUserWorkItem([this, spCtx](System::Object*) {
                    try { HandleConnection(std::move(spCtx->pClient)); }
                    catch (const DotNetDupe::System::Exception& ex) { System::Console::WriteLine(System::String("[Server] ") + ex.What()); }
                    catch (const std::exception& ex) { System::Console::WriteLine(System::String("[Server] ") + ex.what()); }
                }, nullptr);
            }

            void WebApplication::StartServerLoop(const System::String& host, int port) {
                /// Initialize: Create and start TCP listener socket.
                m_sHost = host; m_nPort = port;
                m_pListener = System::SmartPointer<System::Net::Sockets::TcpListener>::NewShared(m_sHost, port);
                m_pListener->Start();
                m_bRunning = true;
                /// Loop: Accept incoming TCP client connections.
                try {
                    while (m_bRunning) {
                        auto pClient = m_pListener->AcceptTcpClient();
                        if (!pClient.IsNull()) QueueAcceptedClient(std::move(pClient));
                    }
                } catch (...) { (void)0; }
            }

            void WebApplication::Stop() {
                /// Guard: Check if server is currently running.
                if (!m_bRunning) return;
                m_bRunning = false;
                /// Terminate: Stop listener socket.
                if (!m_pListener.IsNull()) m_pListener->Stop();
                try {
                    System::Net::Sockets::TcpClient dummy;
                    dummy.Connect(m_sHost, m_nPort);
                    dummy.Close();
                } catch (...) { (void)0; }
                m_spSelf = nullptr;
            }

            static bool ReadHeaderLines(const System::SmartPointer<System::IO::Stream>& stream, std::vector<std::string>& lines, bool isRunning) {
                /// Parse: Read HTTP header stream byte-by-byte until double CRLF delimiter.
                char c; std::string currentLine;
                while (isRunning && stream->Read(&c, 0, 1) > 0) {
                    if (c == '\n') {
                        if (!currentLine.empty() && currentLine.back() == '\r') currentLine.pop_back();
                        if (currentLine.empty()) break;
                        lines.push_back(currentLine);
                        currentLine.clear();
                    } else {
                        currentLine += c;
                    }
                }
                /// Return result: True if at least one header line was parsed.
                return !lines.empty();
            }

            static void ParseQueryParams(const std::string& queryStr, Http::HttpRequest* req) {
                /// Parse: Tokenize query key-value pairs separated by '&'.
                size_t start = 0;
                while (start < queryStr.length()) {
                    size_t ampersand = queryStr.find('&', start);
                    std::string pair = (ampersand == std::string::npos) ? queryStr.substr(start) : queryStr.substr(start, ampersand - start);
                    size_t equals = pair.find('=');
                    if (equals != std::string::npos) {
                        req->GetQuery()[System::String(pair.substr(0, equals).c_str())] = System::String(pair.substr(equals + 1).c_str());
                    } else if (!pair.empty()) {
                        req->GetQuery()[System::String(pair.c_str())] = System::String("");
                    }
                    if (ampersand == std::string::npos) break;
                    start = ampersand + 1;
                }
            }

            static std::string ParseRequestLine(const std::string& reqLine, Http::HttpRequest* req) {
                /// Parse: Extract HTTP method, full URI path, and query strings.
                size_t sp1 = reqLine.find(' '), sp2 = reqLine.find(' ', sp1 + 1);
                if (sp1 == std::string::npos || sp2 == std::string::npos) return "";
                std::string method = reqLine.substr(0, sp1);
                std::string fullPath = reqLine.substr(sp1 + 1, sp2 - sp1 - 1);
                req->SetMethod(System::String(method.c_str()));
                size_t q = fullPath.find('?');
                std::string path = (q != std::string::npos) ? fullPath.substr(0, q) : fullPath;
                if (q != std::string::npos) ParseQueryParams(fullPath.substr(q + 1), req);
                req->SetPath(System::String(path.c_str()));
                /// Return result: Return parsed HTTP method verb.
                return method;
            }

            static void ReadHeadersAndBody(const System::SmartPointer<System::IO::Stream>& stream, const std::vector<std::string>& lines, Http::HttpRequest* req) {
                /// Parse: Process HTTP header fields and extract content length.
                int contentLength = 0;
                for (size_t i = 1; i < lines.size(); ++i) {
                    size_t colon = lines[i].find(':');
                    if (colon == std::string::npos) continue;
                    std::string name = lines[i].substr(0, colon);
                    std::string val = lines[i].substr(colon + 1);
                    size_t f = val.find_first_not_of(" \t");
                    if (f != std::string::npos) val = val.substr(f, val.find_last_not_of(" \t\r\n") - f + 1);
                    std::string nameLower = name;
                    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
                    req->GetHeaders()[System::String(nameLower.c_str())] = System::String(val.c_str());
                    if (nameLower == "content-length") try { contentLength = std::stoi(val); } catch (...) { contentLength = 0; }
                }
                /// Read: Fetch request body bytes if Content-Length header is specified.
                if (contentLength > 0) {
                    std::string body(contentLength, '\0');
                    stream->Read(body.data(), 0, contentLength);
                    req->SetBody(System::String(body.c_str()));
                }
            }

            static void RunWsReceiveLoop(const System::SmartPointer<System::Net::WebSockets::WebSocket>& pWebSocket, const System::SmartPointer<WebSockets::WebSocketContext>& pWsContext, const System::SmartPointer<WebSockets::IWebSocketHandler>& pWsHandler) {
                /// Loop: Receive incoming frames and dispatch to OnMessage until close or disconnect.
                System::String msg;
                try {
                    while (pWebSocket->ReceiveText(msg)) {
                        if (msg.IsEmpty() || msg == "__DISCONNECT__") break;
                        try { pWsHandler->OnMessage(pWsContext, msg); } catch (...) { (void)0; }
                    }
                } catch (...) { (void)0; }
            }

            static bool ProcessWsSession(const System::SmartPointer<System::Net::Sockets::NetworkStream>& stream, const System::SmartPointer<Http::HttpContext>& spContext, const System::SmartPointer<WebSockets::IWebSocketHandler>& pWsHandler, const System::String& sSecKey) {
                /// Handshake: Calculate RFC 6455 accept key and send 101 Switching Protocols response.
                System::String sAcceptKey = System::Net::WebSockets::WebSocket::ComputeSecWebSocketAccept(sSecKey);
                std::string hsResponse = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + std::string(sAcceptKey.GetRawString() ? sAcceptKey.GetRawString() : "") + "\r\n\r\n";
                stream->Write(hsResponse.data(), 0, static_cast<int>(hsResponse.length()));
                /// Initialize: Construct WebSocket and dispatch OnConnected callback.
                auto pWebSocket = System::SmartPointer<System::Net::WebSockets::WebSocket>::NewShared(stream);
                auto pWsContext = System::SmartPointer<WebSockets::WebSocketContext>::NewShared(spContext, pWebSocket);
                try { pWsHandler->OnConnected(pWsContext); } catch (...) { (void)0; }
                /// Execute: Run blocking frame receive loop.
                RunWsReceiveLoop(pWebSocket, pWsContext, pWsHandler);
                try { pWsHandler->OnDisconnected(pWsContext); } catch (...) { (void)0; }
                /// Return result: Session completed successfully.
                return true;
            }

            static void SendHttpResponseData(const System::SmartPointer<System::IO::Stream>& stream, Http::HttpResponse* resp, const std::string& method) {
                /// Prepare: Build HTTP status line, headers, and content length.
                std::string respBody = resp->GetBody().GetRawString();
                int code = resp->GetStatusCode();
                std::string statusMsg = (code == 404) ? "Not Found" : ((code == 500) ? "Internal Server Error" : ((code == 201) ? "Created" : ((code == 204) ? "No Content" : "OK")));
                std::string respStr = "HTTP/1.1 " + std::to_string(code) + " " + statusMsg + "\r\nContent-Type: " + std::string(resp->GetContentType().GetRawString() ? resp->GetContentType().GetRawString() : "") + "\r\nContent-Length: " + std::to_string(respBody.length()) + "\r\nConnection: close\r\nServer: DotNetDupeWebApplication/1.0\r\n";
                auto keys = resp->GetHeaders().GetKeys(); auto values = resp->GetHeaders().GetValues();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    respStr += std::string(keys[i].GetRawString() ? keys[i].GetRawString() : "") + ": " + std::string(values[i].GetRawString() ? values[i].GetRawString() : "") + "\r\n";
                }
                respStr += "\r\n";
                /// Transmit: Write response header block and optional payload body.
                stream->Write(respStr.data(), 0, static_cast<int>(respStr.length()));
                if (!respBody.empty() && method != "HEAD") stream->Write(respBody.data(), 0, static_cast<int>(respBody.length()));
            }

            static bool MatchAndFindHandler(Http::HttpRequest* req, const System::Collections::Generic::Dictionary<System::String, System::Func<System::String, System::SmartPointer<Http::HttpContext>>>& map, System::Func<System::String, System::SmartPointer<Http::HttpContext>>& pHandler) {
                /// Check: Check direct path exact match first.
                if (map.TryGetValue(req->GetPath(), pHandler)) return true;
                /// Parse: Evaluate parameterized pattern routes.
                std::vector<std::string> pathSegs = Internal::GetPathSegments(req->GetPath().GetRawString());
                auto keys = map.GetKeys();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    std::vector<std::string> patternSegs = Internal::GetPathSegments(keys[i].GetRawString());
                    std::vector<std::pair<std::string, std::string>> extractedParams;
                    if (Internal::MatchRoute(patternSegs, pathSegs, extractedParams)) {
                        for (const auto& pair : extractedParams) req->GetRouteValues()[System::String(pair.first.c_str())] = System::String(pair.second.c_str());
                        return map.TryGetValue(keys[i], pHandler);
                    }
                }
                return false;
            }

            static bool MatchTokenString(const std::string& val, const std::string& target) {
                /// Parse: Split comma-separated tokens and compare trimmed value.
                size_t start = 0;
                while (start < val.length()) {
                    size_t comma = val.find(',', start);
                    std::string token = (comma == std::string::npos) ? val.substr(start) : val.substr(start, comma - start);
                    size_t first = token.find_first_not_of(" \t\r\n");
                    if (first != std::string::npos && token.substr(first, token.find_last_not_of(" \t\r\n") - first + 1) == target) return true;
                    if (comma == std::string::npos) break;
                    start = comma + 1;
                }
                return false;
            }

            static bool HeaderContainsToken(const System::String& headerVal, const std::string& targetToken) {
                /// Normalize: Convert header and target to lowercase for case-insensitive token search.
                std::string val = headerVal.GetRawString() ? headerVal.GetRawString() : "";
                std::transform(val.begin(), val.end(), val.begin(), ::tolower);
                std::string target = targetToken;
                std::transform(target.begin(), target.end(), target.begin(), ::tolower);
                return MatchTokenString(val, target);
            }

            static void SendWebSocketErrorResponse(const System::SmartPointer<System::IO::Stream>& stream, int statusCode, const char* pMsg) {
                /// Prepare: Construct RFC 6455 rejection response frame.
                std::string body = pMsg;
                std::string statusText = (statusCode == 426) ? "Upgrade Required" : "Bad Request";
                std::string resp = "HTTP/1.1 " + std::to_string(statusCode) + " " + statusText + "\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(body.length()) + "\r\nConnection: close\r\n";
                if (statusCode == 426) resp += "Upgrade: websocket\r\nConnection: Upgrade\r\n";
                resp += "\r\n" + body;
                /// Transmit: Write rejection error to stream.
                stream->Write(resp.data(), 0, static_cast<int>(resp.length()));
            }

            static int ValidateWebSocketHandshake(const Http::HttpRequest* req, System::String& sSecKey) {
                /// Validate: Verify Upgrade header, Connection header, and Sec-WebSocket-Key presence.
                System::String sUpgrade, sConnection;
                req->GetHeaders().TryGetValue("upgrade", sUpgrade);
                req->GetHeaders().TryGetValue("connection", sConnection);
                req->GetHeaders().TryGetValue("sec-websocket-key", sSecKey);
                if (sUpgrade.IsEmpty() || sUpgrade.ToLower() != "websocket") return 426;
                if (!HeaderContainsToken(sConnection, "upgrade") || sSecKey.IsEmpty()) return 400;
                return 101;
            }

            static bool TryHandleWebSocket(const System::SmartPointer<System::Net::Sockets::NetworkStream>& stream, const System::SmartPointer<Http::HttpContext>& spContext, const System::Collections::Generic::Dictionary<System::String, System::SmartPointer<WebSockets::IWebSocketHandler>>& wsMap) {
                /// Guard: Match route path against registered WebSocket handlers.
                auto spRequest = spContext->GetRequest();
                System::SmartPointer<WebSockets::IWebSocketHandler> spWsHandler;
                if (!wsMap.TryGetValue(spRequest->GetPath(), spWsHandler) || spWsHandler.IsNull()) return false;

                /// Validate: Check RFC 6455 handshake validity.
                System::String sSecKey;
                int status = ValidateWebSocketHandshake(spRequest.Get(), sSecKey);
                if (status != 101) {
                    SendWebSocketErrorResponse(stream, status, (status == 426) ? "426 Upgrade Required" : "400 Bad Request");
                    return true;
                }
                /// Execute: Run WebSocket session loop.
                ProcessWsSession(stream, spContext, spWsHandler, sSecKey);
                return true;
            }

            static void DispatchResponse(Http::HttpResponse* pResp, bool bFound, System::Func<System::String, System::SmartPointer<Http::HttpContext>>& pHandler, const System::SmartPointer<Http::HttpContext>& spContext) {
                /// Dispatch: Invoke matched route handler or set 404 Not Found.
                if (bFound) {
                    try { pResp->SetBody(pHandler(spContext)); }
                    catch (const DotNetDupe::System::Exception& ex) { pResp->SetStatusCode(System::Net::HttpStatusCode::InternalServerError); pResp->SetBody(System::String("500 Internal Server Error: ") + ex.What()); }
                    catch (...) { pResp->SetStatusCode(System::Net::HttpStatusCode::InternalServerError); pResp->SetBody("500 Internal Server Error"); }
                } else {
                    pResp->SetStatusCode(System::Net::HttpStatusCode::NotFound);
                    pResp->SetBody("404 Not Found");
                }
            }

            static void RouteAndSendResponse(const std::string& method, const System::SmartPointer<Http::HttpContext>& spContext, const System::SmartPointer<System::IO::Stream>& spBaseStream,
                const System::Collections::Generic::Dictionary<System::String, System::Func<System::String, System::SmartPointer<Http::HttpContext>>>& getHandlers,
                const System::Collections::Generic::Dictionary<System::String, System::Func<System::String, System::SmartPointer<Http::HttpContext>>>& postHandlers,
                const System::Collections::Generic::Dictionary<System::String, System::Func<System::String, System::SmartPointer<Http::HttpContext>>>& putHandlers,
                const System::Collections::Generic::Dictionary<System::String, System::Func<System::String, System::SmartPointer<Http::HttpContext>>>& deleteHandlers) {
                /// Route: Look up handler delegate based on HTTP method.
                System::Func<System::String, System::SmartPointer<Http::HttpContext>> pHandler;
                bool bFound = false;
                if (method == "GET" || method == "HEAD") bFound = MatchAndFindHandler(spContext->GetRequest().Get(), getHandlers, pHandler);
                else if (method == "POST") bFound = MatchAndFindHandler(spContext->GetRequest().Get(), postHandlers, pHandler);
                else if (method == "PUT") bFound = MatchAndFindHandler(spContext->GetRequest().Get(), putHandlers, pHandler);
                else if (method == "DELETE") bFound = MatchAndFindHandler(spContext->GetRequest().Get(), deleteHandlers, pHandler);
                /// Execute: Dispatch handler and write output response.
                DispatchResponse(spContext->GetResponse().Get(), bFound, pHandler, spContext);
                if (spContext->GetResponse()->IsHeadersSent()) spContext->GetResponse()->Flush();
                else SendHttpResponseData(spBaseStream, spContext->GetResponse().Get(), method);
            }

            void WebApplication::HandleConnection(System::SmartPointer<System::Net::Sockets::TcpClient> spClient) {
                /// Guard: Obtain active client network stream.
                auto stream = spClient->GetStream();
                if (stream.IsNull()) return;
                std::vector<std::string> lines;
                System::SmartPointer<System::IO::Stream> spBaseStream = stream;
                if (!ReadHeaderLines(spBaseStream, lines, m_bRunning)) return;
                /// Parse: Extract request line, headers, and body.
                auto spContext = System::SmartPointer<Http::HttpContext>::NewShared();
                std::string method = ParseRequestLine(lines[0], spContext->GetRequest().Get());
                ReadHeadersAndBody(spBaseStream, lines, spContext->GetRequest().Get());
                spContext->GetResponse()->BindStream(stream);
                /// Process: Handle WebSocket upgrade or dispatch standard HTTP route.
                if (TryHandleWebSocket(stream, spContext, m_wsHandlers)) { spClient->Close(); return; }
                RouteAndSendResponse(method, spContext, spBaseStream, m_getHandlers, m_postHandlers, m_putHandlers, m_deleteHandlers);
                spClient->Close();
            }

        }
    }
}
