#pragma once
#include "Common.h"
#include "Object.h"
#include "BasicChar.h"
namespace DotNetDupe {
    namespace System {
#ifdef UNICODE
        typedef BasicChar<wchar_t> Char;
#else
        typedef BasicChar<char> Char;
#endif
    }
}


