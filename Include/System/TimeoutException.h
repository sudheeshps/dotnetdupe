#pragma once
#include "System/BasicTimeoutException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicTimeoutException<wchar_t> TimeoutException;
#else
        typedef BasicTimeoutException<char> TimeoutException;
#endif
    }
}
