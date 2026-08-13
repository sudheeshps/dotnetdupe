#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

        class DirectoryNotFoundException : public IOException {
        public:
            DirectoryNotFoundException(const char* pchMessage) : IOException(pchMessage) {}
            DirectoryNotFoundException(const String& sMessage) : IOException(sMessage) {}
        };
        }

    }
}
