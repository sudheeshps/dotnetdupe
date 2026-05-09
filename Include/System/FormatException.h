#pragma once
#include "System/BasicFormatException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicFormatException<wchar_t> FormatException;
#else
        typedef BasicFormatException<char> FormatException;
#endif
    }
}
