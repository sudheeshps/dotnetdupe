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

            void HttpResponse::FlushHeaders() {
                if (m_bHeadersSent) return;
                m_bHeadersSent = true;

                std::string statusMsg = "OK";
                if (m_nStatusCode == 404) statusMsg = "Not Found";
                else if (m_nStatusCode == 500) statusMsg = "Internal Server Error";
                else if (m_nStatusCode == 201) statusMsg = "Created";
                else if (m_nStatusCode == 204) statusMsg = "No Content";
                else if (m_nStatusCode == 400) statusMsg = "Bad Request";
                else if (m_nStatusCode == 401) statusMsg = "Unauthorized";

                std::string headerString = "HTTP/1.1 " + std::to_string(m_nStatusCode) + " " + statusMsg + "\r\n";
                headerString += "Content-Type: " + m_sContentType.GetString() + "\r\n";
                if (m_bChunked) {
                    headerString += "Transfer-Encoding: chunked\r\n";
                }
                headerString += "Connection: keep-alive\r\n";
                headerString += "Server: DotNetDupeWebApplication/1.0\r\n";

                auto keys = m_headers.GetKeys();
                auto values = m_headers.GetValues();
                for (int i = 0; i < keys.GetLength(); ++i) {
                    headerString += keys[i].GetString() + ": " + values[i].GetString() + "\r\n";
                }
                headerString += "\r\n";

                WriteRaw(m_pStream, headerString);
            }

            void HttpResponse::WriteChunk(const System::String& data) {
                std::string rawData = data.GetString();
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
                    std::string body = m_sBody.GetString();
                    if (!body.empty()) {
                        WriteRaw(m_pStream, body);
                    }
                }
            }

        }
    }
}
