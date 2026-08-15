#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {

        template <typename T>
        struct HashHelper {
            static int GetHashCode(const T& value) {
                const char* p = reinterpret_cast<const char*>(&value);
                int hash = 17;
                for (size_t i = 0; i < sizeof(T); ++i) {
                    hash = hash * 31 + p[i];
                }
                return hash;
            }
        };

        template <> struct HashHelper<int> { static int GetHashCode(const int& value) { return value; } };
        template <> struct HashHelper<unsigned int> { static int GetHashCode(const unsigned int& value) { return (int)value; } };
        template <> struct HashHelper<long long> { static int GetHashCode(const long long& value) { return (int)(value ^ (value >> 32)); } };
        template <> struct HashHelper<unsigned long long> { static int GetHashCode(const unsigned long long& value) { return (int)(value ^ (value >> 32)); } };
        template <> struct HashHelper<short> { static int GetHashCode(const short& value) { return value; } };
        template <> struct HashHelper<unsigned short> { static int GetHashCode(const unsigned short& value) { return value; } };
        template <> struct HashHelper<char> { static int GetHashCode(const char& value) { return value; } };
        template <> struct HashHelper<unsigned char> { static int GetHashCode(const unsigned char& value) { return value; } };
        template <> struct HashHelper<wchar_t> { static int GetHashCode(const wchar_t& value) { return value; } };
        template <> struct HashHelper<bool> { static int GetHashCode(const bool& value) { return value ? 1 : 0; } };

        template <> struct HashHelper<String> {
            static int GetHashCode(const String& value) {
                return value.GetHashCode();
            }
        };

        template <typename T>
        struct HashHelper<SmartPointer<T>> {
            static int GetHashCode(const SmartPointer<T>& value) {
                if (value.IsNull()) return 0;
                return value->GetHashCode();
            }
        };

        template <typename T>
        struct HashHelper<T*> {
            static int GetHashCode(T* const& value) {
                if (!value) return 0;
                unsigned long long ptr = reinterpret_cast<unsigned long long>(value);
                return (int)(ptr ^ (ptr >> 32));
            }
        };

    }
}
