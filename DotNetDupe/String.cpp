#include "pch.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/Utils/StringConvert.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/NotImplementedException.h"
#include "System/OverflowException.h"
#include "System/FormatException.h"
#include <set>
#include <vector>
#include <initializer_list>
#include <sstream>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <string>


namespace DotNetDupe {
	namespace System {

		struct String::StringImpl {
			std::string s;
			StringImpl() = default;
			StringImpl(const char* str) : s(str) {}
			StringImpl(const std::string& str) : s(str) {}
			StringImpl(std::string&& str) : s(std::move(str)) {}

		};


		namespace {
			template <typename T>
			inline String ToStringHelper(const T& val) {
				using DecayedT = std::decay_t<T>;
				if constexpr (std::is_same_v<DecayedT, String>) {
					return val;
				}
				else if constexpr (std::is_same_v<DecayedT, std::string>) {
					return String(val.c_str());
				}
				else if constexpr (std::is_same_v<DecayedT, std::nullptr_t>) {
					return String("");
				}
				else if constexpr (std::is_same_v<DecayedT, bool>) {
					return val ? "True" : "False";
				}
				else {
					std::ostringstream ss;
					ss << val;
					return String(ss.str().c_str());
				}
			}
		}

		void ThrowArgumentException(const char* msg) {
			throw ArgumentException(msg);
		}

		void ThrowFormatException(const char* msg) {
			throw FormatException(msg);
		}
		String::String() { m_pImpl = new StringImpl("");  }
		String::~String() { delete m_pImpl; }
		static void AppendExplicitArg(const std::string& sFmt, size_t i, size_t end, const String* pArgs, int iArgCount, std::string& sRes) {
			std::string num = sFmt.substr(i + 1, end - i - 1);
			size_t idx = 0;
			int argIdx = std::stoi(num, &idx);
			if (idx != num.length() || argIdx < 0 || argIdx >= iArgCount) throw FormatException("Index out of bounds");
			sRes += pArgs[argIdx].GetRawString() ? pArgs[argIdx].GetRawString() : "";
		}

		static void FormatOpenBrace(const std::string& sFmt, size_t& i, int& iAutoIndex, const String* pArgs, int iArgCount, std::string& sRes) {
			if (i + 1 < sFmt.length() && sFmt[i+1] == '{') {
				sRes += '{'; i++;
			} else if (i + 1 < sFmt.length() && sFmt[i+1] == '}') {
				if (iAutoIndex >= iArgCount) throw FormatException("Index out of bounds");
				sRes += pArgs[iAutoIndex].GetRawString() ? pArgs[iAutoIndex].GetRawString() : "";
				iAutoIndex++; i++;
			} else {
				size_t end = sFmt.find('}', i + 1);
				if (end == std::string::npos) throw FormatException("Unclosed brace");
				AppendExplicitArg(sFmt, i, end, pArgs, iArgCount, sRes);
				i = end;
			}
		}

		static void FormatClosingBrace(const std::string& sFmt, size_t& i, std::string& sRes) {
			if (i + 1 < sFmt.length() && sFmt[i+1] == '}') {
				sRes += '}'; i++;
			} else {
				throw FormatException("Unescaped closing brace");
			}
		}

		String String::InternalFormat(const char* pFormat, const String* pArgs, int iArgCount) {
			if (!pFormat) throw ArgumentException("Format string cannot be null.");
			if (iArgCount == 0 || !pArgs) return String(pFormat);

			std::string sFmt = pFormat;
			std::string sRes;
			int iAutoIndex = 0;
			for (size_t i = 0; i < sFmt.length(); ++i) {
				if (sFmt[i] == '{') {
					FormatOpenBrace(sFmt, i, iAutoIndex, pArgs, iArgCount, sRes);
				} else if (sFmt[i] == '}') {
					FormatClosingBrace(sFmt, i, sRes);
				} else {
					sRes += sFmt[i];
				}
			}
			return String(sRes.c_str());
		}

