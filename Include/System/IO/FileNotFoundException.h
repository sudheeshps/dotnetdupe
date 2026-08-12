#pragma once
#include "System/IO/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

        class FileNotFoundException : public IOException {
        public:
            FileNotFoundException(const char* pchMessage) : IOException(pchMessage) {}
            FileNotFoundException(const String& sMessage) : IOException(sMessage) {}
        };
        }

    }
}
