#pragma once
typedef unsigned char byte;
namespace DotNetDupe {
    namespace System {
        /* enum class UnicodeCategory
         {
             ClosePunctuation = 21,
             ConnectorPunctuation = 18,
             Control = 14,
             CurrencySymbol = 26,
             DashPunctuation = 19,
             DecimalDigitNumber = 8,
             EnclosingMark = 7,
             FinalQuotePunctuation = 23,
             Format = 15,
             InitialQuotePunctuation = 22,
             LetterNumber = 9,
             LineSeparator = 12,
             LowercaseLetter = 1,
             MathSymbol = 25,
             ModifierLetter = 3,
             ModifierSymbol = 27,
             NonSpacingMark = 5,
             OpenPunctuation = 20,
             OtherLetter = 4,
             OtherNotAssigned = 29,
             OtherNumber = 10,
             OtherPunctuation = 24,
             OtherSymbol = 28,
             ParagraphSeparator = 13,
             PrivateUse = 17,
             SpaceSeparator = 11,
             SpacingCombiningMark = 6,
             Surrogate = 16,
             TitlecaseLetter = 2,
             UppercaseLetter = 0

         };*/
        template <class CharT> class BasicChar {
        public:
            BasicChar(CharT c);
            void operator= (CharT c);
            bool Equals(CharT c);
            bool operator == (CharT c);
            double static GetNumericValue(CharT c);
            bool static IsAscii(CharT c);
            bool static IsAsciiDigit(CharT c);
            bool static IsAsciiHexDigit(CharT c);
            bool static IsAsciiHexDigitLower(CharT c);
            bool static IsAsciiHexDigitUpper(CharT c);
            bool static IsAsciiLetter(CharT c);
            bool static IsAsciiLetterLower(CharT c);
            bool static IsAsciiLetterOrDigit(CharT c);
            bool static IsAsciiLetterUpper(CharT c);
            bool static IsBetween(CharT c, CharT minInclusive, CharT maxInclusive);
            bool static IsControl(CharT c);
            bool static IsDigit(CharT c);
            bool static IsLower(CharT c);
            bool static IsNumber(CharT c);
            //bool static IsPunctuation(T c);
            bool static IsLetter(CharT c);
            bool static IsLetterOrDigit(CharT c);

            CharT static ToLower(CharT c);
            CharT GetChar();
        private:
            CharT m_c;
            //const byte categoryForLatin1[287] = 
            //{
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0000 - 0007
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0008 - 000F
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0010 - 0017
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0018 - 001F
            //(byte)UnicodeCategory::SpaceSeparator, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation,    // 0020 - 0027
            //(byte)UnicodeCategory::OpenPunctuation, (byte)UnicodeCategory::ClosePunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::DashPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation,    // 0028 - 002F
            //(byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber,    // 0030 - 0037
            //(byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::OtherPunctuation,    // 0038 - 003F
            //(byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 0040 - 0047
            //(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 0048 - 004F
            //(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 0050 - 0057
            //(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::OpenPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::ClosePunctuation, (byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::ConnectorPunctuation,    // 0058 - 005F
            //(byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 0060 - 0067
            //(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 0068 - 006F
            //(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 0070 - 0077
            //(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::OpenPunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::ClosePunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::Control,    // 0078 - 007F
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0080 - 0087
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0088 - 008F
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0090 - 0097
            //(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0098 - 009F
            //(byte)UnicodeCategory::SpaceSeparator, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::OtherSymbol,    // 00A0 - 00A7
            //(byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::InitialQuotePunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::DashPunctuation, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::ModifierSymbol,    // 00A8 - 00AF
            //(byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::OtherPunctuation,    // 00B0 - 00B7
            //(byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::FinalQuotePunctuation, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherPunctuation,    // 00B8 - 00BF
            //(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 00C0 - 00C7
            //(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 00C8 - 00CF
            //(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::MathSymbol,    // 00D0 - 00D7
            //(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 00D8 - 00DF
            //(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 00E0 - 00E7
            //(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 00E8 - 00EF
            //(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::MathSymbol,    // 00F0 - 00F7
            //(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter   // 00F8 - 00FF
            //};
            //static bool IsLatin1(T c)
            //{
            //    return (c <= '\x00ff');
            //}
            //static UnicodeCategory GetLatin1UnicodeCategory(char c) {
            //    return (UnicodeCategory)(categoryForLatin1[(int)c]);
            //}
            //static bool CheckPunctuation(UnicodeCategory uc)
            //{
            //    switch (uc) {
            //    case UnicodeCategory::ConnectorPunctuation:
            //    case UnicodeCategory::DashPunctuation:
            //    case UnicodeCategory::OpenPunctuation:
            //    case UnicodeCategory::ClosePunctuation:
            //    case UnicodeCategory::InitialQuotePunctuation:
            //    case UnicodeCategory::FinalQuotePunctuation:
            //    case UnicodeCategory::OtherPunctuation:
            //        return (true);
            //    }
            //    return (false);
            //}
        };

