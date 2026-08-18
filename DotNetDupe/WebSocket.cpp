#include "pch.h"
#include "System/Net/WebSockets/WebSocket.h"
#include "System/Console.h"
#include "System/Convert.h"
#include "System/Exception.h"
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
                        Console::WriteLine("[WebSocket] Connection state closed.");
                    }

                    bool ReadExtended16(uint64_t& payloadLen) {
                        uint8_t extLen[2] = { 0 };
                        if (m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 2) <= 0) return false;
                        payloadLen = (static_cast<uint64_t>(extLen[0]) << 8) | extLen[1];
                        return true;
                    }

                    bool ReadExtended64(uint64_t& payloadLen) {
                        uint8_t extLen[8] = { 0 };
                        if (m_pStream->Read(reinterpret_cast<char*>(extLen), 0, 8) <= 0) return false;
                        payloadLen = 0;
                        for (int i = 0; i < 8; ++i) payloadLen = (payloadLen << 8) | extLen[i];
                        return true;
                    }

                    bool ReadFrameHeader(uint8_t& opcode, bool& masked, uint64_t& payloadLen) {
                        try {
                            uint8_t header[2] = { 0 };
                            if (m_pStream->Read(reinterpret_cast<char*>(header), 0, 2) <= 0) return false;
                            opcode = header[0] & 0x0F;
                            masked = (header[1] & 0x80) != 0;
                            payloadLen = header[1] & 0x7F;
                            return true;
                        } catch (const Exception& ex) {
                            Console::WriteLine(String("[WebSocket] Error reading frame header: ") + ex.What());
                            return false;
                        } catch (...) {
                            Console::WriteLine("[WebSocket] Unknown error reading frame header.");
                            return false;
                        }
                    }

                    bool ReadExtendedLength(uint64_t& payloadLen) {
                        try {
                            if (payloadLen == 126) return ReadExtended16(payloadLen);
                            if (payloadLen == 127) return ReadExtended64(payloadLen);
                            return true;
                        } catch (const Exception& ex) {
                            Console::WriteLine(String("[WebSocket] Error reading extended length: ") + ex.What());
                            return false;
                        } catch (...) {
                            Console::WriteLine("[WebSocket] Unknown error reading extended length.");
                            return false;
                        }
                    }

                    bool ReadPayloadBytes(uint64_t payloadLen, std::vector<uint8_t>& payload) {
                        payload.resize(payloadLen, 0);
                        uint64_t totalRead = 0;
                        while (totalRead < payloadLen) {
                            int bytesRead = m_pStream->Read(reinterpret_cast<char*>(payload.data() + totalRead), 0, static_cast<int>(payloadLen - totalRead));
                            if (bytesRead <= 0) return false;
                            totalRead += bytesRead;
                        }
                        return true;
                    }

                    void UnmaskPayload(const uint8_t maskKey[4], uint64_t payloadLen, std::vector<uint8_t>& payload) {
                        for (uint64_t i = 0; i < payloadLen; ++i) {
                            payload[i] ^= maskKey[i % 4];
                        }
                    }

                    bool ReadMaskKeyAndPayload(bool masked, uint64_t payloadLen, std::vector<uint8_t>& payload) {
                        try {
                            uint8_t maskKey[4] = { 0 };
                            if (masked && m_pStream->Read(reinterpret_cast<char*>(maskKey), 0, 4) <= 0) return false;
                            if (!ReadPayloadBytes(payloadLen, payload)) return false;
                            if (masked) UnmaskPayload(maskKey, payloadLen, payload);
                            return true;
                        } catch (const Exception& ex) {
                            Console::WriteLine(String("[WebSocket] Error reading payload: ") + ex.What());
                            return false;
                        } catch (...) {
                            Console::WriteLine("[WebSocket] Unknown error reading payload.");
                            return false;
                        }
                    }

                    bool WriteFrame(uint8_t opcode, const uint8_t* pData, size_t len) {
                        try {
                            Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                            if (m_pStream.IsNull() || m_eState != WebSocketState::Open) return false;
                            std::vector<uint8_t> frame;
                            BuildWebSocketFrameHeader(opcode, len, frame);
                            if (pData != nullptr && len > 0) frame.insert(frame.end(), pData, pData + len);
                            m_pStream->Write(reinterpret_cast<const char*>(frame.data()), 0, static_cast<int>(frame.size()));
                            return true;
                        } catch (const Exception& ex) {
                            Console::WriteLine(String("[WebSocket] Error writing frame: ") + ex.What());
                            return false;
                        } catch (...) {
                            Console::WriteLine("[WebSocket] Unknown error writing frame.");
                            return false;
                        }
                    }
                };

                WebSocket::WebSocket(SmartPointer<Sockets::NetworkStream> pStream)
                    : m_pImpl(SmartPointer<Impl>::NewShared(pStream)) {
                    Console::WriteLine("[WebSocket] WebSocket instance initialized.");
                }

                WebSocket::~WebSocket() {
                    Console::WriteLine("[WebSocket] WebSocket instance destroyed.");
                }

                WebSocketState WebSocket::GetState() const { return m_pImpl->m_eState; }
                void WebSocket::SetState(WebSocketState state) {
                    m_pImpl->m_eState = state;
                    Console::WriteLine("[WebSocket] WebSocket state updated.");
                }

                String WebSocket::ComputeSecWebSocketAccept(const String& secWebSocketKey) {
                    try {
                        std::string key = secWebSocketKey.GetRawString();
                        std::string magic = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
                        unsigned char hash[SHA_DIGEST_LENGTH];
                        SHA1(reinterpret_cast<const unsigned char*>(magic.c_str()), magic.length(), hash);
                        char encoded[128] = { 0 };
                        EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encoded), hash, SHA_DIGEST_LENGTH);
                        return String(encoded);
                    } catch (const Exception& ex) {
                        Console::WriteLine(String("[WebSocket] Exception computing accept key: ") + ex.What());
                        return "";
                    } catch (...) {
                        Console::WriteLine("[WebSocket] Unknown exception computing accept key.");
                        return "";
                    }
                }

                bool WebSocket::SendAsync(const String& message) {
                    std::string text = message.GetRawString();
                    bool bSuccess = m_pImpl->WriteFrame(0x81, reinterpret_cast<const uint8_t*>(text.data()), text.length());
                    if (bSuccess) {
                        Console::WriteLine(String("[WebSocket] Sent text message (") + Convert::ToString(static_cast<long long>(text.length())) + " bytes).");
                    } else {
                        Console::WriteLine("[WebSocket] Failed to send text message.");
                    }
                    return bSuccess;
                }

                bool WebSocket::SendBytes(const Array<uint8_t>& data) {
                    bool bSuccess = m_pImpl->WriteFrame(0x82, data.GetData(), data.GetLength());
                    if (bSuccess) {
                        Console::WriteLine(String("[WebSocket] Sent binary frame (") + Convert::ToString(data.GetLength()) + " bytes).");
                    } else {
                        Console::WriteLine("[WebSocket] Failed to send binary frame.");
                    }
                    return bSuccess;
                }

                bool WebSocket::ReceiveText(String& outMessage) {
                    {
                        Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                        if (m_pImpl->m_pStream.IsNull() || m_pImpl->m_eState != WebSocketState::Open) return false;
                    }
                    uint8_t opcode = 0; bool masked = false; uint64_t payloadLen = 0;
                    if (!m_pImpl->ReadFrameHeader(opcode, masked, payloadLen) || opcode == 0x08) {
                        m_pImpl->CloseState();
                        return false;
                    }
                    std::vector<uint8_t> payload;
                    if (!m_pImpl->ReadExtendedLength(payloadLen) || !m_pImpl->ReadMaskKeyAndPayload(masked, payloadLen, payload)) return false;
                    std::string sPayload(payload.begin(), payload.end());
                    outMessage = String(sPayload.c_str());
                    Console::WriteLine(String("[WebSocket] Received text message: ") + outMessage);
                    return true;
                }

                void WebSocket::Close() {
                    try {
                        Threading::Lock<Threading::CriticalSection> lock(m_pImpl->m_csLock);
                        if (m_pImpl->m_eState == WebSocketState::Open) {
                            uint8_t closeFrame[2] = { 0x88, 0x00 };
                            if (!m_pImpl->m_pStream.IsNull()) {
                                m_pImpl->m_pStream->Write(reinterpret_cast<const char*>(closeFrame), 0, 2);
                            }
                            m_pImpl->m_eState = WebSocketState::Closed;
                            Console::WriteLine("[WebSocket] Sent close frame and closed connection.");
                        }
                    } catch (const Exception& ex) {
                        Console::WriteLine(String("[WebSocket] Exception closing connection: ") + ex.What());
                    } catch (...) {
                        Console::WriteLine("[WebSocket] Unknown exception closing connection.");
                    }
                }

            }
        }
    }
}

