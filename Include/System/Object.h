#pragma once
#include "Common.h"
namespace DotNetDupe {
    namespace System {
        class Object {
        public:
            DOTNETDUPE_API virtual ~Object() = default;
            DOTNETDUPE_API bool operator == (const Object& obj) const;
            DOTNETDUPE_API virtual bool Equals(const Object& obj) const;
            DOTNETDUPE_API static bool Equals(const Object& obj1, const Object& obj2);
            DOTNETDUPE_API int GetHashCode() const;
            DOTNETDUPE_API wchar_t* ToStringW() const;
            DOTNETDUPE_API char* ToStringA() const;
        };

        DOTNETDUPE_API void* AllocateCollectionBuffer(size_t size);
        DOTNETDUPE_API void FreeCollectionBuffer(void* p);
    }
}