#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

        class EndOfStreamException : public IOException {
        public:
            EndOfStreamException(const String& sMessage) : IOException(sMessage) {}
        };
        }

    }
}
