#include "pch.h"
#include "System/Text/StringBuilder.h"
#include <algorithm>

#ifdef UNICODE
#define TO_TSTRING std::to_wstring
#else
#define TO_TSTRING std::to_string
#endif

namespace DotNetDupe {
    namespace System {
        namespace Text {

            StringBuilder::StringBuilder() {}

            StringBuilder::StringBuilder(int nCapacity) {
                m_strBuffer.reserve(nCapacity);
            }

            StringBuilder::StringBuilder(const String& sValue) : m_strBuffer((const char*)sValue) {}

            int StringBuilder::GetLength() const {
                return (int)m_strBuffer.length();
            }

            void StringBuilder::SetLength(int nLength) {
                if (nLength < 0) return;
                m_strBuffer.resize(nLength);
            }

            int StringBuilder::GetCapacity() const {
                return (int)m_strBuffer.capacity();
            }

            void StringBuilder::SetCapacity(int nCapacity) {
                if (nCapacity < (int)m_strBuffer.length()) return;
                m_strBuffer.reserve(nCapacity);
            }

            StringBuilder& StringBuilder::Append(const String& sValue) {
                m_strBuffer.append((const char*)sValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(const char* pValue) {
                if (pValue) m_strBuffer.append(pValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(char chValue) {
                m_strBuffer.append(1, chValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(int iValue) {
                m_strBuffer.append(std::to_string(iValue));
                return *this;
            }

            StringBuilder& StringBuilder::Append(long long llValue) {
                m_strBuffer.append(std::to_string(llValue));
                return *this;
            }

            StringBuilder& StringBuilder::Append(double value) {
                m_strBuffer.append(std::to_string(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(bool bValue) {
                m_strBuffer.append(bValue ? "True" : "False");
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine() {
                m_strBuffer.append("\r\n");
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine(const String& sValue) {
                Append(sValue);
                return AppendLine();
            }

            StringBuilder& StringBuilder::Clear() {
                m_strBuffer.clear();
                return *this;
            }

            String StringBuilder::ToString() const {
                return String(m_strBuffer.c_str());
            }
        }
    }
}
