#pragma once
#include "System/Object.h"
namespace DotNetDupe {
    namespace System {
        template <class T> class IFormatProvider {
        public:
            virtual Object* GetFormat(const T* formatType) = 0;
        };
    }
}
