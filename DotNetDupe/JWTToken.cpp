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
                        /// Step: Convert standard Base64 characters (+, /) to URL-safe characters (-, _) and strip padding.
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
                        /// Step: Convert URL-safe Base64 characters back to standard Base64 with '=' padding.
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
                        /// Step: Initialize default JOSE header claims (HS256, JWT).
                        m_header.Add("alg", "HS256");
                        m_header.Add("typ", "JWT");
                    }

                    Collections::Generic::Dictionary<String, String>& JWTToken::GetHeader() {
                        /// Return: Mutable reference to header claims dictionary.
                        return m_header;
                    }

                    const Collections::Generic::Dictionary<String, String>& JWTToken::GetHeader() const {
                        /// Return: Const reference to header claims dictionary.
                        return m_header;
                    }

                    Collections::Generic::Dictionary<String, String>& JWTToken::GetPayload() {
                        /// Return: Mutable reference to payload claims dictionary.
                        return m_payload;
                    }

                    const Collections::Generic::Dictionary<String, String>& JWTToken::GetPayload() const {
                        /// Return: Const reference to payload claims dictionary.
                        return m_payload;
                    }

                    String JWTToken::GetSignature() const {
                        /// Return: Token cryptographic signature.
                        return m_signature;
                    }

                    static String EncodeDictToBase64Url(const Collections::Generic::Dictionary<String, String>& dict) {
                        /// Step: Serialize dictionary to JSON and encode as Base64URL string.
                        String json = Text::Json::JsonSerializer::Serialize(dict);
                        Array<char> bytes(json.GetLength());
                        std::memcpy(bytes.GetData(), json.GetRawString(), json.GetLength());
                        return Base64ToBase64Url(Convert::ToBase64String(bytes));
                    }

                    static String ComputeJwtSignature(const String& rawToken, const String& secretKey) {
                        /// Step: Compute HMAC-SHA256 signature and return Base64URL encoded hash.
                        Array<char> tokenBytes(rawToken.GetLength());
                        std::memcpy(tokenBytes.GetData(), rawToken.GetRawString(), rawToken.GetLength());
                        Array<char> keyBytes(secretKey.GetLength());
                        std::memcpy(keyBytes.GetData(), secretKey.GetRawString(), secretKey.GetLength());
                        Array<char> sigHash = Security::Cryptography::HMACSHA256::ComputeHash(tokenBytes, keyBytes);
                        return Base64ToBase64Url(Convert::ToBase64String(sigHash));
                    }

                    String JWTToken::CreateToken(const String& secretKey) {
                        /// Step: Encode header, payload, and compute cryptographic signature.
                        String headerB64Url = EncodeDictToBase64Url(m_header);
                        String payloadB64Url = EncodeDictToBase64Url(m_payload);
                        m_rawTokenWithoutSignature = headerB64Url + "." + payloadB64Url;
                        m_signature = ComputeJwtSignature(m_rawTokenWithoutSignature, secretKey);
                        return m_rawTokenWithoutSignature + "." + m_signature;
                    }

                    static Collections::Generic::Dictionary<String, String> DecodeBase64UrlToDict(const String& b64url) {
                        /// Step: Decode Base64URL segment and deserialize JSON dictionary.
                        String b64 = Base64UrlToBase64(b64url);
                        Array<char> bytes = Convert::FromBase64String(b64);
                        std::string jsonStr(bytes.GetData(), bytes.GetLength());
                        return Text::Json::JsonSerializer::Deserialize<Collections::Generic::Dictionary<String, String>>(String(jsonStr.c_str()));
                    }

                    SmartPointer<JWTToken> JWTToken::Parse(const String& tokenStr) {
                        /// Guard: Validate token segment structure.
                        Array<String> parts = tokenStr.Split('.');
                        if (parts.GetLength() != 3) throw ArgumentException("Invalid JWT token format.");

                        /// Step: Deserialize header, payload, and store signature.
                        auto pToken = SmartPointer<JWTToken>::NewShared();
                        pToken->m_header = DecodeBase64UrlToDict(parts[0]);
                        pToken->m_payload = DecodeBase64UrlToDict(parts[1]);
                        pToken->m_rawTokenWithoutSignature = parts[0] + "." + parts[1];
                        pToken->m_signature = parts[2];
                        return pToken;
                    }

                    bool JWTToken::Verify(const String& secretKey) const {
                        using namespace DotNetDupe::System::Security::Cryptography;

                        /// Guard: Ensure token contains valid unsigned payload and signature.
                        if (m_rawTokenWithoutSignature.IsEmpty() || m_signature.IsEmpty()) {
                            return false;
                        }

                        /// Step: Compute expected signature using secret key.
                        Array<char> tokenBytes(m_rawTokenWithoutSignature.GetLength());
                        std::memcpy(tokenBytes.GetData(), m_rawTokenWithoutSignature.GetRawString(), m_rawTokenWithoutSignature.GetLength());

                        Array<char> keyBytes(secretKey.GetLength());
                        std::memcpy(keyBytes.GetData(), secretKey.GetRawString(), secretKey.GetLength());

                        Array<char> signatureHash = HMACSHA256::ComputeHash(tokenBytes, keyBytes);
                        String signatureB64 = Convert::ToBase64String(signatureHash);
                        String expectedSignature = Base64ToBase64Url(signatureB64);

                        /// Return: Verification comparison result.
                        return m_signature == expectedSignature;
                    }

                }
            }
        }
    }
}
