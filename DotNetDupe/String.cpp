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
#include <sstream>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <string>


namespace DotNetDupe {
	namespace System {
        DOTNETDUPE_API std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const String& sStr) {
            os << (sStr.GetRawString() ? sStr.GetRawString() : "");
            return os;
        }

        DOTNETDUPE_API std::basic_istream<char>& operator>>(std::basic_istream<char>& is, String& sStr) {
            std::string s;
            is >> s;
            sStr = s.c_str();
            return is;
        }

        DOTNETDUPE_API void PrintTo(const String& sStr, ::std::ostream* os) {
            if (os) {
                *os << "\"" << (sStr.GetRawString() ? sStr.GetRawString() : "") << "\"";
            }
        }

		class StringImpl {
		public:
			static std::string& Get(const String& str) {
				return *static_cast<std::string*>(str.m_pData);
			}

			static std::string& Get(const String* str) {
				return *static_cast<std::string*>(str->m_pData);
			}
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
		String::String() { m_pData = new std::string("");  }
		String::~String() { delete static_cast<std::string*>(m_pData); }

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

		bool String::operator<(const String& sStr) const {
			return StringImpl::Get(this) < StringImpl::Get(sStr);
		}

		bool operator==(const String& sStr1, const String& sStr2) {
			return StringImpl::Get(sStr1).compare(StringImpl::Get(sStr2)) == 0;
		}
		bool operator==(const String& sStr1, const char* pStr2) {
			return pStr2 ? StringImpl::Get(sStr1).compare(pStr2) == 0 : false;
		}
		bool operator==(const char* pStr1, const String& sStr2) {
			return pStr1 ? StringImpl::Get(sStr2).compare(pStr1) == 0 : false;
		}

		bool operator!=(const String& sStr1, const String& sStr2) {
			return !(sStr1 == sStr2);
		}
		bool operator!=(const String& sStr1, const char* pStr2) {
			return !(sStr1 == pStr2);
		}
		bool operator!=(const char* pStr1, const String& sStr2) {
			return !(pStr1 == sStr2);
		}

		String String::operator+(const char* pStr) const {
			String sNewStr = *this;
			if (pStr) {
				StringImpl::Get(sNewStr).append(pStr);
			}
			return sNewStr;
		}

		String String::operator+(char ch) const {
			String sNewStr = *this;
			StringImpl::Get(sNewStr).push_back(ch);
			return sNewStr;
		}

		String& String::operator+=(const String& sStr) {
			StringImpl::Get(this).append(sStr.GetRawString());
			return *this;
		}

		String& String::operator+=(const char* pStr) {
			if (pStr) {
				StringImpl::Get(this).append(pStr);
			}
			return *this;
		}

		String& String::operator+=(char ch) {
			StringImpl::Get(this).push_back(ch);
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
			m_pData = new std::string();
			StringImpl::Get(this) = pStr;
		}

		String::String(const String& sStr) { m_pData = new std::string(StringImpl::Get(sStr));  }

		String& String::operator=(const String& sStr) {
			if (this != &sStr) {
				StringImpl::Get(this) = StringImpl::Get(sStr);
			}
			return *this;
		}

		String::String(String&& sStr) noexcept { m_pData = new std::string(std::move(StringImpl::Get(sStr)));  }
		String& String::operator=(String&& sStr) noexcept {
			if (this != &sStr) {
				StringImpl::Get(this) = std::move(StringImpl::Get(sStr));
			}
			return *this;
		}
		String::String(const wchar_t* pStr) {
			if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
			m_pData = new std::string();

			StringImpl::Get(this) = Utils::StringConvert::WCharToUtf8(pStr);
		}

		String& String::operator=(const wchar_t* pStr) {
			if (pStr == nullptr) throw ArgumentException("Invalid input pointer");

			StringImpl::Get(this) = Utils::StringConvert::WCharToUtf8(pStr);

			return *this;
		}

		String& String::operator=(const char* pStr) {
			if (pStr == nullptr) throw ArgumentException("Invalid input pointer");
			StringImpl::Get(this) = pStr;
			return *this;
		}
		const char* String::GetRawString() const {
			return StringImpl::Get(this).c_str();
		}
		int String::GetLength() const {
			return static_cast<int>(StringImpl::Get(this).length());
		}

