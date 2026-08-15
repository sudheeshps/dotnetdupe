#include "pch.h"
#include "System/Text/StringBuilder.h"
#include <algorithm>
#include <string>

#ifdef UNICODE
#define TO_TSTRING std::to_wstring
#else
#define TO_TSTRING std::to_string
#endif

namespace DotNetDupe {
    namespace System {
        namespace Text {

            struct StringBuilder::Impl {
                std::string buffer;
            };

            StringBuilder::StringBuilder() : m_pImpl(new Impl()) {}

            StringBuilder::StringBuilder(int nCapacity) : m_pImpl(new Impl()) {
                if (nCapacity > 0) {
                    m_pImpl->buffer.reserve(nCapacity);
                }
            }

            StringBuilder::StringBuilder(const String& sValue) : m_pImpl(new Impl()) {
                m_pImpl->buffer = (const char*)sValue;
            }

            StringBuilder::~StringBuilder() {
                if (m_pImpl) {
                    delete m_pImpl;
                    m_pImpl = nullptr;
                }
            }

            int StringBuilder::GetLength() const {
                return m_pImpl ? static_cast<int>(m_pImpl->buffer.length()) : 0;
            }

            void StringBuilder::SetLength(int nLength) {
                if (nLength < 0 || !m_pImpl) return;
                m_pImpl->buffer.resize(nLength);
            }

            int StringBuilder::GetCapacity() const {
                return m_pImpl ? static_cast<int>(m_pImpl->buffer.capacity()) : 0;
            }

            void StringBuilder::SetCapacity(int nCapacity) {
                if (!m_pImpl || nCapacity < static_cast<int>(m_pImpl->buffer.length())) return;
                m_pImpl->buffer.reserve(nCapacity);
            }

            StringBuilder& StringBuilder::Append(const String& sValue) {
                if (m_pImpl) m_pImpl->buffer.append((const char*)sValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(const char* pValue) {
                if (m_pImpl && pValue) m_pImpl->buffer.append(pValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(char chValue) {
                if (m_pImpl) m_pImpl->buffer.append(1, chValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(int iValue) {
                if (m_pImpl) m_pImpl->buffer.append(std::to_string(iValue));
                return *this;
            }

            StringBuilder& StringBuilder::Append(long long llValue) {
                if (m_pImpl) m_pImpl->buffer.append(std::to_string(llValue));
                return *this;
            }

            StringBuilder& StringBuilder::Append(double value) {
                if (m_pImpl) m_pImpl->buffer.append(std::to_string(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(bool bValue) {
                if (m_pImpl) m_pImpl->buffer.append(bValue ? "True" : "False");
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine() {
                if (m_pImpl) m_pImpl->buffer.append("\r\n");
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine(const String& sValue) {
                Append(sValue);
                return AppendLine();
            }

            StringBuilder& StringBuilder::Clear() {
                if (m_pImpl) m_pImpl->buffer.clear();
                return *this;
            }

            String StringBuilder::ToString() const {
                return m_pImpl ? String(m_pImpl->buffer.c_str()) : String("");
            }
        }
    }
}
