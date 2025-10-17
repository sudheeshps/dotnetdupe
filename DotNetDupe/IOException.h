#pragma once
#include "BasicIOException.h"
namespace DotNetDupe {
    namespace System {
        namespace IO {
#ifdef UNICODE
            typedef BasicIOException<wchar_t> IOException;
#else
            typedef BasicIOException<char> IOException;
#endif
        }
    }
}
