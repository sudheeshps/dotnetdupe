#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/Array.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace ArrayTests {
        TEST(ArrayTest, Given_Length_When_Constructed_Then_ReturnsCorrectLength) {
            Array<int> arr(5);
            EXPECT_EQ(5, arr.GetLength());
        }

        TEST(ArrayTest, Given_InitializerList_When_Constructed_Then_ReturnsCorrectLengthAndValues) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            EXPECT_EQ(5, arr.GetLength());
            EXPECT_EQ(1, arr[0]);
            EXPECT_EQ(5, arr[4]);
        }

        TEST(ArrayTest, Given_Array_When_IndexerUsed_Then_SetsAndGetsValue) {
            Array<int> arr = { 1, 2, 3 };
            arr[1] = 10;
            EXPECT_EQ(10, arr[1]);
        }

        TEST(ArrayTest, Given_ArrayWithValues_When_IndexOfCalledWithValueInArray_Then_ReturnsCorrectIndex) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            EXPECT_EQ(2, arr.IndexOf(3));
        }

        TEST(ArrayTest, Given_ArrayWithValues_When_IndexOfCalledWithUnavailableValue_Then_ReturnsCorrectIndex) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            EXPECT_EQ(-1, arr.IndexOf(10));
        }

        TEST(ArrayTest, Given_ArrayWithValues_When_LastIndexOfCalledWithValueInArray_Then_ReturnsCorrectIndex) {
            Array<int> arr = { 1, 2, 3, 2, 1 };
            EXPECT_EQ(3, arr.LastIndexOf(2));
        }

        TEST(ArrayTest, Given_ArrayWithValues_When_LastIndexOfCalledWithUnavailableValue_Then_ReturnsCorrectIndex) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            EXPECT_EQ(-1, arr.LastIndexOf(10));
        }

        TEST(ArrayTest, Given_UnsortedArray_When_Sort_Then_ArrayIsSorted) {
            Array<int> arr = { 5, 2, 4, 1, 3 };
            arr.Sort();
            EXPECT_EQ(1, arr[0]);
            EXPECT_EQ(2, arr[1]);
            EXPECT_EQ(3, arr[2]);
            EXPECT_EQ(4, arr[3]);
            EXPECT_EQ(5, arr[4]);
        }

        TEST(ArrayTest, Given_Array_When_ReverseCalled_Then_ArrayIsReversed) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            arr.Reverse();
            EXPECT_EQ(5, arr[0]);
            EXPECT_EQ(4, arr[1]);
            EXPECT_EQ(3, arr[2]);
            EXPECT_EQ(2, arr[3]);
            EXPECT_EQ(1, arr[4]);
        }

        TEST(ArrayTest, Given_ArrayWithValues_When_Clear_Then_AllElementsAreSetToDefault) {
            Array<int> arr = { 1, 2, 3, 4, 5 };
            arr.Clear();
            for (int i = 0; i < arr.GetLength(); ++i) {
                EXPECT_EQ(0, arr[i]);
            }
        }

        TEST(ArrayTest, Given_SourceAndDestinationArrays_When_CopyToCalled_Then_ElementsAreCopied) {
            Array<int> source = { 1, 2, 3 };
            Array<int> destination(5);
            source.CopyTo(destination, 1);
            EXPECT_EQ(0, destination[0]);
            EXPECT_EQ(1, destination[1]);
            EXPECT_EQ(2, destination[2]);
            EXPECT_EQ(3, destination[3]);
            EXPECT_EQ(0, destination[4]);
        }

        TEST(ArrayTest, Given_InvalidIndex_When_CopyToCalled_Then_ThrowsArgumentOutOfRangeException) {
            Array<int> source = { 1, 2, 3 };
            Array<int> destination(5);
            EXPECT_THROW(source.CopyTo(destination, -1), ArgumentOutOfRangeException);
        }
        TEST(ArrayTest, Given_InvalidDestinationIndex_When_CopyToCalled_Then_ThrowsArgumentException) {
            Array<int> source = { 1, 2, 3 };
            Array<int> destination(3);
            EXPECT_THROW(source.CopyTo(destination, 1), ArgumentException);
        }

        TEST(ArrayTest, Given_SourceAndDestinationArrays_When_CopyCalled_Then_ElementsAreCopied) {
            Array<int> source = { 1, 2, 3, 4, 5 };
            Array<int> destination(5);
            Array<int>::Copy(source, destination, 3);
            EXPECT_EQ(1, destination[0]);
            EXPECT_EQ(2, destination[1]);
            EXPECT_EQ(3, destination[2]);
            EXPECT_EQ(0, destination[3]);
            EXPECT_EQ(0, destination[4]);
        }

        TEST(ArrayTest, Given_InvalidLength_When_CopyCalled_Then_ThrowsArgumentOutOfRangeException) {
            Array<int> source = { 1, 2, 3 };
            Array<int> destination(3);
            EXPECT_THROW(Array<int>::Copy(source, destination, -1), ArgumentOutOfRangeException);
        }

        TEST(ArrayTest, Given_SourceArraySmallerThanLength_When_CopyCalled_Then_ThrowsArgumentException) {
            Array<int> source = { 1, 2, 3 };
            Array<int> destination(5);
            EXPECT_THROW(Array<int>::Copy(source, destination, 4), ArgumentException);
        }

        TEST(ArrayTest, Given_DestinationArraySmallerThanLength_When_CopyCalled_Then_ThrowsArgumentException) {
            Array<int> source = { 1, 2, 3, 4, 5 };
            Array<int> destination(3);
            EXPECT_THROW(Array<int>::Copy(source, destination, 4), ArgumentException);
        }
    }
}
tion);
        }
    }
}
