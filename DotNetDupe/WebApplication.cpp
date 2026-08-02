#include "pch.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "System/Console.h"
#include "System/Net/Sockets/TcpListener.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Threading/ThreadPool.h"
#include "System/Convert.h"
#include "System/BasicException.h"
#include "System/Net/HttpStatusCode.h"
#include <string>
#include <vector>
#include <algorithm>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {

            namespace {
                std::vector<std::string> GetPathSegments(const std::string& path) {
                    std::vector<std::string> segments;
                    std::string segment;
                    for (char c : path) {
                        if (c == '/') {
                            if (!segment.empty()) {
                                segments.push_back(segment);
                                segment.clear();
                            }
                        } else {
                            segment += c;
                        }
                    }
                    if (!segment.empty()) {
                        segments.push_back(segment);
                    }
                    return segments;
                }

                bool MatchRoute(const std::vector<std::string>& patternSegs, const std::vector<std::string>& pathSegs, std::vector<std::pair<std::string, std::string>>& extractedParams) {
                    // Check for catch-all parameter in patternSegs (e.g. {*filepath})
                    if (!patternSegs.empty() && patternSegs.back().length() >= 3 && patternSegs.back().front() == '{' && patternSegs.back()[1] == '*' && patternSegs.back().back() == '}') {
                        size_t fixedCount = patternSegs.size() - 1;
                        if (pathSegs.size() < fixedCount) return false;

                        for (size_t i = 0; i < fixedCount; ++i) {
                            const std::string& patternSeg = patternSegs[i];
                            const std::string& pathSeg = pathSegs[i];
                            if (patternSeg.length() >= 2 && patternSeg.front() == '{' && patternSeg.back() == '}') {
                                std::string paramName = patternSeg.substr(1, patternSeg.length() - 2);
                                extractedParams.push_back({paramName, pathSeg});
                            } else if (patternSeg != pathSeg) {
                                return false;
                            }
                        }

                        std::string catchAllName = patternSegs.back().substr(2, patternSegs.back().length() - 3);
                        std::string restPath;
                        for (size_t i = fixedCount; i < pathSegs.size(); ++i) {
                            if (!restPath.empty()) restPath += "/";
                            restPath += pathSegs[i];
                        }
                        extractedParams.push_back({catchAllName, restPath});
                        return true;
                    }

                    if (patternSegs.size() != pathSegs.size()) {
                        return false;
                    }
                    for (size_t i = 0; i < patternSegs.size(); ++i) {
                        const std::string& patternSeg = patternSegs[i];
                        const std::string& pathSeg = pathSegs[i];
                        
                        if (patternSeg.length() >= 2 && patternSeg.front() == '{' && patternSeg.back() == '}') {
                            std::string paramName = patternSeg.substr(1, patternSeg.length() - 2);
                            extractedParams.push_back({paramName, pathSeg});
                        } else {
                            if (patternSeg != pathSeg) {
                                return false;
                            }
                        }
                    }
                    return true;
                }
            }

            WebApplication::WebApplication(const System::SmartPointer<System::IServiceProvider>& spServices)
                : m_spServices(spServices), m_bRunning(false), m_nPort(0) {}

            WebApplication::~WebApplication() {
                Stop();
            }

            WebApplication::WebApplication(WebApplication&& other) noexcept
                : m_spServices(std::move(other.m_spServices)),
                  m_getHandlers(std::move(other.m_getHandlers)),
                  m_postHandlers(std::move(other.m_postHandlers)),
                  m_putHandlers(std::move(other.m_putHandlers)),
                  m_deleteHandlers(std::move(other.m_deleteHandlers)),
                  m_pListener(std::move(other.m_pListener)),
                  m_bRunning(other.m_bRunning.load()),
                  m_sHost(std::move(other.m_sHost)),
                  m_nPort(other.m_nPort),
                  m_controllerRegistrars(std::move(other.m_controllerRegistrars)) {}

            WebApplication& WebApplication::operator=(WebApplication&& other) noexcept {
                if (this != &other) {
                    m_spServices = std::move(other.m_spServices);
                    m_getHandlers = std::move(other.m_getHandlers);
                    m_postHandlers = std::move(other.m_postHandlers);
                    m_putHandlers = std::move(other.m_putHandlers);
                    m_deleteHandlers = std::move(other.m_deleteHandlers);
                    m_pListener = std::move(other.m_pListener);
                    m_bRunning = other.m_bRunning.load();
                    m_sHost = std::move(other.m_sHost);
                    m_nPort = other.m_nPort;
                    m_controllerRegistrars = std::move(other.m_controllerRegistrars);
                }
                return *this;
            }

            System::SmartPointer<WebApplicationBuilder> WebApplication::CreateBuilder() {
                return System::SmartPointer<WebApplicationBuilder>::NewShared();
            }

            void WebApplication::MapGet(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                m_getHandlers.Add(pattern, handler);
            }

            void WebApplication::MapPost(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                m_postHandlers.Add(pattern, handler);
            }

            void WebApplication::MapPut(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                m_putHandlers.Add(pattern, handler);
            }

            void WebApplication::MapDelete(const System::String& pattern, System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler) {
                m_deleteHandlers.Add(pattern, handler);
            }

            void WebApplication::MapControllers() {
                for (auto& registerRoutes : m_controllerRegistrars) {
                    registerRoutes(m_spSelf);
                }
            }

            void WebApplication::Run(const System::String& url, int threadCount) {
                if (threadCount > 0) {
                    System::Threading::ThreadPool::SetMinThreads(threadCount);
                }
                std::string sUrl(url.GetRawString());
                std::string host = "127.0.0.1";
                int port = 5000;

                size_t protocolPos = sUrl.find("://");
                std::string hostPort = sUrl;
                if (protocolPos != std::string::npos) {
                    hostPort = sUrl.substr(protocolPos + 3);
                }

                size_t colonPos = hostPort.find(':');
                if (colonPos != std::string::npos) {
                    host = hostPort.substr(0, colonPos);
                    std::string sPort = hostPort.substr(colonPos + 1);
                    size_t slashPos = sPort.find('/');
                    if (slashPos != std::string::npos) {
                        sPort = sPort.substr(0, slashPos);
                    }
                    try {
                        port = std::stoi(sPort);
                    } catch (...) {
                        port = 5000;
                    }
                } else {
                    size_t slashPos = hostPort.find('/');
                    if (slashPos != std::string::npos) {
                        host = hostPort.substr(0, slashPos);
                    } else {
                        host = hostPort;
                    }
                }

                if (host == "localhost") {
                    host = "127.0.0.1";
                }

                m_sHost = System::String(host.c_str());
                m_nPort = port;

                m_pListener = System::SmartPointer<System::Net::Sockets::TcpListener>::NewShared(System::String(host.c_str()), port);
                m_pListener->Start();

                System::Console::WriteLine(System::String("[Server] Started listener on host: ") + m_sHost + " port: " + System::Convert::ToString(port));
                System::Console::WriteLine("Hosting environment: Production");
                System::Console::WriteLine(System::String("Now listening on: ") + url);
                System::Console::WriteLine("Application started. Press Ctrl+C to shut down.");

                m_bRunning = true;
                try {
                    while (m_bRunning) {
                        System::Console::WriteLine("[Server] Waiting for connection in AcceptTcpClient...");
                        auto client = m_pListener->AcceptTcpClient();
                        System::Console::WriteLine("[Server] AcceptTcpClient returned a client!");
                        if (client.IsNull()) {
                            System::Console::WriteLine("[Server] Accepted client is NULL!");
                            continue;
                        }

                        struct ConnectionState : public Object {
                            System::SmartPointer<System::Net::Sockets::TcpClient> Client;
                            WebApplication* App;
                            ConnectionState(System::SmartPointer<System::Net::Sockets::TcpClient> c, WebApplication* a) : Client(std::move(c)), App(a) {}
                        };

                        System::Console::WriteLine("[Server] Queueing connection to ThreadPool...");
                        auto* pState = new ConnectionState(std::move(client), this);
                        System::Threading::ThreadPool::QueueUserWorkItem([](Object* rawState) {
                            System::Console::WriteLine("[Server] ThreadPool callback invoked!");
                            System::SmartPointer<ConnectionState> spState(dynamic_cast<ConnectionState*>(rawState));
                            if (!spState.IsNull() && spState->App) {
                                try {
                                    System::Console::WriteLine("[Server] Calling HandleConnection...");
                                    spState->App->HandleConnection(std::move(spState->Client));
                                    System::Console::WriteLine("[Server] HandleConnection finished!");
                                } catch (const DotNetDupe::System::BasicException<char>& ex) {
                                    System::Console::WriteLine(System::String("[Server Exception] HandleConnection: ") + ex.What());
                                } catch (...) {
                                    System::Console::WriteLine("[Server Exception] HandleConnection unknown");
                                }
                            } else {
                                System::Console::WriteLine("[Server Exception] ConnectionState is NULL or App is NULL");
                            }
                        }, pState);
                    }
                } catch (const System::Net::Sockets::SocketException& ex) {
                    System::Console::WriteLine(System::String("[Server Exception] AcceptTcpClient: ") + ex.What());
                } catch (const System::BasicException<char>& ex) {
                    System::Console::WriteLine(System::String("[Server Exception] Run loop: ") + ex.What());
                } catch (...) {
                    System::Console::WriteLine("[Server Exception] Run loop unknown");
                }
                System::Console::WriteLine("[Server] Run loop exited!");
            }

            void WebApplication::Stop() {
                System::Console::WriteLine("[Server] Stop() invoked!");
                if (!m_bRunning) {
                    System::Console::WriteLine("[Server] Stop(): m_bRunning is already false!");
                    return;
                }
                m_bRunning = false;
                if (!m_pListener.IsNull()) {
                    System::Console::WriteLine("[Server] Stopping TcpListener...");
                    m_pListener->Stop();
                }

                // Send a dummy connection to wake up the AcceptTcpClient block
                try {
                    System::Console::WriteLine(System::String("[Server] Sending dummy wake-up connection to ") + m_sHost + ":" + System::Convert::ToString(m_nPort));
                    System::Net::Sockets::TcpClient dummy;
                    dummy.Connect(m_sHost, m_nPort);
                    dummy.Close();
                    System::Console::WriteLine("[Server] Dummy connection sent successfully.");
                } catch (const System::BasicException<char>& ex) {
                    System::Console::WriteLine(System::String("[Server Exception] Dummy connection failed: ") + ex.What());
                } catch (...) {
                    System::Console::WriteLine("[Server Exception] Dummy connection failed unknown");
                }

                // Break the reference cycle
                m_spSelf = nullptr;
            }

            void WebApplication::HandleConnection(System::SmartPointer<System::Net::Sockets::TcpClient> spClient) {
                System::Console::WriteLine("[Server] HandleConnection started.");
                auto stream = spClient->GetStream();
                if (stream.IsNull()) {
                    System::Console::WriteLine("[Server] HandleConnection: NetworkStream is NULL!");
                    return;
                }

                char c;
                std::string currentLine;
                std::vector<std::string> lines;

                System::Console::WriteLine("[Server] Reading request headers...");
                while (m_bRunning && stream->Read(&c, 0, 1) > 0) {
                    if (c == '\n') {
                        if (!currentLine.empty() && currentLine.back() == '\r') {
                            currentLine.pop_back();
                        }
                        if (currentLine.empty()) {
                            break;
                        }
                        lines.push_back(currentLine);
                        currentLine.clear();
                    } else {
                        currentLine += c;
                    }
                }

                System::Console::WriteLine(System::String("[Server] Headers read complete. Number of lines: ") + System::Convert::ToString((int)lines.size()));
                if (lines.empty()) {
                    System::Console::WriteLine("[Server] Request lines empty, returning.");
                    return;
                }

                std::string reqLine = lines[0];
                size_t space1 = reqLine.find(' ');
                size_t space2 = reqLine.find(' ', space1 + 1);
                if (space1 == std::string::npos || space2 == std::string::npos) return;

                std::string method = reqLine.substr(0, space1);
                std::string fullPath = reqLine.substr(space1 + 1, space2 - space1 - 1);

                auto spContext = System::SmartPointer<Http::HttpContext>::NewShared();
                auto spRequest = spContext->GetRequest();
                auto spResponse = spContext->GetResponse();

                spRequest->SetMethod(System::String(method.c_str()));

                std::string path = fullPath;
                size_t question = fullPath.find('?');
                if (question != std::string::npos) {
                    path = fullPath.substr(0, question);
                    std::string queryStr = fullPath.substr(question + 1);

                    size_t start = 0;
                    while (start < queryStr.length()) {
                        size_t ampersand = queryStr.find('&', start);
                        std::string pair = (ampersand == std::string::npos) ? queryStr.substr(start) : queryStr.substr(start, ampersand - start);
                        size_t equals = pair.find('=');
                        if (equals != std::string::npos) {
                            std::string key = pair.substr(0, equals);
                            std::string val = pair.substr(equals + 1);
                            spRequest->GetQuery().Add(System::String(key.c_str()), System::String(val.c_str()));
                        } else if (!pair.empty()) {
                            spRequest->GetQuery().Add(System::String(pair.c_str()), System::String(""));
                        }
                        if (ampersand == std::string::npos) break;
                        start = ampersand + 1;
                    }
                }
                spRequest->SetPath(System::String(path.c_str()));

                System::Console::WriteLine(System::String("[Server] Parsed request: Method='") + System::String(method.c_str()) + "', Path='" + System::String(path.c_str()) + "'");

                int contentLength = 0;
                for (size_t i = 1; i < lines.size(); ++i) {
                    std::string hLine = lines[i];
                    size_t colon = hLine.find(':');
                    if (colon != std::string::npos) {
                        std::string name = hLine.substr(0, colon);
                        std::string value = hLine.substr(colon + 1);

                        auto trim = [](const std::string& s) {
                            size_t f = s.find_first_not_of(" \t");
                            if (f == std::string::npos) return std::string("");
                            size_t l = s.find_last_not_of(" \t\r\n");
                            return s.substr(f, l - f + 1);
                        };
                        name = trim(name);
                        value = trim(value);

                        std::string nameLower = name;
                        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

                        spRequest->GetHeaders().Add(System::String(nameLower.c_str()), System::String(value.c_str()));

                        if (nameLower == "content-length") {
                            try {
                                contentLength = std::stoi(value);
                            } catch (...) {
                                contentLength = 0;
                            }
                        }
                    }
                }

                if (contentLength > 0) {
                    System::Console::WriteLine(System::String("[Server] Reading content body of length: ") + System::Convert::ToString(contentLength));
                    std::string body;
                    for (int i = 0; i < contentLength; ++i) {
                        if (stream->Read(&c, 0, 1) > 0) {
                            body += c;
                        } else {
                            break;
                        }
                    }
                    spRequest->SetBody(System::String(body.c_str()));
                }

                System::String sBodyResult;
                bool bMatched = false;
                System::Func<System::String, System::SmartPointer<Http::HttpContext>> handler;

                System::Console::WriteLine("[Server] Matching handler...");
                std::string reqPathStr = spRequest->GetPath().GetRawString();
                std::vector<std::string> pathSegs = GetPathSegments(reqPathStr);

                auto findHandler = [&](auto& handlersMap) -> bool {
                    // Try exact match first
                    if (handlersMap.TryGetValue(spRequest->GetPath(), handler)) {
                        return true;
                    }
                    // Try pattern match
                    auto keys = handlersMap.GetKeys();
                    for (int i = 0; i < keys.GetLength(); ++i) {
                        System::String pattern = keys[i];
                        std::vector<std::string> patternSegs = GetPathSegments(pattern.GetRawString());
                        std::vector<std::pair<std::string, std::string>> extractedParams;
                        if (MatchRoute(patternSegs, pathSegs, extractedParams)) {
                            // Found a match! Extract parameters into GetRouteValues()
                            for (const auto& pair : extractedParams) {
                                spRequest->GetRouteValues()[System::String(pair.first.c_str())] = System::String(pair.second.c_str());
                            }
                            if (handlersMap.TryGetValue(pattern, handler)) {
                                return true;
                            }
                        }
                    }
                    return false;
                };

                if (method == "GET") {
                    if (findHandler(m_getHandlers)) {
                        try {
                            System::Console::WriteLine("[Server] Found GET handler, invoking...");
                            sBodyResult = handler(spContext);
                            System::Console::WriteLine("[Server] GET handler invoked successfully.");
                            bMatched = true;
                        } catch (const DotNetDupe::System::BasicException<char>& ex) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = System::String("500 Internal Server Error: ") + ex.What();
                            bMatched = true;
                        } catch (...) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = "500 Internal Server Error";
                            bMatched = true;
                        }
                    }
                } else if (method == "POST") {
                    if (findHandler(m_postHandlers)) {
                        try {
                            System::Console::WriteLine("[Server] Found POST handler, invoking...");
                            sBodyResult = handler(spContext);
                            System::Console::WriteLine("[Server] POST handler invoked successfully.");
                            bMatched = true;
                        } catch (const DotNetDupe::System::BasicException<char>& ex) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = System::String("500 Internal Server Error: ") + ex.What();
                            bMatched = true;
                        } catch (...) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = "500 Internal Server Error";
                            bMatched = true;
                        }
                    }
                } else if (method == "PUT") {
                    if (findHandler(m_putHandlers)) {
                        try {
                            System::Console::WriteLine("[Server] Found PUT handler, invoking...");
                            sBodyResult = handler(spContext);
                            System::Console::WriteLine("[Server] PUT handler invoked successfully.");
                            bMatched = true;
                        } catch (const DotNetDupe::System::BasicException<char>& ex) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = System::String("500 Internal Server Error: ") + ex.What();
                            bMatched = true;
                        } catch (...) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = "500 Internal Server Error";
                            bMatched = true;
                        }
                    }
                } else if (method == "DELETE") {
                    if (findHandler(m_deleteHandlers)) {
                        try {
                            System::Console::WriteLine("[Server] Found DELETE handler, invoking...");
                            sBodyResult = handler(spContext);
                            System::Console::WriteLine("[Server] DELETE handler invoked successfully.");
                            bMatched = true;
                        } catch (const DotNetDupe::System::BasicException<char>& ex) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = System::String("500 Internal Server Error: ") + ex.What();
                            bMatched = true;
                        } catch (...) {
                            spResponse->SetStatusCode(System::Net::HttpStatusCode::InternalServerError);
                            sBodyResult = "500 Internal Server Error";
                            bMatched = true;
                        }
                    }
                }

                if (!bMatched) {
                    System::Console::WriteLine("[Server] Route not matched!");
                    spResponse->SetStatusCode(System::Net::HttpStatusCode::NotFound);
                    sBodyResult = "404 Not Found";
                }

                if (!sBodyResult.IsEmpty()) {
                    spResponse->SetBody(sBodyResult);
                }

                std::string respBody = spResponse->GetBody().GetString();

                std::string statusMsg = "OK";
                int code = spResponse->GetStatusCode();
                if (code == 404) statusMsg = "Not Found";
                else if (code == 500) statusMsg = "Internal Server Error";
                else if (code == 201) statusMsg = "Created";
                else if (code == 204) statusMsg = "No Content";
                else if (code == 400) statusMsg = "Bad Request";
                else if (code == 401) statusMsg = "Unauthorized";

                std::string responseString = "HTTP/1.1 " + std::to_string(code) + " " + statusMsg + "\r\n";
                responseString += "Content-Type: " + spResponse->GetContentType().GetString() + "\r\n";
                responseString += "Content-Length: " + std::to_string(respBody.length()) + "\r\n";
                responseString += "Connection: close\r\n";
                responseString += "Server: DotNetDupeWebApplication/1.0\r\n";

                auto keys = spResponse->GetHeaders().GetKeys();
                auto values = spResponse->GetHeaders().GetValues();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    responseString += keys[i].GetString() + ": " + values[i].GetString() + "\r\n";
                }

                responseString += "\r\n";
                System::Console::WriteLine(System::String("[Server] Sending response. Length: ") + System::Convert::ToString((int)(responseString.length() + respBody.length())));
                stream->Write(responseString.data(), 0, static_cast<int>(responseString.length()));
                if (!respBody.empty()) {
                    stream->Write(respBody.data(), 0, static_cast<int>(respBody.length()));
                }
                spClient->Close();
                System::Console::WriteLine("[Server] Client closed.");
            }

        }
    }
}
