#include "pch.h"
#include "InternalStringConvert.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {

        template <class CharT>
        std::string String::TranscodeWCharToUtf8(const wchar_t* pWStr) {
            return Internal::StringConvertInternal::WCharToUtf8(pWStr);
        }

        template <class CharT>
        std::wstring String::TranscodeUtf8ToWChar(const char* pUtf8Str) {
            return Internal::StringConvertInternal::Utf8ToWChar(pUtf8Str);
        }

        template class String;
        template class String;

    }
}
