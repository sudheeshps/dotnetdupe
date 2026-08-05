#include "pch.h"
#include "gtest/gtest.h"
#include "System/Collections/Generic/List.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

namespace DotNetDupeTests {

    TEST(ListTest, Add_Given_Items_When_Called_Then_CountIncreases) {
        // Given
        List<int> list;

        // When
        list.Add(1);
        list.Add(2);
        list.Add(3);

        // Then
        EXPECT_EQ(list.GetCount(), 3);
        EXPECT_EQ(list[0], 1);
        EXPECT_EQ(list[2], 3);
    }

    TEST(ListTest, InitializerList_Given_Values_When_Constructed_Then_PopulatesList) {
        // Given & When
        List<String> list = { "A", "B", "C" };

        // Then
        EXPECT_EQ(list.GetCount(), 3);
        EXPECT_TRUE(list[1] == "B");
    }

    TEST(ListTest, Remove_Given_ExistingItem_When_Called_Then_RemovesItemAndReturnsTrue) {
        // Given
        List<int> list = { 1, 2, 3 };

        // When
        bool removed = list.Remove(2);

        // Then
        EXPECT_TRUE(removed);
        EXPECT_EQ(list.GetCount(), 2);
        EXPECT_EQ(list[1], 3);
    }

    TEST(ListTest, Contains_Given_Item_When_Checked_Then_ReturnsCorrectResult) {
        // Given
        List<int> list = { 1, 2, 3 };

        // When & Then
        EXPECT_TRUE(list.Contains(2));
        EXPECT_FALSE(list.Contains(4));
    }

    TEST(ListTest, Sort_Given_UnorderedList_When_Called_Then_SortsItems) {
        // Given
        List<int> list = { 3, 1, 2 };

        // When
        list.Sort();

        // Then
        EXPECT_EQ(list[0], 1);
        EXPECT_EQ(list[1], 2);
        EXPECT_EQ(list[2], 3);
    }

    TEST(ListTest, ToArray_When_Called_Then_ReturnsDotNetArray) {
        // Given
        List<int> list = { 1, 2, 3 };

        // When
        auto arr = list.ToArray();

        // Then
        EXPECT_EQ(arr.GetLength(), 3);
        EXPECT_EQ(arr[1], 2);
    }

    TEST(ListTest, RangeBasedFor_When_Used_Then_IteratesCorrectly) {
        // Given
        List<int> list = { 10, 20, 30 };
        int sum = 0;

        // When
        for (int val : list) {
            sum += val;
        }

        // Then
        EXPECT_EQ(sum, 60);
    }

    TEST(ListTest, MoveConstructor_Given_List_When_Moved_Then_TransfersOwnership) {
        // Given
        List<int> source = { 1, 2, 3 };

        // When
        List<int> destination(std::move(source));

        // Then
        EXPECT_EQ(destination.GetCount(), 3);
        EXPECT_EQ(destination[0], 1);
        EXPECT_EQ(destination[2], 3);
        EXPECT_EQ(source.GetCount(), 0);
    }

    TEST(ListTest, MoveAssignment_Given_List_When_MoveAssigned_Then_TransfersOwnership) {
        // Given
        List<int> source = { 10, 20, 30 };
        List<int> destination;

        // When
        destination = std::move(source);

        // Then
        EXPECT_EQ(destination.GetCount(), 3);
        EXPECT_EQ(destination[0], 10);
        EXPECT_EQ(destination[2], 30);
        EXPECT_EQ(source.GetCount(), 0);
    }
}