		String String::Clone() const {
			return String(GetRawString());
		}


		char String::operator[](int iIndex) const {
			if (iIndex >= (int)StringImpl::Get(this).size()) throw ArgumentOutOfRangeException("Invalid iIndex");
			return GetRawString() [iIndex];
		}
		int String::Compare(const String& sStr1,
							int iIndex1,
							const String& sStr2,
							int iIndex2, int iLength,
							bool bIgnoreCase) {
			if (!bIgnoreCase) {
				return StringImpl::Get(sStr1).compare(iIndex1, iLength, StringImpl::Get(sStr2), iIndex2, iLength);
			}

			auto s1 = StringImpl::Get(sStr1).substr(iIndex1, iLength);
			auto s2 = StringImpl::Get(sStr2).substr(iIndex2, iLength);
			auto transform_char = [](char ch) -> char {
				if constexpr (std::is_same_v<char, wchar_t>) {
					return static_cast<wchar_t>(std::towlower(ch));
				}
				else {
					return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
				}
				};
			std::transform(s1.begin(), s1.end(), s1.begin(), transform_char);
			std::transform(s2.begin(), s2.end(), s2.begin(), transform_char);
			return s1.compare(s2);
		}

		int String::CompareTo(const String& sStr) const {
			return StringImpl::Get(this).compare(sStr.GetRawString());
		}
		String String::Concat(
			const std::initializer_list<String> sStrs) const {
			String sNewStr = *this;
			for (auto sStr : sStrs) {
				StringImpl::Get(sNewStr).append(sStr.GetRawString());
			}
			return sNewStr;
		}
		String String::operator+(
			const String& sStr) const {
			return Concat({ sStr });
		}
		bool String::Contains(char ch) const {
			return StringImpl::Get(this).find(ch) != std::string::npos;
		}
		bool String::Contains(const String& sStr) const {
			return StringImpl::Get(this).find(StringImpl::Get(sStr)) != std::string::npos;
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
			StringImpl::Get(this).copy(pDestination + iDestinationIndex, iCount, iSourceIndex);
		}
		bool String::EndsWith(char ch, bool bIgnoreCase) const {
			auto iLen = StringImpl::Get(this).length();
			if (iLen == 0) return false;
			if (bIgnoreCase) {
				return std::tolower(static_cast<unsigned char>(StringImpl::Get(this) [iLen - 1])) == std::tolower(static_cast<unsigned char>(ch));
			}
			return StringImpl::Get(this) [iLen - 1] == ch;
		}
		bool String::EndsWith(const String& sSuffix,
							  bool bIgnoreCase) const {
			int iLen = GetLength();
			int iSuffixLen = sSuffix.GetLength();
			if (iSuffixLen > iLen) return false;

			if (bIgnoreCase) {
				return Compare(*this, iLen - iSuffixLen, sSuffix, 0, iSuffixLen, true) == 0;
			}
			return StringImpl::Get(this).compare(static_cast<size_t>(iLen - iSuffixLen), static_cast<size_t>(iSuffixLen), StringImpl::Get(sSuffix)) == 0;
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
				auto pos = StringImpl::Get(this).find(StringImpl::Get(sSubstring), iStartIndex);
				return (pos == std::string::npos) ? -1 : (int)pos;
			}

			// Case-insensitive search
			auto it = std::search(
				StringImpl::Get(this).begin() + iStartIndex, StringImpl::Get(this).end(),
				StringImpl::Get(sSubstring).begin(), StringImpl::Get(sSubstring).end(),
				[](char c1, char c2) { return std::tolower(static_cast<unsigned char>(c1)) == std::tolower(static_cast<unsigned char>(c2)); }
			);

			return (it == StringImpl::Get(this).end()) ? -1 : (int)std::distance(StringImpl::Get(this).begin(), it);
		}
		int String::IndexOfAny(int iStartIndex,
							   std::initializer_list<char> chChars) {
			if (iStartIndex < 0 || iStartIndex > GetLength())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");

			auto pos = StringImpl::Get(this).find_first_of(std::string(chChars.begin(), chChars.end()), iStartIndex);
			return (pos == std::string::npos) ? -1 : (int)pos;
		}


