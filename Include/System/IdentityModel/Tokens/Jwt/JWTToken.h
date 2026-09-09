/// \file JWTToken.h
/// \brief JSON Web Token (JWT) representation and cryptographic operations per RFC 7519.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        namespace IdentityModel {
            namespace Tokens {
                namespace Jwt {

                    /// \class JWTToken
                    /// \brief Represents a JSON Web Token (JWT) structure conforming to RFC 7519.
                    ///
                    /// Encapsulates token header, claims payload, and HMAC-SHA256 signature verification
                    /// for stateless web authentication and claim-based security tokens.
                    class JWTToken : public Object {
                    public:
                        /// \brief Initializes a new instance of the JWTToken class with default header values.
                        DOTNETDUPE_API JWTToken();

                        /// \brief Default virtual destructor.
                        DOTNETDUPE_API ~JWTToken() = default;

                        /// \brief Gets a mutable reference to the JOSE header dictionary.
                        /// \return Reference to header claims.
                        DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeader();

                        /// \brief Gets a read-only reference to the JOSE header dictionary.
                        /// \return Const reference to header claims.
                        DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeader() const;

                        /// \brief Gets a mutable reference to the payload claims dictionary.
                        /// \return Reference to payload claims.
                        DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetPayload();

                        /// \brief Gets a read-only reference to the payload claims dictionary.
                        /// \return Const reference to payload claims.
                        DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetPayload() const;

                        /// \brief Gets the cryptographic signature string component of the token.
                        /// \return Base64URL-encoded signature.
                        DOTNETDUPE_API String GetSignature() const;

                        /// \brief Encodes and cryptographically signs the token using HMAC-SHA256.
                        /// \param secretKey The symmetric signing secret key.
                        /// \return The complete serialized compact JWT string ("header.payload.signature").
                        DOTNETDUPE_API String CreateToken(const String& secretKey);

                        /// \brief Parses a serialized compact JWT string into a JWTToken instance.
                        /// \param tokenStr The compact JWT string to parse.
                        /// \return SmartPointer to the parsed JWTToken instance.
                        DOTNETDUPE_API static SmartPointer<JWTToken> Parse(const String& tokenStr);

                        /// \brief Verifies the token signature against a specified secret key.
                        /// \param secretKey The symmetric key used for HMAC-SHA256 verification.
                        /// \return True if the signature is valid; otherwise false.
                        DOTNETDUPE_API bool Verify(const String& secretKey) const;

                    private:
                        Collections::Generic::Dictionary<String, String> m_header;
                        Collections::Generic::Dictionary<String, String> m_payload;
                        String m_signature;
                        String m_rawTokenWithoutSignature;
                    };

                }
            }
        }
    }
}
