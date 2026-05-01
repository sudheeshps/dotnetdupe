#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/String.h"
#include <tchar.h>

namespace DotNetDupe {
    namespace System {
        class BitConverter : public Object {
        public:
            DOTNETDUPE_API static const bool IsLittleEndian;

            DOTNETDUPE_API static Array<byte> GetBytes(bool value);
            DOTNETDUPE_API static Array<byte> GetBytes(TCHAR value);
            DOTNETDUPE_API static Array<byte> GetBytes(double value);
            DOTNETDUPE_API static Array<byte> GetBytes(short value);
            DOTNETDUPE_API static Array<byte> GetBytes(int value);
            DOTNETDUPE_API static Array<byte> GetBytes(long long value);
            DOTNETDUPE_API static Array<byte> GetBytes(float value);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned short value);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned int value);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned long long value);

            DOTNETDUPE_API static bool ToBoolean(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static TCHAR ToChar(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static double ToDouble(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static short ToInt16(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static int ToInt32(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static long long ToInt64(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static float ToSingle(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static unsigned short ToUInt16(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static unsigned int ToUInt32(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static unsigned long long ToUInt64(Array<byte>& value, int startIndex);

            DOTNETDUPE_API static String ToString(Array<byte>& value);
            DOTNETDUPE_API static String ToString(Array<byte>& value, int startIndex);
            DOTNETDUPE_API static String ToString(Array<byte>& value, int startIndex, int length);

            DOTNETDUPE_API static long long DoubleToInt64Bits(double value);
            DOTNETDUPE_API static double Int64BitsToDouble(long long value);
            DOTNETDUPE_API static int SingleToInt32Bits(float value);
            DOTNETDUPE_API static float Int32BitsToSingle(int value);
        };
    }
}
