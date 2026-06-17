#include "pch.h"
#include "System/Net/Http/HttpClient.h"
#include "System/Net/Http/HttpRequestException.h"
#include "System/Net/Dns.h"
#include "System/Net/Sockets/TcpClient.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/Net/Sockets/SocketException.h"
#include "System/Net/Security/SslStream.h"
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

                static std::string ReadLine(const SmartPointer<IO::Stream>& stream) {
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

                String HttpClient::ResolveHost(const Uri& uri, int& riPort) {
                    String sHost = uri.GetHost();
                    riPort = uri.GetPort();
                    if (riPort <= 0) {
                        riPort = 80;
                    }

                    // Resolve hostname to IP Address
                    Array<String> arrIpAddresses = Dns::GetHostAddresses(sHost);
                    if (arrIpAddresses.GetLength() == 0) {
                        throw HttpRequestException("Could not resolve host.");
                    }
                    return arrIpAddresses[0];
                }

                std::string HttpClient::PrepareHeaders(const SmartPointer<HttpRequestMessage>& spRequest, const Uri& uri) {
                    String sHost = uri.GetHost();
                    String sPath = uri.GetAbsolutePath();
                    if (sPath.IsEmpty()) {
                        sPath = "/";
                    }
                    String sQuery = uri.GetQuery();
                    std::string sRequestPath = sPath.GetRawString();
                    if (!sQuery.IsEmpty()) {
                        sRequestPath += "?";
                        sRequestPath += sQuery.GetRawString();
                    }

                    // Write HTTP request headers
                    std::ostringstream ssHeadersStream;
                    ssHeadersStream << spRequest->GetMethod().GetMethod().GetRawString() << " " << sRequestPath << " HTTP/1.1\r\n";
                    
                    // Host header
                    ssHeadersStream << "Host: " << sHost.GetRawString();
                    if (uri.GetPort() != 80 && uri.GetPort() > 0) {
                        ssHeadersStream << ":" << uri.GetPort();
                    }
                    ssHeadersStream << "\r\n";

                    // Default request headers
                    auto arrDefaultHeadersKeys = m_defaultRequestHeaders.GetKeys();
                    for (int i = 0; i < arrDefaultHeadersKeys.GetLength(); ++i) {
                        String sKey = arrDefaultHeadersKeys[i];
                        ssHeadersStream << sKey.GetRawString() << ": " << m_defaultRequestHeaders[sKey].GetRawString() << "\r\n";
                    }

                    // Request headers
                    auto arrRequestHeadersKeys = spRequest->GetHeaders().GetKeys();
                    for (int i = 0; i < arrRequestHeadersKeys.GetLength(); ++i) {
                        String sKey = arrRequestHeadersKeys[i];
                        ssHeadersStream << sKey.GetRawString() << ": " << spRequest->GetHeaders()[sKey].GetRawString() << "\r\n";
                    }

                    // Content headers
                    auto spContent = spRequest->GetContent();
                    if (!spContent.IsNull()) {
                        auto arrContentHeadersKeys = spContent->GetHeaders().GetKeys();
                        for (int i = 0; i < arrContentHeadersKeys.GetLength(); ++i) {
                            String sKey = arrContentHeadersKeys[i];
                            ssHeadersStream << sKey.GetRawString() << ": " << spContent->GetHeaders()[sKey].GetRawString() << "\r\n";
                        }

                        long lLen = spContent->GetLength();
                        if (lLen >= 0) {
                            ssHeadersStream << "Content-Length: " << lLen << "\r\n";
                        }
                    }

                    ssHeadersStream << "Connection: close\r\n\r\n";

                    return ssHeadersStream.str();
                }

                void HttpClient::SendRequest(const SmartPointer<IO::Stream>& spStream, const std::string& sHeaders, const SmartPointer<HttpContent>& spContent) {
                    spStream->Write(sHeaders.data(), 0, static_cast<int>(sHeaders.size()));

                    // Write content
                    if (!spContent.IsNull()) {
                        spContent->CopyTo(spStream);
                    }
                }

                SmartPointer<HttpResponseMessage> HttpClient::PrepareResponse(const SmartPointer<IO::Stream>& spStream) {
                    // Parse HTTP response
                    std::string sStatusLine = ReadLine(spStream);
                    if (sStatusLine.empty()) {
                        throw HttpRequestException("No response from server.");
                    }

                    // HTTP/1.1 StatusCode ReasonPhrase
                    size_t iFirstSpace = sStatusLine.find(' ');
                    if (iFirstSpace == std::string::npos) {
                        throw HttpRequestException("Invalid response status line.");
                    }

                    size_t iSecondSpace = sStatusLine.find(' ', iFirstSpace + 1);
                    int iStatusCodeVal = 0;
                    std::string sReasonPhrase;
                    if (iSecondSpace == std::string::npos) {
                        iStatusCodeVal = std::atoi(sStatusLine.substr(iFirstSpace + 1).c_str());
                    } else {
                        iStatusCodeVal = std::atoi(sStatusLine.substr(iFirstSpace + 1, iSecondSpace - iFirstSpace - 1).c_str());
                        sReasonPhrase = sStatusLine.substr(iSecondSpace + 1);
                    }

                    auto spResponse = SmartPointer<HttpResponseMessage>::NewShared(static_cast<HttpStatusCode>(iStatusCodeVal));
                    spResponse->SetReasonPhrase(String(sReasonPhrase.c_str()));

                    auto& dictRespHeaders = spResponse->GetHeaders();
                    bool bChunked = false;
                    long lContentLength = -1;
                    String sContentType = "text/plain";

                    while (true) {
                        std::string sHeaderLine = ReadLine(spStream);
                        if (sHeaderLine.empty()) break;

                        size_t iColon = sHeaderLine.find(':');
                        if (iColon != std::string::npos) {
                            std::string sKey = sHeaderLine.substr(0, iColon);
                            std::string sVal = sHeaderLine.substr(iColon + 1);

                            // trim whitespace
                            while (!sKey.empty() && std::isspace(static_cast<unsigned char>(sKey.front()))) sKey.erase(sKey.begin());
                            while (!sKey.empty() && std::isspace(static_cast<unsigned char>(sKey.back()))) sKey.pop_back();
                            while (!sVal.empty() && std::isspace(static_cast<unsigned char>(sVal.front()))) sVal.erase(sVal.begin());
                            while (!sVal.empty() && std::isspace(static_cast<unsigned char>(sVal.back()))) sVal.pop_back();

                            String sKeyObj(sKey.c_str());
                            String sValObj(sVal.c_str());
                            dictRespHeaders[sKeyObj] = sValObj;

                            if (sKeyObj.ToLower() == "transfer-encoding" && sValObj.ToLower() == "chunked") {
                                bChunked = true;
                            } else if (sKeyObj.ToLower() == "content-length") {
                                lContentLength = std::atol(sVal.c_str());
                            } else if (sKeyObj.ToLower() == "content-type") {
                                sContentType = sValObj;
                            }
                        }
                    }

                    // Read body content
                    std::vector<char> vecBodyData;
                    if (bChunked) {
                        while (true) {
                            std::string sSizeLine = ReadLine(spStream);
                            if (sSizeLine.empty()) break;

                            long lChunkSize = std::strtol(sSizeLine.c_str(), nullptr, 16);
                            if (lChunkSize <= 0) {
                                ReadLine(spStream); // read trailing CRLF of the final chunk
                                break;
                            }

                            std::vector<char> vecChunk(lChunkSize);
                            int iTotalRead = 0;
                            while (iTotalRead < lChunkSize) {
                                int iRead = spStream->Read(vecChunk.data() + iTotalRead, 0, static_cast<int>(lChunkSize - iTotalRead));
                                if (iRead <= 0) {
                                    throw HttpRequestException("Connection closed prematurely while reading chunk data.");
                                }
                                iTotalRead += iRead;
                            }
                            vecBodyData.insert(vecBodyData.end(), vecChunk.begin(), vecChunk.end());

                            ReadLine(spStream); // read trailing CRLF of the chunk
                        }
                    } else if (lContentLength >= 0) {
                        vecBodyData.resize(lContentLength);
                        int iTotalRead = 0;
                        while (iTotalRead < lContentLength) {
                            int iRead = spStream->Read(vecBodyData.data() + iTotalRead, 0, static_cast<int>(lContentLength - iTotalRead));
                            if (iRead <= 0) {
                                throw HttpRequestException("Connection closed prematurely while reading content.");
                            }
                            iTotalRead += iRead;
                        }
                    } else {
                        // Read until EOF
                        char arrBuffer[4096];
                        int iBytesRead = 0;
                        while ((iBytesRead = spStream->Read(arrBuffer, 0, sizeof(arrBuffer))) > 0) {
                            vecBodyData.insert(vecBodyData.end(), arrBuffer, arrBuffer + iBytesRead);
                        }
                    }

                    Array<char> arrData(static_cast<int>(vecBodyData.size()));
                    if (!vecBodyData.empty()) {
                        std::memcpy(arrData.GetData(), vecBodyData.data(), vecBodyData.size());
                    }

                    auto spResponseContent = SmartPointer<HttpContent>(new ByteArrayContent(arrData), true);
                    spResponseContent->GetHeaders()["Content-Type"] = sContentType;
                    spResponse->SetContent(spResponseContent);

                    return spResponse;
                }

                SmartPointer<HttpResponseMessage> HttpClient::Send(const SmartPointer<HttpRequestMessage>& request) {
                    if (request.IsNull()) {
                        throw ArgumentNullException("request");
                    }

                    Uri uri = request->GetRequestUri();
                    String scheme = uri.GetScheme().ToLower();
                    if (scheme != "http" && scheme != "https") {
                        throw ArgumentException("Only 'http' and 'https' schemes are supported.");
                    }

                    int iPort = uri.GetPort();
                    if (iPort <= 0) {
                        iPort = (scheme == "https") ? 443 : 80;
                    }
                    String sResolvedIp = ResolveHost(uri, iPort);

                    // Connect
                    Sockets::TcpClient tcpClient;
                    try {
                        tcpClient.Connect(sResolvedIp, iPort);
                    } catch (const DotNetDupe::System::Net::Sockets::SocketException& ex) {
                        throw HttpRequestException(ex.What());
                    }

                    SmartPointer<IO::Stream> spStream = tcpClient.GetStream();

                    if (scheme == "https") {
                        auto spSslStream = SmartPointer<Net::Security::SslStream>::NewShared(spStream, false);
                        try {
                            spSslStream->AuthenticateAsClient(uri.GetHost());
                        } catch (const BasicException<char>& ex) {
                            throw HttpRequestException(ex.What());
                        }
                        spStream = spSslStream;
                    }

                    std::string sHeaders = PrepareHeaders(request, uri);

                    SendRequest(spStream, sHeaders, request->GetContent());

                    return PrepareResponse(spStream);
                }

            }
        }
    }
}
