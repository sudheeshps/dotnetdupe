#pragma once

#include "../Common.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class IDisposable {
            public:
                virtual ~IDisposable() = default;
                virtual void Dispose() = 0;
            };
        }
    }
}
