#pragma once

#ifndef _INC_TCHAR
#ifdef UNICODE
#define _T(x) L ## x
#else
#define _T(x) ## x
#endif // UNICODE
#endif


#include "ArgumentException.h"
#include "ArgumentOutOfRangeException.h"
#include "NotImplementedException.h"
#include "OverflowException.h"
#include "Utils.h"

#include <algorithm>
#include <iomanip>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <set>

#include <Shlwapi.h>

namespace DotNetDupe {
    namespace System {
        enum class StringSplitOptions {
            None,
            RemoveEmptyEntries,
            TrimEntries
        };
        template <class CharT>
        class BasicString {
        public:
            BasicString(const CharT* pStr);
            BasicString(const BasicString<CharT>& str) = default;
            BasicString<CharT>& operator=(const BasicString<CharT>& str) = default;
            BasicString<CharT>(const BasicString<CharT>&& str) noexcept;
            BasicString<CharT>& operator=(const BasicString<CharT>&& str) noexcept;
            const CharT* GetRawString() const;
            int GetLength() const;
            std::basic_string<CharT>& GetString();
            BasicString<CharT> Clone() const;

            bool operator==(const BasicString<CharT>& str) const;
            CharT operator[](int index) const;
            int static Compare(const BasicString<CharT>& str1, int index1,
                               const BasicString<CharT>& str2, int index2, int length,
                               bool ignoreCase);
            int CompareTo(const BasicString& str) const;
            BasicString<CharT>& operator+(const BasicString<CharT>& str);
            BasicString<CharT>& Concat(
                const std::initializer_list<BasicString<CharT>> strs);

            bool Contains(CharT c) const;
            bool Contains(const BasicString<CharT>& str);

            void CopyTo(int sourceIndex, CharT* pDestination, int destinationIndex,
                        int destArraySize, int count) const;

            bool EndsWith(CharT c, bool ignoreCase) const;
            bool EndsWith(const BasicString<CharT>& suffix, bool ignoreCase) const;

            bool static Equals(const BasicString<CharT>& str1,
                               const BasicString<CharT>& str2);
            bool Equals(const BasicString<CharT>& str) const;

            int IndexOf(const BasicString<CharT>& substring);
            int IndexOf(const BasicString<CharT>& substring, bool ignoreCase);
            int IndexOf(const BasicString<CharT>& substring, int startIndex,
                        bool ignoreCase);
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
            BasicString<CharT> Replace(const BasicString<CharT>& orignalStr,
                                       const BasicString<CharT>& replaceStr);
            std::vector<BasicString<CharT>> Split(BasicString<CharT> separator [], int count, StringSplitOptions options);
        private:
            std::basic_string<CharT> m_str;
        };

        template <class CharT>
        inline BasicString<CharT>::BasicString(const CharT* pStr) {
            if (pStr == nullptr) throw ArgumentException(_T("Invalid input pointer"));
            m_str = pStr;
        }

