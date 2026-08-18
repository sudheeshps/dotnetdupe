#include "pch.h"
#include "System/Net/WebSockets/WebSocket.h"
#include "System/Net/WebSockets/WebSocketException.h"
#include "System/ArgumentException.h"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace WebSockets {

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

                    bool ReadExtended16(uint64_t& payloadLen) {
                        uint8_t extLen[2] = { 0 };
                        int bytesRead = m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 2);
                        if (bytesRead <= 0) {
                            throw WebSocketException(WebSocketError::ConnectionClosedPrematurely, "Unexpected EOF reading 16-bit extended length.");
                        }
                        payloadLen = (static_cast<uint64_t>(extLen[0]) << 8) | extLen[1];
                        return true;
                    }

                    bool ReadExtended64(uint64_t& payloadLen) {
                        uint8_t extLen[8] = { 0 };
                        int bytesRead = m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 8);
                        if (bytesRead <= 0) {
                            throw WebSocketException(WebSocketError::ConnectionClosedPrematurely, "Unexpected EOF reading 64-bit extended length.");
                        }
                        payloadLen = 0;
                        for (int i = 0; i < 8; ++i) payloadLen = (payloadLen << 8) | extLen[i];
                        return true;
                    }

                    bool ReadFrameHeader(uint8_t& opcode, bool& masked, uint64_t& payloadLen) {
                        uint8_t header[2] = { 0 };
                        int bytesRead = 0;
                        try {
                            bytesRead = m_pStream->Read(reinterpret_cast<char*>(header), 0, 2);
                        } catch (const Exception& ex) {
                            throw WebSocketException(WebSocketError::NativeError, ex);
                        }
                        if (bytesRead <= 0) return false;
                        opcode = header[0] & 0x0F;
                        masked = (header[1] & 0x80) != 0;
                        payloadLen = header[1] & 0x7F;
                        return true;
                    }

                    bool ReadExtendedLength(uint64_t& payloadLen) {
                        if (payloadLen == 126) return ReadExtended16(payloadLen);
                        if (payloadLen == 127) return ReadExtended64(payloadLen);
                        return true;
                    }

                    void ReadPayloadBytes(uint64_t payloadLen, std::vector<uint8_t>& payload) {
                        payload.resize(payloadLen, 0);
                        uint64_t totalRead = 0;
                        while (totalRead < payloadLen) {
                            int toRead = static_cast<int>(payloadLen - totalRead);
                            int bytesRead = m_pStream->Read(reinterpret_cast<char*>(payload.data() + totalRead), 0, toRead);
                            if (bytesRead <= 0) {
                                throw WebSocketException(WebSocketError::ConnectionClosedPrematurely, "Connection lost while reading payload bytes.");
                            }
                            totalRead += bytesRead;
                        }
                    }

                    void UnmaskPayload(const uint8_t maskKey[4], uint64_t payloadLen, std::vector<uint8_t>& payload) {
                        for (uint64_t i = 0; i < payloadLen; ++i) {
                            payload[i] ^= maskKey[i % 4];
                        }
                    }

                    bool ReadMaskKeyAndPayload(bool masked, uint64_t payloadLen, std::vector<uint8_t>& payload) {
                        uint8_t maskKey[4] = { 0 };
                        if (masked && m_pStream->Read(reinterpret_cast<char*>(maskKey), 0, 4) <= 0) {
                            throw WebSocketException(WebSocketError::ConnectionClosedPrematurely, "Connection lost while reading mask key.");
                        }
                        ReadPayloadBytes(payloadLen, payload);
                        if (masked) UnmaskPayload(maskKey, payloadLen, payload);
                        return true;
                    }

                    bool WriteFrame(uint8_t opcode, const uint8_t* pData, size_t len) {
                        Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                        if (m_pStream.IsNull() || m_eState != WebSocketState::Open) {
                            throw WebSocketException(WebSocketError::InvalidState, "WebSocket is not connected or already closed.");
                        }
                        std::vector<uint8_t> frame;
                        BuildWebSocketFrameHeader(opcode, len, frame);
                        if (pData != nullptr && len > 0) frame.insert(frame.end(), pData, pData + len);
                        try {
                            m_pStream->Write(reinterpret_cast<const char*>(frame.data()), 0, static_cast<int>(frame.size()));
                        } catch (const Exception& ex) {
                            throw WebSocketException(WebSocketError::NativeError, ex);
                        }
                        return true;
                    }
                };

                WebSocket::WebSocket(SmartPointer<Sockets::NetworkStream> pStream)
                    : m_pImpl(SmartPointer<Impl>::NewShared(pStream)) {
                }

                WebSocket::~WebSocket() {
                }

                WebSocketState WebSocket::GetState() const { return m_pImpl->m_eState; }
                void WebSocket::SetState(WebSocketState state) {
                    m_pImpl->m_eState = state;
                }

                String WebSocket::ComputeSecWebSocketAccept(const String& secWebSocketKey) {
                    if (secWebSocketKey.IsEmpty()) {
                        throw ArgumentException("secWebSocketKey cannot be empty.");
                    }
                    std::string key = secWebSocketKey.GetRawString();
                    std::string magic = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
                    unsigned char hash[SHA_DIGEST_LENGTH];
                    SHA1(reinterpret_cast<const unsigned char*>(magic.c_str()), magic.length(), hash);
                    char encoded[128] = { 0 };
                    EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encoded), hash, SHA_DIGEST_LENGTH);
                    return String(encoded);
                }

                bool WebSocket::SendAsync(const String& message) {
                    std::string text = message.GetRawString();
                    return m_pImpl->WriteFrame(0x81, reinterpret_cast<const uint8_t*>(text.data()), text.length());
                }

                bool WebSocket::SendBytes(const Array<uint8_t>& data) {
                    return m_pImpl->WriteFrame(0x82, data.GetData(), data.GetLength());
                }

                bool WebSocket::ReceiveText(String& outMessage) {
                    {
                        Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                        if (m_pImpl->m_pStream.IsNull() || m_pImpl->m_eState != WebSocketState::Open) {
                            throw WebSocketException(WebSocketError::InvalidState, "WebSocket is not connected or already closed.");
                        }
                    }
                    uint8_t opcode = 0; bool masked = false; uint64_t payloadLen = 0;
                    if (!m_pImpl->ReadFrameHeader(opcode, masked, payloadLen) || opcode == 0x08) {
                        m_pImpl->CloseState();
                        return false;
                    }
                    std::vector<uint8_t> payload;
                    m_pImpl->ReadExtendedLength(payloadLen);
                    m_pImpl->ReadMaskKeyAndPayload(masked, payloadLen, payload);
                    outMessage = String(std::string(payload.begin(), payload.end()).c_str());
                    return true;
                }

                void WebSocket::Close() {
                    Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                    if (m_pImpl->m_eState == WebSocketState::Open) {
                        uint8_t closeFrame[2] = { 0x88, 0x00 };
                        if (!m_pImpl->m_pStream.IsNull()) {
                            try {
                                m_pImpl->m_pStream->Write(reinterpret_cast<const char*>(closeFrame), 0, 2);
                            } catch (...) {
                                // Suppress stream write errors during close handshake
                                (void)0;
                            }
                        }
                        m_pImpl->m_eState = WebSocketState::Closed;
                    }
                }

            }
        }
    }
}
