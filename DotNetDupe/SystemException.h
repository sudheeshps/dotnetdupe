#pragma once
#include "BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicSystemException<wchar_t> SystemException;
#else
        typedef BasicSystemException<char> SystemException;
#endif
    }
}

