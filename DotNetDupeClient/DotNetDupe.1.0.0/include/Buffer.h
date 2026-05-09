#pragma once

#include "Common.h"
#include "Object.h"
#include "Array.h"
#include <tchar.h>
#include "ArgumentException.h"
#include "ArgumentNullException.h"
#include "ArgumentOutOfRangeException.h"

namespace DotNetDupe {
    namespace System {
        using byte = unsigned char;

        class Buffer : public Object {
        public:
            template <typename T>
            static void BlockCopy(Array<T>& src, int srcOffset, Array<T>& dst, int dstOffset, int count) {
                if (src.IsNull())
                    throw ArgumentNullException(_T("src"));
                if (dst.IsNull())
                    throw ArgumentNullException(_T("dst"));

                if (srcOffset < 0)
                    throw ArgumentOutOfRangeException(_T("srcOffset"));
                if (dstOffset < 0)
                    throw ArgumentOutOfRangeException(_T("dstOffset"));
                if (count < 0)
                    throw ArgumentOutOfRangeException(_T("count"));

                if (srcOffset + count > src.GetLength() * sizeof(T))
                    throw ArgumentException(_T("srcOffset + count > src.ByteLength"));
                if (dstOffset + count > dst.GetLength() * sizeof(T))
                    throw ArgumentException(_T("dstOffset + count > dst.ByteLength"));

                // TODO: This is not a real block copy, it's a byte-wise copy.
                // A real block copy would use something like memcpy.
                // For now, this is a placeholder.
                for (int i = 0; i < count; ++i) {
                    *((char*)dst.GetData() + dstOffset + i) = *((char*)src.GetData() + srcOffset + i);
                }
            }

            template <typename T>
            static int ByteLength(Array<T>& array) {
                if (array.IsNull())
                    throw ArgumentNullException(_T("array"));
                return array.GetLength() * sizeof(T);
            }

            template <typename T>
            static byte GetByte(Array<T>& array, int index) {
                if (index < 0 || index >= ByteLength(array))
                    throw ArgumentOutOfRangeException(_T("index"));
                return *((byte*)array.GetData() + index);
            }

            template <typename T>
            static void SetByte(Array<T>& array, int index, byte value) {
                if (index < 0 || index >= ByteLength(array))
                    throw ArgumentOutOfRangeException(_T("index"));
                *((byte*)array.GetData() + index) = value;
            }
        };
    }
}