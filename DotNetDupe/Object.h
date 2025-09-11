#pragma once
#include "Common.h"
namespace DotNetDupe {
    namespace System {
        class DOTNETDUPE_API Object {
        public:
            virtual bool operator == (const Object& obj) const;
            virtual bool Equals(const Object& obj);
            static bool Equals(const Object& obj1, const Object& obj2);
            int GetHashCode();
            wchar_t* ToStringW();
            char* ToStringA();
        };
    }
}