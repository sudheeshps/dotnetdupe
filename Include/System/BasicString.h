#pragma once

#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/NotImplementedException.h"
#include "System/OverflowException.h"
#include "System/BasicFormatException.h"
#include "Utils.h"

#include "System/Array.h"
#include <algorithm>
#include <type_traits>
#include <iomanip>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
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
        template<typename CharT>
        struct CaseInsensitiveCompare {
            static bool Equals(CharT c1, CharT c2) {
                return std::tolower(static_cast<unsigned char>(c1)) == std::tolower(static_cast<unsigned char>(c2));
            }
        };

        template<>
        struct CaseInsensitiveCompare<wchar_t> {
            static bool Equals(wchar_t c1, wchar_t c2) {
                return std::towlower(c1) == std::towlower(c2);
            }
        };

        class DateTimeOffset;

        // Helper to check if T has a ToString() method
        template<typename T, typename = void>
        struct has_ToString : std::false_type {};

        template<typename T>
        struct has_ToString<T, std::void_t<decltype(std::declval<const T&>().ToString())>> : std::true_type {};

        template<typename T>
        inline constexpr bool has_ToString_v = has_ToString<T>::value;

        template<typename CharT>
        struct BoolRepresentation;

        template<>
        struct BoolRepresentation<char> {
            static constexpr const char* True = "True";
            static constexpr const char* False = "False";
        };

        template<>
        struct BoolRepresentation<wchar_t> {
            static constexpr const wchar_t* True = L"True";
            static constexpr const wchar_t* False = L"False";
        };

        template<typename CharT>
        struct FormatExceptionMessage;

        template<>
        struct FormatExceptionMessage<char> {
            static constexpr const char* Message = "Input string was not in a correct format.";
        };

        template<>
        struct FormatExceptionMessage<wchar_t> {
            static constexpr const wchar_t* Message = L"Input string was not in a correct format.";
        };

        template <class CharT>
        class BasicString {
        public:
            BasicString() = default;
            BasicString(const CharT* pStr);
            BasicString(const std::basic_string<CharT>& str) : m_str(str) {}
            BasicString(std::basic_string<CharT>&& str) noexcept : m_str(std::move(str)) {}
            BasicString(const BasicString<CharT>& sStr);
            BasicString<CharT>& operator=(const BasicString<CharT>& sStr);
            BasicString(BasicString<CharT>&& sStr) noexcept;
            BasicString<CharT>& operator=(BasicString<CharT>&& sStr) noexcept;
            BasicString<CharT>& operator=(const CharT* pStr);

            // Converting constructor & assignment for opposite character pointer type
            using OppositeCharT = std::conditional_t<std::is_same_v<CharT, char>, wchar_t, char>;
            BasicString(const OppositeCharT* pStr);
            BasicString<CharT>& operator=(const OppositeCharT* pStr);

            // std::nullptr_t & int (0/NULL) constructors & assignments to throw ArgumentException
            BasicString(std::nullptr_t) { throw ArgumentException("Invalid input pointer"); }
            BasicString<CharT>& operator=(std::nullptr_t) { throw ArgumentException("Invalid input pointer"); }
            BasicString(int nNull) {
                if (nNull == 0) throw ArgumentException("Invalid input pointer");
                throw ArgumentException("Invalid input integer");
            }
            const CharT* GetRawString() const;
            operator const CharT* () const { return GetRawString(); }
            int GetLength() const;
            std::basic_string<CharT>& GetString() { return m_str; }
            const std::basic_string<CharT>& GetString() const { return m_str; }
            auto begin() { return m_str.begin(); }
            auto end() { return m_str.end(); }
            auto begin() const { return m_str.begin(); }
            auto end() const { return m_str.end(); }
            BasicString<CharT> Clone() const;

            friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const BasicString<CharT>& sStr) {
                os << sStr.GetRawString();
                return os;
            }

            friend std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is, BasicString<CharT>& sStr) {
                std::basic_string<CharT> s;
                is >> s;
                sStr = s.c_str();
                return is;
            }

            friend void PrintTo(const BasicString<CharT>& sStr, ::std::ostream* os) {
                if (os) {
                    if constexpr (std::is_same_v<CharT, wchar_t>) {
                        *os << "\"" << (sStr.GetRawString() ? "wchar_t string" : "") << "\"";
                    } else {
                        *os << "\"" << (sStr.GetRawString() ? sStr.GetRawString() : "") << "\"";
                    }
                }
            }

            bool operator<(const BasicString<CharT>& sStr) const {
                return m_str < sStr.m_str;
            }

            friend bool operator==(const BasicString<CharT>& sStr1, const BasicString<CharT>& sStr2) {
                return sStr1.m_str.compare(sStr2.m_str) == 0;
            }
            friend bool operator==(const BasicString<CharT>& sStr1, const CharT* pStr2) {
                return pStr2 ? sStr1.m_str.compare(pStr2) == 0 : sStr1.m_str.empty();
            }
            friend bool operator==(const CharT* pStr1, const BasicString<CharT>& sStr2) {
                return pStr1 ? sStr2.m_str.compare(pStr1) == 0 : sStr2.m_str.empty();
            }

            friend bool operator!=(const BasicString<CharT>& sStr1, const BasicString<CharT>& sStr2) {
                return !(sStr1 == sStr2);
            }
            friend bool operator!=(const BasicString<CharT>& sStr1, const CharT* pStr2) {
                return !(sStr1 == pStr2);
            }
            friend bool operator!=(const CharT* pStr1, const BasicString<CharT>& sStr2) {
                return !(pStr1 == sStr2);
            }
            CharT operator[](int iIndex) const;
            static int Compare(const BasicString<CharT>& sStr1, int iIndex1,
                               const BasicString<CharT>& sStr2, int iIndex2, int iLength,
                               bool bIgnoreCase);
            int CompareTo(const BasicString<CharT>& sStr) const;
            BasicString<CharT> operator+(const BasicString<CharT>& sStr) const;

            BasicString<CharT> operator+(const CharT* pStr) const {
                BasicString<CharT> sNewStr = *this;
                if (pStr) {
                    sNewStr.m_str.append(pStr);
                }
                return sNewStr;
            }

            BasicString<CharT> operator+(CharT ch) const {
                BasicString<CharT> sNewStr = *this;
                sNewStr.m_str.push_back(ch);
                return sNewStr;
            }

            BasicString<CharT>& operator+=(const BasicString<CharT>& sStr) {
                m_str.append(sStr.GetRawString());
                return *this;
            }

            BasicString<CharT>& operator+=(const CharT* pStr) {
                if (pStr) {
                    m_str.append(pStr);
                }
                return *this;
            }

            BasicString<CharT>& operator+=(CharT ch) {
                m_str.push_back(ch);
                return *this;
            }

            friend BasicString<CharT> operator+(const CharT* pStr, const BasicString<CharT>& sStr) {
                if constexpr (std::is_same_v<CharT, char>) {
                    BasicString<CharT> newStr(pStr ? pStr : "");
                    return newStr + sStr;
                } else {
                    BasicString<CharT> newStr(pStr ? pStr : L"");
                    return newStr + sStr;
                }
            }

            friend BasicString<CharT> operator+(CharT ch, const BasicString<CharT>& sStr) {
                CharT buf[2] = { ch, 0 };
                BasicString<CharT> newStr(buf);
                return newStr + sStr;
            }

            BasicString<CharT> Concat(
                const std::initializer_list<BasicString<CharT>> sStrs) const;

            bool Contains(CharT ch) const;
            bool Contains(const BasicString<CharT>& sStr) const;

            void CopyTo(int iSourceIndex, CharT* pDestination, int iDestinationIndex,
                        int iDestArraySize, int iCount) const;

            bool EndsWith(CharT ch, bool bIgnoreCase) const;
            bool EndsWith(const BasicString<CharT>& sSuffix, bool bIgnoreCase) const;

            static bool Equals(const BasicString<CharT>& sStr1,
                               const BasicString<CharT>& sStr2);
            bool Equals(const BasicString<CharT>& sStr) const;

            int IndexOf(const BasicString<CharT>& sSubstring) const;
            int IndexOf(const BasicString<CharT>& sSubstring, bool bIgnoreCase) const;
            int IndexOf(const BasicString<CharT>& sSubstring, int iStartIndex,
                        bool bIgnoreCase) const;
            int IndexOfAny(int iStartIndex, std::initializer_list<CharT> chChars);

            BasicString<CharT>& Append(const CharT ch);
            BasicString<CharT>& Append(const BasicString<CharT>& sStr);
            BasicString<CharT>& Insert(int iIndex, const BasicString<CharT>& sStr);

            bool IsEmpty() const;

            static BasicString<CharT> Join(
                CharT chSeparator, std::initializer_list<BasicString<CharT>> sStrings);
            static BasicString<CharT> Join(
                CharT chSeparator, std::initializer_list<BasicString<CharT>> sStrings,
                int iStartIndex, int iCount);
            static BasicString<CharT> Join(
                const BasicString<CharT>& sSeparator,
                std::initializer_list<BasicString<CharT>> sStrings);
            static BasicString<CharT> Join(
                const BasicString<CharT>& sSeparator,
                std::initializer_list<BasicString<CharT>> sStrings, int iStartIndex,
                int iCount);

            template <class... Args>
            static BasicString<CharT> Format(const BasicString<CharT>& sFormat, const Args&... args);
            template <class... Args>
            static BasicString<CharT> Format(const CharT* pFormat, const Args&... args);

            int LastIndexOf(const BasicString<CharT>& sStr, bool bIgnoreCase);
            int LastIndexOfAny(int iStartIndex, std::initializer_list<CharT> chChars,
                               bool bIgnoreCase);

            BasicString<CharT> PadLeft(int iTotalWidth);
            BasicString<CharT> PadLeft(int iTotalWidth, CharT ch);
            BasicString<CharT> PadRight(int iTotalWidth);
            BasicString<CharT> PadRight(int iTotalWidth, CharT ch);

            BasicString<CharT> Remove(int iStartIndex) const;
            BasicString<CharT> Remove(int iStartIndex, int iCount) const;

            BasicString<CharT> Replace(CharT chOriginalChar, CharT chReplaceChar);
            BasicString<CharT> Replace(const BasicString<CharT>& sOriginalStr,
                                       const BasicString<CharT>& sReplaceStr);
            Array<BasicString<CharT>> Split(CharT chSeparator) const;
            Array<BasicString<CharT>> Split(BasicString<CharT> sSeparator[], int iCount,
                                                  StringSplitOptions eOptions) const;
            bool StartsWith(const BasicString<CharT>& sPrefix) const;
            bool StartsWith(const BasicString<CharT>& sPrefix, bool bIgnoreCase) const;
            BasicString<CharT> Substring(int iStartIndex) const;
            BasicString<CharT> Substring(int iStartIndex, int iLength) const;

            static BasicString<CharT> FromInt(int iValue);
            static BasicString<CharT> ValueOf(int iValue);
            static BasicString<CharT> ValueOf(long long llValue);
            static BasicString<CharT> ValueOf(double dValue);
            static BasicString<CharT> ToString(int iValue);
            static BasicString<CharT> ToString(long long llValue);
            static BasicString<CharT> ToString(double dValue);
            static BasicString<CharT> ToString(bool bValue);

            BasicString<CharT> ToLower() const;
            BasicString<CharT> ToUpper() const;
            BasicString<CharT> Trim() const;
            BasicString<CharT> TrimStart() const;
            BasicString<CharT> TrimEnd() const;
        private:
            template<typename T>
            static BasicString<CharT> ToStringHelper(const T& val);

            DOTNETDUPE_API static std::string TranscodeWCharToUtf8(const wchar_t* pWStr);
            DOTNETDUPE_API static std::wstring TranscodeUtf8ToWChar(const char* pUtf8Str);

            std::basic_string<CharT> m_str;
        };

        template <class CharT>
        inline BasicString<CharT>::BasicString(const CharT* pStr) {
            if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
            m_str = pStr;
        }

        template <class CharT>
        inline BasicString<CharT>::BasicString(const BasicString<CharT>& sStr) : m_str(sStr.m_str) { }

        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::operator=(const BasicString<CharT>& sStr) {
            if (this != &sStr) {
                m_str = sStr.m_str;
            }
            return *this;
        }

        template<class CharT>
        inline BasicString<CharT>::BasicString(BasicString<CharT>&& sStr) noexcept : m_str(std::move(sStr.m_str)) { }
        template<class CharT>
        inline BasicString<CharT>& BasicString<CharT>::operator=(BasicString<CharT>&& sStr) noexcept {
            if (this != &sStr) {
                m_str = std::move(sStr.m_str);
            }
            return *this;
        }
        template<class CharT>
        inline BasicString<CharT>::BasicString(const OppositeCharT* pStr) {
            if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
            if constexpr (std::is_same_v<CharT, char>) {
                m_str = TranscodeWCharToUtf8(pStr);
            } else {
                m_str = TranscodeUtf8ToWChar(pStr);
            }
        }

        template<class CharT>
        inline BasicString<CharT>& BasicString<CharT>::operator=(const OppositeCharT* pStr) {
            if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
            if constexpr (std::is_same_v<CharT, char>) {
                m_str = TranscodeWCharToUtf8(pStr);
            } else {
                m_str = TranscodeUtf8ToWChar(pStr);
            }
            return *this;
        }

        template<class CharT>
        inline BasicString<CharT>& BasicString<CharT>::operator=(const CharT* pStr) {
            if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
            m_str = pStr;
            return *this;
        }
        template <class CharT>
        inline const CharT* BasicString<CharT>::GetRawString() const {
            return m_str.c_str();
        }
        template <class CharT>
        inline int BasicString<CharT>::GetLength() const {
            return static_cast<int>(m_str.length());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Clone() const {
            return BasicString(GetRawString());
        }


        template<class CharT>
        inline CharT BasicString<CharT>::operator[](int iIndex) const {
            if (iIndex >= (int)m_str.size()) throw ArgumentOutOfRangeException("Invalid iIndex");
            return GetRawString() [iIndex];
        }
        template <class CharT>
        inline int BasicString<CharT>::Compare(const BasicString<CharT>& sStr1,
                                               int iIndex1,
                                               const BasicString<CharT>& sStr2,
                                               int iIndex2, int iLength,
                                               bool bIgnoreCase) {
            if (!bIgnoreCase) {
                return sStr1.m_str.compare(iIndex1, iLength, sStr2.m_str, iIndex2, iLength);
            }

            auto s1 = sStr1.m_str.substr(iIndex1, iLength);
            auto s2 = sStr2.m_str.substr(iIndex2, iLength);
            auto transform_char = [](CharT ch) -> CharT {
                if constexpr (std::is_same_v<CharT, wchar_t>) {
                    return static_cast<wchar_t>(std::towlower(ch));
                } else {
                    return static_cast<CharT>(std::tolower(static_cast<unsigned char>(ch)));
                }
            };
            std::transform(s1.begin(), s1.end(), s1.begin(), transform_char);
            std::transform(s2.begin(), s2.end(), s2.begin(), transform_char);
            return s1.compare(s2);
        }

        // Specialization for wchar_t to use towlower
        template<>
        inline int BasicString<wchar_t>::Compare(const BasicString<wchar_t>& sStr1,
                                               int iIndex1,
                                               const BasicString<wchar_t>& sStr2,
                                               int iIndex2, int iLength,
                                               bool bIgnoreCase) {
            if (!bIgnoreCase) {
                return sStr1.m_str.compare(iIndex1, iLength, sStr2.m_str, iIndex2, iLength);
            }

            auto s1 = sStr1.m_str.substr(iIndex1, iLength);
            auto s2 = sStr2.m_str.substr(iIndex2, iLength);
            std::transform(s1.begin(), s1.end(), s1.begin(),
                           [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
            std::transform(s2.begin(), s2.end(), s2.begin(),
                           [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
            return s1.compare(s2);
        }

        template <class CharT>
        inline int BasicString<CharT>::CompareTo(const BasicString<CharT>& sStr) const {
            return m_str.compare(sStr.GetRawString());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Concat(
            const std::initializer_list<BasicString<CharT>> sStrs) const {
            BasicString<CharT> sNewStr = *this;
            for (auto sStr : sStrs) {
                sNewStr.m_str.append(sStr.GetRawString());
            }
            return sNewStr;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::operator+(
            const BasicString<CharT>& sStr) const {
            return Concat({ sStr });
        }
        template <class CharT>
        inline bool BasicString<CharT>::Contains(CharT ch) const {
            return m_str.find(ch) != std::basic_string<CharT>::npos;
        }
        template <class CharT>
        inline bool BasicString<CharT>::Contains(const BasicString<CharT>& sStr) const {
            return m_str.find(sStr.m_str) != std::basic_string<CharT>::npos;
        }

        template <class CharT>
        inline void BasicString<CharT>::CopyTo(int iSourceIndex, CharT* pDestination,
                                               int iDestinationIndex, int iDestArraySize,
                                               int iCount) const {
            if (nullptr == pDestination)
                throw ArgumentException("Invalid destination buffer");
            int iLen = GetLength();
            if (iSourceIndex < 0 || iSourceIndex >= iLen)
                throw ArgumentOutOfRangeException("Invalid iSourceIndex");
            if (iCount > iLen)
                throw ArgumentOutOfRangeException(
                    "Source array size is smaller than iCount");
            if (iCount > iDestArraySize)
                throw ArgumentOutOfRangeException(
                    "Destination array is smaller than iCount");
            m_str.copy(pDestination + iDestinationIndex, iCount, iSourceIndex);
        }
        template <class CharT>
        inline bool BasicString<CharT>::EndsWith(CharT ch, bool bIgnoreCase) const {
            auto iLen = m_str.length();
            if (iLen == 0) return false;
            if (bIgnoreCase) {
                return CaseInsensitiveCompare<CharT>::Equals(m_str [iLen - 1], ch);
            }
            return m_str [iLen - 1] == ch;
        }
        template <class CharT>
        inline bool BasicString<CharT>::EndsWith(const BasicString<CharT>& sSuffix,
                                                 bool bIgnoreCase) const {
            int iLen = GetLength();
            int iSuffixLen = sSuffix.GetLength();
            if (iSuffixLen > iLen) return false;

            if (bIgnoreCase) {
                return Compare(*this, iLen - iSuffixLen, sSuffix, 0, iSuffixLen, true) == 0;
            }
            return m_str.compare(static_cast<size_t>(iLen - iSuffixLen), static_cast<size_t>(iSuffixLen), sSuffix.m_str) == 0;
        }
        template <class CharT>
        inline bool BasicString<CharT>::Equals(const BasicString<CharT>& sStr1,
                                               const BasicString<CharT>& sStr2) {
            return sStr1 == sStr2;
        }
        template <class CharT>
        inline bool BasicString<CharT>::Equals(const BasicString<CharT>& sStr) const {
            return *this == sStr;
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& sSubstring) const {
            return IndexOf(sSubstring, 0, false);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& sSubstring,
                                               bool bIgnoreCase) const {
            return IndexOf(sSubstring, 0, bIgnoreCase);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& sSubstring,
                                               int iStartIndex, bool bIgnoreCase) const {
            if (iStartIndex < 0 || iStartIndex > GetLength())
                throw ArgumentOutOfRangeException("Invalid iStartIndex");
            
            if (sSubstring.IsEmpty()) return iStartIndex;

            if (!bIgnoreCase) {
                auto pos = m_str.find(sSubstring.m_str, iStartIndex);
                return (pos == std::basic_string<CharT>::npos) ? -1 : (int)pos;
            }

            // Case-insensitive search
            auto it = std::search(
                m_str.begin() + iStartIndex, m_str.end(),
                sSubstring.m_str.begin(), sSubstring.m_str.end(),
                [](CharT c1, CharT c2) { return CaseInsensitiveCompare<CharT>::Equals(c1, c2); }
            );

            return (it == m_str.end()) ? -1 : (int)std::distance(m_str.begin(), it);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOfAny(int iStartIndex,
                                                  std::initializer_list<CharT> chChars) {
            if (iStartIndex < 0 || iStartIndex > GetLength())
                throw ArgumentOutOfRangeException("Invalid iStartIndex");
            
            auto pos = m_str.find_first_of(std::basic_string<CharT>(chChars.begin(), chChars.end()), iStartIndex);
            return (pos == std::basic_string<CharT>::npos) ? -1 : (int)pos;
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::Append(const CharT ch) {
            m_str += ch;
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::Append(
            const BasicString<CharT>& sStr) {
            m_str.append(sStr.GetRawString());
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::Insert(
            int iIndex, const BasicString<CharT>& sStr) {
            int iLen = GetLength();
            if (iIndex < 0 || iIndex > iLen)
                throw ArgumentOutOfRangeException("Invalid iIndex");

            m_str.insert(iIndex, sStr.GetRawString(), sStr.GetLength());
            return *this;
        }
        template <class CharT>
        inline bool BasicString<CharT>::IsEmpty() const {
            return m_str.empty();
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            CharT chSeparator, std::initializer_list<BasicString<CharT>> sStrings) {
            return Join(chSeparator, sStrings, 0, (int)sStrings.size());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            CharT chSeparator, std::initializer_list<BasicString<CharT>> sStrings,
            int iStartIndex, int iCount) {
            if (iStartIndex < 0 || iStartIndex > (int)sStrings.size())
                throw ArgumentOutOfRangeException("Invalid iStartIndex");
            if (iCount < 0 || (iStartIndex + iCount) > (int)sStrings.size())
                throw ArgumentOutOfRangeException("Invalid iCount");

            std::vector<BasicString<CharT>> vStrs(sStrings);
            BasicString<CharT> sJoinStr("");

            for (int i = iStartIndex; i < iStartIndex + iCount; i++) {
                sJoinStr.Append(vStrs [i]);
                if (i != iStartIndex + iCount - 1) {
                    sJoinStr.Append(chSeparator);
                }
            }
            return sJoinStr;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            const BasicString<CharT>& sSeparator,
            std::initializer_list<BasicString<CharT>> sStrings) {
            return Join(sSeparator, sStrings, 0, (int)sStrings.size());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            const BasicString<CharT>& sSeparator,
            std::initializer_list<BasicString<CharT>> sStrings, int iStartIndex,
            int iCount) {
            if (iStartIndex < 0 || iStartIndex > (int)sStrings.size())
                throw ArgumentOutOfRangeException("Invalid iStartIndex");
            if (iCount < 0 || (iStartIndex + iCount) > (int)sStrings.size())
                throw ArgumentOutOfRangeException("Invalid iCount");

            std::vector<BasicString<CharT>> vStrs(sStrings);
            BasicString<CharT> sJoinStr("");

            for (int i = iStartIndex; i < iStartIndex + iCount; i++) {
                sJoinStr.Append(vStrs [i]);
                if (i != iStartIndex + iCount - 1) {
                    sJoinStr.Append(sSeparator);
                }
            }
            return sJoinStr;
        }
        template <class CharT>
        inline int BasicString<CharT>::LastIndexOf(const BasicString<CharT>& sStr,
                                                   bool bIgnoreCase) {
            if (sStr.IsEmpty()) return GetLength();

            if (!bIgnoreCase) {
                auto pos = m_str.rfind(sStr.m_str);
                return (pos == std::basic_string<CharT>::npos) ? -1 : (int)pos;
            }

            auto it = std::find_end(
                m_str.begin(), m_str.end(),
                sStr.m_str.begin(), sStr.m_str.end(),
                [](CharT c1, CharT c2) { return CaseInsensitiveCompare<CharT>::Equals(c1, c2); }
            );

            return (it == m_str.end()) ? -1 : (int)std::distance(m_str.begin(), it);
        }
        template <class CharT>
        inline int BasicString<CharT>::LastIndexOfAny(
            int iStartIndex, std::initializer_list<CharT> chChars, bool bIgnoreCase) {
            if (iStartIndex < 0 || iStartIndex > GetLength())
                throw ArgumentOutOfRangeException("Invalid iStartIndex");
            
            if (IsEmpty()) return -1;

            for (auto ch : chChars) {
                size_t lastFound = std::basic_string<CharT>::npos;
                for (int i = GetLength() - 1; i >= iStartIndex; --i) {
                    bool match = false;
                    if (bIgnoreCase) {
                        match = CaseInsensitiveCompare<CharT>::Equals(m_str[i], ch);
                    } else {
                        match = (m_str[i] == ch);
                    }

                    if (match) {
                        lastFound = i;
                        break;
                    }
                }
                if (lastFound != std::basic_string<CharT>::npos) {
                    return static_cast<int>(lastFound);
                }
            }
            return -1;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadLeft(int iTotalWidth) {
            return PadLeft(iTotalWidth, (CharT)' ');
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadLeft(int iTotalWidth,
                                                              CharT ch) {
            if (iTotalWidth < 0) {
                throw ArgumentException("Invalid iTotalWidth");
            }
            int iLen = GetLength();
            if (iTotalWidth <= iLen) {
                return *this;
            }
            std::basic_string<CharT> sPadding(iTotalWidth - iLen, ch);
            m_str.insert(0, sPadding);
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadRight(int iTotalWidth) {
            return PadRight(iTotalWidth, (CharT)' ');
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadRight(int iTotalWidth,
                                                               CharT ch) {
            if (iTotalWidth < 0) {
                throw ArgumentException("Invalid iTotalWidth");
            }
            int iLen = GetLength();
            if (iTotalWidth <= iLen) {
                return *this;
            }
            m_str.append(iTotalWidth - iLen, ch);
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Remove(int iStartIndex) const {
            return Remove(iStartIndex, GetLength() - iStartIndex);
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Remove(int iStartIndex,
                                                             int iCount) const {
            int iLen = GetLength();
            if (iStartIndex < 0 || iStartIndex > iLen || iCount < 0 || (iStartIndex + iCount) > iLen) {
                throw ArgumentOutOfRangeException("Invalid iStartIndex or iCount");
            }

            std::basic_string<CharT> sRet = m_str;
            sRet.erase(iStartIndex, iCount);
            return BasicString<CharT>(sRet.c_str());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Replace(CharT chOriginalChar,
                                                              CharT chReplaceChar) {
            std::basic_string<CharT> sRet = m_str;
            std::replace(sRet.begin(), sRet.end(), chOriginalChar, chReplaceChar);
            return BasicString<CharT>(sRet.c_str());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Replace(
            const BasicString<CharT>& sOriginalStr,
            const BasicString<CharT>& sReplaceStr) {
            if (sOriginalStr.IsEmpty()) return *this;

            std::basic_string<CharT> sRet = m_str;
            size_t pos = 0;
            while ((pos = sRet.find(sOriginalStr.m_str, pos)) != std::basic_string<CharT>::npos) {
                sRet.replace(pos, sOriginalStr.GetLength(), sReplaceStr.m_str);
                pos += sReplaceStr.GetLength();
            }
            return BasicString<CharT>(sRet.c_str());
        }
        template<class CharT>
        inline Array<BasicString<CharT>> BasicString<CharT>::Split(CharT chSeparator) const
        {
            std::vector<BasicString<CharT>> vTempResult;
            std::basic_stringstream<CharT> ss(m_str);
            std::basic_string<CharT> sToken;
            while (std::getline(ss, sToken, chSeparator))
            {
                vTempResult.push_back(BasicString<CharT>(sToken.c_str()));
            }
            
            Array<BasicString<CharT>> result((int)vTempResult.size());
            for (int i = 0; i < (int)vTempResult.size(); i++) result[i] = vTempResult[i];
            return result;
        }

        template<class CharT>
        inline Array<BasicString<CharT>> BasicString<CharT>::Split(BasicString<CharT> sSeparator[], int iCount,
                                                                         StringSplitOptions eOptions) const {
            std::vector<BasicString<CharT>> vTempResult;
            std::set<CharT> charSet;
            for (int i = 0; i < iCount; ++i) {
                for (auto c : sSeparator [i].m_str) charSet.insert(c);
            }
            std::basic_string<CharT> sCurrent;
            for (auto c : m_str) {
                if (charSet.find(c) == charSet.end()) {
                    sCurrent += c;
                }
                else {
                    BasicString<CharT> s(sCurrent.c_str());
                    if (eOptions == StringSplitOptions::TrimEntries) s = s.Trim();
                    if (eOptions != StringSplitOptions::RemoveEmptyEntries || !s.IsEmpty()) {
                        vTempResult.push_back(s);
                    }
                    sCurrent.clear();
                }
            }
            BasicString<CharT> sFinal(sCurrent.c_str());
            if (eOptions == StringSplitOptions::TrimEntries) sFinal = sFinal.Trim();
            if (eOptions != StringSplitOptions::RemoveEmptyEntries || !sFinal.IsEmpty()) {
                vTempResult.push_back(sFinal);
            }

            Array<BasicString<CharT>> result((int)vTempResult.size());
            for (int i = 0; i < (int)vTempResult.size(); i++) result[i] = vTempResult[i];
            return result;
        }

        template <class CharT>
        inline bool BasicString<CharT>::StartsWith(const BasicString<CharT>& sPrefix) const {
            return StartsWith(sPrefix, false);
        }

        template <class CharT>
        inline bool BasicString<CharT>::StartsWith(const BasicString<CharT>& sPrefix, bool bIgnoreCase) const {
            if (sPrefix.GetLength() > GetLength()) return false;
            
            if (!bIgnoreCase) {
                return m_str.compare(0, sPrefix.GetLength(), sPrefix.m_str) == 0;
            }

            return Compare(*this, 0, sPrefix, 0, sPrefix.GetLength(), true) == 0;
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::FromInt(int iValue) {
            return ValueOf(iValue);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ValueOf(int iValue) {
            return ToStringHelper(iValue);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ValueOf(long long llValue) {
            return ToStringHelper(llValue);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ValueOf(double dValue) {
            std::basic_ostringstream<CharT> ss;
            ss << dValue;
            return BasicString<CharT>(ss.str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToString(int iValue) {
            return ToStringHelper(iValue);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToString(long long llValue) {
            return ToStringHelper(llValue);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToString(double dValue) {
            return ToStringHelper(dValue);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToString(bool bValue) {
            return ToStringHelper(bValue);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Substring(int iStartIndex) const {
            return Substring(iStartIndex, GetLength() - iStartIndex);
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Substring(int iStartIndex, int iLength) const {
            int iLen = GetLength();
            if (iStartIndex < 0 || iStartIndex > iLen || iLength < 0 || (iStartIndex + iLength) > iLen) {
                throw ArgumentOutOfRangeException("Invalid iStartIndex or iLength");
            }
            return BasicString<CharT>(m_str.substr(iStartIndex, iLength).c_str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToLower() const {
            std::basic_string<CharT> sRet = m_str;
            std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](CharT ch) -> CharT {
                if constexpr (std::is_same_v<CharT, wchar_t>) {
                    return static_cast<wchar_t>(std::towlower(ch));
                } else {
                    return static_cast<CharT>(std::tolower(static_cast<unsigned char>(ch)));
                }
            });
            return BasicString<CharT>(sRet.c_str());
        }

        template<>
        inline BasicString<wchar_t> BasicString<wchar_t>::ToLower() const {
            std::basic_string<wchar_t> sRet = m_str;
            std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
            return BasicString<wchar_t>(sRet.c_str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToUpper() const {
            std::basic_string<CharT> sRet = m_str;
            std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](CharT ch) -> CharT {
                if constexpr (std::is_same_v<CharT, wchar_t>) {
                    return static_cast<wchar_t>(std::towupper(ch));
                } else {
                    return static_cast<CharT>(std::toupper(static_cast<unsigned char>(ch)));
                }
            });
            return BasicString<CharT>(sRet.c_str());
        }

        template<>
        inline BasicString<wchar_t> BasicString<wchar_t>::ToUpper() const {
            std::basic_string<wchar_t> sRet = m_str;
            std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](wchar_t ch) { return static_cast<wchar_t>(std::towupper(ch)); });
            return BasicString<wchar_t>(sRet.c_str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Trim() const {
            return TrimStart().TrimEnd();
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::TrimStart() const {
            auto it = std::find_if(m_str.begin(), m_str.end(), [](CharT ch) {
                if constexpr (std::is_same_v<CharT, wchar_t>) {
                    return !std::iswspace(ch);
                } else {
                    return !std::isspace(static_cast<unsigned char>(ch));
                }
            });
            if (it == m_str.end()) {
                if constexpr (std::is_same_v<CharT, char>) return "";
                else return L"";
            }
            return BasicString<CharT>(m_str.substr(std::distance(m_str.begin(), it)).c_str());
        }

        template<>
        inline BasicString<wchar_t> BasicString<wchar_t>::TrimStart() const {
            auto it = std::find_if(m_str.begin(), m_str.end(), [](wchar_t ch) {
                return !std::iswspace(ch);
            });
            if (it == m_str.end()) return L"";
            return BasicString<wchar_t>(m_str.substr(std::distance(m_str.begin(), it)).c_str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::TrimEnd() const {
            auto it = std::find_if(m_str.rbegin(), m_str.rend(), [](CharT ch) {
                if constexpr (std::is_same_v<CharT, wchar_t>) {
                    return !std::iswspace(ch);
                } else {
                    return !std::isspace(static_cast<unsigned char>(ch));
                }
            });
            if (it == m_str.rend()) {
                if constexpr (std::is_same_v<CharT, char>) return "";
                else return L"";
            }
            return BasicString<CharT>(m_str.substr(0, m_str.length() - std::distance(m_str.rbegin(), it)).c_str());
        }

        template<>
        inline BasicString<wchar_t> BasicString<wchar_t>::TrimEnd() const {
            auto it = std::find_if(m_str.rbegin(), m_str.rend(), [](wchar_t ch) {
                return !std::iswspace(ch);
            });
            if (it == m_str.rend()) return L"";
            return BasicString<wchar_t>(m_str.substr(0, m_str.length() - std::distance(m_str.rbegin(), it)).c_str());
        }

        template <class CharT>
        inline BasicString<CharT> operator+(const CharT* pStr, const BasicString<CharT>& sStr) {
            if constexpr (std::is_same_v<CharT, char>) {
                BasicString<CharT> newStr(pStr ? pStr : "");
                return newStr + sStr;
            } else {
                BasicString<CharT> newStr(pStr ? pStr : L"");
                return newStr + sStr;
            }
        }

        template <class CharT>
        inline BasicString<CharT> operator+(CharT ch, const BasicString<CharT>& sStr) {
            CharT buf[2] = { ch, 0 };
            BasicString<CharT> newStr(buf);
            return newStr + sStr;
        }

        template <class CharT>
        template <typename T>
        inline BasicString<CharT> BasicString<CharT>::ToStringHelper(const T& val) {
            using DecayedT = std::decay_t<T>;
            if constexpr (std::is_same_v<DecayedT, BasicString<CharT>>) {
                return val;
            } else if constexpr (std::is_same_v<DecayedT, std::basic_string<CharT>>) {
                return BasicString<CharT>(val.c_str());
            } else if constexpr (std::is_same_v<DecayedT, std::nullptr_t>) {
                if constexpr (std::is_same_v<CharT, char>) {
                    return BasicString<CharT>("");
                } else {
                    return BasicString<CharT>(L"");
                }
            } else if constexpr (std::is_same_v<DecayedT, bool>) {
                return val ? BoolRepresentation<CharT>::True : BoolRepresentation<CharT>::False;
            } else if constexpr (has_ToString_v<DecayedT>) {
                return val.ToString();
            } else {
                std::basic_ostringstream<CharT> ss;
                ss << val;
                return BasicString<CharT>(ss.str());
            }
        }

        template <class CharT, typename T>
        inline auto FormatArgHelper(const T& val) {
            using DecayedT = std::decay_t<T>;
            if constexpr (std::is_same_v<DecayedT, BasicString<CharT>>) {
                return val.GetRawString();
            } else if constexpr (std::is_same_v<DecayedT, std::nullptr_t> || std::is_same_v<DecayedT, const char*> || std::is_same_v<DecayedT, const wchar_t*> || std::is_same_v<DecayedT, char*> || std::is_same_v<DecayedT, wchar_t*>) {
                if constexpr (std::is_same_v<DecayedT, std::nullptr_t>) {
                    if constexpr (std::is_same_v<CharT, char>) return "";
                    else return L"";
                } else {
                    if (val == nullptr) {
                        if constexpr (std::is_same_v<CharT, char>) return "";
                        else return L"";
                    }
                    return val;
                }
            } else if constexpr (std::is_same_v<DecayedT, bool>) {
                return val ? BoolRepresentation<CharT>::True : BoolRepresentation<CharT>::False;
            } else {
                return val;
            }
        }

        template <class CharT>
        template <class... Args>
        inline BasicString<CharT> BasicString<CharT>::Format(const BasicString<CharT>& sFormat, const Args&... args) {
            return Format(sFormat.GetRawString(), args...);
        }

        template <class CharT>
        template <class... Args>
        inline BasicString<CharT> BasicString<CharT>::Format(const CharT* pFormat, const Args&... args) {
            if (pFormat == nullptr) {
                throw ArgumentException("Invalid format string pointer");
            }
            try {
                if constexpr (sizeof...(Args) == 0) {
                    return BasicString<CharT>(pFormat);
                } else {
                    auto argsTuple = std::make_tuple(FormatArgHelper<CharT>(args)...);
                    std::basic_string<CharT> resultStr = std::apply([&](const auto&... formattedArgs) {
                        if constexpr (std::is_same_v<CharT, char>) {
                            return std::vformat(std::string_view(pFormat), std::make_format_args(formattedArgs...));
                        } else {
                            return std::vformat(std::wstring_view(pFormat), std::make_wformat_args(formattedArgs...));
                        }
                    }, argsTuple);
                    return BasicString<CharT>(resultStr.c_str());
                }
            } catch (const std::format_error&) {
                throw BasicFormatException<CharT>(FormatExceptionMessage<CharT>::Message);
            }
        }
        template <class CharT>
        inline void PrintTo(const BasicString<CharT>& sStr, ::std::ostream* os) {
            if (os) {
                *os << "\"" << (sStr.GetRawString() ? sStr.GetRawString() : "") << "\"";
            }
        }
    }  // namespace System
}  // namespace DotNetDupe

template <class CharT>
inline void PrintTo(const DotNetDupe::System::BasicString<CharT>& sStr, ::std::ostream* os) {
    if (os) {
        *os << "\"" << (sStr.GetRawString() ? sStr.GetRawString() : "") << "\"";
    }
}

namespace std {
    template<class CharT>
    struct hash<DotNetDupe::System::BasicString<CharT>> {
        size_t operator()(const DotNetDupe::System::BasicString<CharT>& s) const {
            return hash<basic_string<CharT>>()(s.GetRawString());
        }
    };

    template<class CharT>
    struct formatter<DotNetDupe::System::BasicString<CharT>, CharT> : formatter<basic_string_view<CharT>, CharT> {
        template<typename FormatContext>
        auto format(const DotNetDupe::System::BasicString<CharT>& s, FormatContext& ctx) const {
            return formatter<basic_string_view<CharT>, CharT>::format(
                basic_string_view<CharT>(s.GetRawString(), static_cast<size_t>(s.GetLength())), ctx);
        }
    };
}

