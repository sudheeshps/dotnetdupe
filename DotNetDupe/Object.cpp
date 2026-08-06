#include "pch.h"
#include "System/Object.h"
namespace DotNetDupe {
    namespace System {
        bool Object::operator==(const Object& obj) const {
            return this == &obj;
        }

        bool Object::Equals(const Object& obj) const {
            return *this == obj;
        }

        bool Object::Equals(const Object& obj1, const Object& obj2) {
            return obj1 == obj2;
        }
        int Object::GetHashCode() const {
            return 0;
        }
        wchar_t* Object::ToStringW() const {
            return nullptr;
        }
        char* Object::ToStringA() const {
            return nullptr;
        }
    }
}
