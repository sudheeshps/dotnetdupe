/// \file TextEncoding.h
/// \brief Represents character encodings and code page conversions.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Text {
            class Encoding;

            /// \brief SmartPointer alias for Encoding instances.
            using EncodingPtr = SmartPointer<Encoding>;

            /// \class Encoding
            /// \brief Represents a character encoding.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.46 (System.Text.Encoding).
            class Encoding : public Object {
            public:
                /// \brief Virtual destructor.
                virtual ~Encoding() = default;

                /// \brief Encodes a set of characters from the specified String into a sequence of bytes.
                /// \param s The String containing the characters to encode.
                /// \return An Array containing the results of encoding the specified set of characters.
                virtual Array<char> GetBytes(const String& s) = 0;

                /// \brief Decodes a sequence of bytes from the specified byte array into a String.
                /// \param bytes An Array containing the sequence of bytes to decode.
                /// \return A String that contains the results of decoding the specified sequence of bytes.
                virtual String GetString(const Array<char>& bytes) = 0;

                /// \brief Decodes a sequence of bytes from the specified buffer into a String.
                /// \param bytes Pointer to the byte array to decode.
                /// \param byteCount The number of bytes to decode.
                /// \return A String that contains the results of decoding the specified sequence of bytes.
                virtual String GetString(const char* bytes, int byteCount) = 0;
            };

            class UTF8Encoding;

            /// \brief SmartPointer alias for UTF8Encoding instances.
            using UTF8EncodingPtr = SmartPointer<UTF8Encoding>;

            /// \class UTF8Encoding
            /// \brief Represents a UTF-8 encoding of Unicode characters.
            ///
            /// \note Conforms to RFC 3629 and ECMA-335 Partition IV Section 5.47 (System.Text.UTF8Encoding).
            class UTF8Encoding : public Encoding {
            public:
                /// \brief Encodes a set of characters into a sequence of UTF-8 bytes.
                Array<char> GetBytes(const String& s) override;

                /// \brief Decodes a sequence of UTF-8 bytes into a String.
                String GetString(const Array<char>& bytes) override;

                /// \brief Decodes a sequence of UTF-8 bytes from raw buffer into a String.
                String GetString(const char* bytes, int byteCount) override;
            };

            /// \class TextEncoding
            /// \brief Provides static access to standard character encodings.
            class TextEncoding : public Object {
            public:
                /// \brief Gets an encoding for the UTF-8 format.
                /// \return An encoding for the UTF-8 format.
                DOTNETDUPE_API static EncodingPtr UTF8();

            private:
                static UTF8EncodingPtr s_utf8EncodingInstance;
            };
        }
    }
}
