#include "pch.h"
#include "System/Utils/StringConvert.h"
#include "InternalStringConvert.h"

namespace DotNetDupe {
    namespace System {
        namespace Utils {

            std::string StringConvert::WCharToUtf8(const wchar_t* pWStr) {
                return Internal::StringConvertInternal::WCharToUtf8(pWStr);
            }

            std::wstring StringConvert::Utf8ToWChar(const char* pUtf8Str) {
                return Internal::StringConvertInternal::Utf8ToWChar(pUtf8Str);
            }

        }
    }
}
