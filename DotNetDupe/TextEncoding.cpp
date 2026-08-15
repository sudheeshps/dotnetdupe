#include "pch.h"
#include "System/Text/TextEncoding.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            // Static member initialization
            UTF8EncodingPtr TextEncoding::s_utf8EncodingInstance(nullptr);

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

            EncodingPtr TextEncoding::UTF8() {
                if (s_utf8EncodingInstance.IsNull()) {
                    s_utf8EncodingInstance = SmartPointer<UTF8Encoding>::NewShared();
                }
                return s_utf8EncodingInstance.DynamicCast<Encoding>();
            }
        }
    }
}
