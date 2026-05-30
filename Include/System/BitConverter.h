#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/String.h"
#include "System/Char.h"

namespace DotNetDupe {
    namespace System {
        class BitConverter : public Object {
        public:
            DOTNETDUPE_API static const bool IsLittleEndian;

            DOTNETDUPE_API static Array<byte> GetBytes(bool bValue);
            DOTNETDUPE_API static Array<byte> GetBytes(char chValue);
            DOTNETDUPE_API static Array<byte> GetBytes(double dValue);
            DOTNETDUPE_API static Array<byte> GetBytes(short iValue);
            DOTNETDUPE_API static Array<byte> GetBytes(int iValue);
            DOTNETDUPE_API static Array<byte> GetBytes(long long llValue);
            DOTNETDUPE_API static Array<byte> GetBytes(float fValue);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned short iValue);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned int iValue);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned long long llValue);

            DOTNETDUPE_API static bool ToBoolean(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static char ToChar(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static double ToDouble(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static short ToInt16(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static int ToInt32(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static long long ToInt64(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static float ToSingle(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static unsigned short ToUInt16(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static unsigned int ToUInt32(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static unsigned long long ToUInt64(Array<byte>& arrValue, int iStartIndex);

            DOTNETDUPE_API static String ToString(Array<byte>& arrValue);
            DOTNETDUPE_API static String ToString(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static String ToString(Array<byte>& arrValue, int iStartIndex, int iLength);

            DOTNETDUPE_API static long long DoubleToInt64Bits(double dValue);
            DOTNETDUPE_API static double Int64BitsToDouble(long long llValue);
            DOTNETDUPE_API static int SingleToInt32Bits(float fValue);
            DOTNETDUPE_API static float Int32BitsToSingle(int iValue);
        };
    }
}
