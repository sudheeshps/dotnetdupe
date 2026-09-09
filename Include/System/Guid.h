/// \file Guid.h
/// \brief Represents a globally unique identifier (GUID) mirroring .NET System.Guid.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        /// \brief Represents a 128-bit globally unique identifier (GUID).
        ///
        /// Provides RFC 4122 version 4 generation, parsing from 32-hex or hyphenated strings,
        /// binary conversion, and comparison operations. Immutable value object.
        /// Thread-safe for all concurrent const operations.
        ///
        /// \note Conforms to RFC 4122 and ECMA-335 Partition IV Section 5.7 (System.Guid).
        /// \see String, BitConverter
        class Guid : public Object {
        public:
            /// \brief Initializes a new instance of the Guid structure initialized to all zeros.
            DOTNETDUPE_API Guid();

            /// \brief Initializes a new instance of the Guid structure using the specified array of bytes.
            /// \param b A 16-element byte array containing values with which to initialize the GUID.
            DOTNETDUPE_API Guid(const Array<uint8_t>& b);

            /// \brief Initializes a new instance of the Guid structure using the value represented by the specified string.
            /// \param g String that contains a GUID in "D", "N", or "B" format.
            DOTNETDUPE_API Guid(const String& g);

            /// \brief Initializes a new instance of the Guid structure (RFC 4122 Version 4 random UUID).
            /// \return A new Guid initialized with pseudo-random Version 4 octets.
            DOTNETDUPE_API static Guid NewGuid();

            /// \brief A read-only instance of the Guid structure whose value is all zeros.
            DOTNETDUPE_API static const Guid Empty;

            /// \brief Returns a 16-element byte array that contains the value of this instance.
            /// \return A 16-element byte array containing the GUID bytes.
            DOTNETDUPE_API Array<uint8_t> ToByteArray() const;

            /// \brief Returns a string representation of the value of this instance in standard "D" format (xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx).
            /// \return A string representation formatted according to RFC 4122.
            DOTNETDUPE_API String ToString() const;

            /// \brief Determines whether two Guid instances have identical values.
            /// \param other The Guid instance to compare with the current instance.
            /// \return True if all 16 octets match; otherwise, false.
            DOTNETDUPE_API bool operator==(const Guid& other) const;

            /// \brief Determines whether two Guid instances have differing values.
            /// \param other The Guid instance to compare with the current instance.
            /// \return True if any octet differs; otherwise, false.
            DOTNETDUPE_API bool operator!=(const Guid& other) const;

        private:
            uint8_t _data[16];
        };
    }
}
