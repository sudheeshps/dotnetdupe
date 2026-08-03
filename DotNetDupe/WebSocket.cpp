#include "pch.h"
#include "System/Net/WebSockets/WebSocket.h"
#include "System/Console.h"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <vector>
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                WebSocket::WebSocket(SmartPointer<Sockets::NetworkStream> pStream)
                    : m_pStream(pStream), m_eState(WebSocketState::Open) {
                }

                String WebSocket::ComputeSecWebSocketAccept(const String& secWebSocketKey) {
                    std::string key = secWebSocketKey.GetString();
                    std::string magic = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

                    unsigned char hash[SHA_DIGEST_LENGTH];
                    SHA1(reinterpret_cast<const unsigned char*>(magic.c_str()), magic.length(), hash);

                    char encoded[128] = { 0 };
                    EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encoded), hash, SHA_DIGEST_LENGTH);
                    return String(encoded);
                }

                bool WebSocket::SendAsync(const String& message) {
                    std::string text = message.GetString();
                    std::vector<uint8_t> frame;
                    frame.push_back(0x81); // FIN bit set + Text frame opcode (0x1)

                    size_t len = text.length();
                    if (len <= 125) {
                        frame.push_back(static_cast<uint8_t>(len));
                    } else if (len <= 65535) {
                        frame.push_back(126);
                        frame.push_back(static_cast<uint8_t>((len >> 8) & 0xFF));
                        frame.push_back(static_cast<uint8_t>(len & 0xFF));
                    } else {
                        frame.push_back(127);
                        for (int i = 7; i >= 0; --i) {
                            frame.push_back(static_cast<uint8_t>((len >> (i * 8)) & 0xFF));
                        }
                    }

                    frame.insert(frame.end(), text.begin(), text.end());
                    Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                    if (m_pStream.IsNull() || m_eState != WebSocketState::Open) return false;
                    m_pStream->Write(reinterpret_cast<const char*>(frame.data()), 0, static_cast<int>(frame.size()));
                    return true;
                }

                bool WebSocket::SendBytes(const std::vector<uint8_t>& data) {
                    std::vector<uint8_t> frame;
                    frame.push_back(0x82); // FIN bit set + Binary frame opcode (0x2)

                    size_t len = data.size();
                    if (len <= 125) {
                        frame.push_back(static_cast<uint8_t>(len));
                    } else if (len <= 65535) {
                        frame.push_back(126);
                        frame.push_back(static_cast<uint8_t>((len >> 8) & 0xFF));
                        frame.push_back(static_cast<uint8_t>(len & 0xFF));
                    } else {
                        frame.push_back(127);
                        for (int i = 7; i >= 0; --i) {
                            frame.push_back(static_cast<uint8_t>((len >> (i * 8)) & 0xFF));
                        }
                    }

                    frame.insert(frame.end(), data.begin(), data.end());
                    Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                    if (m_pStream.IsNull() || m_eState != WebSocketState::Open) return false;
                    m_pStream->Write(reinterpret_cast<const char*>(frame.data()), 0, static_cast<int>(frame.size()));
                    return true;
                }

                bool WebSocket::ReceiveText(String& outMessage) {
                    if (m_pStream.IsNull() || m_eState != WebSocketState::Open) return false;

                    uint8_t header[2] = { 0 };
                    if (m_pStream->Read(reinterpret_cast<char*>(header), 0, 2) <= 0) {
                        m_eState = WebSocketState::Closed;
                        return false;
                    }

                    uint8_t opcode = header[0] & 0x0F;
                    if (opcode == 0x08) { // Close frame
                        m_eState = WebSocketState::Closed;
                        return false;
                    }

                    bool masked = (header[1] & 0x80) != 0;
                    uint64_t payloadLen = header[1] & 0x7F;

                    if (payloadLen == 126) {
                        uint8_t extLen[2] = { 0 };
                        if (m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 2) <= 0) return false;
                        payloadLen = (static_cast<uint64_t>(extLen[0]) << 8) | extLen[1];
                    } else if (payloadLen == 127) {
                        uint8_t extLen[8] = { 0 };
                        if (m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 8) <= 0) return false;
                        payloadLen = 0;
                        for (int i = 0; i < 8; ++i) {
                            payloadLen = (payloadLen << 8) | extLen[i];
                        }
                    }

                    uint8_t maskKey[4] = { 0 };
                    if (masked) {
                        if (m_pStream->Read(reinterpret_cast<char*>(maskKey), 0, 4) <= 0) return false;
                    }

                    std::vector<uint8_t> payload(payloadLen, 0);
                    uint64_t totalRead = 0;
                    while (totalRead < payloadLen) {
                        int bytesRead = m_pStream->Read(reinterpret_cast<char*>(payload.data() + totalRead), 0, static_cast<int>(payloadLen - totalRead));
                        if (bytesRead <= 0) break;
                        totalRead += bytesRead;
                    }

                    if (masked) {
                        for (uint64_t i = 0; i < payloadLen; ++i) {
                            payload[i] ^= maskKey[i % 4];
                        }
                    }

                    outMessage = String(std::string(payload.begin(), payload.end()).c_str());
                    return true;
                }

                void WebSocket::Close() {
                    Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                    if (m_eState == WebSocketState::Open) {
                        uint8_t closeFrame[2] = { 0x88, 0x00 };
                        if (!m_pStream.IsNull()) {
                            m_pStream->Write(reinterpret_cast<const char*>(closeFrame), 0, 2);
                        }
                        m_eState = WebSocketState::Closed;
                    }
                }

            }
        }
    }
}
