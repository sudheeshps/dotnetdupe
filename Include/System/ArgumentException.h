#pragma once
#include "System/BasicArgumentException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicArgumentException<wchar_t> ArgumentException;
#else
        typedef BasicArgumentException<char> ArgumentException;
#endif
    }
}

