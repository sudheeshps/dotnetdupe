#pragma once
#include "System/BasicArithmeticException.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicArithmeticException<wchar_t> ArithmeticException;
#else
        typedef BasicArithmeticException<char> ArithmeticException;
#endif
    }
}

