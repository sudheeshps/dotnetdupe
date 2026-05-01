#pragma once
#include "System/Object.h"
namespace DotNetDupe {
    namespace System {
        class IComparable {
        public:
            virtual int CompareTo(const Object& obj) = 0;
        };
    }
}
