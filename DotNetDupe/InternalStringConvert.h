#pragma once

#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Internal {
            class StringConvertInternal {
            public:
                static std::string WCharToUtf8(const wchar_t* pWStr);
                static std::wstring Utf8ToWChar(const char* pUtf8Str);
            };

            inline std::string WCharToUtf8(const wchar_t* pWStr) {
                return StringConvertInternal::WCharToUtf8(pWStr);
            }

            inline std::wstring Utf8ToWChar(const char* pUtf8Str) {
                return StringConvertInternal::Utf8ToWChar(pUtf8Str);
            }
        }
    }
}
