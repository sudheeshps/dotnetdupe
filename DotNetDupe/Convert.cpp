#include "pch.h"
#include "System/Convert.h"
#include "System/OverflowException.h"
#include "System/FormatException.h"
#include <string>
#include <limits>
#include <algorithm>
#include <cwctype>

#ifdef UNICODE
#define STOLL std::stoll
#define STOULL std::stoull
#define STOD std::stod
#define STOF std::stof
#define TO_TSTRING std::to_wstring
#else
#define STOLL std::stoll
#define STOULL std::stoull
#define STOD std::stod
#define STOF std::stof
#define TO_TSTRING std::to_string
#endif

namespace DotNetDupe {
    namespace System {

        template<typename Target, typename Source>
        static Target CheckRange(Source value) {
            if (value < (Source)(std::numeric_limits<Target>::min)() || value > (Source)(std::numeric_limits<Target>::max)()) {
                throw OverflowException(_T("Value was either too large or too small for the target type."));
            }
            return (Target)value;
        }

        // Boolean
        bool Convert::ToBoolean(bool value) { return value; }
        bool Convert::ToBoolean(int value) { return value != 0; }
        bool Convert::ToBoolean(long long value) { return value != 0; }
        bool Convert::ToBoolean(double value) { return value != 0.0; }
        bool Convert::ToBoolean(const String& value) {
            std::basic_string<TCHAR> s = (const TCHAR*)value;
            std::transform(s.begin(), s.end(), s.begin(), [](TCHAR c) { return (TCHAR)std::towlower(c); });
            if (s == _T("true")) return true;
            if (s == _T("false")) return false;
            throw FormatException(_T("String was not recognized as a valid Boolean."));
        }

        // Byte
        unsigned char Convert::ToByte(bool value) { return value ? 1 : 0; }
        unsigned char Convert::ToByte(unsigned char value) { return value; }
        unsigned char Convert::ToByte(signed char value) { return CheckRange<unsigned char>(value); }
        unsigned char Convert::ToByte(short value) { return CheckRange<unsigned char>(value); }
        unsigned char Convert::ToByte(int value) { return CheckRange<unsigned char>(value); }
        unsigned char Convert::ToByte(long long value) { return CheckRange<unsigned char>(value); }
        unsigned char Convert::ToByte(double value) { return CheckRange<unsigned char>(value); }
        unsigned char Convert::ToByte(const String& value) { return ToByte(value, 10); }
        unsigned char Convert::ToByte(const String& value, int fromBase) {
            try {
                size_t pos;
                long long val = STOLL((const TCHAR*)value, &pos, fromBase);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return CheckRange<unsigned char>(val);
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for an unsigned byte."));
            }
        }

        // SByte
        signed char Convert::ToSByte(bool value) { return value ? 1 : 0; }
        signed char Convert::ToSByte(unsigned char value) { return CheckRange<signed char>(value); }
        signed char Convert::ToSByte(signed char value) { return value; }
        signed char Convert::ToSByte(short value) { return CheckRange<signed char>(value); }
        signed char Convert::ToSByte(int value) { return CheckRange<signed char>(value); }
        signed char Convert::ToSByte(long long value) { return CheckRange<signed char>(value); }
        signed char Convert::ToSByte(double value) { return CheckRange<signed char>(value); }
        signed char Convert::ToSByte(const String& value) {
            try {
                size_t pos;
                long long val = STOLL((const TCHAR*)value, &pos, 10);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return CheckRange<signed char>(val);
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for a signed byte."));
            }
        }

        // Char
        TCHAR Convert::ToChar(unsigned short value) { return (TCHAR)value; }
        TCHAR Convert::ToChar(int value) { return CheckRange<TCHAR>(value); }
        TCHAR Convert::ToChar(long long value) { return CheckRange<TCHAR>(value); }
        TCHAR Convert::ToChar(const String& value) {
            if (value.GetLength() != 1) throw FormatException(_T("String must be exactly one character long."));
            return value[0];
        }

        // Double
        double Convert::ToDouble(int value) { return (double)value; }
        double Convert::ToDouble(long long value) { return (double)value; }
        double Convert::ToDouble(float value) { return (double)value; }
        double Convert::ToDouble(const String& value) {
            try {
                size_t pos;
                double val = STOD((const TCHAR*)value, &pos);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return val;
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for a Double."));
            }
        }

        // Single
        float Convert::ToSingle(int value) { return (float)value; }
        float Convert::ToSingle(long long value) { return (float)value; }
        float Convert::ToSingle(double value) { return (float)value; }
        float Convert::ToSingle(const String& value) {
            try {
                size_t pos;
                float val = STOF((const TCHAR*)value, &pos);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return val;
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for a Single."));
            }
        }

        // Int16
        short Convert::ToInt16(bool value) { return value ? 1 : 0; }
        short Convert::ToInt16(short value) { return value; }
        short Convert::ToInt16(int value) { return CheckRange<short>(value); }
        short Convert::ToInt16(long long value) { return CheckRange<short>(value); }
        short Convert::ToInt16(double value) { return CheckRange<short>(value); }
        short Convert::ToInt16(const String& value) { return ToInt16(value, 10); }
        short Convert::ToInt16(const String& value, int fromBase) {
            try {
                size_t pos;
                long long val = STOLL((const TCHAR*)value, &pos, fromBase);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return CheckRange<short>(val);
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for an Int16."));
            }
        }

