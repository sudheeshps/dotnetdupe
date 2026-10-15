/// \file SHA256.h
/// \brief Computes the SHA256 hash for the input data using FIPS 180-2 standard.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/IO/Stream.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Cryptography {

                /// \class SHA256
                /// \brief Computes the SHA256 hash for input data conforming to FIPS 180-2.
                class SHA256 : public Object {
                public:
                    /// \brief Initializes a new instance of the SHA256 class.
                    DOTNETDUPE_API SHA256();

                    /// \brief Destructor for SHA256.
                    DOTNETDUPE_API virtual ~SHA256() = default;

                    /// \brief Computes the hash value for the specified byte array.
                    /// \param buffer The input to compute the hash code for.
                    /// \return The computed 32-byte SHA-256 hash code.
                    DOTNETDUPE_API Array<byte> ComputeHash(const Array<byte>& buffer);

                    /// \brief Computes the hash value for the specified Stream object.
                    /// \param pStream Smart pointer to the input stream to compute the hash code for.
                    /// \return The computed 32-byte SHA-256 hash code.
                    /// \throws ArgumentNullException Thrown when pStream is null.
                    DOTNETDUPE_API Array<byte> ComputeHash(const SmartPointer<IO::Stream>& pStream);

                    /// \brief Creates an instance of the default implementation of SHA256.
                    DOTNETDUPE_API static SmartPointer<SHA256> Create();

                    /// \brief Computes the hash value for the specified byte array using a static helper.
                    DOTNETDUPE_API static Array<byte> HashData(const Array<byte>& buffer);

                    /// \brief Computes the hash value for the specified stream using a static helper.
                    /// \param pStream Smart pointer to the input stream to hash.
                    /// \return The computed 32-byte SHA-256 hash code.
                    /// \throws ArgumentNullException Thrown when pStream is null.
                    DOTNETDUPE_API static Array<byte> HashData(const SmartPointer<IO::Stream>& pStream);

                    /// \brief Formats a byte array into a lowercase hex string.
                    DOTNETDUPE_API static String ToHexString(const Array<byte>& arrHash);
                };

            }
        }
    }
}