		void* String::operator new(size_t size) {
			return ::operator new(size);
		}

		void String::operator delete(void* p) {
			::operator delete(p);
		}

		void* String::operator new[](size_t size) {
			return ::operator new[](size);
		}

		void String::operator delete[](void* p) {
			::operator delete[](p);
		}

		String String::operator+(const char* pStr) const {
			String sNewStr = *this;
			if (pStr) {
				sNewStr.m_pImpl->s.append(pStr);
			}
			return sNewStr;
		}

		String String::operator+(char ch) const {
			String sNewStr = *this;
			sNewStr.m_pImpl->s.push_back(ch);
			return sNewStr;
		}

		String& String::operator+=(const String& sStr) {
			m_pImpl->s.append(sStr.GetRawString());
			return *this;
		}

		String& String::operator+=(const char* pStr) {
			if (pStr) {
				m_pImpl->s.append(pStr);
			}
			return *this;
		}

		String& String::operator+=(char ch) {
			m_pImpl->s.push_back(ch);
			return *this;
		}

		String operator+(const char* pStr, const String& sStr) {
			String newStr(pStr ? pStr : "");
			return newStr + sStr;
		}

		String operator+(char ch, const String& sStr) {
			char buf[2] = { ch, 0 };
			String newStr(buf);
			return newStr + sStr;
		}


		String::String(const char* pStr) {
			if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
			m_pImpl = new StringImpl();
			m_pImpl->s = pStr;
		}

		String::String(const String& sStr) { m_pImpl = new StringImpl(sStr.m_pImpl->s);  }

		String& String::operator=(const String& sStr) {
			if (this != &sStr) {
				m_pImpl->s = sStr.m_pImpl->s;
			}
			return *this;
		}

		String::String(String&& sStr) noexcept { m_pImpl = new StringImpl(std::move(sStr.m_pImpl->s));  }
		String& String::operator=(String&& sStr) noexcept {
			if (this != &sStr) {
				m_pImpl->s = std::move(sStr.m_pImpl->s);
			}
			return *this;
		}
		String::String(const wchar_t* pStr) {
			if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
			m_pImpl = new StringImpl();

			m_pImpl->s = Utils::StringConvert::WCharToUtf8(pStr);
		}

		String& String::operator=(const wchar_t* pStr) {
			if (pStr == nullptr) throw ArgumentException("Invalid input pointer");

			m_pImpl->s = Utils::StringConvert::WCharToUtf8(pStr);

			return *this;
		}

		String& String::operator=(const char* pStr) {
			if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
			m_pImpl->s = pStr;
			return *this;
		}
		const char* String::GetRawString() const {
			return m_pImpl->s.c_str();
		}
		int String::GetHashCode() const {
			const char* p = GetRawString();
			if (!p) return 0;
			unsigned int hash = 2166136261u;
			while (*p) {
				hash ^= (unsigned char)(*p);
				hash *= 16777619u;
				++p;
			}
			return (int)hash;
		}
		int String::GetLength() const {
			return static_cast<int>(m_pImpl->s.length());
		}

		String String::Clone() const {
			return String(GetRawString());
		}


		char String::operator[](int iIndex) const {
			if (iIndex >= (int)m_pImpl->s.size()) throw ArgumentOutOfRangeException("Invalid iIndex");
			return GetRawString() [iIndex];
		}
		static std::string ToLowerString(const std::string& s) {
			std::string res = s;
			std::transform(res.begin(), res.end(), res.begin(), [](char ch) {
				return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
			});
			return res;
		}

		int String::Compare(const String& sStr1, int iIndex1, const String& sStr2, int iIndex2, int iLength, bool bIgnoreCase) {
			if (!bIgnoreCase) {
				return sStr1.m_pImpl->s.compare(iIndex1, iLength, sStr2.m_pImpl->s, iIndex2, iLength);
			}
			auto s1 = ToLowerString(sStr1.m_pImpl->s.substr(iIndex1, iLength));
			auto s2 = ToLowerString(sStr2.m_pImpl->s.substr(iIndex2, iLength));
			return s1.compare(s2);
		}

