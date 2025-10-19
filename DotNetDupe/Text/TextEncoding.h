#pragma once

#include "../Common.h"
#include "../Object.h"
#include "../String.h"
#include <vector>
#include <memory>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            // Abstract base class for encodings
            class Encoding : public Object {
            public:
                virtual ~Encoding() = default;

                // Converts a string to a byte array
                virtual std::vector<char> GetBytes(const String& s) = 0;

                // Converts a byte array to a string
                virtual String GetString(const std::vector<char>& bytes) = 0;
                virtual String GetString(const char* bytes, int byteCount) = 0;
            };

            // UTF8 Encoding implementation
            class UTF8Encoding : public Encoding {
            public:
                std::vector<char> GetBytes(const String& s) override;
                String GetString(const std::vector<char>& bytes) override;
                String GetString(const char* bytes, int byteCount) override;
            };

            // Main TextEncoding class with static factory methods
            class TextEncoding : public Object {
            public:
                DOTNETDUPE_API static std::shared_ptr<Encoding> UTF8();

            private:
                static std::shared_ptr<UTF8Encoding> _utf8EncodingInstance;
            };
        }
    }
}
