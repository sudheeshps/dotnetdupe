/// \file Buffer.h
/// \brief Manipulates arrays of primitive types at byte-level boundaries.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Char.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"

namespace DotNetDupe {
    namespace System {
        using byte = unsigned char;

        /// \class Buffer
        /// \brief Manipulates arrays of primitive types efficiently at the byte level.
        ///
        /// Modeled after .NET System.Buffer. Provides methods to copy bytes between arrays,
        /// retrieve individual bytes, assign byte values, and calculate byte lengths.
        class Buffer : public Object {
        public:
            /// \brief Copies a specified number of bytes from a source array starting at a particular offset to a destination array.
            /// \tparam T Element type of the arrays.
            /// \param src Source array.
            /// \param srcOffset Zero-based byte offset into src.
            /// \param dst Destination array.
            /// \param dstOffset Zero-based byte offset into dst.
            /// \param count Number of bytes to copy.
            /// \throws DotNetDupe::System::ArgumentNullException If src or dst is null.
            /// \throws DotNetDupe::System::ArgumentOutOfRangeException If offsets or count are negative.
            /// \throws DotNetDupe::System::ArgumentException If offsets + count exceeds byte length of either array.
            template <typename T>
            static void BlockCopy(Array<T>& src, int srcOffset, Array<T>& dst, int dstOffset, int count) {
                if (src.IsNull())
                    throw ArgumentNullException("src");
                if (dst.IsNull())
                    throw ArgumentNullException("dst");

                if (srcOffset < 0)
                    throw ArgumentOutOfRangeException("srcOffset");
                if (dstOffset < 0)
                    throw ArgumentOutOfRangeException("dstOffset");
                if (count < 0)
                    throw ArgumentOutOfRangeException("count");

                if (srcOffset + count > src.GetLength() * sizeof(T))
                    throw ArgumentException("srcOffset + count > src.ByteLength");
                if (dstOffset + count > dst.GetLength() * sizeof(T))
                    throw ArgumentException("dstOffset + count > dst.ByteLength");

                /// Step: Copy contiguous byte segments from source into destination offset.
                for (int i = 0; i < count; ++i) {
                    *((char*)dst.GetData() + dstOffset + i) = *((char*)src.GetData() + srcOffset + i);
                }
            }

            /// \brief Returns the number of bytes in the specified array.
            /// \tparam T Element type of the array.
            /// \param array The array whose byte length is determined.
            /// \return Total byte length of array.
            /// \throws DotNetDupe::System::ArgumentNullException If array is null.
            template <typename T>
            static int ByteLength(Array<T>& array) {
                if (array.IsNull())
                    throw ArgumentNullException("array");
                return array.GetLength() * sizeof(T);
            }

            /// \brief Retrieves the byte at a specified location in a specified array.
            /// \tparam T Element type of the array.
            /// \param array The array to read from.
            /// \param index Zero-based byte index.
            /// \return The byte at the specified index.
            /// \throws DotNetDupe::System::ArgumentOutOfRangeException If index is out of range.
            template <typename T>
            static byte GetByte(Array<T>& array, int index) {
                if (index < 0 || index >= ByteLength(array))
                    throw ArgumentOutOfRangeException("index");
                return *((byte*)array.GetData() + index);
            }

            /// \brief Assigns a specified value to a byte at a particular location in a specified array.
            /// \tparam T Element type of the array.
            /// \param array The array to modify.
            /// \param index Zero-based byte index.
            /// \param value Byte value to write.
            /// \throws DotNetDupe::System::ArgumentOutOfRangeException If index is out of range.
            template <typename T>
            static void SetByte(Array<T>& array, int index, byte value) {
                if (index < 0 || index >= ByteLength(array))
                    throw ArgumentOutOfRangeException("index");
                *((byte*)array.GetData() + index) = value;
            }
        };

    }
}