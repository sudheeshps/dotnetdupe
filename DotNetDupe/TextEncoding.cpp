#include "pch.h"
#include "System/Text/TextEncoding.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            // Static member initialization
            UTF8EncodingPtr TextEncoding::s_utf8EncodingInstance(nullptr);

            Array<char> UTF8Encoding::GetBytes(const String& s) {
                /// Guard: Return empty array for empty strings.
                const char* raw = (const char*)s;
                if (!raw) return Array<char>(0);

                /// Allocate and copy raw UTF-8 bytes into managed array.
                int length = s.GetLength();
                Array<char> bytes(length);
                for (int i = 0; i < length; i++) bytes[i] = raw[i];
                return bytes;
            }

            String UTF8Encoding::GetString(const Array<char>& bytes) {
                /// Guard: Return empty string for zero-length arrays.
                if (bytes.GetLength() == 0) {
                    return String("");
                }

                /// Delegate to raw pointer decoding.
                return GetString(bytes.GetData(), bytes.GetLength());
            }

            String UTF8Encoding::GetString(const char* bytes, int byteCount) {
                /// Guard: Validate input buffer and byte count.
                if (bytes == nullptr || byteCount == 0) {
                    return String("");
                }

                /// Construct string from contiguous byte span.
                std::string tempString(bytes, byteCount);
                return String(tempString.c_str());
            }

            EncodingPtr TextEncoding::UTF8() {
                /// Lazy-initialize singleton UTF-8 encoding instance.
                if (s_utf8EncodingInstance.IsNull()) {
                    s_utf8EncodingInstance = SmartPointer<UTF8Encoding>::NewShared();
                }

                /// Return polymorphic Encoding smart pointer.
                return s_utf8EncodingInstance.DynamicCast<Encoding>();
            }
        }
    }
}
