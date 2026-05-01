#pragma once
#include "System/Object.h"
namespace DotNetDupe {
    namespace System {
        class IClonable {
        public:
            virtual Object Clone() = 0;
        };
    }
}
