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

        Array<byte> BitConverter::GetBytes(bool bValue) {
            /// Step: Convert boolean to single byte.
            Array<byte> result(1);
            result[0] = bValue ? 1 : 0;
            return result;
        }

        Array<byte> BitConverter::GetBytes(char chValue) {
            /// Step: Convert char to single byte.
            Array<byte> result(1);
            result[0] = (byte)chValue;
            return result;
        }

        Array<byte> BitConverter::GetBytes(double value) {
            /// Step: Copy IEEE 754 64-bit double bytes into byte array.
            Array<byte> result(8);
            std::memcpy(result.GetData(), &value, 8);
            return result;
        }

        Array<byte> BitConverter::GetBytes(short iValue) {
            /// Step: Copy 16-bit signed integer bytes into byte array.
            Array<byte> result(2);
            std::memcpy(result.GetData(), &iValue, 2);
            return result;
        }

        Array<byte> BitConverter::GetBytes(int iValue) {
            /// Step: Copy 32-bit signed integer bytes into byte array.
            Array<byte> result(4);
            std::memcpy(result.GetData(), &iValue, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(long long llValue) {
            /// Step: Copy 64-bit signed integer bytes into byte array.
            Array<byte> result(8);
            std::memcpy(result.GetData(), &llValue, 8);
            return result;
        }

        Array<byte> BitConverter::GetBytes(float value) {
            /// Step: Copy IEEE 754 32-bit float bytes into byte array.
            Array<byte> result(4);
            std::memcpy(result.GetData(), &value, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned short iValue) {
            /// Step: Copy 16-bit unsigned integer bytes into byte array.
            Array<byte> result(2);
            std::memcpy(result.GetData(), &iValue, 2);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned int iValue) {
            /// Step: Copy 32-bit unsigned integer bytes into byte array.
            Array<byte> result(4);
            std::memcpy(result.GetData(), &iValue, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned long long llValue) {
            /// Step: Copy 64-bit unsigned integer bytes into byte array.
            Array<byte> result(8);
            std::memcpy(result.GetData(), &llValue, 8);
            return result;
        }

        bool BitConverter::ToBoolean(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex >= value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Return: Boolean evaluation.
            return value[iStartIndex] != 0;
        }

        char BitConverter::ToChar(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex >= value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Return: Char conversion.
            return (char)value[iStartIndex];
        }

        double BitConverter::ToDouble(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 8 bytes into IEEE 754 double.
            double result;
            std::memcpy(&result, value.GetData() + iStartIndex, 8);
            return result;
        }

        short BitConverter::ToInt16(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 2 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 2 bytes into 16-bit integer.
            short iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 2);
            return iResult;
        }

        int BitConverter::ToInt32(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 4 bytes into 32-bit integer.
            int iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 4);
            return iResult;
        }

        long long BitConverter::ToInt64(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 8 bytes into 64-bit integer.
            long long llResult;
            std::memcpy(&llResult, value.GetData() + iStartIndex, 8);
            return llResult;
        }

        float BitConverter::ToSingle(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 4 bytes into IEEE 754 float.
            float result;
            std::memcpy(&result, value.GetData() + iStartIndex, 4);
            return result;
        }

        unsigned short BitConverter::ToUInt16(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 2 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 2 bytes into 16-bit unsigned integer.
            unsigned short iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 2);
            return iResult;
        }

        unsigned int BitConverter::ToUInt32(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 4 bytes into 32-bit unsigned integer.
            unsigned int iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 4);
            return iResult;
        }

        unsigned long long BitConverter::ToUInt64(Array<byte>& value, int iStartIndex) {
            /// Guard: Validate input buffer bounds.
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            /// Step: Copy 8 bytes into 64-bit unsigned integer.
            unsigned long long llResult;
            std::memcpy(&llResult, value.GetData() + iStartIndex, 8);
            return llResult;
        }

        String BitConverter::ToString(Array<byte>& value) {
            /// Forward: Delegate with full array range.
            return ToString(value, 0, value.GetLength());
        }

        String BitConverter::ToString(Array<byte>& value, int iStartIndex) {
            /// Forward: Delegate with start index and remaining range.
            return ToString(value, iStartIndex, value.GetLength() - iStartIndex);
        }

        String BitConverter::ToString(Array<byte>& value, int iStartIndex, int nLength) {
            /// Guard: Validate index and range parameters.
            int nArrayLength = value.GetLength();
            if (iStartIndex < 0 || (iStartIndex >= nArrayLength && nArrayLength > 0))
                throw ArgumentOutOfRangeException("iStartIndex");
            if (nLength < 0)
                throw ArgumentOutOfRangeException("nLength");
            if (iStartIndex + nLength > nArrayLength)
                throw ArgumentException("iStartIndex + nLength > value.Length");

            if (nLength == 0) return String("");

            /// Stream each byte as an uppercase 2-digit zero-padded hexadecimal number separated by '-'.
            std::stringstream ss;
            for (int iIndex = 0; iIndex < nLength; ++iIndex) {
                if (iIndex > 0) ss << "-";
                ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)value [iStartIndex + iIndex];
            }
            return String(ss.str().c_str());
        }

        long long BitConverter::DoubleToInt64Bits(double value) {
            /// Reinterpret IEEE 754 floating-point bits to 64-bit integer via std::memcpy.
            long long llResult;
            std::memcpy(&llResult, &value, sizeof(double));
            return llResult;
        }

        double BitConverter::Int64BitsToDouble(long long llValue) {
            /// Reinterpret 64-bit integer bits to IEEE 754 double via std::memcpy.
            double result;
            std::memcpy(&result, &llValue, sizeof(long long));
            return result;
        }

        int BitConverter::SingleToInt32Bits(float value) {
            /// Reinterpret 32-bit float bits to 32-bit integer via std::memcpy.
            int iResult;
            std::memcpy(&iResult, &value, sizeof(float));
            return iResult;
        }

        float BitConverter::Int32BitsToSingle(int iValue) {
            /// Reinterpret 32-bit integer bits to 32-bit float via std::memcpy.
            float result;
            std::memcpy(&result, &iValue, sizeof(int));
            return result;
        }
    }
}
