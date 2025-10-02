#include "pch.h"
#include "Object.h"
namespace DotNetDupe {
    namespace System {
        bool Object::operator==(const Object& obj) const {
            if (this == &obj) return true;
            return *this == obj;
        }

        bool Object::Equals(const Object& obj) {
            if (this == &obj) return true;
            return *this == obj;
        }

        bool Object::Equals(const Object& obj1, const Object& obj2) {
            return obj1 == obj2;
        }
        int Object::GetHashCode() {
            return 0;
        }
        wchar_t* Object::ToStringW() {
            return nullptr;
        }
        char* Object::ToStringA() {
            return nullptr;
        }
    }
}
