#pragma once
#include "System/BasicOverflowException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicOverflowException<wchar_t> OverflowException;
#else
        typedef BasicOverflowException<char> OverflowException;
#endif
    }
}