        // Int32
        int Convert::ToInt32(bool value) { return value ? 1 : 0; }
        int Convert::ToInt32(int value) { return value; }
        int Convert::ToInt32(long long value) { return CheckRange<int>(value); }
        int Convert::ToInt32(double value) { return CheckRange<int>(value); }
        int Convert::ToInt32(const String& value) { return ToInt32(value, 10); }
        int Convert::ToInt32(const String& value, int fromBase) {
            try {
                size_t pos;
                long long val = STOLL((const TCHAR*)value, &pos, fromBase);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return CheckRange<int>(val);
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for an Int32."));
            }
        }

        // Int64
        long long Convert::ToInt64(bool value) { return value ? 1 : 0; }
        long long Convert::ToInt64(int value) { return (long long)value; }
        long long Convert::ToInt64(long long value) { return value; }
        long long Convert::ToInt64(double value) { return (long long)value; }
        long long Convert::ToInt64(const String& value) { return ToInt64(value, 10); }
        long long Convert::ToInt64(const String& value, int fromBase) {
            try {
                size_t pos;
                long long val = STOLL((const TCHAR*)value, &pos, fromBase);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return val;
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for an Int64."));
            }
        }

        // UInt16
        unsigned short Convert::ToUInt16(bool value) { return value ? 1 : 0; }
        unsigned short Convert::ToUInt16(int value) { return CheckRange<unsigned short>(value); }
        unsigned short Convert::ToUInt16(long long value) { return CheckRange<unsigned short>(value); }
        unsigned short Convert::ToUInt16(double value) { return CheckRange<unsigned short>(value); }
        unsigned short Convert::ToUInt16(const String& value) {
            try {
                size_t pos;
                unsigned long long val = STOULL((const TCHAR*)value, &pos, 10);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return CheckRange<unsigned short>(val);
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for a UInt16."));
            }
        }

        // UInt32
        unsigned int Convert::ToUInt32(bool value) { return value ? 1 : 0; }
        unsigned int Convert::ToUInt32(int value) { return CheckRange<unsigned int>(value); }
        unsigned int Convert::ToUInt32(long long value) { return CheckRange<unsigned int>(value); }
        unsigned int Convert::ToUInt32(double value) { return CheckRange<unsigned int>(value); }
        unsigned int Convert::ToUInt32(const String& value) {
            try {
                size_t pos;
                unsigned long long val = STOULL((const TCHAR*)value, &pos, 10);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return CheckRange<unsigned int>(val);
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for a UInt32."));
            }
        }

        // UInt64
        unsigned long long Convert::ToUInt64(bool value) { return value ? 1 : 0; }
        unsigned long long Convert::ToUInt64(int value) { return CheckRange<unsigned long long>(value); }
        unsigned long long Convert::ToUInt64(long long value) { return CheckRange<unsigned long long>(value); }
        unsigned long long Convert::ToUInt64(double value) { return (unsigned long long)value; }
        unsigned long long Convert::ToUInt64(const String& value) {
            try {
                size_t pos;
                unsigned long long val = STOULL((const TCHAR*)value, &pos, 10);
                if (pos != value.GetLength()) throw FormatException(_T("Input string was not in a correct format."));
                return val;
            } catch (const std::invalid_argument&) {
                throw FormatException(_T("Input string was not in a correct format."));
            } catch (const std::out_of_range&) {
                throw OverflowException(_T("Value was either too large or too small for a UInt64."));
            }
        }

        // String
        String Convert::ToString(bool value) { return value ? _T("True") : _T("False"); }
        String Convert::ToString(unsigned char value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(signed char value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(TCHAR value) { TCHAR buf[2] = { value, 0 }; return String(buf); }
        String Convert::ToString(double value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(float value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(short value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(int value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(long long value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(unsigned short value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(unsigned int value) { return String(TO_TSTRING(value).c_str()); }
        String Convert::ToString(unsigned long long value) { return String(TO_TSTRING(value).c_str()); }

        static String ToBaseString(unsigned long long value, int toBase) {
            if (toBase != 2 && toBase != 8 && toBase != 10 && toBase != 16)
                throw ArgumentException(_T("The base must be 2, 8, 10, or 16."));

            if (value == 0) return _T("0");

            std::basic_string<TCHAR> res;
            const TCHAR* digits = _T("0123456789ABCDEF");
            while (value > 0) {
                res += digits[value % toBase];
                value /= toBase;
            }
            std::reverse(res.begin(), res.end());
            return String(res.c_str());
        }

        String Convert::ToString(int value, int toBase) {
            return ToBaseString((unsigned int)value, toBase);
        }
        String Convert::ToString(long long value, int toBase) {
            return ToBaseString((unsigned long long)value, toBase);
        }
    }
}
