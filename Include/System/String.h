#pragma once

#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/NotImplementedException.h"
#include "System/OverflowException.h"
#include "System/FormatException.h"
#include "Utils.h"

#include "System/Array.h"
#include "System/Utils/StringConvert.h"
#include <algorithm>
#include <type_traits>
#include <iomanip>
#include <memory>
#include <regex>
#include <sstream>

#include <vector>
#include <set>
#include <cstring>
#include <cwctype>
#include <format>

namespace DotNetDupe {
    namespace System {
        enum class StringSplitOptions {
            None,
            RemoveEmptyEntries,
            TrimEntries
        };

        // Helper for portable case-insensitive comparison
        template<typename char>
        struct CaseInsensitiveCompare {
            DOTNETDUPE_API static bool Equals(char c1, char c2) {
                return std::tolower(static_cast<unsigned char>(c1)) == std::tolower(static_cast<unsigned char>(c2));
            }
        };

        template<>
        struct CaseInsensitiveCompareWChar {
            DOTNETDUPE_API static bool Equals(wchar_t c1, wchar_t c2) {
                return std::towlower(c1) == std::towlower(c2);
            }
        };

        class DateTimeOffset;

        // Helper to check if T has a ToString() method
        template<typename T, typename = void>
        struct has_ToString : std::false_type {};

        template<typename T>
        DOTNETDUPE_API struct has_ToString<T, std::void_t<decltype(std::declval<const T&>().ToString())>> : std::true_type {};

        template<typename T>
        inline constexpr bool has_ToString_v = has_ToString<T>::value;

        template<typename char>
        struct BoolRepresentation;

        template<>
        struct BoolRepresentation<char> {
            static constexpr const char* True = "True";
            static constexpr const char* False = "False";
        };

        template<>
        struct BoolRepresentationWChar {
            static constexpr const wchar_t* True = L"True";
            static constexpr const wchar_t* False = L"False";
        };

        template<typename char>
        struct FormatExceptionMessage;

        template<>
        struct FormatExceptionMessage<char> {
            static constexpr const char* Message = "Input string was not in a correct format.";
        };

        template<>
        struct FormatExceptionMessageWChar {
            static constexpr const wchar_t* Message = L"Input string was not in a correct format.";
        };

        class String {
        public:
            DOTNETDUPE_API String() = default;
            DOTNETDUPE_API String(const char* pStr);
            DOTNETDUPE_API String(const String& sStr);
            DOTNETDUPE_API String& operator=(const String& sStr);
            DOTNETDUPE_API String(String&& sStr) noexcept;
            DOTNETDUPE_API String& operator=(String&& sStr) noexcept;
            DOTNETDUPE_API String& operator=(const char* pStr);

            // Converting constructor & assignment for opposite character pointer type
            
            DOTNETDUPE_API String(const wchar_t* pStr);
            DOTNETDUPE_API String& operator=(const wchar_t* pStr);

            // std::nullptr_t & int (0/NULL) constructors & assignments to throw ArgumentException
            DOTNETDUPE_API String(std::nullptr_t) { throw ArgumentException("Invalid input pointer"); }
            DOTNETDUPE_API String& operator=(std::nullptr_t) { throw ArgumentException("Invalid input pointer"); }
            DOTNETDUPE_API String(int nNull) {
                if (nNull == 0) throw ArgumentException("Invalid input pointer");
                DOTNETDUPE_API throw ArgumentException("Invalid input integer");
            }
            DOTNETDUPE_API const char* GetRawString() const;
            operator const char* () const { return GetRawString(); }
            DOTNETDUPE_API int GetLength() const;
            DOTNETDUPE_API 
            DOTNETDUPE_API 
            auto begin() { return m_str.begin(); }
            auto end() { return m_str.end(); }
            auto begin() const { return m_str.begin(); }
            auto end() const { return m_str.end(); }
            DOTNETDUPE_API String Clone() const;

            friend std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const String& sStr) {
                os << sStr.GetRawString();
                return os;
            }

            friend std::basic_istream<char>& operator>>(std::basic_istream<char>& is, String& sStr) {
                std::string s;
                is >> s;
                sStr = s.c_str();
                return is;
            }

            friend void PrintTo(const String& sStr, ::std::ostream* os) {
                if (os) {
                    if constexpr (std::is_same_v<char, wchar_t>) {
                        *os << "\"" << (sStr.GetRawString() ? "wchar_t string" : "") << "\"";
                    } else {
                        *os << "\"" << (sStr.GetRawString() ? sStr.GetRawString() : "") << "\"";
                    }
                }
            }

            DOTNETDUPE_API bool operator<(const String& sStr) const {
                return m_str < sStr.m_str;
            }

