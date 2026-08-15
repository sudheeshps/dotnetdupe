#include "pch.h"
#include "System/Net/WebSockets/WebSocket.h"
#include "System/Console.h"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

                class WebSocket::Impl : public Object {
                public:
                    SmartPointer<Sockets::NetworkStream> m_pStream;
                    WebSocketState m_eState;
                    Threading::CriticalSection m_csLock;

                    Impl(SmartPointer<Sockets::NetworkStream> pStream)
                        : m_pStream(pStream), m_eState(WebSocketState::Open) {}

                    void CloseState() {
                        Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                        m_eState = WebSocketState::Closed;
                    }

                    bool ReadFrameHeader(uint8_t& opcode, bool& masked, uint64_t& payloadLen) {
                        try {
                            uint8_t header[2] = { 0 };
                            if (m_pStream->Read(reinterpret_cast<char*>(header), 0, 2) <= 0) return false;
                            opcode = header[0] & 0x0F;
                            masked = (header[1] & 0x80) != 0;
                            payloadLen = header[1] & 0x7F;
                            return true;
                        } catch (...) {
                            return false;
                        }
                    }

                    bool ReadExtendedLength(uint64_t& payloadLen) {
                        try {
                            if (payloadLen == 126) {
                                uint8_t extLen[2] = { 0 };
                                if (m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 2) <= 0) return false;
                                payloadLen = (static_cast<uint64_t>(extLen[0]) << 8) | extLen[1];
                            } else if (payloadLen == 127) {
                                uint8_t extLen[8] = { 0 };
                                if (m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 8) <= 0) return false;
                                payloadLen = 0;
                                for (int i = 0; i < 8; ++i) payloadLen = (payloadLen << 8) | extLen[i];
                            }
                            return true;
                        } catch (...) {
                            return false;
                        }
                    }

                    bool ReadMaskKeyAndPayload(bool masked, uint64_t payloadLen, std::vector<uint8_t>& payload) {
                        try {
                            uint8_t maskKey[4] = { 0 };
                            if (masked && m_pStream->Read(reinterpret_cast<char*>(maskKey), 0, 4) <= 0) return false;
                            payload.resize(payloadLen, 0);
                            uint64_t totalRead = 0;
                            while (totalRead < payloadLen) {
                                int bytesRead = m_pStream->Read(reinterpret_cast<char*>(payload.data() + totalRead), 0, static_cast<int>(payloadLen - totalRead));
                                if (bytesRead <= 0) return false;
                                totalRead += bytesRead;
                            }
                            if (masked) {
                                for (uint64_t i = 0; i < payloadLen; ++i) payload[i] ^= maskKey[i % 4];
                            }
                            return true;
                        } catch (...) {
                            return false;
                        }
                    }
                };

                WebSocket::WebSocket(SmartPointer<Sockets::NetworkStream> pStream)
                    : m_pImpl(SmartPointer<Impl>::NewShared(pStream)) {
                }

                WebSocket::~WebSocket() = default;

                WebSocketState WebSocket::GetState() const { return m_pImpl->m_eState; }
                void WebSocket::SetState(WebSocketState state) { m_pImpl->m_eState = state; }

                String WebSocket::ComputeSecWebSocketAccept(const String& secWebSocketKey) {
                    std::string key = secWebSocketKey.GetRawString();
                    std::string magic = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

                    unsigned char hash[SHA_DIGEST_LENGTH];
                    SHA1(reinterpret_cast<const unsigned char*>(magic.c_str()), magic.length(), hash);

                    char encoded[128] = { 0 };
                    EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encoded), hash, SHA_DIGEST_LENGTH);
                    return String(encoded);
                }

                static void BuildWebSocketFrameHeader(uint8_t opcode, size_t len, std::vector<uint8_t>& frame) {
                    frame.push_back(opcode);
                    if (len <= 125) {
                        frame.push_back(static_cast<uint8_t>(len));
                    } else if (len <= 65535) {
                        frame.push_back(126);
                        frame.push_back(static_cast<uint8_t>((len >> 8) & 0xFF));
                        frame.push_back(static_cast<uint8_t>(len & 0xFF));
                    } else {
                        frame.push_back(127);
                        for (int i = 7; i >= 0; --i) frame.push_back(static_cast<uint8_t>((len >> (i * 8)) & 0xFF));
                    }
                }

                bool WebSocket::SendAsync(const String& message) {
                    std::string text = message.GetRawString();
                    std::vector<uint8_t> frame;
                    BuildWebSocketFrameHeader(0x81, text.length(), frame);
                    frame.insert(frame.end(), text.begin(), text.end());
                    Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                    if (m_pImpl->m_pStream.IsNull() || m_pImpl->m_eState != WebSocketState::Open) return false;
                    m_pImpl->m_pStream->Write(reinterpret_cast<const char*>(frame.data()), 0, static_cast<int>(frame.size()));
                    return true;
                }

                bool WebSocket::SendBytes(const Array<uint8_t>& data) {
                    std::vector<uint8_t> frame;
                    BuildWebSocketFrameHeader(0x82, data.GetLength(), frame);
                    for (int i = 0; i < data.GetLength(); ++i) frame.push_back(data[i]);
                    Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                    if (m_pImpl->m_pStream.IsNull() || m_pImpl->m_eState != WebSocketState::Open) return false;
                    m_pImpl->m_pStream->Write(reinterpret_cast<const char*>(frame.data()), 0, static_cast<int>(frame.size()));
                    return true;
                }

                bool WebSocket::ReceiveText(String& outMessage) {
                    {
                        Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                        if (m_pImpl->m_pStream.IsNull() || m_pImpl->m_eState != WebSocketState::Open) return false;
                    }
                    uint8_t opcode = 0; bool masked = false; uint64_t payloadLen = 0;
                    if (!m_pImpl->ReadFrameHeader(opcode, masked, payloadLen) || opcode == 0x08) {
                        m_pImpl->CloseState(); return false;
                    }
                    std::vector<uint8_t> payload;
                    if (!m_pImpl->ReadExtendedLength(payloadLen) || !m_pImpl->ReadMaskKeyAndPayload(masked, payloadLen, payload)) return false;
                    std::string sPayload(payload.begin(), payload.end());
                    outMessage = String(sPayload.c_str());
                    return true;
                }

                void WebSocket::Close() {
                    Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                    if (m_pImpl->m_eState == WebSocketState::Open) {
                        uint8_t closeFrame[2] = { 0x88, 0x00 };
                        if (!m_pImpl->m_pStream.IsNull()) {
                            m_pImpl->m_pStream->Write(reinterpret_cast<const char*>(closeFrame), 0, 2);
                        }
                        m_pImpl->m_eState = WebSocketState::Closed;
                    }
                }

            }
        }
    }
}
