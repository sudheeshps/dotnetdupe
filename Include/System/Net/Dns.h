#pragma once
#include "Common.h"
#include "System/String.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            class Dns {
            public:
                DOTNETDUPE_API static Array<String> GetHostAddresses(const String& hostName);

            private:
                static void InitializeSockets();
            };
        }
    }
}
