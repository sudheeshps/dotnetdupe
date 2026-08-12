#pragma once

#include "Common.h"
#include <string>

namespace DotNetDupe {
    namespace System {
        class String;

        namespace Utils {
            class StringConvert {
            public:
                // Converts UTF-16 (wchar_t) string to std::string UTF-8
                DOTNETDUPE_API static std::string WCharToUtf8(const wchar_t* pWStr);

                // Converts UTF-8 (char) string to std::wstring UTF-16
                DOTNETDUPE_API static std::wstring Utf8ToWChar(const char* pUtf8Str);
            };
        }
    }
}
