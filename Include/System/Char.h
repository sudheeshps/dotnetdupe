#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        
        class Char : public Object {
        private:
            char32_t m_c;

        public:
            DOTNETDUPE_API Char();
            DOTNETDUPE_API Char(char32_t c);

            DOTNETDUPE_API void operator=(char32_t c);
            DOTNETDUPE_API bool Equals(char32_t c) const;
            DOTNETDUPE_API bool operator==(char32_t c) const;

            DOTNETDUPE_API static double GetNumericValue(char32_t c);
            DOTNETDUPE_API static bool IsAscii(char32_t c);
            DOTNETDUPE_API static bool IsAsciiDigit(char32_t c);
            DOTNETDUPE_API static bool IsAsciiHexDigit(char32_t c);
            DOTNETDUPE_API static bool IsAsciiHexDigitLower(char32_t c);
            DOTNETDUPE_API static bool IsAsciiHexDigitUpper(char32_t c);
            DOTNETDUPE_API static bool IsAsciiLetter(char32_t c);
            DOTNETDUPE_API static bool IsAsciiLetterLower(char32_t c);
            DOTNETDUPE_API static bool IsAsciiLetterOrDigit(char32_t c);
            DOTNETDUPE_API static bool IsAsciiLetterUpper(char32_t c);
            DOTNETDUPE_API static bool IsBetween(char32_t c, char32_t minInclusive, char32_t maxInclusive);

            DOTNETDUPE_API static bool IsControl(char32_t c);
            DOTNETDUPE_API static bool IsDigit(char32_t c);
            DOTNETDUPE_API static bool IsLetter(char32_t c);
            DOTNETDUPE_API static bool IsLetterOrDigit(char32_t c);
            DOTNETDUPE_API static bool IsLower(char32_t c);
            DOTNETDUPE_API static bool IsNumber(char32_t c);
            DOTNETDUPE_API static bool IsPunctuation(char32_t c);
            DOTNETDUPE_API static bool IsSeparator(char32_t c);
            DOTNETDUPE_API static bool IsSurrogate(char32_t c);
            DOTNETDUPE_API static bool IsSymbol(char32_t c);
            DOTNETDUPE_API static bool IsUpper(char32_t c);
            DOTNETDUPE_API static bool IsWhiteSpace(char32_t c);

            DOTNETDUPE_API static char32_t ToLower(char32_t c);
            DOTNETDUPE_API static char32_t ToUpper(char32_t c);
            
            DOTNETDUPE_API char32_t GetChar() const { return m_c; }

            // ToString returns a single-character string (UTF-8 encoded)
            DOTNETDUPE_API String ToString() const;
            DOTNETDUPE_API static String ToString(char32_t c);
        };
    }
}