        template<class CharT>
        inline BasicChar<CharT>::BasicChar(CharT c) {
            this->m_c = c;
        }

        template<class CharT>
        inline void BasicChar<CharT>::operator=(CharT c) {
            return this->m_c = c;
        }

        template<class CharT>
        inline bool BasicChar<CharT>::Equals(CharT c) {
            return this->m_c == c;
        }

        template<class CharT>
        inline bool BasicChar<CharT>::operator==(CharT c) {
            return this->m_c == c;
        }

        template<class CharT>
        inline double BasicChar<CharT>::GetNumericValue(CharT c) {
            if (c >= CharT('0') && c <= CharT('9')) {
                return c - '0';
            }
            return -1.0;
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAscii(CharT c) {
            return (c <= CharT('\x007f'));
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiDigit(CharT c) {
            return c >= CharT('0') && c <= CharT('9');
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiHexDigit(CharT c) {
            return (c >= CharT('0') && c <= CharT('9')) ||
                (c >= CharT('A') && c <= CharT('F')) || (c >= CharT('a') && c <= CharT('f'));
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiHexDigitLower(CharT c) {
            return (c >= CharT('0') && c <= CharT('9')) ||
                (c >= CharT('a') && c <= CharT('f'));

        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiHexDigitUpper(CharT c) {
            return (c >= CharT('0') && c <= CharT('9')) ||
                (c >= CharT('A') && c <= CharT('F'));

        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiLetter(CharT c) {
            return (c >= CharT('A') && c <= CharT('Z')) || (c >= CharT('a') && c <= CharT('z'));
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiLetterLower(CharT c) {
            return (c >= CharT('a') && c <= CharT('z'));
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiLetterOrDigit(CharT c) {
            return (c >= CharT('A') && c <= CharT('Z')) || (c >= CharT('a') && c <= CharT('z')) ||
                (c >= CharT('0') && c <= CharT('9'));
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsAsciiLetterUpper(CharT c) {
            return (c >= CharT('A') && c <= CharT('Z'));
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsBetween(CharT c, CharT minInclusive, CharT maxInclusive) {
            return c >= minInclusive && c <= maxInclusive;
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsControl(CharT c) {
            return (c >= 0x0000 && c <= 0x001F) || c == 0x007F ||
                (c >= 0x0080 && c <= 0x009F);
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsDigit(CharT c) {
            return c >= CharT('0') && c <= CharT('9');
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsLower(CharT c) {
            return c >= CharT('a') && c <= CharT('z');;
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsLetter(CharT c) {
            return c >= CharT('0') && c <= CharT('9');
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsLetterOrDigit(CharT c) {
            return IsAsciiLetterOrDigit(c);
        }

        template<class CharT>
        inline CharT BasicChar<CharT>::ToLower(CharT c) {
            if (c >= CharT('A') && c <= CharT('Z')) {
                return CharT('a') + c - CharT('A');
            }
            return c;
        }

        template<class CharT>
        inline bool BasicChar<CharT>::IsNumber(CharT c) {
            return (c >= CharT('0') && c <= CharT('9'));
        }

        /*template<class T>
        inline bool BasicChar<T>::IsPunctuation(T c)
        {
            if (IsLatin1(c)) {
                return (CheckPunctuation(GetLatin1UnicodeCategory(c)));
            }
            return (CheckPunctuation(CharUnicodeInfo.GetUnicodeCategory(c)));
        }*/

        template<class CharT>
        inline CharT BasicChar<CharT>::GetChar() {
            return m_c;
        }
    }
}
