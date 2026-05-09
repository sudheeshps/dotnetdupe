#include "pch.h"
#include "System/Guid.h"
#include "System/FormatException.h"
#include <objbase.h>
#include <cstdio>
#include <algorithm>

namespace DotNetDupe {
    namespace System {

        const Guid Guid::Empty = Guid();

        Guid::Guid() {
            _data.fill(0);
        }

        Guid::Guid(const std::array<uint8_t, 16>& b) : _data(b) {}

        Guid::Guid(const String& g) {
            std::basic_string<TCHAR> s = (const TCHAR*)g;
            // Remove braces or hyphens if needed, but for simplicity we expect "dddddddd-dddd-dddd-dddd-dddddddddddd"
            s.erase(std::remove(s.begin(), s.end(), _T('{')), s.end());
            s.erase(std::remove(s.begin(), s.end(), _T('}')), s.end());
            s.erase(std::remove(s.begin(), s.end(), _T('-')), s.end());

            if (s.length() != 32) throw FormatException(_T("Guid string should only contain 32 hexadecimal characters."));

            for (size_t i = 0; i < 16; ++i) {
                unsigned int byte;
#ifdef UNICODE
                if (swscanf_s(s.substr(i * 2, 2).c_str(), L"%02x", &byte) != 1)
#else
                if (sscanf_s(s.substr(i * 2, 2).c_str(), "%02x", &byte) != 1)
#endif
                    throw FormatException(_T("Guid string should only contain 32 hexadecimal characters."));
                _data[i] = (uint8_t)byte;
            }
        }

        Guid Guid::NewGuid() {
            GUID guid;
            if (CoCreateGuid(&guid) != S_OK) return Guid();
            
            std::array<uint8_t, 16> b;
            // GUID structure: Data1 (4), Data2 (2), Data3 (2), Data4 (8)
            // .NET Guid byte array order is slightly different but we'll stick to a simple mapping
            memcpy(b.data(), &guid, 16);
            return Guid(b);
        }

        std::array<uint8_t, 16> Guid::ToByteArray() const {
            return _data;
        }

        String Guid::ToString() const {
            TCHAR buf[37];
            _stprintf_s(buf, 37, _T("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x"),
                _data[0], _data[1], _data[2], _data[3],
                _data[4], _data[5],
                _data[6], _data[7],
                _data[8], _data[9],
                _data[10], _data[11], _data[12], _data[13], _data[14], _data[15]);
            return String(buf);
        }

        bool Guid::operator==(const Guid& other) const {
            return _data == other._data;
        }

        bool Guid::operator!=(const Guid& other) const {
            return !(*this == other);
        }
    }
}
