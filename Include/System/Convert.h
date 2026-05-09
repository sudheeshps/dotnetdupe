#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Char.h"

namespace DotNetDupe {
    namespace System {
        class Convert : public Object {
        public:
            // Boolean
            DOTNETDUPE_API static bool ToBoolean(bool value);
            DOTNETDUPE_API static bool ToBoolean(int value);
            DOTNETDUPE_API static bool ToBoolean(long long value);
            DOTNETDUPE_API static bool ToBoolean(double value);
            DOTNETDUPE_API static bool ToBoolean(const String& value);

            // Byte
            DOTNETDUPE_API static unsigned char ToByte(bool value);
            DOTNETDUPE_API static unsigned char ToByte(unsigned char value);
            DOTNETDUPE_API static unsigned char ToByte(signed char value);
            DOTNETDUPE_API static unsigned char ToByte(short value);
            DOTNETDUPE_API static unsigned char ToByte(int value);
            DOTNETDUPE_API static unsigned char ToByte(long long value);
            DOTNETDUPE_API static unsigned char ToByte(double value);
            DOTNETDUPE_API static unsigned char ToByte(const String& value);
            DOTNETDUPE_API static unsigned char ToByte(const String& value, int fromBase);

            // SByte
            DOTNETDUPE_API static signed char ToSByte(bool value);
            DOTNETDUPE_API static signed char ToSByte(unsigned char value);
            DOTNETDUPE_API static signed char ToSByte(signed char value);
            DOTNETDUPE_API static signed char ToSByte(short value);
            DOTNETDUPE_API static signed char ToSByte(int value);
            DOTNETDUPE_API static signed char ToSByte(long long value);
            DOTNETDUPE_API static signed char ToSByte(double value);
            DOTNETDUPE_API static signed char ToSByte(const String& value);

            // Char
            DOTNETDUPE_API static TCHAR ToChar(unsigned short value);
            DOTNETDUPE_API static TCHAR ToChar(int value);
            DOTNETDUPE_API static TCHAR ToChar(long long value);
            DOTNETDUPE_API static TCHAR ToChar(const String& value);

            // Double
            DOTNETDUPE_API static double ToDouble(int value);
            DOTNETDUPE_API static double ToDouble(long long value);
            DOTNETDUPE_API static double ToDouble(float value);
            DOTNETDUPE_API static double ToDouble(const String& value);

            // Single
            DOTNETDUPE_API static float ToSingle(int value);
            DOTNETDUPE_API static float ToSingle(long long value);
            DOTNETDUPE_API static float ToSingle(double value);
            DOTNETDUPE_API static float ToSingle(const String& value);

            // Int16
            DOTNETDUPE_API static short ToInt16(bool value);
            DOTNETDUPE_API static short ToInt16(short value);
            DOTNETDUPE_API static short ToInt16(int value);
            DOTNETDUPE_API static short ToInt16(long long value);
            DOTNETDUPE_API static short ToInt16(double value);
            DOTNETDUPE_API static short ToInt16(const String& value);
            DOTNETDUPE_API static short ToInt16(const String& value, int fromBase);

            // Int32
            DOTNETDUPE_API static int ToInt32(bool value);
            DOTNETDUPE_API static int ToInt32(int value);
            DOTNETDUPE_API static int ToInt32(long long value);
            DOTNETDUPE_API static int ToInt32(double value);
            DOTNETDUPE_API static int ToInt32(const String& value);
            DOTNETDUPE_API static int ToInt32(const String& value, int fromBase);

            // Int64
            DOTNETDUPE_API static long long ToInt64(bool value);
            DOTNETDUPE_API static long long ToInt64(int value);
            DOTNETDUPE_API static long long ToInt64(long long value);
            DOTNETDUPE_API static long long ToInt64(double value);
            DOTNETDUPE_API static long long ToInt64(const String& value);
            DOTNETDUPE_API static long long ToInt64(const String& value, int fromBase);

            // UInt16
            DOTNETDUPE_API static unsigned short ToUInt16(bool value);
            DOTNETDUPE_API static unsigned short ToUInt16(int value);
            DOTNETDUPE_API static unsigned short ToUInt16(long long value);
            DOTNETDUPE_API static unsigned short ToUInt16(double value);
            DOTNETDUPE_API static unsigned short ToUInt16(const String& value);

            // UInt32
            DOTNETDUPE_API static unsigned int ToUInt32(bool value);
            DOTNETDUPE_API static unsigned int ToUInt32(int value);
            DOTNETDUPE_API static unsigned int ToUInt32(long long value);
            DOTNETDUPE_API static unsigned int ToUInt32(double value);
            DOTNETDUPE_API static unsigned int ToUInt32(const String& value);

            // UInt64
            DOTNETDUPE_API static unsigned long long ToUInt64(bool value);
            DOTNETDUPE_API static unsigned long long ToUInt64(int value);
            DOTNETDUPE_API static unsigned long long ToUInt64(long long value);
            DOTNETDUPE_API static unsigned long long ToUInt64(double value);
            DOTNETDUPE_API static unsigned long long ToUInt64(const String& value);

            // String
            DOTNETDUPE_API static String ToString(bool value);
            DOTNETDUPE_API static String ToString(unsigned char value);
            DOTNETDUPE_API static String ToString(signed char value);
            DOTNETDUPE_API static String ToString(TCHAR value);
            DOTNETDUPE_API static String ToString(double value);
            DOTNETDUPE_API static String ToString(float value);
            DOTNETDUPE_API static String ToString(short value);
            DOTNETDUPE_API static String ToString(int value);
            DOTNETDUPE_API static String ToString(long long value);
            DOTNETDUPE_API static String ToString(unsigned short value);
            DOTNETDUPE_API static String ToString(unsigned int value);
            DOTNETDUPE_API static String ToString(unsigned long long value);
            DOTNETDUPE_API static String ToString(int value, int toBase);
            DOTNETDUPE_API static String ToString(long long value, int toBase);
        };
    }
}
