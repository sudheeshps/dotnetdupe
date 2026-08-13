#pragma once

#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/NotImplementedException.h"
#include "System/OverflowException.h"
#include "System/FormatException.h"
#include "Utils.h"

// Forward declaration moved
#include "System/Utils/StringConvert.h"
#include <initializer_list>
#include <ctype.h>
#include <wctype.h>

namespace DotNetDupe {
    namespace System {
        template<typename T> class Array;
        enum class StringSplitOptions {
            None,
            RemoveEmptyEntries,
            TrimEntries
        };

        // Helper for portable case-insensitive comparison
        struct CaseInsensitiveCompare {
            static bool Equals(char c1, char c2) {
                return tolower(static_cast<unsigned char>(c1)) == tolower(static_cast<unsigned char>(c2));
            }
        };

        struct CaseInsensitiveCompareWChar {
            static bool Equals(wchar_t c1, wchar_t c2) {
                return towlower(c1) == towlower(c2);
            }
        };

        class DateTimeOffset;

        struct BoolRepresentation {
            static constexpr const char* True = "True";
            static constexpr const char* False = "False";
        };

        struct BoolRepresentationWChar {
            static constexpr const wchar_t* True = L"True";
            static constexpr const wchar_t* False = L"False";
        };

        struct FormatExceptionMessage {
            static constexpr const char* Message = "Input string was not in a correct format.";
        };

        struct FormatExceptionMessageWChar {
            static constexpr const wchar_t* Message = L"Input string was not in a correct format.";
        };

        class String {
        public:
            DOTNETDUPE_API String();
            DOTNETDUPE_API String(const char* pStr);
            DOTNETDUPE_API String(const String& sStr);
            DOTNETDUPE_API String& operator=(const String& sStr);
            DOTNETDUPE_API String(String&& sStr) noexcept;
            DOTNETDUPE_API String& operator=(String&& sStr) noexcept;
            DOTNETDUPE_API String& operator=(const char* pStr);
            DOTNETDUPE_API ~String();

            // Converting constructor & assignment for opposite character pointer type
            
            DOTNETDUPE_API String(const wchar_t* pStr);
            DOTNETDUPE_API String& operator=(const wchar_t* pStr);

            // std::nullptr_t & int (0/NULL) constructors & assignments to throw ArgumentException
            DOTNETDUPE_API String(std::nullptr_t) { throw ArgumentException("Invalid input pointer"); }
            DOTNETDUPE_API String& operator=(std::nullptr_t) { throw ArgumentException("Invalid input pointer"); }
            DOTNETDUPE_API String(int nNull) {
                if (nNull == 0) throw ArgumentException("Invalid input pointer");
                throw ArgumentException("Invalid input integer");
            }
            DOTNETDUPE_API const char* GetRawString() const;
            operator const char* () const { return GetRawString(); }
            DOTNETDUPE_API int GetLength() const;
            DOTNETDUPE_API String Clone() const;

            DOTNETDUPE_API bool operator<(const String& sStr) const {
                const char* p1 = GetRawString();
                const char* p2 = sStr.GetRawString();
                if (!p1 && !p2) return false;
                if (!p1) return true;
                if (!p2) return false;
                return strcmp(p1, p2) < 0;
            }

            friend bool operator==(const String& sStr1, const String& sStr2) {
                const char* p1 = sStr1.GetRawString();
                const char* p2 = sStr2.GetRawString();
                if (p1 == p2) return true;
                if (!p1 || !p2) return false;
                return strcmp(p1, p2) == 0;
            }
            friend bool operator==(const String& sStr1, const char* pStr2) {
                const char* p1 = sStr1.GetRawString();
                if (p1 == pStr2) return true;
                if (!p1 || !pStr2) return false;
                return strcmp(p1, pStr2) == 0;
            }
            friend bool operator==(const char* pStr1, const String& sStr2) {
                return sStr2 == pStr1;
            }