		int String::CompareTo(const String& sStr) const {
			return m_pImpl->s.compare(sStr.GetRawString());
		}
		String String::Concat(
			const std::initializer_list<String> sStrs) const {
			String sNewStr = *this;
			for (auto sStr : sStrs) {
				sNewStr.m_pImpl->s.append(sStr.GetRawString());
			}
			return sNewStr;
		}
		String String::operator+(
			const String& sStr) const {
			return Concat({ sStr });
		}
		bool String::Contains(char ch) const {
			return m_pImpl->s.find(ch) != std::string::npos;
		}
		bool String::Contains(const String& sStr) const {
			return m_pImpl->s.find(sStr.m_pImpl->s) != std::string::npos;
		}

		void String::CopyTo(int iSourceIndex, char* pDestination,
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
			m_pImpl->s.copy(pDestination + iDestinationIndex, iCount, iSourceIndex);
		}
		bool String::EndsWith(char ch, bool bIgnoreCase) const {
			auto iLen = m_pImpl->s.length();
			if (iLen == 0) return false;
			if (bIgnoreCase) {
				return std::tolower(static_cast<unsigned char>(m_pImpl->s [iLen - 1])) == std::tolower(static_cast<unsigned char>(ch));
			}
			return m_pImpl->s [iLen - 1] == ch;
		}
		bool String::EndsWith(const String& sSuffix,
							  bool bIgnoreCase) const {
			int iLen = GetLength();
			int iSuffixLen = sSuffix.GetLength();
			if (iSuffixLen > iLen) return false;

			if (bIgnoreCase) {
				return Compare(*this, iLen - iSuffixLen, sSuffix, 0, iSuffixLen, true) == 0;
			}
			return m_pImpl->s.compare(static_cast<size_t>(iLen - iSuffixLen), static_cast<size_t>(iSuffixLen), sSuffix.m_pImpl->s) == 0;
		}
		bool String::Equals(const String& sStr1,
							const String& sStr2) {
			return sStr1 == sStr2;
		}
		bool String::Equals(const String& sStr) const {
			return *this == sStr;
		}
		int String::IndexOf(const String& sSubstring) const {
			return IndexOf(sSubstring, 0, false);
		}
		int String::IndexOf(const String& sSubstring,
							bool bIgnoreCase) const {
			return IndexOf(sSubstring, 0, bIgnoreCase);
		}
		int String::IndexOf(const String& sSubstring,
							int iStartIndex, bool bIgnoreCase) const {
			if (iStartIndex < 0 || iStartIndex > GetLength())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");

			if (sSubstring.IsEmpty()) return iStartIndex;

			if (!bIgnoreCase) {
				auto pos = m_pImpl->s.find(sSubstring.m_pImpl->s, iStartIndex);
				return (pos == std::string::npos) ? -1 : (int)pos;
			}

			// Case-insensitive search
			auto it = std::search(
				m_pImpl->s.begin() + iStartIndex, m_pImpl->s.end(),
				sSubstring.m_pImpl->s.begin(), sSubstring.m_pImpl->s.end(),
				[](char c1, char c2) { return std::tolower(static_cast<unsigned char>(c1)) == std::tolower(static_cast<unsigned char>(c2)); }
			);

			return (it == m_pImpl->s.end()) ? -1 : (int)std::distance(m_pImpl->s.begin(), it);
		}
		int String::IndexOfAny(int iStartIndex,
							   std::initializer_list<char> chChars) {
			if (iStartIndex < 0 || iStartIndex > GetLength())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");

			auto pos = m_pImpl->s.find_first_of(std::string(chChars.begin(), chChars.end()), iStartIndex);
			return (pos == std::string::npos) ? -1 : (int)pos;
		}



