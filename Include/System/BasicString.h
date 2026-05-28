#pragma once

#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/NotImplementedException.h"
#include "System/OverflowException.h"
#include "Utils.h"

#include "System/Array.h"
#include <algorithm>
#include <iomanip>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <cwctype>

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

        template <class CharT>
        class BasicString {
        public:
            BasicString() = default;
            BasicString(const CharT* pStr);
            BasicString(const BasicString<CharT>& str) = default;
            BasicString<CharT>& operator=(const BasicString<CharT>& str) = default;
            BasicString<CharT>(const BasicString<CharT>&& str) noexcept;
            BasicString<CharT>& operator=(const BasicString<CharT>&& str) noexcept;
            const CharT* GetRawString() const;
            operator const CharT* () const { return GetRawString(); }
            int GetLength() const;
            std::basic_string<CharT>& GetString();
            BasicString<CharT> Clone() const;

            friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const BasicString<CharT>& str) {
                os << str.GetRawString();
                return os;
            }

            friend std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is, BasicString<CharT>& str) {
                std::basic_string<CharT> s;
                is >> s;
                str = s.c_str();
                return is;
            }

            bool operator<(const BasicString<CharT>& str) const;
            bool operator==(const BasicString<CharT>& str) const { return m_str.compare(str.m_str) == 0; }
            bool operator==(const CharT* str) const { return m_str.compare(str) == 0; }
            friend bool operator==(const CharT* str1, const BasicString<CharT>& str2) { return str2 == str1; }
            bool operator!=(const BasicString<CharT>& str) const { return !(*this == str); }
            bool operator!=(const CharT* str) const { return !(*this == str); }
            friend bool operator!=(const CharT* str1, const BasicString<CharT>& str2) { return !(str2 == str1); }
            CharT operator[](int index) const;
            int static Compare(const BasicString<CharT>& str1, int index1,
                               const BasicString<CharT>& str2, int index2, int length,
                               bool ignoreCase);
            int CompareTo(const BasicString& str) const;
            BasicString<CharT> operator+(const BasicString<CharT>& str) const;
            BasicString<CharT> Concat(
                const std::initializer_list<BasicString<CharT>> strs) const;

            bool Contains(CharT c) const;
            bool Contains(const BasicString<CharT>& str);

            void CopyTo(int sourceIndex, CharT* pDestination, int destinationIndex,
                        int destArraySize, int count) const;

            bool EndsWith(CharT c, bool ignoreCase) const;
            bool EndsWith(const BasicString<CharT>& suffix, bool ignoreCase) const;

            bool static Equals(const BasicString<CharT>& str1,
                               const BasicString<CharT>& str2);
            bool Equals(const BasicString<CharT>& str) const;

            int IndexOf(const BasicString<CharT>& substring) const;
            int IndexOf(const BasicString<CharT>& substring, bool ignoreCase) const;
            int IndexOf(const BasicString<CharT>& substring, int startIndex,
                        bool ignoreCase) const;
            int IndexOfAny(int startIndex, std::initializer_list<CharT> chars);

            BasicString<CharT>& Append(const CharT ch);
            BasicString<CharT>& Append(const BasicString<CharT>& str);
            BasicString<CharT>& Insert(int index, const BasicString<CharT>& str);

            bool IsEmpty() const;

            BasicString<CharT> static Join(
                CharT separator, std::initializer_list<BasicString<CharT>> strings);
            BasicString<CharT> static Join(
                CharT separator, std::initializer_list<BasicString<CharT>> strings,
                int startIndex, int count);
            BasicString<CharT> static Join(
                const BasicString<CharT>& separator,
                std::initializer_list<BasicString<CharT>> strings);
            BasicString<CharT> static Join(
                const BasicString<CharT>& separator,
                std::initializer_list<BasicString<CharT>> strings, int startIndex,
                int count);

            int LastIndexOf(const BasicString<CharT>& str, bool ignoreCase);
            int LastIndexOfAny(int startIndex, std::initializer_list<CharT> chars,
                               bool ignoreCase);

            BasicString<CharT> PadLeft(int totalWidth);
            BasicString<CharT> PadLeft(int totalWidth, CharT ch);
            BasicString<CharT> PadRight(int totalWidth);
            BasicString<CharT> PadRight(int totalWidth, CharT ch);

            BasicString<CharT> Remove(int startIndex) const;
            BasicString<CharT> Remove(int startIndex, int count) const;

            BasicString<CharT> Replace(CharT originalChar, CharT replaceChar);
            BasicString<CharT> Replace(const BasicString<CharT>& originalStr,
                                       const BasicString<CharT>& replaceStr);
            Array<BasicString<CharT>> Split(CharT separator);
            Array<BasicString<CharT>> Split(BasicString<CharT> separator [], int count,
                                                  StringSplitOptions options);
            bool StartsWith(const BasicString<CharT>& prefix, bool ignoreCase) const;
            BasicString<CharT> Substring(int startIndex, int length) const;

            BasicString<CharT> ToLower() const;
            BasicString<CharT> ToUpper() const;
            BasicString<CharT> Trim() const;
            BasicString<CharT> TrimStart() const;
            BasicString<CharT> TrimEnd() const;
        private:
            std::basic_string<CharT> m_str;
        };

        template <class CharT>
        inline BasicString<CharT>::BasicString(const CharT* pStr) {
            if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
            m_str = pStr;
        }

        template<class CharT>
        inline BasicString<CharT>::BasicString(const BasicString<CharT>&& str) noexcept : m_str(std::move(str.m_str)) { }
        template<class CharT>
        inline BasicString<CharT>& BasicString<CharT>::operator=(const BasicString<CharT>&& str) noexcept {
            if (this != &str) {
                m_str = std::move(str.m_str);
            }
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
        inline std::basic_string<CharT>& BasicString<CharT>::GetString() {
            return m_str;
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Clone() const {
            return BasicString(GetRawString());
        }
        template <class CharT>
        inline bool BasicString<CharT>::operator<(
            const BasicString<CharT>& str) const {
            return m_str < str.m_str;
        }

        template<class CharT>
        inline CharT BasicString<CharT>::operator[](int index) const {
            if (index >= (int)m_str.size()) throw ArgumentOutOfRangeException("Invalid index");
            return GetRawString() [index];
        }
        template <class CharT>
        inline int BasicString<CharT>::Compare(const BasicString<CharT>& str1,
                                               int index1,
                                               const BasicString<CharT>& str2,
                                               int index2, int length,
                                               bool ignoreCase) {
            if (!ignoreCase) {
                return str1.m_str.compare(index1, length, str2.m_str, index2, length);
            }

            auto s1 = str1.m_str.substr(index1, length);
            auto s2 = str2.m_str.substr(index2, length);
            std::transform(s1.begin(), s1.end(), s1.begin(),
                           [](CharT c) { return static_cast<CharT>(std::tolower(static_cast<unsigned char>(c))); });
            std::transform(s2.begin(), s2.end(), s2.begin(),
                           [](CharT c) { return static_cast<CharT>(std::tolower(static_cast<unsigned char>(c))); });
            return s1.compare(s2);
        }

        // Specialization for wchar_t to use towlower
        template<>
        inline int BasicString<wchar_t>::Compare(const BasicString<wchar_t>& str1,
                                               int index1,
                                               const BasicString<wchar_t>& str2,
                                               int index2, int length,
                                               bool ignoreCase) {
            if (!ignoreCase) {
                return str1.m_str.compare(index1, length, str2.m_str, index2, length);
            }

            auto s1 = str1.m_str.substr(index1, length);
            auto s2 = str2.m_str.substr(index2, length);
            std::transform(s1.begin(), s1.end(), s1.begin(),
                           [](wchar_t c) { return static_cast<wchar_t>(std::towlower(c)); });
            std::transform(s2.begin(), s2.end(), s2.begin(),
                           [](wchar_t c) { return static_cast<wchar_t>(std::towlower(c)); });
            return s1.compare(s2);
        }

        template <class CharT>
        inline int BasicString<CharT>::CompareTo(const BasicString<CharT>& str) const {
            return m_str.compare(str.GetRawString());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Concat(
            const std::initializer_list<BasicString<CharT>> strs) const {
            BasicString<CharT> newStr = *this;
            for (auto str : strs) {
                newStr.m_str.append(str.GetRawString());
            }
            return newStr;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::operator+(
            const BasicString<CharT>& str) const {
            return Concat({ str });
        }
        template <class CharT>
        inline bool BasicString<CharT>::Contains(CharT c) const {
            return m_str.find(c) != std::basic_string<CharT>::npos;
        }
        template <class CharT>
        inline bool BasicString<CharT>::Contains(const BasicString<CharT>& str) {
            return m_str.find(str.m_str) != std::basic_string<CharT>::npos;
        }

        template <class CharT>
        inline void BasicString<CharT>::CopyTo(int sourceIndex, CharT* pDestination,
                                               int destinationIndex, int destArraySize,
                                               int count) const {
            if (nullptr == pDestination)
                throw ArgumentException("Invalid destination buffer");
            int len = GetLength();
            if (sourceIndex < 0 || sourceIndex >= len)
                throw ArgumentOutOfRangeException("Invalid source index");
            if (count > len)
                throw ArgumentOutOfRangeException(
                    "Source array size is smaller than count");
            if (count > destArraySize)
                throw ArgumentOutOfRangeException(
                    "Destination array is smaller than count");
            m_str.copy(pDestination + destinationIndex, count, sourceIndex);
        }
        template <class CharT>
        inline bool BasicString<CharT>::EndsWith(CharT c, bool ignoreCase) const {
            auto len = m_str.length();
            if (len == 0) return false;
            if (ignoreCase) {
                return CaseInsensitiveCompare<CharT>::Equals(m_str [len - 1], c);
            }
            return m_str [len - 1] == c;
        }
        template <class CharT>
        inline bool BasicString<CharT>::EndsWith(const BasicString<CharT>& suffix,
                                                 bool ignoreCase) const {
            auto len = m_str.length();
            auto suffixLen = suffix.GetLength();
            if (suffixLen > len) return false;

            if (ignoreCase) {
                return Compare(*this, (int)(len - suffixLen), suffix, 0, (int)suffixLen, true) == 0;
            }
            return m_str.compare(len - suffixLen, suffixLen, suffix.m_str) == 0;
        }
        template <class CharT>
        inline bool BasicString<CharT>::Equals(const BasicString<CharT>& str1,
                                               const BasicString<CharT>& str2) {
            return str1 == str2;
        }
        template <class CharT>
        inline bool BasicString<CharT>::Equals(const BasicString<CharT>& str) const {
            return *this == str;
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& substring) const {
            return IndexOf(substring, 0, false);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& substring,
                                               bool ignoreCase) const {
            return IndexOf(substring, 0, ignoreCase);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& substring,
                                               int startIndex, bool ignoreCase) const {
            if (startIndex < 0 || startIndex > GetLength())
                throw ArgumentOutOfRangeException("Invalid startIndex");
            
            if (substring.IsEmpty()) return startIndex;

            if (!ignoreCase) {
                auto pos = m_str.find(substring.m_str, startIndex);
                return (pos == std::basic_string<CharT>::npos) ? -1 : (int)pos;
            }

            // Case-insensitive search
            auto it = std::search(
                m_str.begin() + startIndex, m_str.end(),
                substring.m_str.begin(), substring.m_str.end(),
                [](CharT c1, CharT c2) { return CaseInsensitiveCompare<CharT>::Equals(c1, c2); }
            );

            return (it == m_str.end()) ? -1 : (int)std::distance(m_str.begin(), it);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOfAny(int startIndex,
                                                  std::initializer_list<CharT> chars) {
            if (startIndex < 0 || startIndex > GetLength())
                throw ArgumentOutOfRangeException("Invalid startIndex");
            
            auto pos = m_str.find_first_of(std::basic_string<CharT>(chars.begin(), chars.end()), startIndex);
            return (pos == std::basic_string<CharT>::npos) ? -1 : (int)pos;
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::Append(const CharT ch) {
            m_str += ch;
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::Append(
            const BasicString<CharT>& str) {
            m_str.append(str.GetRawString());
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::Insert(
            int index, const BasicString<CharT>& str) {
            int len = GetLength();
            if (index < 0 || index > len)
                throw ArgumentOutOfRangeException("Invalid index");

            m_str.insert(index, str.GetRawString(), str.GetLength());
            return *this;
        }
        template <class CharT>
        inline bool BasicString<CharT>::IsEmpty() const {
            return m_str.empty();
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            CharT separator, std::initializer_list<BasicString<CharT>> strings) {
            return Join(separator, strings, 0, (int)strings.size());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            CharT separator, std::initializer_list<BasicString<CharT>> strings,
            int startIndex, int count) {
            if (startIndex < 0 || startIndex > (int)strings.size())
                throw ArgumentOutOfRangeException("Invalid startIndex");
            if (count < 0 || (startIndex + count) > (int)strings.size())
                throw ArgumentOutOfRangeException("Invalid count");

            auto strs = _init_list_with_indexer<BasicString<CharT>>(strings);
            BasicString<CharT> joinStr("");

            for (int i = startIndex; i < startIndex + count; i++) {
                joinStr.Append(strs [i]);
                if (i != startIndex + count - 1) {
                    joinStr.Append(separator);
                }
            }
            return joinStr;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            const BasicString<CharT>& separator,
            std::initializer_list<BasicString<CharT>> strings) {
            return Join(separator, strings, 0, (int)strings.size());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            const BasicString<CharT>& separator,
            std::initializer_list<BasicString<CharT>> strings, int startIndex,
            int count) {
            if (startIndex < 0 || startIndex > (int)strings.size())
                throw ArgumentOutOfRangeException("Invalid startIndex");
            if (count < 0 || (startIndex + count) > (int)strings.size())
                throw ArgumentOutOfRangeException("Invalid count");

            auto strs = _init_list_with_indexer<BasicString<CharT>>(strings);
            BasicString<CharT> joinStr("");

            for (int i = startIndex; i < startIndex + count; i++) {
                joinStr.Append(strs [i]);
                if (i != startIndex + count - 1) {
                    joinStr.Append(separator);
                }
            }
            return joinStr;
        }
        template <class CharT>
        inline int BasicString<CharT>::LastIndexOf(const BasicString<CharT>& str,
                                                   bool ignoreCase) {
            if (str.IsEmpty()) return GetLength();

            if (!ignoreCase) {
                auto pos = m_str.rfind(str.m_str);
                return (pos == std::basic_string<CharT>::npos) ? -1 : (int)pos;
            }

            auto it = std::find_end(
                m_str.begin(), m_str.end(),
                str.m_str.begin(), str.m_str.end(),
                [](CharT c1, CharT c2) { return CaseInsensitiveCompare<CharT>::Equals(c1, c2); }
            );

            return (it == m_str.end()) ? -1 : (int)std::distance(m_str.begin(), it);
        }
        template <class CharT>
        inline int BasicString<CharT>::LastIndexOfAny(
            int startIndex, std::initializer_list<CharT> chars, bool ignoreCase) {
            if (startIndex < 0 || startIndex > GetLength())
                throw ArgumentOutOfRangeException("Invalid startIndex");
            
            if (IsEmpty()) return -1;

            for (auto ch : chars) {
                size_t lastFound = std::basic_string<CharT>::npos;
                for (int i = GetLength() - 1; i >= startIndex; --i) {
                    bool match = false;
                    if (ignoreCase) {
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
        inline BasicString<CharT> BasicString<CharT>::PadLeft(int totalWidth) {
            return PadLeft(totalWidth, (CharT)' ');
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadLeft(int totalWidth,
                                                              CharT ch) {
            if (totalWidth < 0) {
                throw ArgumentException("Invalid totalWidth");
            }
            int len = GetLength();
            if (totalWidth <= len) {
                return *this;
            }
            std::basic_string<CharT> padding(totalWidth - len, ch);
            m_str.insert(0, padding);
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadRight(int totalWidth) {
            return PadRight(totalWidth, (CharT)' ');
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadRight(int totalWidth,
                                                               CharT ch) {
            if (totalWidth < 0) {
                throw ArgumentException("Invalid totalWidth");
            }
            int len = GetLength();
            if (totalWidth <= len) {
                return *this;
            }
            m_str.append(totalWidth - len, ch);
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Remove(int startIndex) const {
            return Remove(startIndex, GetLength() - startIndex);
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Remove(int startIndex,
                                                             int count) const {
            int len = GetLength();
            if (startIndex < 0 || startIndex > len || count < 0 || (startIndex + count) > len) {
                throw ArgumentOutOfRangeException("Invalid startIndex or count");
            }

            std::basic_string<CharT> ret = m_str;
            ret.erase(startIndex, count);
            return BasicString<CharT>(ret.c_str());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Replace(CharT originalChar,
                                                              CharT replaceChar) {
            std::basic_string<CharT> ret = m_str;
            std::replace(ret.begin(), ret.end(), originalChar, replaceChar);
            return BasicString<CharT>(ret.c_str());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Replace(
            const BasicString<CharT>& originalStr,
            const BasicString<CharT>& replaceStr) {
            if (originalStr.IsEmpty()) return *this;

            std::basic_string<CharT> ret = m_str;
            size_t pos = 0;
            while ((pos = ret.find(originalStr.m_str, pos)) != std::basic_string<CharT>::npos) {
                ret.replace(pos, originalStr.GetLength(), replaceStr.m_str);
                pos += replaceStr.GetLength();
            }
            return BasicString<CharT>(ret.c_str());
        }
        template<class CharT>
        inline Array<BasicString<CharT>> BasicString<CharT>::Split(CharT separator)
        {
            std::vector<BasicString<CharT>> tempResult;
            std::basic_stringstream<CharT> ss(m_str);
            std::basic_string<CharT> token;
            while (std::getline(ss, token, separator))
            {
                tempResult.push_back(BasicString<CharT>(token.c_str()));
            }
            
            Array<BasicString<CharT>> result((int)tempResult.size());
            for (int i = 0; i < (int)tempResult.size(); i++) result[i] = tempResult[i];
            return result;
        }

        template<class CharT>
        inline Array<BasicString<CharT>> BasicString<CharT>::Split(BasicString<CharT> separator [], int count,
                                                                         StringSplitOptions options) {
            std::vector<BasicString<CharT>> tempResult;
            std::set<CharT> charSet;
            for (int i = 0; i < count; ++i) {
                for (auto c : separator [i].m_str) charSet.insert(c);
            }
            std::basic_string<CharT> current;
            for (auto c : m_str) {
                if (charSet.find(c) == charSet.end()) {
                    current += c;
                }
                else {
                    BasicString<CharT> s(current.c_str());
                    if (options == StringSplitOptions::TrimEntries) s = s.Trim();
                    if (options != StringSplitOptions::RemoveEmptyEntries || !s.IsEmpty()) {
                        tempResult.push_back(s);
                    }
                    current.clear();
                }
            }
            BasicString<CharT> s(current.c_str());
            if (options == StringSplitOptions::TrimEntries) s = s.Trim();
            if (options != StringSplitOptions::RemoveEmptyEntries || !s.IsEmpty()) {
                tempResult.push_back(s);
            }

            Array<BasicString<CharT>> result((int)tempResult.size());
            for (int i = 0; i < (int)tempResult.size(); i++) result[i] = tempResult[i];
            return result;
        }

        template <class CharT>
        inline bool BasicString<CharT>::StartsWith(const BasicString<CharT>& prefix, bool ignoreCase) const {
            if (prefix.GetLength() > GetLength()) return false;
            
            if (!ignoreCase) {
                return m_str.compare(0, prefix.GetLength(), prefix.m_str) == 0;
            }

            return Compare(*this, 0, prefix, 0, prefix.GetLength(), true) == 0;
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Substring(int startIndex, int length) const {
            int len = GetLength();
            if (startIndex < 0 || startIndex > len || length < 0 || (startIndex + length) > len) {
                throw ArgumentOutOfRangeException("Invalid startIndex or length");
            }
            return BasicString<CharT>(m_str.substr(startIndex, length).c_str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToLower() const {
            std::basic_string<CharT> ret = m_str;
            std::transform(ret.begin(), ret.end(), ret.begin(), [](CharT c) { return static_cast<CharT>(std::tolower(static_cast<unsigned char>(c))); });
            return BasicString<CharT>(ret.c_str());
        }

        template<>
        inline BasicString<wchar_t> BasicString<wchar_t>::ToLower() const {
            std::basic_string<wchar_t> ret = m_str;
            std::transform(ret.begin(), ret.end(), ret.begin(), [](wchar_t c) { return static_cast<wchar_t>(std::towlower(c)); });
            return BasicString<wchar_t>(ret.c_str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::ToUpper() const {
            std::basic_string<CharT> ret = m_str;
            std::transform(ret.begin(), ret.end(), ret.begin(), [](CharT c) { return static_cast<CharT>(std::toupper(static_cast<unsigned char>(c))); });
            return BasicString<CharT>(ret.c_str());
        }

        template<>
        inline BasicString<wchar_t> BasicString<wchar_t>::ToUpper() const {
            std::basic_string<wchar_t> ret = m_str;
            std::transform(ret.begin(), ret.end(), ret.begin(), [](wchar_t c) { return static_cast<wchar_t>(std::towupper(c)); });
            return BasicString<wchar_t>(ret.c_str());
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Trim() const {
            return TrimStart().TrimEnd();
        }

        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::TrimStart() const {
            auto it = std::find_if(m_str.begin(), m_str.end(), [](CharT ch) {
                return !std::isspace(static_cast<unsigned char>(ch));
            });
            if (it == m_str.end()) return "";
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
                return !std::isspace(static_cast<unsigned char>(ch));
            });
            if (it == m_str.rend()) return "";
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
    }  // namespace System
}  // namespace DotNetDupe

namespace std {
    template<class CharT>
    struct hash<DotNetDupe::System::BasicString<CharT>> {
        size_t operator()(const DotNetDupe::System::BasicString<CharT>& s) const {
            return hash<basic_string<CharT>>()(s.GetRawString());
        }
    };
}