            friend bool operator!=(const String& sStr1, const String& sStr2) {
                return !(sStr1 == sStr2);
            }
            friend bool operator!=(const String& sStr1, const char* pStr2) {
                return !(sStr1 == pStr2);
            }
            friend bool operator!=(const char* pStr1, const String& sStr2) {
                return !(pStr1 == sStr2);
            }
            DOTNETDUPE_API char operator[](int iIndex) const;
            DOTNETDUPE_API static int Compare(const String& sStr1, int iIndex1,
                               const String& sStr2, int iIndex2, int iLength,
                               bool bIgnoreCase);
            DOTNETDUPE_API int CompareTo(const String& sStr) const;
            DOTNETDUPE_API String operator+(const String& sStr) const;
            DOTNETDUPE_API String operator+(const char* pStr) const;
            DOTNETDUPE_API String operator+(char ch) const;

            DOTNETDUPE_API String& operator+=(const String& sStr);
            DOTNETDUPE_API String& operator+=(const char* pStr);
            DOTNETDUPE_API String& operator+=(char ch);

            DOTNETDUPE_API friend String operator+(const char* pStr, const String& sStr);
            DOTNETDUPE_API friend String operator+(char ch, const String& sStr);
            DOTNETDUPE_API String Concat(
                const std::initializer_list<String> sStrs) const;

            DOTNETDUPE_API bool Contains(char ch) const;
            DOTNETDUPE_API bool Contains(const String& sStr) const;

            DOTNETDUPE_API void CopyTo(int iSourceIndex, char* pDestination, int iDestinationIndex,
                        int iDestArraySize, int iCount) const;

            DOTNETDUPE_API bool EndsWith(char ch, bool bIgnoreCase) const;
            DOTNETDUPE_API bool EndsWith(const String& sSuffix, bool bIgnoreCase) const;

            DOTNETDUPE_API static bool Equals(const String& sStr1,
                               const String& sStr2);
            DOTNETDUPE_API bool Equals(const String& sStr) const;

            DOTNETDUPE_API int IndexOf(const String& sSubstring) const;
            DOTNETDUPE_API int IndexOf(const String& sSubstring, bool bIgnoreCase) const;
            DOTNETDUPE_API int IndexOf(const String& sSubstring, int iStartIndex,
                        bool bIgnoreCase) const;
            DOTNETDUPE_API int IndexOfAny(int iStartIndex, std::initializer_list<char> chChars);

            DOTNETDUPE_API String& Append(const char ch);
            DOTNETDUPE_API String& Append(const String& sStr);
            DOTNETDUPE_API String& Insert(int iIndex, const String& sStr);

            DOTNETDUPE_API bool IsEmpty() const;

            DOTNETDUPE_API static String Join(
                char chSeparator, std::initializer_list<String> sStrings);
            DOTNETDUPE_API static String Join(
                char chSeparator, std::initializer_list<String> sStrings,
                int iStartIndex, int iCount);
            DOTNETDUPE_API static String Join(
                const String& sSeparator,
                std::initializer_list<String> sStrings);
            DOTNETDUPE_API static String Join(
                const String& sSeparator,
                std::initializer_list<String> sStrings, int iStartIndex,
                int iCount);

