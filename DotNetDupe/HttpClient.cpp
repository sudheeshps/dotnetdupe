#include "pch.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/Dns.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentException.h"
#include "System/IO/MemoryStream.h"
#include <sstream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                static std::string ReadLine(const SmartPointer<Sockets::NetworkStream>& stream) {
                    std::string line;
                    char c;
                    while (true) {
                        try {
                            int read = stream->Read(&c, 0, 1);
                            if (read <= 0) break;
                            if (c == '\n') break;
                            if (c != '\r') {
                                line += c;
                            }
                        } catch (...) {
                            break;
                        }
                    }
                    return line;
                }

                HttpClient::HttpClient() {}

                SmartPointer<HttpResponseMessage> HttpClient::Get(const String& requestUri) {
                    return Get(Uri(requestUri));
                }

                SmartPointer<HttpResponseMessage> HttpClient::Get(const Uri& requestUri) {
                    auto request = SmartPointer<HttpRequestMessage>::NewShared(HttpMethod::Get, requestUri);
                    return Send(request);
                }

                SmartPointer<HttpResponseMessage> HttpClient::Post(const String& requestUri, const SmartPointer<HttpContent>& content) {
                    return Post(Uri(requestUri), content);
                }

                SmartPointer<HttpResponseMessage> HttpClient::Post(const Uri& requestUri, const SmartPointer<HttpContent>& content) {
                    auto request = SmartPointer<HttpRequestMessage>::NewShared(HttpMethod::Post, requestUri);
                    request->SetContent(content);
                    return Send(request);
                }

                SmartPointer<HttpResponseMessage> HttpClient::Put(const String& requestUri, const SmartPointer<HttpContent>& content) {
                    return Put(Uri(requestUri), content);
                }

                SmartPointer<HttpResponseMessage> HttpClient::Put(const Uri& requestUri, const SmartPointer<HttpContent>& content) {
                    auto request = SmartPointer<HttpRequestMessage>::NewShared(HttpMethod::Put, requestUri);
                    request->SetContent(content);
                    return Send(request);
                }

                SmartPointer<HttpResponseMessage> HttpClient::Delete(const String& requestUri) {
                    return Delete(Uri(requestUri));
                }

                SmartPointer<HttpResponseMessage> HttpClient::Delete(const Uri& requestUri) {
                    auto request = SmartPointer<HttpRequestMessage>::NewShared(HttpMethod::Delete, requestUri);
                    return Send(request);
                }

                String HttpClient::GetString(const String& requestUri) {
                    return GetString(Uri(requestUri));
                }

                String HttpClient::GetString(const Uri& requestUri) {
                    auto response = Get(requestUri);
                    response->EnsureSuccessStatusCode();
                    auto content = response->GetContent();
                    if (content.IsNull()) return String("");
                    return content->ReadAsString();
                }

                Array<char> HttpClient::GetByteArray(const String& requestUri) {
                    return GetByteArray(Uri(requestUri));
                }

                Array<char> HttpClient::GetByteArray(const Uri& requestUri) {
                    auto response = Get(requestUri);
                    response->EnsureSuccessStatusCode();
                    auto content = response->GetContent();
                    if (content.IsNull()) return Array<char>(0);
                    return content->ReadAsByteArray();
                }

                Collections::Generic::Dictionary<String, String>& HttpClient::GetDefaultRequestHeaders() {
                    return m_defaultRequestHeaders;
                }

                const Collections::Generic::Dictionary<String, String>& HttpClient::GetDefaultRequestHeaders() const {
                    return m_defaultRequestHeaders;
                }

                SmartPointer<HttpResponseMessage> HttpClient::Send(const SmartPointer<HttpRequestMessage>& request) {
                    if (request.IsNull()) {
                        throw ArgumentNullException("request");
                    }

                    Uri uri = request->GetRequestUri();
                    String scheme = uri.GetScheme();
                    if (scheme.ToLower() != "http") {
                        throw ArgumentException("Only 'http' scheme is supported.");
                    }

                    String host = uri.GetHost();
                    int port = uri.GetPort();
                    if (port <= 0) port = 80;

                    // Resolve hostname to IP Address
                    Array<String> ipAddresses = Dns::GetHostAddresses(host);
                    if (ipAddresses.GetLength() == 0) {
                        throw HttpRequestException("Could not resolve host.");
                    }
                    String resolvedIp = ipAddresses[0];

                    // Connect
                    Sockets::TcpClient tcpClient;
                    try {
                        tcpClient.Connect(resolvedIp, port);
                    } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                        throw HttpRequestException(ex.What());
                    }

                    auto stream = tcpClient.GetStream();

                    // Format path and query
                    String path = uri.GetAbsolutePath();
                    if (path.IsEmpty()) {
                        path = "/";
                    }
                    String query = uri.GetQuery();
                    std::string requestPath = path.GetRawString();
                    if (!query.IsEmpty()) {
                        requestPath += "?";
                        requestPath += query.GetRawString();
                    }

                    // Write HTTP request headers
                    std::ostringstream requestHeadersStream;
                    requestHeadersStream << request->GetMethod().GetMethod().GetRawString() << " " << requestPath << " HTTP/1.1\r\n";
                    
                    // Host header
                    requestHeadersStream << "Host: " << host.GetRawString();
                    if (uri.GetPort() != 80 && uri.GetPort() > 0) {
                        requestHeadersStream << ":" << uri.GetPort();
                    }
                    requestHeadersStream << "\r\n";

                    // Default request headers
                    auto defaultHeadersKeys = m_defaultRequestHeaders.GetKeys();
                    for (int i = 0; i < defaultHeadersKeys.GetLength(); ++i) {
                        String key = defaultHeadersKeys[i];
                        requestHeadersStream << key.GetRawString() << ": " << m_defaultRequestHeaders[key].GetRawString() << "\r\n";
                    }

                    // Request headers
                    auto requestHeadersKeys = request->GetHeaders().GetKeys();
                    for (int i = 0; i < requestHeadersKeys.GetLength(); ++i) {
                        String key = requestHeadersKeys[i];
                        requestHeadersStream << key.GetRawString() << ": " << request->GetHeaders()[key].GetRawString() << "\r\n";
                    }

                    // Content headers
                    auto content = request->GetContent();
                    if (!content.IsNull()) {
                        auto contentHeadersKeys = content->GetHeaders().GetKeys();
                        for (int i = 0; i < contentHeadersKeys.GetLength(); ++i) {
                            String key = contentHeadersKeys[i];
                            requestHeadersStream << key.GetRawString() << ": " << content->GetHeaders()[key].GetRawString() << "\r\n";
                        }

                        long len = content->GetLength();
                        if (len >= 0) {
                            requestHeadersStream << "Content-Length: " << len << "\r\n";
                        }
                    }

                    requestHeadersStream << "Connection: close\r\n\r\n";

                    std::string headersStr = requestHeadersStream.str();
                    stream->Write(headersStr.data(), 0, static_cast<int>(headersStr.size()));

                    // Write content
                    if (!content.IsNull()) {
                        content->CopyTo(stream);
                    }

                    // Parse HTTP response
                    std::string statusLine = ReadLine(stream);
                    if (statusLine.empty()) {
                        throw HttpRequestException("No response from server.");
                    }

                    // HTTP/1.1 StatusCode ReasonPhrase
                    size_t firstSpace = statusLine.find(' ');
                    if (firstSpace == std::string::npos) {
                        throw HttpRequestException("Invalid response status line.");
                    }

                    size_t secondSpace = statusLine.find(' ', firstSpace + 1);
                    int statusCodeVal = 0;
                    std::string reasonPhrase;
                    if (secondSpace == std::string::npos) {
                        statusCodeVal = std::atoi(statusLine.substr(firstSpace + 1).c_str());
                    } else {
                        statusCodeVal = std::atoi(statusLine.substr(firstSpace + 1, secondSpace - firstSpace - 1).c_str());
                        reasonPhrase = statusLine.substr(secondSpace + 1);
                    }

                    auto response = SmartPointer<HttpResponseMessage>::NewShared(static_cast<HttpStatusCode>(statusCodeVal));
                    response->SetReasonPhrase(String(reasonPhrase.c_str()));

                    auto& respHeaders = response->GetHeaders();
                    bool chunked = false;
                    long contentLength = -1;
                    String contentType = "text/plain";

                    while (true) {
                        std::string headerLine = ReadLine(stream);
                        if (headerLine.empty()) break;

                        size_t colon = headerLine.find(':');
                        if (colon != std::string::npos) {
                            std::string key = headerLine.substr(0, colon);
                            std::string val = headerLine.substr(colon + 1);

                            // trim whitespace
                            while (!key.empty() && std::isspace(static_cast<unsigned char>(key.front()))) key.erase(key.begin());
                            while (!key.empty() && std::isspace(static_cast<unsigned char>(key.back()))) key.pop_back();
                            while (!val.empty() && std::isspace(static_cast<unsigned char>(val.front()))) val.erase(val.begin());
                            while (!val.empty() && std::isspace(static_cast<unsigned char>(val.back()))) val.pop_back();

                            String sKey(key.c_str());
                            String sVal(val.c_str());
                            respHeaders[sKey] = sVal;

                            if (sKey.ToLower() == "transfer-encoding" && sVal.ToLower() == "chunked") {
                                chunked = true;
                            } else if (sKey.ToLower() == "content-length") {
                                contentLength = std::atol(val.c_str());
                            } else if (sKey.ToLower() == "content-type") {
                                contentType = sVal;
                            }
                        }
                    }

                    // Read body content
                    std::vector<char> bodyData;
                    if (chunked) {
                        while (true) {
                            std::string sizeLine = ReadLine(stream);
                            if (sizeLine.empty()) break;

                            long chunkSize = std::strtol(sizeLine.c_str(), nullptr, 16);
                            if (chunkSize <= 0) {
                                ReadLine(stream); // read trailing CRLF of the final chunk
                                break;
                            }

                            std::vector<char> chunk(chunkSize);
                            int totalRead = 0;
                            while (totalRead < chunkSize) {
                                int read = stream->Read(chunk.data() + totalRead, 0, static_cast<int>(chunkSize - totalRead));
                                if (read <= 0) {
                                    throw HttpRequestException("Connection closed prematurely while reading chunk data.");
                                }
                                totalRead += read;
                            }
                            bodyData.insert(bodyData.end(), chunk.begin(), chunk.end());

                            ReadLine(stream); // read trailing CRLF of the chunk
                        }
                    } else if (contentLength >= 0) {
                        bodyData.resize(contentLength);
                        int totalRead = 0;
                        while (totalRead < contentLength) {
                            int read = stream->Read(bodyData.data() + totalRead, 0, static_cast<int>(contentLength - totalRead));
                            if (read <= 0) {
                                throw HttpRequestException("Connection closed prematurely while reading content.");
                            }
                            totalRead += read;
                        }
                    } else {
                        // Read until EOF
                        char buffer[4096];
                        int bytesRead = 0;
                        while ((bytesRead = stream->Read(buffer, 0, sizeof(buffer))) > 0) {
                            bodyData.insert(bodyData.end(), buffer, buffer + bytesRead);
                        }
                    }

                    Array<char> arrData(static_cast<int>(bodyData.size()));
                    if (!bodyData.empty()) {
                        std::memcpy(arrData.GetData(), bodyData.data(), bodyData.size());
                    }

                    auto responseContent = SmartPointer<HttpContent>(new ByteArrayContent(arrData), true);
                    responseContent->GetHeaders()["Content-Type"] = contentType;
                    response->SetContent(responseContent);

                    return response;
                }

            }
        }
    }
}
