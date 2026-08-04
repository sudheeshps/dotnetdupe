#include "pch.h"
#include "InternalStringConvert.h"
#include "System/BasicString.h"

namespace DotNetDupe {
    namespace System {

        template <class CharT>
        std::string BasicString<CharT>::TranscodeWCharToUtf8(const wchar_t* pWStr) {
            return Internal::StringConvertInternal::WCharToUtf8(pWStr);
        }

        template <class CharT>
        std::wstring BasicString<CharT>::TranscodeUtf8ToWChar(const char* pUtf8Str) {
            return Internal::StringConvertInternal::Utf8ToWChar(pUtf8Str);
        }

        template class BasicString<char>;
        template class BasicString<wchar_t>;

    }
}