		String::String(std::nullptr_t) {
			throw ArgumentException("Invalid input pointer");
		}

		String& String::operator=(std::nullptr_t) {
			throw ArgumentException("Invalid input pointer");
			return *this;
		}

		String::String(int nNull) {
			if (nNull == 0) throw ArgumentException("Invalid input pointer");
			throw ArgumentException("Invalid input integer");
		}
		String& String::Append(const char ch) {
			StringImpl::Get(this) += ch;
			return *this;
		}
		String& String::Append(
			const String& sStr) {
			StringImpl::Get(this).append(sStr.GetRawString());
			return *this;
		}
		String& String::Insert(
			int iIndex, const String& sStr) {
			int iLen = GetLength();
			if (iIndex < 0 || iIndex > iLen)
				throw ArgumentOutOfRangeException("Invalid iIndex");

			StringImpl::Get(this).insert(iIndex, sStr.GetRawString(), sStr.GetLength());
			return *this;
		}
		bool String::IsEmpty() const {
			return StringImpl::Get(this).empty();
		}

		String String::Join(
			char chSeparator, std::initializer_list<String> sStrings) {
			return Join(chSeparator, sStrings, 0, (int)sStrings.size());
		}
		String String::Join(
			char chSeparator, std::initializer_list<String> sStrings,
			int iStartIndex, int iCount) {
			if (iStartIndex < 0 || iStartIndex >(int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");
			if (iCount < 0 || (iStartIndex + iCount) >(int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iCount");

			std::vector<String> vStrs(sStrings);
			String sJoinStr("");

			for (int i = iStartIndex; i < iStartIndex + iCount; i++) {
				sJoinStr.Append(vStrs [i]);
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
			if (iStartIndex < 0 || iStartIndex >(int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");
			if (iCount < 0 || (iStartIndex + iCount) >(int)sStrings.size())
				throw ArgumentOutOfRangeException("Invalid iCount");

			std::vector<String> vStrs(sStrings);
			String sJoinStr("");

			for (int i = iStartIndex; i < iStartIndex + iCount; i++) {
				sJoinStr.Append(vStrs [i]);
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
				auto pos = StringImpl::Get(this).rfind(StringImpl::Get(sStr));
				return (pos == std::string::npos) ? -1 : (int)pos;
			}

			auto it = std::find_end(
				StringImpl::Get(this).begin(), StringImpl::Get(this).end(),
				StringImpl::Get(sStr).begin(), StringImpl::Get(sStr).end(),
				[](char c1, char c2) { return std::tolower(static_cast<unsigned char>(c1)) == std::tolower(static_cast<unsigned char>(c2)); }
			);

			return (it == StringImpl::Get(this).end()) ? -1 : (int)std::distance(StringImpl::Get(this).begin(), it);
		}
		int String::LastIndexOfAny(
			int iStartIndex, std::initializer_list<char> chChars, bool bIgnoreCase) {
			if (iStartIndex < 0 || iStartIndex > GetLength())
				throw ArgumentOutOfRangeException("Invalid iStartIndex");

			if (IsEmpty()) return -1;

			for (auto ch : chChars) {
				size_t lastFound = std::string::npos;
				for (int i = GetLength() - 1; i >= iStartIndex; --i) {
					bool match = false;
					if (bIgnoreCase) {
						match = std::tolower(static_cast<unsigned char>(StringImpl::Get(this) [i])) == std::tolower(static_cast<unsigned char>(ch));
					}
					else {
						match = (StringImpl::Get(this) [i] == ch);
					}

					if (match) {
						lastFound = i;
						break;
					}
				}
				if (lastFound != std::string::npos) {
					return static_cast<int>(lastFound);
				}
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
			StringImpl::Get(this).insert(0, sPadding);
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
			StringImpl::Get(this).append(iTotalWidth - iLen, ch);
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

			std::string sRet = StringImpl::Get(this);
			sRet.erase(iStartIndex, iCount);
			return String(sRet.c_str());
		}
		String String::Replace(char chOriginalChar,
							   char chReplaceChar) {
			std::string sRet = StringImpl::Get(this);
			std::replace(sRet.begin(), sRet.end(), chOriginalChar, chReplaceChar);
			return String(sRet.c_str());
		}
		String String::Replace(
			const String& sOriginalStr,
			const String& sReplaceStr) {
			if (sOriginalStr.IsEmpty()) return *this;

			std::string sRet = StringImpl::Get(this);
			size_t pos = 0;
			while ((pos = sRet.find(StringImpl::Get(sOriginalStr), pos)) != std::string::npos) {
				sRet.replace(pos, sOriginalStr.GetLength(), StringImpl::Get(sReplaceStr));
				pos += sReplaceStr.GetLength();
			}
			return String(sRet.c_str());
		}
		Array<String> String::Split(char chSeparator) const {
			std::vector<String> vTempResult;
			std::stringstream ss(StringImpl::Get(this));
			std::string sToken;
			while (std::getline(ss, sToken, chSeparator)) {
				vTempResult.push_back(String(sToken.c_str()));
			}

			Array<String> result((int)vTempResult.size());
			for (int i = 0; i < (int)vTempResult.size(); i++) result [i] = vTempResult [i];
			return result;
		}

		Array<String> String::Split(String sSeparator [], int iCount,
									StringSplitOptions eOptions) const {
			std::vector<String> vTempResult;
			std::set<char> charSet;
			for (int i = 0; i < iCount; ++i) {
				for (auto c : StringImpl::Get(sSeparator[i])) charSet.insert(c);
			}
			std::string sCurrent;
			for (auto c : StringImpl::Get(this)) {
				if (charSet.find(c) == charSet.end()) {
					sCurrent += c;
				}
				else {
					String s(sCurrent.c_str());
					if (eOptions == StringSplitOptions::TrimEntries) s = s.Trim();
					if (eOptions != StringSplitOptions::RemoveEmptyEntries || !s.IsEmpty()) {
						vTempResult.push_back(s);
					}
					sCurrent.clear();
				}
			}
			String sFinal(sCurrent.c_str());
			if (eOptions == StringSplitOptions::TrimEntries) sFinal = sFinal.Trim();
			if (eOptions != StringSplitOptions::RemoveEmptyEntries || !sFinal.IsEmpty()) {
				vTempResult.push_back(sFinal);
			}

			Array<String> result((int)vTempResult.size());
			for (int i = 0; i < (int)vTempResult.size(); i++) result [i] = vTempResult [i];
			return result;
		}

		bool String::StartsWith(const String& sPrefix) const {
			return StartsWith(sPrefix, false);
		}

		bool String::StartsWith(const String& sPrefix, bool bIgnoreCase) const {
			if (sPrefix.GetLength() > GetLength()) return false;

			if (!bIgnoreCase) {
				return StringImpl::Get(this).compare(0, sPrefix.GetLength(), StringImpl::Get(sPrefix)) == 0;
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
			return String(StringImpl::Get(this).substr(iStartIndex, iLength).c_str());
		}


		String String::ToLower() const {
			std::string sRet = StringImpl::Get(this);
			std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](char ch) -> char {
				return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
						   });
			return String(sRet.c_str());
		}

		String String::ToUpper() const {
			std::string sRet = StringImpl::Get(this);
			std::transform(sRet.begin(), sRet.end(), sRet.begin(), [](char ch) -> char {
				return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
						   });
			return String(sRet.c_str());
		}

		String String::Trim() const {
			return TrimStart().TrimEnd();
		}

		String String::TrimStart() const {
			auto it = std::find_if(StringImpl::Get(this).begin(), StringImpl::Get(this).end(), [](char ch) {
				return !std::isspace(static_cast<unsigned char>(ch));
								   });
			if (it == StringImpl::Get(this).end()) return "";
			return String(StringImpl::Get(this).substr(std::distance(StringImpl::Get(this).begin(), it)).c_str());
		}

		String String::TrimEnd() const {
			auto it = std::find_if(StringImpl::Get(this).rbegin(), StringImpl::Get(this).rend(), [](char ch) {
				return !std::isspace(static_cast<unsigned char>(ch));
								   });
			if (it == StringImpl::Get(this).rend()) return "";
			return String(StringImpl::Get(this).substr(0, StringImpl::Get(this).length() - std::distance(StringImpl::Get(this).rbegin(), it)).c_str());
		}
	}  // namespace System
}  // namespace DotNetDupe
