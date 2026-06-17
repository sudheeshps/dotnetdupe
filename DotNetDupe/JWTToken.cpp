#include "pch.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
#include "System/Security/Cryptography/HMACSHA256.h"
#include "System/Convert.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/ArgumentException.h"
#include <cstring>
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace IdentityModel {
            namespace Tokens {
                namespace Jwt {

                    static String Base64ToBase64Url(const String& base64) {
                        std::string s = base64.GetRawString();
                        for (char& c : s) {
                            if (c == '+') c = '-';
                            else if (c == '/') c = '_';
                        }
                        while (!s.empty() && s.back() == '=') {
                            s.pop_back();
                        }
                        return String(s.c_str());
                    }

                    static String Base64UrlToBase64(const String& base64Url) {
                        std::string s = base64Url.GetRawString();
                        for (char& c : s) {
                            if (c == '-') c = '+';
                            else if (c == '_') c = '/';
                        }
                        while (s.length() % 4 != 0) {
                            s += '=';
                        }
                        return String(s.c_str());
                    }

                    JWTToken::JWTToken() {
                        m_header.Add("alg", "HS256");
                        m_header.Add("typ", "JWT");
                    }

                    Collections::Generic::Dictionary<String, String>& JWTToken::GetHeader() {
                        return m_header;
                    }

                    const Collections::Generic::Dictionary<String, String>& JWTToken::GetHeader() const {
                        return m_header;
                    }

                    Collections::Generic::Dictionary<String, String>& JWTToken::GetPayload() {
                        return m_payload;
                    }

                    const Collections::Generic::Dictionary<String, String>& JWTToken::GetPayload() const {
                        return m_payload;
                    }

                    String JWTToken::GetSignature() const {
                        return m_signature;
                    }

                    String JWTToken::CreateToken(const String& secretKey) {
                        using namespace DotNetDupe::System::Text::Json;
                        using namespace DotNetDupe::System::Security::Cryptography;

                        String headerJson = JsonSerializer::Serialize(m_header);
                        Array<char> headerBytes(headerJson.GetLength());
                        std::memcpy(headerBytes.GetData(), headerJson.GetRawString(), headerJson.GetLength());
                        String headerB64 = Convert::ToBase64String(headerBytes);
                        String headerB64Url = Base64ToBase64Url(headerB64);

                        String payloadJson = JsonSerializer::Serialize(m_payload);
                        Array<char> payloadBytes(payloadJson.GetLength());
                        std::memcpy(payloadBytes.GetData(), payloadJson.GetRawString(), payloadJson.GetLength());
                        String payloadB64 = Convert::ToBase64String(payloadBytes);
                        String payloadB64Url = Base64ToBase64Url(payloadB64);

                        m_rawTokenWithoutSignature = headerB64Url + "." + payloadB64Url;

                        Array<char> tokenBytes(m_rawTokenWithoutSignature.GetLength());
                        std::memcpy(tokenBytes.GetData(), m_rawTokenWithoutSignature.GetRawString(), m_rawTokenWithoutSignature.GetLength());

                        Array<char> keyBytes(secretKey.GetLength());
                        std::memcpy(keyBytes.GetData(), secretKey.GetRawString(), secretKey.GetLength());

                        Array<char> signatureHash = HMACSHA256::ComputeHash(tokenBytes, keyBytes);
                        String signatureB64 = Convert::ToBase64String(signatureHash);
                        m_signature = Base64ToBase64Url(signatureB64);

                        return m_rawTokenWithoutSignature + "." + m_signature;
                    }

                    SmartPointer<JWTToken> JWTToken::Parse(const String& tokenStr) {
                        using namespace DotNetDupe::System::Text::Json;

                        Array<String> parts = tokenStr.Split('.');
                        if (parts.GetLength() != 3) {
                            throw ArgumentException("Invalid JWT token format.");
                        }

                        auto token = SmartPointer<JWTToken>::NewShared();

                        String headerB64 = Base64UrlToBase64(parts[0]);
                        Array<char> headerBytes = Convert::FromBase64String(headerB64);
                        String headerJson;
                        headerJson.GetString().assign(headerBytes.GetData(), headerBytes.GetLength());
                        token->m_header = JsonSerializer::Deserialize<Collections::Generic::Dictionary<String, String>>(headerJson);

                        String payloadB64 = Base64UrlToBase64(parts[1]);
                        Array<char> payloadBytes = Convert::FromBase64String(payloadB64);
                        String payloadJson;
                        payloadJson.GetString().assign(payloadBytes.GetData(), payloadBytes.GetLength());
                        token->m_payload = JsonSerializer::Deserialize<Collections::Generic::Dictionary<String, String>>(payloadJson);

                        token->m_rawTokenWithoutSignature = parts[0] + "." + parts[1];
                        token->m_signature = parts[2];

                        return token;
                    }

                    bool JWTToken::Verify(const String& secretKey) const {
                        using namespace DotNetDupe::System::Security::Cryptography;

                        if (m_rawTokenWithoutSignature.IsEmpty() || m_signature.IsEmpty()) {
                            return false;
                        }

                        Array<char> tokenBytes(m_rawTokenWithoutSignature.GetLength());
                        std::memcpy(tokenBytes.GetData(), m_rawTokenWithoutSignature.GetRawString(), m_rawTokenWithoutSignature.GetLength());

                        Array<char> keyBytes(secretKey.GetLength());
                        std::memcpy(keyBytes.GetData(), secretKey.GetRawString(), secretKey.GetLength());

                        Array<char> signatureHash = HMACSHA256::ComputeHash(tokenBytes, keyBytes);
                        String signatureB64 = Convert::ToBase64String(signatureHash);
                        String expectedSignature = Base64ToBase64Url(signatureB64);

                        return m_signature == expectedSignature;
                    }

                }
            }
        }
    }
}
