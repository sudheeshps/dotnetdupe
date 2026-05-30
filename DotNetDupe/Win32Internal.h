#pragma once

#if defined(_WIN32)
#include <windows.h>
#include <string>
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace Internal {
            // Convert UTF-16 (wchar_t) to UTF-8 (char)
            inline std::string WCharToUtf8(const wchar_t* wstr) {
                if (!wstr || wstr[0] == 0) return "";
                int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
                if (size_needed <= 0) return "";
                std::string strTo(size_needed, 0);
                WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &strTo[0], size_needed, NULL, NULL);
                if (strTo.back() == '\0') strTo.pop_back();
                return strTo;
            }

            // Convert UTF-8 (char) to UTF-16 (wchar_t)
            inline std::wstring Utf8ToWChar(const char* str) {
                if (!str || str[0] == 0) return L"";
                int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
                if (size_needed <= 0) return L"";
                std::wstring wstrTo(size_needed, 0);
                MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstrTo[0], size_needed);
                if (wstrTo.back() == L'\0') wstrTo.pop_back();
                return wstrTo;
            }
        }
    }
}
#endif
