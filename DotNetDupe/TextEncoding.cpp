#include "pch.h"
#include "System/Text/TextEncoding.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            // Static member initialization
            std::shared_ptr<UTF8Encoding> TextEncoding::_utf8EncodingInstance = nullptr;

            std::vector<char> UTF8Encoding::GetBytes(const String& s) {
                const char* raw = (const char*)s;
                if (!raw) return std::vector<char>();
                int len = s.GetLength();
                return std::vector<char>(raw, raw + len);
            }

            String UTF8Encoding::GetString(const std::vector<char>& bytes) {
                if (bytes.empty()) {
                    return String("");
                }
                return GetString(bytes.data(), static_cast<int>(bytes.size()));
            }

            String UTF8Encoding::GetString(const char* bytes, int byteCount) {
                if (bytes == nullptr || byteCount == 0) {
                    return String("");
                }
                std::string s(bytes, byteCount);
                return String(s.c_str());
            }

            std::shared_ptr<Encoding> TextEncoding::UTF8() {
                if (_utf8EncodingInstance == nullptr) {
                    _utf8EncodingInstance = std::make_shared<UTF8Encoding>();
                }
                return _utf8EncodingInstance;
            }
        }
    }
}
