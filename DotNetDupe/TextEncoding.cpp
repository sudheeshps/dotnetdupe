#include "pch.h"
#include "System/Text/TextEncoding.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            // Static member initialization
            std::shared_ptr<UTF8Encoding> TextEncoding::s_utf8EncodingInstance = nullptr;

            Array<char> UTF8Encoding::GetBytes(const String& s) {
                const char* raw = (const char*)s;
                if (!raw) return Array<char>(0);
                int length = s.GetLength();
                Array<char> bytes(length);
                for (int i = 0; i < length; i++) bytes[i] = raw[i];
                return bytes;
            }

            String UTF8Encoding::GetString(const Array<char>& bytes) {
                if (bytes.GetLength() == 0) {
                    return String("");
                }
                return GetString(bytes.GetData(), bytes.GetLength());
            }

            String UTF8Encoding::GetString(const char* bytes, int byteCount) {
                if (bytes == nullptr || byteCount == 0) {
                    return String("");
                }
                std::string tempString(bytes, byteCount);
                return String(tempString.c_str());
            }

            std::shared_ptr<Encoding> TextEncoding::UTF8() {
                if (s_utf8EncodingInstance == nullptr) {
                    s_utf8EncodingInstance = std::make_shared<UTF8Encoding>();
                }
                return s_utf8EncodingInstance;
            }
        }
    }
}
