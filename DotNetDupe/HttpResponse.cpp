#include "pch.h"
#include "WebAppCore/Http/HttpContext.h"
#include "System/Console.h"
#include "System/Convert.h"
#include <string>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Http {

            static void WriteRaw(System::SmartPointer<System::Net::Sockets::NetworkStream> pStream, const std::string& str) {
                if (!pStream.IsNull() && !str.empty()) {
                    pStream->Write(str.data(), 0, static_cast<int>(str.length()));
                }
            }

            static std::string GetHttpStatusMessage(int code) {
                switch (code) {
                case 201: return "Created";
                case 204: return "No Content";
                case 400: return "Bad Request";
                case 401: return "Unauthorized";
                case 404: return "Not Found";
                case 500: return "Internal Server Error";
                default: return "OK";
                }
            }

            static std::string BuildResponseHeaderString(int statusCode, const System::String& contentType, bool bChunked, const System::Collections::Generic::Dictionary<System::String, System::String>& headers) {
                std::string headerString = "HTTP/1.1 " + std::to_string(statusCode) + " " + GetHttpStatusMessage(statusCode) + "\r\n";
                headerString += "Content-Type: " + std::string(contentType.GetRawString() ? contentType.GetRawString() : "") + "\r\n";
                if (bChunked) headerString += "Transfer-Encoding: chunked\r\n";
                headerString += "Connection: keep-alive\r\nServer: DotNetDupeWebApplication/1.0\r\n";
                auto keys = headers.GetKeys(); auto values = headers.GetValues();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    headerString += std::string(keys[i].GetRawString() ? keys[i].GetRawString() : "") + ": " + std::string(values[i].GetRawString() ? values[i].GetRawString() : "") + "\r\n";
                }
                return headerString + "\r\n";
            }

            void HttpResponse::FlushHeaders() {
                if (m_bHeadersSent) return;
                m_bHeadersSent = true;
                WriteRaw(m_pStream, BuildResponseHeaderString(m_nStatusCode, m_sContentType, m_bChunked, m_headers));
            }

            void HttpResponse::WriteChunk(const System::String& data) {
                std::string rawData = std::string(data.GetRawString() ? data.GetRawString() : "");
                if (rawData.empty()) return;
                if (!m_bHeadersSent) {
                    m_bChunked = true;
                    FlushHeaders();
                }

                char hexBuf[32] = { 0 };
                snprintf(hexBuf, sizeof(hexBuf), "%zX\r\n", rawData.length());
                std::string chunk = std::string(hexBuf) + rawData + "\r\n";
                WriteRaw(m_pStream, chunk);
            }

            void HttpResponse::Flush() {
                if (!m_bHeadersSent) {
                    FlushHeaders();
                }
                if (m_bChunked) {
                    WriteRaw(m_pStream, "0\r\n\r\n");
                } else {
                    std::string body = std::string(m_sBody.GetRawString() ? m_sBody.GetRawString() : "");
                    if (!body.empty()) {
                        WriteRaw(m_pStream, body);
                    }
                }
            }

        }
    }
}
