#pragma once
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicException<wchar_t> Exception;
#else
        typedef BasicException<char> Exception;
#endif
    }
}

