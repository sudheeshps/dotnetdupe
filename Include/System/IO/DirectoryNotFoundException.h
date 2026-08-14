#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

        class DirectoryNotFoundException : public IOException {
        public:
            DirectoryNotFoundException(const String& sMessage) : IOException(sMessage) {}
        };
        }

    }
}
