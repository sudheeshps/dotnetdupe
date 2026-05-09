#pragma once
#include "Common.h"
namespace DotNetDupe {
    namespace System {
        class  Object {
        public:
            DOTNETDUPE_API bool operator == (const Object& obj) const;
            DOTNETDUPE_API virtual bool Equals(const Object& obj);
            DOTNETDUPE_API static bool Equals(const Object& obj1, const Object& obj2);
            DOTNETDUPE_API int GetHashCode();
            DOTNETDUPE_API wchar_t* ToStringW();
            DOTNETDUPE_API char* ToStringA();
        };
    }
}