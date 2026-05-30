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
            Array<byte> result(1);
            result[0] = bValue ? 1 : 0;
            return result;
        }

        Array<byte> BitConverter::GetBytes(char chValue) {
            Array<byte> result(1);
            result[0] = (byte)chValue;
            return result;
        }

        Array<byte> BitConverter::GetBytes(double value) {
            Array<byte> result(8);
            std::memcpy(result.GetData(), &value, 8);
            return result;
        }

        Array<byte> BitConverter::GetBytes(short iValue) {
            Array<byte> result(2);
            std::memcpy(result.GetData(), &iValue, 2);
            return result;
        }

        Array<byte> BitConverter::GetBytes(int iValue) {
            Array<byte> result(4);
            std::memcpy(result.GetData(), &iValue, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(long long llValue) {
            Array<byte> result(8);
            std::memcpy(result.GetData(), &llValue, 8);
            return result;
        }

        Array<byte> BitConverter::GetBytes(float value) {
            Array<byte> result(4);
            std::memcpy(result.GetData(), &value, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned short iValue) {
            Array<byte> result(2);
            std::memcpy(result.GetData(), &iValue, 2);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned int iValue) {
            Array<byte> result(4);
            std::memcpy(result.GetData(), &iValue, 4);
            return result;
        }

        Array<byte> BitConverter::GetBytes(unsigned long long llValue) {
            Array<byte> result(8);
            std::memcpy(result.GetData(), &llValue, 8);
            return result;
        }

        bool BitConverter::ToBoolean(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex >= value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            return value[iStartIndex] != 0;
        }

        char BitConverter::ToChar(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex >= value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            return (char)value[iStartIndex];
        }

        double BitConverter::ToDouble(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            double result;
            std::memcpy(&result, value.GetData() + iStartIndex, 8);
            return result;
        }

        short BitConverter::ToInt16(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 2 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            short iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 2);
            return iResult;
        }

        int BitConverter::ToInt32(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            int iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 4);
            return iResult;
        }

        long long BitConverter::ToInt64(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            long long llResult;
            std::memcpy(&llResult, value.GetData() + iStartIndex, 8);
            return llResult;
        }

        float BitConverter::ToSingle(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            float result;
            std::memcpy(&result, value.GetData() + iStartIndex, 4);
            return result;
        }

        unsigned short BitConverter::ToUInt16(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 2 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            unsigned short iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 2);
            return iResult;
        }

        unsigned int BitConverter::ToUInt32(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 4 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            unsigned int iResult;
            std::memcpy(&iResult, value.GetData() + iStartIndex, 4);
            return iResult;
        }

        unsigned long long BitConverter::ToUInt64(Array<byte>& value, int iStartIndex) {
            if (value.GetLength() == 0) throw ArgumentNullException("value");
            if (iStartIndex < 0 || iStartIndex + 8 > value.GetLength()) throw ArgumentOutOfRangeException("iStartIndex");
            unsigned long long llResult;
            std::memcpy(&llResult, value.GetData() + iStartIndex, 8);
            return llResult;
        }

        String BitConverter::ToString(Array<byte>& value) {
            return ToString(value, 0, value.GetLength());
        }

        String BitConverter::ToString(Array<byte>& value, int iStartIndex) {
            return ToString(value, iStartIndex, value.GetLength() - iStartIndex);
        }

        String BitConverter::ToString(Array<byte>& value, int iStartIndex, int nLength) {
            int nArrayLength = value.GetLength();
            if (iStartIndex < 0 || (iStartIndex >= nArrayLength && nArrayLength > 0))
                throw ArgumentOutOfRangeException("iStartIndex");
            if (nLength < 0)
                throw ArgumentOutOfRangeException("nLength");
            if (iStartIndex + nLength > nArrayLength)
                throw ArgumentException("iStartIndex + nLength > value.Length");

            if (nLength == 0) return String("");

            std::stringstream ss;
            for (int iIndex = 0; iIndex < nLength; ++iIndex) {
                if (iIndex > 0) ss << "-";
                ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)value [iStartIndex + iIndex];
            }
            return String(ss.str().c_str());
        }

        long long BitConverter::DoubleToInt64Bits(double value) {
            long long llResult;
            std::memcpy(&llResult, &value, sizeof(double));
            return llResult;
        }

        double BitConverter::Int64BitsToDouble(long long llValue) {
            double result;
            std::memcpy(&result, &llValue, sizeof(long long));
            return result;
        }

        int BitConverter::SingleToInt32Bits(float value) {
            int iResult;
            std::memcpy(&iResult, &value, sizeof(float));
            return iResult;
        }

        float BitConverter::Int32BitsToSingle(int iValue) {
            float result;
            std::memcpy(&result, &iValue, sizeof(int));
            return result;
        }
    }
}
