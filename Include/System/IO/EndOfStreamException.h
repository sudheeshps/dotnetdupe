#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

        class EndOfStreamException : public IOException {
        public:
            EndOfStreamException(const char* pchMessage) : IOException(pchMessage) {}
            EndOfStreamException(const String& sMessage) : IOException(sMessage) {}
        };
        }

    }
}
