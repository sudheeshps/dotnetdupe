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

            StringBuilder::StringBuilder() : m_pImpl(new Impl()) {
                /// Allocate default buffer implementation.
            }

            StringBuilder::StringBuilder(int nCapacity) : m_pImpl(new Impl()) {
                /// Reserve initial buffer capacity if greater than zero.
                if (nCapacity > 0) {
                    m_pImpl->buffer.reserve(nCapacity);
                }
            }

            StringBuilder::StringBuilder(const String& sValue) : m_pImpl(new Impl()) {
                /// Initialize buffer with initial string content.
                m_pImpl->buffer = (const char*)sValue;
            }

            StringBuilder::~StringBuilder() {
                /// Release internal Pimpl buffer.
                if (m_pImpl) {
                    delete m_pImpl;
                    m_pImpl = nullptr;
                }
            }

            int StringBuilder::GetLength() const {
                /// Query accumulated buffer character count.
                return m_pImpl ? static_cast<int>(m_pImpl->buffer.length()) : 0;
            }

            void StringBuilder::SetLength(int nLength) {
                /// Guard: Validate length boundary and resize buffer.
                if (nLength < 0 || !m_pImpl) return;
                m_pImpl->buffer.resize(nLength);
            }

            int StringBuilder::GetCapacity() const {
                /// Query underlying buffer allocated capacity.
                return m_pImpl ? static_cast<int>(m_pImpl->buffer.capacity()) : 0;
            }

            void StringBuilder::SetCapacity(int nCapacity) {
                /// Guard: Capacity must accommodate current length.
                if (!m_pImpl || nCapacity < static_cast<int>(m_pImpl->buffer.length())) return;
                m_pImpl->buffer.reserve(nCapacity);
            }

            StringBuilder& StringBuilder::Append(const String& sValue) {
                /// Append String content to buffer.
                if (m_pImpl) m_pImpl->buffer.append((const char*)sValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(const char* pValue) {
                /// Append C-string characters to buffer.
                if (m_pImpl && pValue) m_pImpl->buffer.append(pValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(char chValue) {
                /// Append single character to buffer.
                if (m_pImpl) m_pImpl->buffer.append(1, chValue);
                return *this;
            }

            StringBuilder& StringBuilder::Append(int iValue) {
                /// Format 32-bit integer and append.
                if (m_pImpl) m_pImpl->buffer.append(std::to_string(iValue));
                return *this;
            }

            StringBuilder& StringBuilder::Append(long long llValue) {
                /// Format 64-bit integer and append.
                if (m_pImpl) m_pImpl->buffer.append(std::to_string(llValue));
                return *this;
            }

            StringBuilder& StringBuilder::Append(double value) {
                /// Format double-precision float and append.
                if (m_pImpl) m_pImpl->buffer.append(std::to_string(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(bool bValue) {
                /// Append Boolean literal representation.
                if (m_pImpl) m_pImpl->buffer.append(bValue ? "True" : "False");
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine() {
                /// Append standard CRLF line termination.
                if (m_pImpl) m_pImpl->buffer.append("\r\n");
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine(const String& sValue) {
                /// Append value followed by line break.
                Append(sValue);
                return AppendLine();
            }

            StringBuilder& StringBuilder::Clear() {
                /// Clear buffer contents while preserving capacity.
                if (m_pImpl) m_pImpl->buffer.clear();
                return *this;
            }

            String StringBuilder::ToString() const {
                /// Convert internal character sequence to immutable String.
                return m_pImpl ? String(m_pImpl->buffer.c_str()) : String("");
            }
        }
    }
}
