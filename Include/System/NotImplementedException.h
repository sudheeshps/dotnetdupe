#pragma once
#include "System/BasicNotImplementedException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicNotImplementedException<wchar_t> NotImplementedException;
#else
        typedef BasicNotImplementedException<char> NotImplementedException;
#endif
    }
}

