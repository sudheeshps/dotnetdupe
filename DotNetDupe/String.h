#pragma once
#include "BasicString.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicString<wchar_t> String;
#else
        typedef BasicString<char> String;
#endif
    }
}