            static String FormatArgHelper(bool b) { return b ? "True" : "False"; }
            static String FormatArgHelper(std::nullptr_t) { return ""; }
            static String FormatArgHelper(const String& s) { return s; }
            static String FormatArgHelper(const char* s) { return String(s); }
            static String FormatArgHelper(char c) { char arr[2] = {c, 0}; return String(arr); }
            static String FormatArgHelper(short i) { return String::FromInt(i); }
            static String FormatArgHelper(unsigned short i) { return String::FromInt(i); }
            static String FormatArgHelper(int i) { return String::FromInt(i); }
            static String FormatArgHelper(unsigned int u) { return String::ValueOf((long long)u); }
            static String FormatArgHelper(long l) { return String::ValueOf((long long)l); }
            static String FormatArgHelper(unsigned long ul) { return String::ValueOf((long long)ul); }
            static String FormatArgHelper(long long ll) { return String::ValueOf(ll); }
            static String FormatArgHelper(unsigned long long ull) { return String::ValueOf((long long)ull); }
            static String FormatArgHelper(float f) { return String::ValueOf((double)f); }
            static String FormatArgHelper(double d) { return String::ValueOf(d); }
            template<typename T>
            static String FormatArgHelper(const T& arg) { return arg.ToString(); }

            DOTNETDUPE_API static String InternalFormat(const char* pFormat, const String* pArgs, int iArgCount);

            template <typename... Args>
            static String Format(const char* pFormat, const Args&... args) {
                String arrArgs[] = { FormatArgHelper(args)... };
                return InternalFormat(pFormat, arrArgs, sizeof...(Args));
            }
            static String Format(const char* pFormat) {
                return InternalFormat(pFormat, nullptr, 0);
            }
            template <typename... Args>
            static String Format(const String& sFormat, const Args&... args) {
                return Format(sFormat.GetRawString() ? sFormat.GetRawString() : "", args...);
            }

            DOTNETDUPE_API int LastIndexOf(const String& sStr, bool bIgnoreCase);
            DOTNETDUPE_API int LastIndexOfAny(int iStartIndex, std::initializer_list<char> chChars,
                               bool bIgnoreCase);

            DOTNETDUPE_API String PadLeft(int iTotalWidth);
            DOTNETDUPE_API String PadLeft(int iTotalWidth, char ch);
            DOTNETDUPE_API String PadRight(int iTotalWidth);
            DOTNETDUPE_API String PadRight(int iTotalWidth, char ch);

            DOTNETDUPE_API String Remove(int iStartIndex) const;
            DOTNETDUPE_API String Remove(int iStartIndex, int iCount) const;

            DOTNETDUPE_API String Replace(char chOriginalChar, char chReplaceChar) const;
            DOTNETDUPE_API String Replace(const String& sOriginalStr,
                                       const String& sReplaceStr) const;
            DOTNETDUPE_API Array<String> Split(char chSeparator) const;
            DOTNETDUPE_API Array<String> Split(String sSeparator[], int iCount,
                                                  StringSplitOptions eOptions) const;
            DOTNETDUPE_API bool StartsWith(const String& sPrefix) const;
            DOTNETDUPE_API bool StartsWith(const String& sPrefix, bool bIgnoreCase) const;
            DOTNETDUPE_API String Substring(int iStartIndex) const;
            DOTNETDUPE_API String Substring(int iStartIndex, int iLength) const;

            DOTNETDUPE_API static String FromInt(int iValue);
            DOTNETDUPE_API static String ValueOf(int iValue);
            DOTNETDUPE_API static String ValueOf(long long llValue);
            DOTNETDUPE_API static String ValueOf(double dValue);
            DOTNETDUPE_API static String ToString(int iValue);
            DOTNETDUPE_API static String ToString(long long llValue);
            DOTNETDUPE_API static String ToString(double dValue);
            DOTNETDUPE_API static String ToString(bool bValue);

            DOTNETDUPE_API String ToLower() const;
            DOTNETDUPE_API String ToUpper() const;
            DOTNETDUPE_API String Trim() const;
            DOTNETDUPE_API String TrimStart() const;
            DOTNETDUPE_API String TrimEnd() const;

            DOTNETDUPE_API void* operator new(size_t size);
            DOTNETDUPE_API void operator delete(void* p);
            DOTNETDUPE_API void* operator new[](size_t size);
            DOTNETDUPE_API void operator delete[](void* p);

        private:
            struct StringImpl;
            StringImpl* m_pImpl;
        };
    }
}