		String& String::Append(const char ch) {
			m_pImpl->s += ch;
			return *this;
		}
		String& String::Append(
			const String& sStr) {
			m_pImpl->s.append(sStr.GetRawString());
			return *this;
		}
		String& String::Insert(
			int iIndex, const String& sStr) {
			int iLen = GetLength();
			if (iIndex < 0 || iIndex > iLen)
				throw ArgumentOutOfRangeException("Invalid iIndex");

			m_pImpl->s.insert(iIndex, sStr.GetRawString(), sStr.GetLength());
			return *this;
		}
		bool String::IsEmpty() const {
			return m_pImpl->s.empty();
		}

		String String::Join(
			char chSeparator, std::initializer_list<String> sStrings) {
			return Join(chSeparator, sStrings, 0, (int)sStrings.size());
		}
		String String::Join(
			char chSeparator, std::initializer_list<String> sStrings,
			int iStartIndex, int iCount) {
			if (iStartIndex < 0 || iStartIndex > (int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");
			if (iCount < 0 || (iStartIndex + iCount) > (int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iCount");

			const String* pStrs = sStrings.begin();
			String sJoinStr("");

			for (int i = iStartIndex; i < iStartIndex + iCount; i++) {
				sJoinStr.Append(pStrs[i]);
				if (i != iStartIndex + iCount - 1) {
					sJoinStr.Append(chSeparator);
				}
			}
			return sJoinStr;
		}
		String String::Join(
			const String& sSeparator,
			std::initializer_list<String> sStrings) {
			return Join(sSeparator, sStrings, 0, (int)sStrings.size());
		}
		String String::Join(
			const String& sSeparator,
			std::initializer_list<String> sStrings, int iStartIndex,
			int iCount) {
			if (iStartIndex < 0 || iStartIndex > (int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");
			if (iCount < 0 || (iStartIndex + iCount) > (int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iCount");

			const String* pStrs = sStrings.begin();
			String sJoinStr("");

			for (int i = iStartIndex; i < iStartIndex + iCount; i++) {
				sJoinStr.Append(pStrs[i]);
				if (i != iStartIndex + iCount - 1) {
					sJoinStr.Append(sSeparator);
				}
			}
			return sJoinStr;
		}
		int String::LastIndexOf(const String& sStr,
								bool bIgnoreCase) {
			if (sStr.IsEmpty()) return GetLength();

			if (!bIgnoreCase) {
				auto pos = m_pImpl->s.rfind(sStr.m_pImpl->s);
				return (pos == std::string::npos) ? -1 : (int)pos;
			}

			auto it = std::find_end(
				m_pImpl->s.begin(), m_pImpl->s.end(),
				sStr.m_pImpl->s.begin(), sStr.m_pImpl->s.end(),
				[](char c1, char c2) { return std::tolower(static_cast<unsigned char>(c1)) == std::tolower(static_cast<unsigned char>(c2)); }
			);

			return (it == m_pImpl->s.end()) ? -1 : (int)std::distance(m_pImpl->s.begin(), it);
		}

		static int FindLastCharIndex(const std::string& s, int iStartIndex, char ch, bool bIgnoreCase) {
			for (int i = static_cast<int>(s.length()) - 1; i >= iStartIndex; --i) {
				bool match = bIgnoreCase 
					? std::tolower(static_cast<unsigned char>(s[i])) == std::tolower(static_cast<unsigned char>(ch))
					: (s[i] == ch);
				if (match) return i;
			}
			return -1;
		}

		int String::LastIndexOfAny(int iStartIndex, std::initializer_list<char> chChars, bool bIgnoreCase) {
			if (iStartIndex < 0 || iStartIndex > GetLength()) throw ArgumentOutOfRangeException("Invalid iStartIndex");
			if (IsEmpty()) return -1;

			for (auto ch : chChars) {
				int idx = FindLastCharIndex(m_pImpl->s, iStartIndex, ch, bIgnoreCase);
				if (idx != -1) return idx;
			}
			return -1;
		}
		String String::PadLeft(int iTotalWidth) {
			return PadLeft(iTotalWidth, (char)' ');
		}
		String String::PadLeft(int iTotalWidth,
							   char ch) {
			if (iTotalWidth < 0) {
				throw ArgumentException("Invalid iTotalWidth");
			}
			int iLen = GetLength();
			if (iTotalWidth <= iLen) {
				return *this;
			}
			std::string sPadding(iTotalWidth - iLen, ch);
			m_pImpl->s.insert(0, sPadding);
			return *this;
		}
		String String::PadRight(int iTotalWidth) {
			return PadRight(iTotalWidth, (char)' ');
		}
		String String::PadRight(int iTotalWidth,
								char ch) {
			if (iTotalWidth < 0) {
				throw ArgumentException("Invalid iTotalWidth");
			}
			int iLen = GetLength();
			if (iTotalWidth <= iLen) {
				return *this;
			}
			m_pImpl->s.append(iTotalWidth - iLen, ch);
			return *this;
		}
		String String::Remove(int iStartIndex) const {
			return Remove(iStartIndex, GetLength() - iStartIndex);
		}
		String String::Remove(int iStartIndex,
							  int iCount) const {
			int iLen = GetLength();
			if (iStartIndex < 0 || iStartIndex > iLen || iCount < 0 || (iStartIndex + iCount) > iLen) {
				throw ArgumentOutOfRangeException("Invalid iStartIndex or iCount");
			}

			std::string sRet = m_pImpl->s;
			sRet.erase(iStartIndex, iCount);
			return String(sRet.c_str());
		}
		String String::Replace(char chOriginalChar,
							   char chReplaceChar) const {
			std::string sRet = m_pImpl->s;
			std::replace(sRet.begin(), sRet.end(), chOriginalChar, chReplaceChar);
			return String(sRet.c_str());
		}
		String String::Replace(
			const String& sOriginalStr,
			const String& sReplaceStr) const {
			if (sOriginalStr.IsEmpty()) return *this;

			std::string sRet = m_pImpl->s;
			size_t pos = 0;
			while ((pos = sRet.find(sOriginalStr.m_pImpl->s, pos)) != std::string::npos) {
				sRet.replace(pos, sOriginalStr.GetLength(), sReplaceStr.m_pImpl->s);
				pos += sReplaceStr.GetLength();
			}
			return String(sRet.c_str());
		}
		Array<String> String::Split(char chSeparator) const {
			std::vector<String> vTempResult;
			std::stringstream ss(m_pImpl->s);
			std::string sToken;
			while (std::getline(ss, sToken, chSeparator)) {
				vTempResult.push_back(String(sToken.c_str()));
			}

			Array<String> result((int)vTempResult.size());
			for (int i = 0; i < (int)vTempResult.size(); i++) result [i] = vTempResult [i];
			return result;
		}

		static void AddSplitToken(const std::string& sCurrent, StringSplitOptions eOptions, std::vector<String>& vResult) {
			String s(sCurrent.c_str());
			if (eOptions == StringSplitOptions::TrimEntries) s = s.Trim();
			if (eOptions != StringSplitOptions::RemoveEmptyEntries || !s.IsEmpty()) {
				vResult.push_back(s);
			}
		}

		static std::set<char> PopulateSplitCharSet(const String* pSeparator, int iCount) {
			std::set<char> charSet;
			if (!pSeparator || iCount <= 0) return charSet;
			for (int i = 0; i < iCount; ++i) {
				const char* raw = pSeparator[i].GetRawString();
				if (raw) { while (*raw) charSet.insert(*raw++); }
			}
			return charSet;
		}

		static std::vector<String> SplitByCharSet(const std::string& sText, const std::set<char>& charSet, StringSplitOptions eOptions) {
			std::vector<String> vResult;
			std::string sCurrent;
			for (char c : sText) {
				if (charSet.find(c) == charSet.end()) {
					sCurrent += c;
				} else {
					AddSplitToken(sCurrent, eOptions, vResult);
					sCurrent.clear();
				}
			}
			AddSplitToken(sCurrent, eOptions, vResult);
			return vResult;
		}

		Array<String> String::Split(const Array<String>& arrSeparators, StringSplitOptions eOptions) const {
			return Split(arrSeparators.GetData(), arrSeparators.GetLength(), eOptions);
		}

		Array<String> String::Split(const String* pSeparator, int iCount, StringSplitOptions eOptions) const {
			std::set<char> charSet = PopulateSplitCharSet(pSeparator, iCount);
			std::vector<String> vTempResult = SplitByCharSet(m_pImpl->s, charSet, eOptions);
			Array<String> result(static_cast<int>(vTempResult.size()));
			for (int i = 0; i < result.GetLength(); i++) result[i] = vTempResult[i];
			return result;
		}

		bool String::StartsWith(const String& sPrefix) const {
			return StartsWith(sPrefix, false);
		}

		bool String::StartsWith(const String& sPrefix, bool bIgnoreCase) const {
			if (sPrefix.GetLength() > GetLength()) return false;

			if (!bIgnoreCase) {
				return m_pImpl->s.compare(0, sPrefix.GetLength(), sPrefix.m_pImpl->s) == 0;
			}

			return Compare(*this, 0, sPrefix, 0, sPrefix.GetLength(), true) == 0;
		}

		String String::FromInt(int iValue) {
			return ValueOf(iValue);
		}

		String String::ValueOf(int iValue) {
			return ToStringHelper(iValue);
		}


		String String::ValueOf(long long llValue) {
			return ToStringHelper(llValue);
		}

		String String::ValueOf(double dValue) {
			std::ostringstream ss;
			ss << dValue;
			return String(ss.str().c_str());
		}

		String String::ToString(int iValue) {
			return ToStringHelper(iValue);
		}

		String String::ToString(long long llValue) {
			return ToStringHelper(llValue);
		}

		String String::ToString(double dValue) {
			return ToStringHelper(dValue);
		}

		String String::ToString(bool bValue) {
			return ToStringHelper(bValue);
		}

		String String::Substring(int iStartIndex) const {
			return Substring(iStartIndex, GetLength() - iStartIndex);
		}

		String String::Substring(int iStartIndex, int iLength) const {
			int iLen = GetLength();
			if (iStartIndex < 0 || iStartIndex > iLen || iLength < 0 || (iStartIndex + iLength) > iLen) {
				throw ArgumentOutOfRangeException("Invalid iStartIndex or iLength");
			}
			return String(m_pImpl->s.substr(iStartIndex, iLength).c_str());
		}


		String String::ToLower() const {
			std::string sRet = m_pImpl->s;
			std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](char ch) -> char {
				return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
						   });
			return String(sRet.c_str());
		}

		String String::ToUpper() const {
			std::string sRet = m_pImpl->s;
			std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](char ch) -> char {
				return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
						   });
			return String(sRet.c_str());
		}

		String String::Trim() const {
			return TrimStart().TrimEnd();
		}

		String String::TrimStart() const {
			auto it = std::find_if(m_pImpl->s.begin(), m_pImpl->s.end(), [](char ch) {
				return !std::isspace(static_cast<unsigned char>(ch));
								   });
			if (it == m_pImpl->s.end()) return "";
			return String(m_pImpl->s.substr(std::distance(m_pImpl->s.begin(), it)).c_str());
		}

		String String::TrimEnd() const {
			auto it = std::find_if(m_pImpl->s.rbegin(), m_pImpl->s.rend(), [](char ch) {
				return !std::isspace(static_cast<unsigned char>(ch));
								   });
			if (it == m_pImpl->s.rend()) return "";
			return String(m_pImpl->s.substr(0, m_pImpl->s.length() - std::distance(m_pImpl->s.rbegin(), it)).c_str());
		}
	}  // namespace System
}  // namespace DotNetDupe
