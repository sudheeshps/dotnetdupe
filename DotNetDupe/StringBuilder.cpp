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

            StringBuilder::StringBuilder(int capacity) {
                _buffer.reserve(capacity);
            }

            StringBuilder::StringBuilder(const String& value) : _buffer((const TCHAR*)value) {}

            int StringBuilder::GetLength() const {
                return (int)_buffer.length();
            }

            void StringBuilder::SetLength(int value) {
                if (value < 0) return;
                _buffer.resize(value);
            }

            int StringBuilder::GetCapacity() const {
                return (int)_buffer.capacity();
            }

            void StringBuilder::SetCapacity(int value) {
                if (value < (int)_buffer.length()) return;
                _buffer.reserve(value);
            }

            StringBuilder& StringBuilder::Append(const String& value) {
                _buffer.append((const TCHAR*)value);
                return *this;
            }

            StringBuilder& StringBuilder::Append(const TCHAR* value) {
                if (value) _buffer.append(value);
                return *this;
            }

            StringBuilder& StringBuilder::Append(TCHAR value) {
                _buffer.append(1, value);
                return *this;
            }

            StringBuilder& StringBuilder::Append(int value) {
                _buffer.append(TO_TSTRING(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(long long value) {
                _buffer.append(TO_TSTRING(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(double value) {
                _buffer.append(TO_TSTRING(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(bool value) {
                _buffer.append(value ? _T("True") : _T("False"));
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine() {
                _buffer.append(_T("\r\n"));
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine(const String& value) {
                Append(value);
                return AppendLine();
            }

            StringBuilder& StringBuilder::Clear() {
                _buffer.clear();
                return *this;
            }

            String StringBuilder::ToString() const {
                return String(_buffer.c_str());
            }
        }
    }
}
