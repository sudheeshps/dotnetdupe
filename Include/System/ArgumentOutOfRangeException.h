#pragma once
#include "System/BasicArgumentOutOfRangeException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicArgumentOutOfRangeException<wchar_t> ArgumentOutOfRangeException;
#else
        typedef BasicArgumentOutOfRangeException<char> ArgumentOutOfRangeException;
#endif
    }
}

