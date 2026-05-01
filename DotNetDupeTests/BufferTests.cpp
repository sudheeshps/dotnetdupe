#include "pch.h"
#include "gtest/gtest.h"
#include "System/Buffer.h"
#include "System/Array.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace BufferTests {
        TEST(BufferTest, Given_SourceAndDestinationArrays_When_BlockCopyIsCalled_Then_BytesAreCopied) {
            Array<int> src = { 1, 2, 3, 4, 5 };
            Array<int> dst(5);

            Buffer::BlockCopy(src, 0, dst, 0, 5 * sizeof(int));

            for (int i = 0; i < 5; ++i) {
                EXPECT_EQ(src [i], dst [i]);
            }
        }

        TEST(BufferTest, Given_AnArray_When_ByteLengthIsCalled_Then_ReturnsTheCorrectLengthInBytes) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            EXPECT_EQ(5 * sizeof(int), Buffer::ByteLength(arr));
        }

        TEST(BufferTest, Given_AnArrayAndAnIndex_When_GetByteIsCalled_Then_ReturnsTheByteAtThatIndex) {
            Array<int> arr = { 1 };
            byte b = Buffer::GetByte(arr, 0);
            EXPECT_EQ(1, b);
        }

        TEST(BufferTest, Given_AnArrayAnIndexAndAValue_When_SetByteIsCalled_Then_TheByteAtThatIndexIsSetToTheValue) {
            Array<int> arr = { 1 };
            Buffer::SetByte(arr, 0, 2);
            EXPECT_EQ(2, Buffer::GetByte(arr, 0));
        }
    }
}