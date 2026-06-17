#include "pch.h"
#include "System/Convert.h"
#include "System/OverflowException.h"
#include "System/FormatException.h"
#include <string>
#include <limits>
#include <algorithm>
#include <cwctype>

namespace DotNetDupe {
    namespace System {

        template<typename Target, typename Source>
        static Target CheckRange(Source value) {
            if (value < (Source)(std::numeric_limits<Target>::min)() || value > (Source)(std::numeric_limits<Target>::max)()) {
                throw OverflowException("Value was either too large or too small for the target type.");
            }
            return (Target)value;
        }

        // Boolean
        bool Convert::ToBoolean(bool bValue) { return bValue; }
        bool Convert::ToBoolean(int iValue) { return iValue != 0; }
        bool Convert::ToBoolean(long long llValue) { return llValue != 0; }
        bool Convert::ToBoolean(double value) { return value != 0.0; }
        bool Convert::ToBoolean(const String& sValue) {
            std::string s = (const char*)sValue;
            std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)std::towlower(c); });
            if (s == "true") return true;
            if (s == "false") return false;
            throw FormatException("String was not recognized as a valid Boolean.");
        }

        // Byte
        unsigned char Convert::ToByte(bool bValue) { return bValue ? 1 : 0; }
        unsigned char Convert::ToByte(unsigned char chValue) { return chValue; }
        unsigned char Convert::ToByte(signed char chValue) { return CheckRange<unsigned char>(chValue); }
        unsigned char Convert::ToByte(short iValue) { return CheckRange<unsigned char>(iValue); }
        unsigned char Convert::ToByte(int iValue) { return CheckRange<unsigned char>(iValue); }
        unsigned char Convert::ToByte(long long llValue) { return CheckRange<unsigned char>(llValue); }
        unsigned char Convert::ToByte(double value) { return CheckRange<unsigned char>(value); }
        unsigned char Convert::ToByte(const String& sValue) { return ToByte(sValue, 10); }
        unsigned char Convert::ToByte(const String& sValue, int iFromBase) {
            try {
                size_t nPos;
                long long llVal = std::stoll((const char*)sValue, &nPos, iFromBase);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return CheckRange<unsigned char>(llVal);
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for an unsigned byte.");
            }
        }

        // SByte
        signed char Convert::ToSByte(bool bValue) { return bValue ? 1 : 0; }
        signed char Convert::ToSByte(unsigned char chValue) { return CheckRange<signed char>(chValue); }
        signed char Convert::ToSByte(signed char chValue) { return chValue; }
        signed char Convert::ToSByte(short iValue) { return CheckRange<signed char>(iValue); }
        signed char Convert::ToSByte(int iValue) { return CheckRange<signed char>(iValue); }
        signed char Convert::ToSByte(long long llValue) { return CheckRange<signed char>(llValue); }
        signed char Convert::ToSByte(double value) { return CheckRange<signed char>(value); }
        signed char Convert::ToSByte(const String& sValue) {
            try {
                size_t nPos;
                long long llVal = std::stoll((const char*)sValue, &nPos, 10);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return CheckRange<signed char>(llVal);
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for a signed byte.");
            }
        }

        // Char
        char Convert::ToChar(unsigned short iValue) { return (char)iValue; }
        char Convert::ToChar(int iValue) { return CheckRange<char>(iValue); }
        char Convert::ToChar(long long llValue) { return CheckRange<char>(llValue); }
        char Convert::ToChar(const String& sValue) {
            if (sValue.GetLength() != 1) throw FormatException("String must be exactly one character long.");
            return sValue[0];
        }

        // Double
        double Convert::ToDouble(int iValue) { return (double)iValue; }
        double Convert::ToDouble(long long llValue) { return (double)llValue; }
        double Convert::ToDouble(float value) { return (double)value; }
        double Convert::ToDouble(const String& sValue) {
            try {
                size_t nPos;
                double val = std::stod((const char*)sValue, &nPos);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return val;
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for a Double.");
            }
        }

        // Single
        float Convert::ToSingle(int iValue) { return (float)iValue; }
        float Convert::ToSingle(long long llValue) { return (float)llValue; }
        float Convert::ToSingle(double value) { return (float)value; }
        float Convert::ToSingle(const String& sValue) {
            try {
                size_t nPos;
                float val = std::stof((const char*)sValue, &nPos);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return val;
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for a Single.");
            }
        }

        // Int16
        short Convert::ToInt16(bool bValue) { return bValue ? 1 : 0; }
        short Convert::ToInt16(short iValue) { return iValue; }
        short Convert::ToInt16(int iValue) { return CheckRange<short>(iValue); }
        short Convert::ToInt16(long long llValue) { return CheckRange<short>(llValue); }
        short Convert::ToInt16(double value) { return CheckRange<short>(value); }
        short Convert::ToInt16(const String& sValue) { return ToInt16(sValue, 10); }
        short Convert::ToInt16(const String& sValue, int iFromBase) {
            try {
                size_t nPos;
                long long llVal = std::stoll((const char*)sValue, &nPos, iFromBase);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return CheckRange<short>(llVal);
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for an Int16.");
            }
        }

        // Int32
        int Convert::ToInt32(bool bValue) { return bValue ? 1 : 0; }
        int Convert::ToInt32(int iValue) { return iValue; }
        int Convert::ToInt32(long long llValue) { return CheckRange<int>(llValue); }
        int Convert::ToInt32(double value) { return CheckRange<int>(value); }
        int Convert::ToInt32(const String& sValue) { return ToInt32(sValue, 10); }
        int Convert::ToInt32(const String& sValue, int iFromBase) {
            try {
                size_t nPos;
                long long llVal = std::stoll((const char*)sValue, &nPos, iFromBase);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return CheckRange<int>(llVal);
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for an Int32.");
            }
        }

        // Int64
        long long Convert::ToInt64(bool bValue) { return bValue ? 1 : 0; }
        long long Convert::ToInt64(int iValue) { return (long long)iValue; }
        long long Convert::ToInt64(long long llValue) { return llValue; }
        long long Convert::ToInt64(double value) { return (long long)value; }
        long long Convert::ToInt64(const String& sValue) { return ToInt64(sValue, 10); }
        long long Convert::ToInt64(const String& sValue, int iFromBase) {
            try {
                size_t nPos;
                long long llVal = std::stoll((const char*)sValue, &nPos, iFromBase);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return llVal;
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for an Int64.");
            }
        }

        // UInt16
        unsigned short Convert::ToUInt16(bool bValue) { return bValue ? 1 : 0; }
        unsigned short Convert::ToUInt16(int iValue) { return CheckRange<unsigned short>(iValue); }
        unsigned short Convert::ToUInt16(long long llValue) { return CheckRange<unsigned short>(llValue); }
        unsigned short Convert::ToUInt16(double value) { return CheckRange<unsigned short>(value); }
        unsigned short Convert::ToUInt16(const String& sValue) {
            try {
                size_t nPos;
                unsigned long long llVal = std::stoull((const char*)sValue, &nPos, 10);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return CheckRange<unsigned short>(llVal);
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for a UInt16.");
            }
        }

        // UInt32
        unsigned int Convert::ToUInt32(bool bValue) { return bValue ? 1 : 0; }
        unsigned int Convert::ToUInt32(int iValue) { return CheckRange<unsigned int>(iValue); }
        unsigned int Convert::ToUInt32(long long llValue) { return CheckRange<unsigned int>(llValue); }
        unsigned int Convert::ToUInt32(double value) { return CheckRange<unsigned int>(value); }
        unsigned int Convert::ToUInt32(const String& sValue) {
            try {
                size_t nPos;
                unsigned long long llVal = std::stoull((const char*)sValue, &nPos, 10);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return CheckRange<unsigned int>(llVal);
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for a UInt32.");
            }
        }

        // UInt64
        unsigned long long Convert::ToUInt64(bool bValue) { return bValue ? 1 : 0; }
        unsigned long long Convert::ToUInt64(int iValue) { return CheckRange<unsigned long long>(iValue); }
        unsigned long long Convert::ToUInt64(long long llValue) { return CheckRange<unsigned long long>(llValue); }
        unsigned long long Convert::ToUInt64(double value) { return (unsigned long long)value; }
        unsigned long long Convert::ToUInt64(const String& sValue) {
            try {
                size_t nPos;
                unsigned long long llVal = std::stoull((const char*)sValue, &nPos, 10);
                if (nPos != sValue.GetLength()) throw FormatException("Input string was not in a correct format.");
                return llVal;
            } catch (const std::invalid_argument&) {
                throw FormatException("Input string was not in a correct format.");
            } catch (const std::out_of_range&) {
                throw OverflowException("Value was either too large or too small for a UInt64.");
            }
        }

        // String
        String Convert::ToString(bool bValue) { return bValue ? "True" : "False"; }
        String Convert::ToString(unsigned char chValue) { return String(std::to_string(chValue).c_str()); }
        String Convert::ToString(signed char chValue) { return String(std::to_string(chValue).c_str()); }
        String Convert::ToString(char chValue) { char buf[2] = { chValue, 0 }; return String(buf); }
        String Convert::ToString(double value) { return String(std::to_string(value).c_str()); }
        String Convert::ToString(float value) { return String(std::to_string(value).c_str()); }
        String Convert::ToString(short iValue) { return String(std::to_string(iValue).c_str()); }
        String Convert::ToString(int iValue) { return String(std::to_string(iValue).c_str()); }
        String Convert::ToString(long long llValue) { return String(std::to_string(llValue).c_str()); }
        String Convert::ToString(unsigned short iValue) { return String(std::to_string(iValue).c_str()); }
        String Convert::ToString(unsigned int iValue) { return String(std::to_string(iValue).c_str()); }
        String Convert::ToString(unsigned long long llValue) { return String(std::to_string(llValue).c_str()); }

        static String ToBaseString(unsigned long long llValue, int iToBase) {
            if (iToBase != 2 && iToBase != 8 && iToBase != 10 && iToBase != 16)
                throw ArgumentException("The base must be 2, 8, 10, or 16.");

            if (llValue == 0) return "0";

            std::string res;
            const char* digits = "0123456789ABCDEF";
            while (llValue > 0) {
                res += digits[llValue % iToBase];
                llValue /= iToBase;
            }
            std::reverse(res.begin(), res.end());
            return String(res.c_str());
        }

        String Convert::ToString(int iValue, int iToBase) {
            return ToBaseString((unsigned int)iValue, iToBase);
        }
        String Convert::ToString(long long llValue, int iToBase) {
            return ToBaseString((unsigned long long)llValue, iToBase);
        }

        static const char base64_chars[] = 
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz"
                     "0123456789+/";

        static inline bool is_base64(unsigned char c) {
            return (isalnum(c) || (c == '+') || (c == '/'));
        }

        String Convert::ToBase64String(const Array<char>& inArray) {
            int in_len = inArray.GetLength();
            if (in_len == 0) return String("");
            const unsigned char* bytes_to_encode = reinterpret_cast<const unsigned char*>(inArray.GetData());
            std::string ret;
            int i = 0;
            int j = 0;
            unsigned char char_array_3[3];
            unsigned char char_array_4[4];

            while (in_len--) {
                char_array_3[i++] = *(bytes_to_encode++);
                if (i == 3) {
                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;

                    for(i = 0; (i < 4) ; i++)
                        ret += base64_chars[char_array_4[i]];
                    i = 0;
                }
            }

            if (i) {
                for(j = i; j < 3; j++)
                    char_array_3[j] = '\0';

                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

                for (j = 0; (j < i + 1); j++)
                    ret += base64_chars[char_array_4[j]];

                while((i++ < 3))
                    ret += '=';
            }

            return String(ret.c_str());
        }

        Array<char> Convert::FromBase64String(const String& s) {
            std::string encoded_string = s.GetRawString();
            int in_len = static_cast<int>(encoded_string.size());
            int i = 0;
            int j = 0;
            int in_ = 0;
            unsigned char char_array_4[4], char_array_3[3];
            std::vector<char> decoded;

            while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
                char_array_4[i++] = encoded_string[in_]; in_++;
                if (i == 4) {
                    for (i = 0; i < 4; i++) {
                        const char* ptr = std::strchr(base64_chars, char_array_4[i]);
                        if (ptr == nullptr) throw ArgumentException("Invalid base64 character");
                        char_array_4[i] = static_cast<unsigned char>(ptr - base64_chars);
                    }

                    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                    for (i = 0; (i < 3); i++)
                        decoded.push_back(char_array_3[i]);
                    i = 0;
                }
            }

            if (i) {
                for (j = 0; j < i; j++) {
                    const char* ptr = std::strchr(base64_chars, char_array_4[j]);
                    if (ptr == nullptr) throw ArgumentException("Invalid base64 character");
                    char_array_4[j] = static_cast<unsigned char>(ptr - base64_chars);
                }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

                for (j = 0; (j < i - 1); j++) 
                    decoded.push_back(char_array_3[j]);
            }

            Array<char> result(static_cast<int>(decoded.size()));
            if (!decoded.empty()) {
                std::memcpy(result.GetData(), decoded.data(), decoded.size());
            }
            return result;
        }
    }
}
