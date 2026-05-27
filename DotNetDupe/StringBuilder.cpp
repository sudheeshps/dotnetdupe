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

            StringBuilder::StringBuilder(const String& value) : _buffer((const char*)value) {}

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
                _buffer.append((const char*)value);
                return *this;
            }

            StringBuilder& StringBuilder::Append(const char* value) {
                if (value) _buffer.append(value);
                return *this;
            }

            StringBuilder& StringBuilder::Append(char value) {
                _buffer.append(1, value);
                return *this;
            }

            StringBuilder& StringBuilder::Append(int value) {
                _buffer.append(std::to_string(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(long long value) {
                _buffer.append(std::to_string(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(double value) {
                _buffer.append(std::to_string(value));
                return *this;
            }

            StringBuilder& StringBuilder::Append(bool value) {
                _buffer.append(value ? "True" : "False");
                return *this;
            }

            StringBuilder& StringBuilder::AppendLine() {
                _buffer.append("\r\n");
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
