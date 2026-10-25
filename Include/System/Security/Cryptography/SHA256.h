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

                    /// \brief Computes the SHA-256 hash over a bounded byte range within a seekable stream.
                    /// \param pStream Smart pointer to the seekable input stream.
                    /// \param llOffset Zero-based 64-bit byte offset from which to start reading.
                    /// \param llCount Maximum number of bytes to hash from the stream.
                    /// \return The computed 32-byte SHA-256 hash code.
                    DOTNETDUPE_API static Array<byte> HashData(const SmartPointer<IO::Stream>& pStream, long long llOffset, long long llCount);

                    /// \brief Computes a fast Stage-2 duplicate elimination fingerprint hashing the head, tail, and 64-bit length of a file.
                    /// \param sFilePath Path to the file to fingerprint.
                    /// \param iEdgeBytes Number of bytes to sample at the beginning and end of the file (default 4096).
                    /// \return Lowercase 64-character hexadecimal SHA-256 edge fingerprint.
                    DOTNETDUPE_API static String ComputeFastEdgeHash(const String& sFilePath, int iEdgeBytes = 4096);

                    /// \brief Computes the full streaming SHA-256 hash of a file on disk and formats it as a lowercase hex string.
                    /// \param sFilePath Path to the file to hash.
                    /// \return Lowercase 64-character hexadecimal SHA-256 digest.
                    DOTNETDUPE_API static String HashFileToHex(const String& sFilePath);

                    /// \brief Formats a byte array into a lowercase hex string.
                    DOTNETDUPE_API static String ToHexString(const Array<byte>& arrHash);
                };

            }
        }
    }
}
