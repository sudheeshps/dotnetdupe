#include "pch.h"
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        bool Object::operator==(const Object& obj) const {
            /// Compare underlying memory addresses to determine pointer-level identity.
            return this == &obj;
        }

        bool Object::Equals(const Object& obj) const {
            /// Virtual equality delegates to operator== by default for derived overrides.
            return *this == obj;
        }

        bool Object::Equals(const Object& obj1, const Object& obj2) {
            /// Delegate to equality operator between instances.
            return obj1 == obj2;
        }

        int Object::GetHashCode() const {
            /// Return default zero base; specialized derived classes override with custom hash algorithms.
            return 0;
        }

        wchar_t* Object::ToStringW() const {
            /// Base object returns null wide-character representation.
            return nullptr;
        }

        char* Object::ToStringA() const {
            /// Base object returns null narrow-character representation.
            return nullptr;
        }
    }
}

namespace DotNetDupe::System {
    DOTNETDUPE_API void* AllocateCollectionBuffer(size_t size) {
        /// Route directly to global operator new for aligned collection buffer allocation.
        return ::operator new(size);
    }

    DOTNETDUPE_API void FreeCollectionBuffer(void* p) {
        /// Release collection buffer memory back to heap via global operator delete.
        ::operator delete(p);
    }
}

