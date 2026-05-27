#include "pch.h"
#include "System/BitConverter.h"
#include <cstring>
#include <iomanip>
#include <sstream>
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"

namespace DotNetDupe {
    namespace System {
        const bool BitConverter::IsLittleEndian = true; // Most modern systems are little-endian

        Array<byte> BitConverter::GetBytes(bool value) {
            Array<byte> result(1);
            result[0] = value ? 1 : 0;
            return result;
        }

        Array<byte> BitConverter::GetBytes(char value) {
            Array<byte> result(1);
            result[0] = (byte)value;
            return result;
        }

        Array<byte> BitConverter::GetBytes(double value) {
            Array<byte> result(8);
            std::memcpy(result.GetData(), &value, 8);
            return result;
        }

        Array<byte> BitConverter::GetBytes(short value) {
            Array<byte> result(2);
            std::memcpy(result.GetData(), &value, 2);
            return result;
        }

        Array<byte> BitConverter::GetBytes(int value) {
            Array<byte> result(4);
            std::memcpy(result.GetData(), &value, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(long long value) {
            Array<byte> result(8);
            std::memcpy(result.GetData(), &value, 8);
            return result;
        }

        Array<byte> BitConverter::GetBytes(float value) {
            Array<byte> result(4);
            std::memcpy(result.GetData(), &value, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned short value) {
            Array<byte> result(2);
            std::memcpy(result.GetData(), &value, 2);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned int value) {
            Array<byte> result(4);
            std::memcpy(result.GetData(), &value, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned long long value) {
            Array<byte> result(8);
            std::memcpy(result.GetData(), &value, 8);
            return result;
        }

        bool BitConverter::ToBoolean(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex >= value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            return value[startIndex] != 0;
        }

        char BitConverter::ToChar(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex >= value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            return (char)value[startIndex];
        }

        double BitConverter::ToDouble(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            double result;
            std::memcpy(&result, value.GetData() + startIndex, 8);
            return result;
        }

        short BitConverter::ToInt16(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 2 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            short result;
            std::memcpy(&result, value.GetData() + startIndex, 2);
            return result;
        }

        int BitConverter::ToInt32(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            int result;
            std::memcpy(&result, value.GetData() + startIndex, 4);
            return result;
        }

        long long BitConverter::ToInt64(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            long long result;
            std::memcpy(&result, value.GetData() + startIndex, 8);
            return result;
        }

        float BitConverter::ToSingle(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            float result;
            std::memcpy(&result, value.GetData() + startIndex, 4);
            return result;
        }

        unsigned short BitConverter::ToUInt16(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 2 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            unsigned short result;
            std::memcpy(&result, value.GetData() + startIndex, 2);
            return result;
        }

        unsigned int BitConverter::ToUInt32(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            unsigned int result;
            std::memcpy(&result, value.GetData() + startIndex, 4);
            return result;
        }

        unsigned long long BitConverter::ToUInt64(Array<byte>& value, int startIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (startIndex < 0 || startIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("startIndex");
            unsigned long long result;
            std::memcpy(&result, value.GetData() + startIndex, 8);
            return result;
        }

        String BitConverter::ToString(Array<byte>& value) {
            return ToString(value, 0, value.GetLength());
        }

        String BitConverter::ToString(Array<byte>& value, int startIndex) {
            return ToString(value, startIndex, value.GetLength() - startIndex);
        }

        String BitConverter::ToString(Array<byte>& value, int startIndex, int length) {
            int arrayLength = value.GetLength();
            if (startIndex < 0 || (startIndex >= arrayLength && arrayLength > 0))
                throw ArgumentOutOfRangeException("startIndex");
            if (length < 0)
                throw ArgumentOutOfRangeException("length");
            if (startIndex + length > arrayLength)
                throw ArgumentException("startIndex + length > value.Length");

            if (length == 0) return String("");

            std::stringstream ss;
            for (int i = 0; i < length; ++i) {
                if (i > 0) ss << "-";
                ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)value [startIndex + i];
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
