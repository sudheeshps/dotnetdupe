#include "pch.h"
#include "System/Guid.h"
#include "System/String.h"
#include "System/FormatException.h"
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cstring>

namespace DotNetDupe {
    namespace System {
        const Guid Guid::Empty = Guid();

        Guid::Guid() {
            for (size_t i = 0; i < 16; ++i) {
                _data[i] = 0;
            }
        }

        Guid::Guid(const Array<uint8_t>& b) {
            if (b.GetLength() != 16) throw ArgumentException("Guid should be 16 bytes.");
            for (size_t i = 0; i < 16; ++i) {
                _data[i] = b[static_cast<int>(i)];
            }
        }

        Guid::Guid(const String& g) {
            std::string s = (const char*)g;
            // Remove braces or hyphens if needed
            s.erase(std::remove(s.begin(), s.end(), '{'), s.end());
            s.erase(std::remove(s.begin(), s.end(), '}'), s.end());
            s.erase(std::remove(s.begin(), s.end(), '-'), s.end());

            if (s.length() != 32) throw FormatException("Guid string should only contain 32 hexadecimal characters.");

            for (size_t i = 0; i < 16; ++i) {
                unsigned int byteVal;
                if (sscanf_s(s.substr(i * 2, 2).c_str(), "%02x", &byteVal) != 1)
                    throw FormatException("Guid string should only contain 32 hexadecimal characters.");
                _data[i] = (uint8_t)byteVal;
            }
        }

        Guid Guid::NewGuid() {
            // Very simple random Guid for now
            Array<uint8_t> data(16);
            for (size_t i = 0; i < 16; ++i) {
                data[static_cast<int>(i)] = static_cast<uint8_t>(rand() % 256);
            }
            // Set version 4 and variant
            data[6] = (data[6] & 0x0F) | 0x40;
            data[8] = (data[8] & 0x3F) | 0x80;
            return Guid(data);
        }

        Array<uint8_t> Guid::ToByteArray() const {
            Array<uint8_t> result(16);
            for (size_t i = 0; i < 16; ++i) {
                result[static_cast<int>(i)] = _data[i];
            }
            return result;
        }

        String Guid::ToString() const {
            char buf[37];
            sprintf_s(buf, 37, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                _data[0], _data[1], _data[2], _data[3],
                _data[4], _data[5],
                _data[6], _data[7],
                _data[8], _data[9],
                _data[10], _data[11], _data[12], _data[13], _data[14], _data[15]);
            return String(buf);
        }

        bool Guid::operator==(const Guid& other) const {
            return std::memcmp(_data, other._data, 16) == 0;
        }

        bool Guid::operator!=(const Guid& other) const {
            return !(*this == other);
        }
    }
}