            friend bool operator==(const String& sStr1, const String& sStr2) {
                return sStr1.m_str.compare(sStr2.m_str) == 0;
            }
            friend bool operator==(const String& sStr1, const char* pStr2) {
                return pStr2 ? sStr1.m_str.compare(pStr2) == 0 : sStr1.m_str.empty();
            }
            friend bool operator==(const char* pStr1, const String& sStr2) {
                return pStr1 ? sStr2.m_str.compare(pStr1) == 0 : sStr2.m_str.empty();
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
            static int Compare(const String& sStr1, int iIndex1,
                               const String& sStr2, int iIndex2, int iLength,
                               bool bIgnoreCase);
            DOTNETDUPE_API int CompareTo(const String& sStr) const;
            DOTNETDUPE_API String operator+(const String& sStr) const;

            DOTNETDUPE_API String operator+(const char* pStr) const {
                String sNewStr = *this;
                if (pStr) {
                    sNewStr.m_str.append(pStr);
                }
                return sNewStr;
            }

            DOTNETDUPE_API String operator+(char ch) const {
                String sNewStr = *this;
                sNewStr.m_str.push_back(ch);
                return sNewStr;
            }

            DOTNETDUPE_API String& operator+=(const String& sStr) {
                m_str.append(sStr.GetRawString());
                return *this;
            }

            DOTNETDUPE_API String& operator+=(const char* pStr) {
                if (pStr) {
                    m_str.append(pStr);
                }
                return *this;
            }

            DOTNETDUPE_API String& operator+=(char ch) {
                m_str.push_back(ch);
                return *this;
            }

            friend String operator+(const char* pStr, const String& sStr) {
                if constexpr (std::is_same_v<char, char>) {
                    String newStr(pStr ? pStr : "");
                    return newStr + sStr;
                } else {
                    String newStr(pStr ? pStr : L"");
                    return newStr + sStr;
                }
            }

            friend String operator+(char ch, const String& sStr) {
                char buf[2] = { ch, 0 };
                String newStr(buf);
                return newStr + sStr;
            }

            String Concat(
                const std::initializer_list<String> sStrs) const;

            DOTNETDUPE_API bool Contains(char ch) const;
            DOTNETDUPE_API bool Contains(const String& sStr) const;

            void CopyTo(int iSourceIndex, char* pDestination, int iDestinationIndex,
                        int iDestArraySize, int iCount) const;

            DOTNETDUPE_API bool EndsWith(char ch, bool bIgnoreCase) const;
            DOTNETDUPE_API bool EndsWith(const String& sSuffix, bool bIgnoreCase) const;

            static bool Equals(const String& sStr1,
                               const String& sStr2);
            DOTNETDUPE_API bool Equals(const String& sStr) const;

            DOTNETDUPE_API int IndexOf(const String& sSubstring) const;
            DOTNETDUPE_API int IndexOf(const String& sSubstring, bool bIgnoreCase) const;
            int IndexOf(const String& sSubstring, int iStartIndex,
                        bool bIgnoreCase) const;
            DOTNETDUPE_API int IndexOfAny(int iStartIndex, std::initializer_list<char> chChars);

            DOTNETDUPE_API String& Append(const char ch);
            DOTNETDUPE_API String& Append(const String& sStr);
            DOTNETDUPE_API String& Insert(int iIndex, const String& sStr);

            DOTNETDUPE_API bool IsEmpty() const;

            static String Join(
                char chSeparator, std::initializer_list<String> sStrings);
            static String Join(
                char chSeparator, std::initializer_list<String> sStrings,
                int iStartIndex, int iCount);
            static String Join(
                const String& sSeparator,
                std::initializer_list<String> sStrings);
            static String Join(
                const String& sSeparator,
                std::initializer_list<String> sStrings, int iStartIndex,
                int iCount);

            template <class... Args>
            DOTNETDUPE_API static String Format(const String& sFormat, const Args&... args);
            template <class... Args>
            DOTNETDUPE_API static String Format(const char* pFormat, const Args&... args);

            DOTNETDUPE_API int LastIndexOf(const String& sStr, bool bIgnoreCase);
            int LastIndexOfAny(int iStartIndex, std::initializer_list<char> chChars,
                               bool bIgnoreCase);

            DOTNETDUPE_API String PadLeft(int iTotalWidth);
            DOTNETDUPE_API String PadLeft(int iTotalWidth, char ch);
            DOTNETDUPE_API String PadRight(int iTotalWidth);
            DOTNETDUPE_API String PadRight(int iTotalWidth, char ch);

            DOTNETDUPE_API String Remove(int iStartIndex) const;
            DOTNETDUPE_API String Remove(int iStartIndex, int iCount) const;

            DOTNETDUPE_API String Replace(char chOriginalChar, char chReplaceChar) const;
            String Replace(const String& sOriginalStr,
                                       const String& sReplaceStr) const;
            DOTNETDUPE_API Array<String> Split(char chSeparator) const;
            Array<String> Split(String sSeparator[], int iCount,
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

        public:
            ~String();
        private:
            template<typename T>
            DOTNETDUPE_API static String ToStringHelper(const T& val);

            DOTNETDUPE_API static std::string TranscodeWcharoUtf8(const wchar_t* pWStr);
            DOTNETDUPE_API static std::wstring TranscodeUtf8ToWChar(const char* pUtf8Str);

            void* m_pData;
        };

    }
}
