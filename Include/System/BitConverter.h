/// \file BitConverter.h
/// \brief Converts base data types to arrays of bytes, and arrays of bytes to base data types.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/String.h"
#include "System/Char.h"

namespace DotNetDupe {
    namespace System {
        /// \brief Converts base data types to arrays of bytes, and vice versa.
        ///
        /// Conforms to .NET System.BitConverter specifications with endianness detection,
        /// primitive type byte extraction, and IEEE 754 floating-point bit-cast conversions.
        /// All methods are thread-safe and stateless.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.14 (System.BitConverter) and IEEE 754.
        /// \see Convert, Buffer
        class BitConverter : public Object {
        public:
            /// \brief Indicates whether this computer architecture is little-endian.
            DOTNETDUPE_API static const bool IsLittleEndian;

            /**
             * @brief Returns the specified Boolean value as a byte array.
             */
            DOTNETDUPE_API static Array<byte> GetBytes(bool bValue);

            /**
             * @brief Returns the specified character value as a byte array.
             */
            DOTNETDUPE_API static Array<byte> GetBytes(char chValue);

            /**
             * @brief Returns the specified double-precision floating-point value as a byte array.
             */
            DOTNETDUPE_API static Array<byte> GetBytes(double dValue);

            /**
             * @brief Returns the specified 16-bit signed integer value as a byte array.
             */
            DOTNETDUPE_API static Array<byte> GetBytes(short iValue);

            /**
             * @brief Returns the specified 32-bit signed integer value as a byte array.
             */
            DOTNETDUPE_API static Array<byte> GetBytes(int iValue);

            /**
             * @brief Returns the specified 64-bit signed integer value as a byte array.
             */
            DOTNETDUPE_API static Array<byte> GetBytes(long long llValue);

            /**
             * @brief Returns the specified single-precision floating-point value as a byte array.
             */
            DOTNETDUPE_API static Array<byte> GetBytes(float fValue);

            DOTNETDUPE_API static Array<byte> GetBytes(unsigned short iValue);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned int iValue);
            DOTNETDUPE_API static Array<byte> GetBytes(unsigned long long llValue);

            /**
             * @brief Returns a Boolean value converted from one byte at a specified position in a byte array.
             */
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

            /**
             * @brief Converts the numeric value of each element of a specified array of bytes to its equivalent hexadecimal string representation.
             */
            DOTNETDUPE_API static String ToString(Array<byte>& arrValue);
            DOTNETDUPE_API static String ToString(Array<byte>& arrValue, int iStartIndex);
            DOTNETDUPE_API static String ToString(Array<byte>& arrValue, int iStartIndex, int iLength);

            /**
             * @brief Converts the specified 64-bit double-precision floating point number to a 64-bit signed integer bit pattern.
             */
            DOTNETDUPE_API static long long DoubleToInt64Bits(double dValue);

            /**
             * @brief Reinterprets the specified 64-bit signed integer bit pattern as a 64-bit double-precision floating point number.
             */
            DOTNETDUPE_API static double Int64BitsToDouble(long long llValue);

            /**
             * @brief Converts the specified 32-bit single-precision floating point number to a 32-bit signed integer bit pattern.
             */
            DOTNETDUPE_API static int SingleToInt32Bits(float fValue);

            /**
             * @brief Reinterprets the specified 32-bit signed integer bit pattern as a 32-bit single-precision floating point number.
             */
            DOTNETDUPE_API static float Int32BitsToSingle(int iValue);
        };
    }
}
