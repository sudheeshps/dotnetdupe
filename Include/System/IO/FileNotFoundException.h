#pragma once
#include "System/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

        class FileNotFoundException : public IOException {
        public:
            FileNotFoundException(const String& sMessage) : IOException(sMessage) {}
        };
        }

    }
}
