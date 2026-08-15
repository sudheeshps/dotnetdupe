#include "pch.h"
#include "System/Char.h"
#include <cwctype>

namespace DotNetDupe {
    namespace System {
        
        Char::Char() : m_c('\0') { }

        Char::Char(char32_t c) {
            m_c = c;
        }

        void Char::operator=(char32_t c) {
            m_c = c;
        }

        bool Char::Equals(char32_t c) const {
            return m_c == c;
        }

        bool Char::operator==(char32_t c) const {
            return m_c == c;
        }

        double Char::GetNumericValue(char32_t c) {
            if (c >= '0' && c <= '9') {
                return c - '0';
            }
            return -1.0;
        }

        bool Char::IsAscii(char32_t c) {
            return (c <= 0x7f);
        }

        bool Char::IsAsciiDigit(char32_t c) {
            return c >= '0' && c <= '9';
        }

        bool Char::IsAsciiHexDigit(char32_t c) {
            return (c >= '0' && c <= '9') ||
                   (c >= 'A' && c <= 'F') || 
                   (c >= 'a' && c <= 'f');
        }

        bool Char::IsAsciiHexDigitLower(char32_t c) {
            return (c >= '0' && c <= '9') ||
                   (c >= 'a' && c <= 'f');
        }

        bool Char::IsAsciiHexDigitUpper(char32_t c) {
            return (c >= '0' && c <= '9') ||
                   (c >= 'A' && c <= 'F');
        }

        bool Char::IsAsciiLetter(char32_t c) {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        }

        bool Char::IsAsciiLetterLower(char32_t c) {
            return (c >= 'a' && c <= 'z');
        }

        bool Char::IsAsciiLetterOrDigit(char32_t c) {
            return (c >= 'A' && c <= 'Z') || 
                   (c >= 'a' && c <= 'z') ||
                   (c >= '0' && c <= '9');
        }

        bool Char::IsAsciiLetterUpper(char32_t c) {
            return (c >= 'A' && c <= 'Z');
        }

        bool Char::IsBetween(char32_t c, char32_t minInclusive, char32_t maxInclusive) {
            return c >= minInclusive && c <= maxInclusive;
        }

        bool Char::IsControl(char32_t c) {
            if (c <= 0x1F || (c >= 0x7F && c <= 0x9F)) return true;
            if (c >= 0xFFFFFF7F && c <= 0xFFFFFF9F) return true; // Handling signed char cast
            if (c <= 0xFFFF) return std::iswcntrl(static_cast<wint_t>(c)) != 0;
            return (c >= 0xE0000 && c <= 0xE007F);
        }

        bool Char::IsDigit(char32_t c) {
            if (c <= 0xFFFF) return std::iswdigit(static_cast<wint_t>(c)) != 0;
            return (c >= 0x1D7CE && c <= 0x1D7FF);
        }

        bool Char::IsLower(char32_t c) {
            if (c <= 0xFFFF) return std::iswlower(static_cast<wint_t>(c)) != 0;
            return (c >= 0x10428 && c <= 0x1044F);
        }

        bool Char::IsLetter(char32_t c) {
            if (c >= 'A' && c <= 'Z') return true;
            if (c >= 'a' && c <= 'z') return true;
            if (c >= 0xC0 && c <= 0xD6) return true;
            if (c >= 0xD8 && c <= 0xF6) return true;
            if (c >= 0xF8 && c <= 0xFF) return true;
            if (c <= 0xFFFF) return std::iswalpha(static_cast<wint_t>(c)) != 0;
            return (c >= 0x20000 && c <= 0x2A6DF);
        }

        bool Char::IsLetterOrDigit(char32_t c) {
            if (c <= 0xFFFF) return std::iswalnum(static_cast<wint_t>(c)) != 0;
            return IsLetter(c) || IsDigit(c);
        }

        char32_t Char::ToLower(char32_t c) {
            if (c >= 'A' && c <= 'Z') return c + 0x20;
            if (c >= 0xC0 && c <= 0xD6) return c + 0x20;
            if (c >= 0xD8 && c <= 0xDE) return c + 0x20;
            if (c <= 0xFFFF) {
                wint_t lower = std::towlower(static_cast<wint_t>(c));
                if (lower != static_cast<wint_t>(c)) return static_cast<char32_t>(lower);
            }
            if (c >= 0x10400 && c <= 0x10427) return c + 0x28;
            return c;
        }

        bool Char::IsNumber(char32_t c) {
            return IsDigit(c);
        }

    }
}