#pragma once
namespace DotNetDupe {
    namespace System {
        template <class T> class IComparable {
        public:
            virtual int CompareTo<T>(const T& other) = 0;
        };
    }
}
