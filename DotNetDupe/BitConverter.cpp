#include "pch.h"
#include "System/BitConverter.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include <cstring>
#include <iomanip>
#include <sstream>

namespace DotNetDupe {
    namespace System {
        const bool BitConverter::IsLittleEndian = true;

        template <typename T>
        static Array<byte> GetBytesInternal(T value) {
            Array<byte> bytes(sizeof(T));
            std::memcpy(bytes.GetData(), &value, sizeof(T));
            return bytes;
        }

        template <typename T>
        static T ToTypeInternal(Array<byte>& value, int startIndex) {
            if (value.IsNull())
                throw ArgumentNullException(_T("value"));
            if (startIndex < 0 || startIndex > value.GetLength() - (int)sizeof(T))
                throw ArgumentOutOfRangeException(_T("startIndex"));
            T result;
            std::memcpy(&result, (byte*)value.GetData() + startIndex, sizeof(T));
            return result;
        }

        Array<byte> BitConverter::GetBytes(bool value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(TCHAR value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(double value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(short value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(int value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(long long value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(float value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(unsigned short value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(unsigned int value) { return GetBytesInternal(value); }
        Array<byte> BitConverter::GetBytes(unsigned long long value) { return GetBytesInternal(value); }

        bool BitConverter::ToBoolean(Array<byte>& value, int startIndex) { return ToTypeInternal<bool>(value, startIndex); }
        TCHAR BitConverter::ToChar(Array<byte>& value, int startIndex) { return ToTypeInternal<TCHAR>(value, startIndex); }
        double BitConverter::ToDouble(Array<byte>& value, int startIndex) { return ToTypeInternal<double>(value, startIndex); }
        short BitConverter::ToInt16(Array<byte>& value, int startIndex) { return ToTypeInternal<short>(value, startIndex); }
        int BitConverter::ToInt32(Array<byte>& value, int startIndex) { return ToTypeInternal<int>(value, startIndex); }
        long long BitConverter::ToInt64(Array<byte>& value, int startIndex) { return ToTypeInternal<long long>(value, startIndex); }
        float BitConverter::ToSingle(Array<byte>& value, int startIndex) { return ToTypeInternal<float>(value, startIndex); }
        unsigned short BitConverter::ToUInt16(Array<byte>& value, int startIndex) { return ToTypeInternal<unsigned short>(value, startIndex); }
        unsigned int BitConverter::ToUInt32(Array<byte>& value, int startIndex) { return ToTypeInternal<unsigned int>(value, startIndex); }
        unsigned long long BitConverter::ToUInt64(Array<byte>& value, int startIndex) { return ToTypeInternal<unsigned long long>(value, startIndex); }

        String BitConverter::ToString(Array<byte>& value) {
            if (value.IsNull()) throw ArgumentNullException(_T("value"));
            return ToString(value, 0, value.GetLength());
        }

        String BitConverter::ToString(Array<byte>& value, int startIndex) {
            if (value.IsNull()) throw ArgumentNullException(_T("value"));
            return ToString(value, startIndex, value.GetLength() - startIndex);
        }

        String BitConverter::ToString(Array<byte>& value, int startIndex, int length) {
            if (value.IsNull())
                throw ArgumentNullException(_T("value"));
            
            int arrayLength = value.GetLength();
            if (startIndex < 0 || (startIndex >= arrayLength && startIndex != 0))
                throw ArgumentOutOfRangeException(_T("startIndex"));
            if (length < 0)
                throw ArgumentOutOfRangeException(_T("length"));
            if (startIndex + length > arrayLength)
                throw ArgumentException(_T("startIndex + length > value.Length"));

            if (length == 0) return String(_T(""));

            std::basic_ostringstream<TCHAR> ss;
            for (int i = 0; i < length; ++i) {
                if (i > 0) ss << _T("-");
                ss << std::uppercase << std::hex << std::setw(2) << std::setfill(_T('0')) << (int)value [startIndex + i];
            }
            return String(ss.str().c_str());
        }

        long long BitConverter::DoubleToInt64Bits(double value) {
            long long result;
            std::memcpy(&result, &value, sizeof(double));
            return result;
        }

        double BitConverter::Int64BitsToDouble(long long value) {
            double result;
            std::memcpy(&result, &value, sizeof(long long));
            return result;
        }

        int BitConverter::SingleToInt32Bits(float value) {
            int result;
            std::memcpy(&result, &value, sizeof(float));
            return result;
        }

        float BitConverter::Int32BitsToSingle(int value) {
            float result;
            std::memcpy(&result, &value, sizeof(int));
            return result;
        }
    }
}
