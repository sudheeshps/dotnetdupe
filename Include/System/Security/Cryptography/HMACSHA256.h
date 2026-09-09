/// \file HMACSHA256.h
/// \brief Hash-based Message Authentication Code using SHA-256 per RFC 2104.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Cryptography {

                /// \class HMACSHA256
                /// \brief Computes a Hash-based Message Authentication Code (HMAC) using the SHA-256 hash function per RFC 2104.
                ///
                /// Mirrors .NET System.Security.Cryptography.HMACSHA256, providing symmetric key hashing
                /// for message integrity verification and token signing.
                class HMACSHA256 : public Object {
                public:
                    /// \brief Initializes a new instance of the HMACSHA256 class with a randomly generated key.
                    DOTNETDUPE_API HMACSHA256();

                    /// \brief Initializes a new instance of the HMACSHA256 class with the specified key data.
                    /// \param key The secret key for HMACSHA256 encryption.
                    DOTNETDUPE_API explicit HMACSHA256(const Array<char>& key);

                    /// \brief Default virtual destructor.
                    DOTNETDUPE_API ~HMACSHA256() = default;

                    /// \brief Computes the HMAC hash value for the specified byte array.
                    /// \param buffer The input byte array to compute the hash code for.
                    /// \return The computed HMAC-SHA256 hash code as a byte array.
                    DOTNETDUPE_API Array<char> ComputeHash(const Array<char>& buffer);

                    /// \brief Computes the HMAC hash value for the specified buffer and key.
                    /// \param buffer The input byte array.
                    /// \param key The secret key byte array.
                    /// \return The computed HMAC-SHA256 hash code.
                    DOTNETDUPE_API static Array<char> ComputeHash(const Array<char>& buffer, const Array<char>& key);

                private:
                    Array<char> m_key;
                };

            }
        }
    }
}
