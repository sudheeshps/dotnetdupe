#include "../pch.h"
#include "TextEncoding.h"
#include <windows.h>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            // Static member initialization
            std::shared_ptr<UTF8Encoding> TextEncoding::_utf8EncodingInstance = nullptr;

            std::vector<char> UTF8Encoding::GetBytes(const String& s) {
                int num_chars = s.GetLength();
                if (num_chars == 0) {
                    return std::vector<char>();
                }

                // Calculate required buffer size for UTF-8
                int buffer_size = WideCharToMultiByte(
                    CP_UTF8,                // UTF-8 code page
                    0,                      // No special flags
                    s.GetRawString(),     // Source wide string
                    -1,              // Number of wide characters
                    nullptr,                   // No output buffer, just calculate size
                    0,                      // Output buffer size 0
                    nullptr, nullptr              // No default character or bool used
                );

                if (buffer_size == 0) {
                    // Handle error, e.g., throw an exception
                    return std::vector<char>();
                }

                std::vector<char> bytes(buffer_size);
                WideCharToMultiByte(
                    CP_UTF8,                // UTF-8 code page
                    0,                      // No special flags
                    s.GetRawString(),     // Source wide string
                    -1,
                    bytes.data(),           // Output buffer
                    buffer_size,
                    nullptr, nullptr
                );
                return bytes;
            }

            String UTF8Encoding::GetString(const std::vector<char>& bytes) {
                if (bytes.empty()) {
                    return String();
                }
                return static_cast<UTF8Encoding*>(this)->GetString(bytes.data(), static_cast<int>(bytes.size()));
            }

            String UTF8Encoding::GetString(const char* bytes, int byteCount) {
                if (bytes == nullptr || byteCount == 0) {
                    return String();
                }

                // Calculate required buffer size for wide characters
                int buffer_size = MultiByteToWideChar(
                    CP_UTF8,                // UTF-8 code page
                    0,                      // No special flags
                    bytes,                  // Source byte array
                    -1,              // Number of bytes
                    nullptr,                   // No output buffer, just calculate size
                    0                       // Output buffer size 0
                );

                if (buffer_size == 0) {
                    // Handle error
                    return String();
                }

                std::vector<wchar_t> wide_chars(buffer_size);
                MultiByteToWideChar(
                    CP_UTF8,                // UTF-8 code page
                    0,                      // No special flags
                    bytes,                  // Source byte array
                    -1,
                    wide_chars.data(),      // Output buffer
                    buffer_size
                );
                return String(wide_chars.data());
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
