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
            using EncodingPtr = SmartPointer<Encoding>;

            // Abstract base class for encodings
            class Encoding : public Object {
            public:
                virtual ~Encoding() = default;

                // Converts a string to a byte array
                virtual Array<char> GetBytes(const String& s) = 0;

                // Converts a byte array to a string
                virtual String GetString(const Array<char>& bytes) = 0;
                virtual String GetString(const char* bytes, int byteCount) = 0;
            };

            class UTF8Encoding;
            using UTF8EncodingPtr = SmartPointer<UTF8Encoding>;

            // UTF8 Encoding implementation
            class UTF8Encoding : public Encoding {
            public:
                Array<char> GetBytes(const String& s) override;
                String GetString(const Array<char>& bytes) override;
                String GetString(const char* bytes, int byteCount) override;
            };

            // Main TextEncoding class with static factory methods
            class TextEncoding : public Object {
            public:
                DOTNETDUPE_API static EncodingPtr UTF8();

            private:
                static UTF8EncodingPtr s_utf8EncodingInstance;
            };
        }
    }
}