        template<class CharT>
        inline BasicString<CharT>::BasicString(const BasicString<CharT>&& str) noexcept : m_str(std::move(str.m_str)) { 
        }
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
        inline bool BasicString<CharT>::operator==(
            const BasicString<CharT>& str) const {
            return m_str.compare(str.GetRawString()) == 0;
        }
        template<class CharT>
        inline CharT BasicString<CharT>::operator[](int index) const {
            if (index >= m_str.size()) throw ArgumentOutOfRangeException(_T("Invalid index"));
            return GetRawString() [index];
        }
        template <class CharT>
        inline int BasicString<CharT>::Compare(const BasicString<CharT>& str1,
                                               int index1,
                                               const BasicString<CharT>& str2,
                                               int index2, int length,
                                               bool ignoreCase) {
            auto basicStr1 = std::basic_string<CharT>(str1.GetRawString());
            auto basicStr2 = std::basic_string<CharT>(str2.GetRawString());
            if (!ignoreCase) {
                return basicStr1.compare(index1, length, basicStr2, index2, length);
            }

            std::transform(basicStr1.begin(), basicStr1.end(), basicStr1.begin(),
                           [](CharT c) { return static_cast<CharT> (std::tolower(c)); });
            std::transform(basicStr2.begin(), basicStr2.end(), basicStr2.begin(),
                           [](CharT c) { return static_cast<CharT>(std::tolower(c)); });
            return basicStr1.compare(index1, length, basicStr2, index2, length);
        }
        template <class CharT>
        inline int BasicString<CharT>::CompareTo(const BasicString& str) const {
            return m_str.compare(str.GetRawString());
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::Concat(
            const std::initializer_list<BasicString<CharT>> strs) {
            for (auto str : strs) {
                m_str.append(str.GetString());
            }
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT>& BasicString<CharT>::operator+(
            const BasicString<CharT>& str) {
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
                throw ArgumentException(_T("Invalid destination buffer"));
            int len = GetLength();
            if (sourceIndex < 0 || sourceIndex >= len)
                throw ArgumentOutOfRangeException(_T("Invalid source index"));
            if (count > len)
                throw ArgumentOutOfRangeException(
                    _T("Source array size is smaller than count"));
            if (count > destArraySize)
                throw ArgumentOutOfRangeException(
                    _T("Destination array is smaller than count"));
            m_str.copy(pDestination + destinationIndex, count, sourceIndex);
        }
        template <class CharT>
        inline bool BasicString<CharT>::EndsWith(CharT c, bool ignoreCase) const {
            auto len = m_str.length();
            if (ignoreCase) {
                c = tolower(c);
                return tolower(m_str [len - 1]) == c;
            }
            return m_str [len - 1] == c;
        }
        template <class CharT>
        inline bool BasicString<CharT>::EndsWith(const BasicString<CharT>& suffix,
                                                 bool ignoreCase) const {
            auto len = m_str.length();
            auto suffixLen = suffix.GetLength();
            if (ignoreCase) {
                return _tcsicmp(m_str.c_str() + len - suffixLen, suffix.GetRawString()) == 0;
            }
            return _tcscmp(m_str.c_str() + len - suffixLen, suffix.GetRawString()) == 0;
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
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& substring) {
            return IndexOf(substring, 0, false);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& substring,
                                               bool ignoreCase) {
            return IndexOf(substring, 0, ignoreCase);
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOf(const BasicString<CharT>& substring,
                                               int startIndex, bool ignoreCase) {
            if (startIndex < 0 || startIndex >= GetLength())
                throw ArgumentOutOfRangeException(_T("Invalid startIndex"));
            CharT* pSubstring = NULL;
            if (ignoreCase) {
                pSubstring = StrStrI(m_str.c_str() + startIndex, substring.GetRawString());
            }
            else {
                pSubstring = StrStr(m_str.c_str() + startIndex, substring.GetRawString());
            }

            if (pSubstring != NULL) {
                auto index = pSubstring - m_str.c_str();
                
                if (index >= INT_MAX) throw OverflowException(_T("Index found out of range"));
                
                return static_cast<int>(index);
            }
            return -1;
        }
        template <class CharT>
        inline int BasicString<CharT>::IndexOfAny(int startIndex,
                                                  std::initializer_list<CharT> chars) {
            if (startIndex < 0 || startIndex >= GetLength())
                throw ArgumentOutOfRangeException(_T("Invalid startIndex"));
            for (auto ch : chars) {
                CharT* pChar = StrChrI(GetString().c_str() + startIndex, ch);
                if (pChar != NULL) {
                    auto index = pChar - GetString().c_str();
                
                    if (index >= INT_MAX) throw OverflowException(_T("Index found out of range"));
                    
                    return static_cast<int>(index);
                }
            }
            return -1;
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
            if (index < 0 || index >= len)
                throw ArgumentOutOfRangeException(_T("Invalid index"));

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
            return Join(separator, strings, 0, (int) strings.size());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            CharT separator, std::initializer_list<BasicString<CharT>> strings,
            int startIndex, int count) {
            if (startIndex < 0 || startIndex >= count)
                throw ArgumentOutOfRangeException(_T("Invalid startIndex"));
            if (count < 0 || count >(int)strings.size())
                throw ArgumentOutOfRangeException(_T("Invalid count"));

            auto strs = _init_list_with_indexer<BasicString<CharT>>(strings);
            count = min(startIndex + count, (int)strings.size());

            BasicString<CharT> joinStr(_T(""));

            for (int i = startIndex; i < count; i++) {
                joinStr.Append(strs [i]);
                if (i != count - 1) {
                    joinStr.Append(separator);
                }
            }
            return joinStr;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            const BasicString<CharT>& separator,
            std::initializer_list<BasicString<CharT>> strings) {
            return Join(separator, strings, 0, (int) strings.size());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Join(
            const BasicString<CharT>& separator,
            std::initializer_list<BasicString<CharT>> strings, int startIndex,
            int count) {
            if (startIndex < 0 || startIndex >= count)
                throw ArgumentOutOfRangeException(_T("Invalid startIndex"));
            if (count < 0 || count >(int)strings.size())
                throw ArgumentOutOfRangeException(_T("Invalid count"));

            auto strs = _init_list_with_indexer<BasicString<CharT>>(strings);
            count = min(startIndex + count, (int)strings.size());

            BasicString<CharT> joinStr(_T(""));

            for (int i = startIndex; i < count; i++) {
                joinStr.Append(strs [i]);
                if (i != count - 1) {
                    joinStr.Append(separator);
                }
            }
            return joinStr;
        }
        template <class CharT>
        inline int BasicString<CharT>::LastIndexOf(const BasicString<CharT>& str,
                                                   bool ignoreCase) {
            if (ignoreCase) {
                CharT* pStr = StrRStrI(m_str.c_str(), NULL, str.m_str.c_str());
                return pStr == NULL ? -1 : static_cast<int>(pStr - m_str.c_str());
            }
            throw NotImplementedException(
                _T("Case sensitive last index search not supported"));
        }
        template <class CharT>
        inline int BasicString<CharT>::LastIndexOfAny(
            int startIndex, std::initializer_list<CharT> chars, bool ignoreCase) {
            if (startIndex < 0 || startIndex >= GetLength())
                throw ArgumentOutOfRangeException(_T("Invalid startIndex"));
            for (auto ch : chars) {
                CharT* pChar = NULL;
                if (ignoreCase) {
                    pChar = StrRChrI(m_str.c_str() + startIndex, NULL, ch);
                }
                else {
                    pChar = StrRChr(m_str.c_str() + startIndex, NULL, ch);
                }
                if (pChar != NULL) {
                    auto index = pChar - m_str.c_str();
                    
                    if (index >= INT_MAX) throw OverflowException(_T("Index found out of range"));
                    
                    return static_cast<int>(index);
                }
            }
            return -1;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadLeft(int totalWidth) {
            return PadLeft(totalWidth, _T(' '));
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadLeft(int totalWidth,
                                                              CharT ch) {
            if (totalWidth < 0) {
                throw ArgumentException(_T("Invalid totalWidth"));
            }
            int len = GetLength();
            if (totalWidth <= len) {
                return *this;
            }
            std::basic_stringstream<CharT> ss;
            ss << std::setw(totalWidth) << std::setfill(ch) << m_str;
            m_str.assign(ss.str());
            return *this;
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadRight(int totalWidth) {
            return PadRight(totalWidth, _T(' '));
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::PadRight(int totalWidth,
                                                               CharT ch) {
            if (totalWidth < 0) {
                throw ArgumentException(_T("Invalid totalWidth"));
            }
            int len = GetLength();
            if (totalWidth <= len) {
                return *this;
            }

            std::basic_stringstream<CharT> ss;
            ss << std::setw(totalWidth) << std::left << std::setfill(ch) << m_str;
            m_str.assign(ss.str());

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
            if (startIndex < 0 || startIndex >= len || count > len) {
                throw ArgumentOutOfRangeException(_T("Invalid startIndex or count"));
            }
            if ((count - startIndex) == len) {
                return _T("");
            }

            std::basic_string<CharT> ret;
            ret.assign(m_str);
            ret.erase(startIndex, count);

            return BasicString<CharT>(ret.c_str());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Replace(CharT originalChar,
                                                              CharT replaceChar) {
            std::basic_string<CharT> ret;
            ret.assign(m_str);
            std::replace(ret.begin(), ret.end(), originalChar, replaceChar);

            return BasicString<CharT>(ret.c_str());
        }
        template <class CharT>
        inline BasicString<CharT> BasicString<CharT>::Replace(
            const BasicString<CharT>& orignalStr,
            const BasicString<CharT>& replaceStr) {
            std::basic_string<CharT> ret = std::regex_replace(
                m_str, std::basic_regex<CharT>(orignalStr.m_str), replaceStr.m_str);
            return ret.c_str();
        }
        template<class CharT>
        inline std::vector<BasicString<CharT>> BasicString<CharT>::Split(BasicString<CharT> separator [], int count, StringSplitOptions options) {
            std::vector<BasicString<CharT>> ss;
            std::set<CharT> charSet;
            for (auto c : separator) {
                charSet.insert(c);
            }
            BasicString<CharT> str;
            for (auto c : m_str) {
                if (!charSet.contains(c)) {
                    str += c;
                }
                else {
                    if (options == StringSplitOptions::RemoveEmptyEntries) {
                        if (!str.IsEmpty()) {
                            ss.push_back(str);
                        }
                    }
                    else if (options == StringSplitOptions::TrimEntries) {
                        //strings.push_back(str.Trim())
                    }
                }
            }
            
            return ss;
        }
        
}  // namespace System
}  // namespace DotNetDupe
