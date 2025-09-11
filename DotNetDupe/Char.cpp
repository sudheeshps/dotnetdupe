#include "pch.h"
#include "Char.h"
namespace DotNetDupe {
    namespace System {
        Char::Char() :_char(L'\0') {
        }

        Char::Char(wchar_t c) : _char(c) {
        }

        void Char::operator=(wchar_t c) {
            _char = c;
        }

        bool Char::Equals(wchar_t c) {
            return *this == c;
        }


        bool Char::operator==(wchar_t c) {
            return _char == (wchar_t)c;
        }

        bool Char::IsDigit(wchar_t c) {
            return c >= '0' && c <= '9';
        }

        bool Char::IsLower(wchar_t c) {
            return c >= 'a' && c <= 'z';
        }

        bool Char::IsLetter(wchar_t c) {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');;
        }

        wchar_t Char::ToLower(wchar_t c) {
            if (c >= 'A' && c <= 'Z') {
                return 'a' + c - 'A';
            }
            return c;
        }
    }
}

