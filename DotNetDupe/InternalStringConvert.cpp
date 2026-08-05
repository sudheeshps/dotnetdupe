#include "pch.h"
#include "InternalStringConvert.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Internal {

            std::string StringConvertInternal::WCharToUtf8(const wchar_t* pWStr) {
                if (!pWStr || pWStr[0] == 0) return std::string("");
#if defined(_WIN32)
                int iSizeNeeded = ::WideCharToMultiByte(CP_UTF8, 0, pWStr, -1, NULL, 0, NULL, NULL);
                if (iSizeNeeded <= 0) return std::string("");
                std::string sResult(iSizeNeeded, 0);
                ::WideCharToMultiByte(CP_UTF8, 0, pWStr, -1, &sResult[0], iSizeNeeded, NULL, NULL);
                if (!sResult.empty() && sResult.back() == '\0') sResult.pop_back();
                return sResult;
#else
                std::string sResult;
                for (; *pWStr; ++pWStr) {
                    sResult.push_back(static_cast<char>(*pWStr));
                }
                return sResult;
#endif
            }

            std::wstring StringConvertInternal::Utf8ToWChar(const char* pUtf8Str) {
                if (!pUtf8Str || pUtf8Str[0] == 0) return std::wstring(L"");
#if defined(_WIN32)
                int iSizeNeeded = ::MultiByteToWideChar(CP_UTF8, 0, pUtf8Str, -1, NULL, 0);
                if (iSizeNeeded <= 0) return std::wstring(L"");
                std::wstring sResult(iSizeNeeded, 0);
                ::MultiByteToWideChar(CP_UTF8, 0, pUtf8Str, -1, &sResult[0], iSizeNeeded);
                if (!sResult.empty() && sResult.back() == L'\0') sResult.pop_back();
                return sResult;
#else
                std::wstring sResult;
                for (; *pUtf8Str; ++pUtf8Str) {
                    sResult.push_back(static_cast<wchar_t>(*pUtf8Str));
                }
                return sResult;
#endif
            }

        }
    }
}
