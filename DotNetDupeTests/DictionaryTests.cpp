#include "pch.h"
#include "gtest/gtest.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

namespace DotNetDupeTests {

    TEST(DictionaryTest, Add_Given_KeyValue_When_Called_Then_CountIncreases) {
        // Given
        Dictionary<String, int> dict;

        // When
        dict.Add(_T("One"), 1);
        dict.Add(_T("Two"), 2);

        // Then
        EXPECT_EQ(dict.GetCount(), 2);
        EXPECT_EQ(dict[_T("One")], 1);
    }

    TEST(DictionaryTest, TryGetValue_Given_ExistingKey_When_Called_Then_ReturnsTrueAndValue) {
        // Given
        Dictionary<int, String> dict;
        dict.Add(1, _T("Value1"));

        // When
        String val;
        bool found = dict.TryGetValue(1, val);

        // Then
        EXPECT_TRUE(found);
        EXPECT_TRUE(val == _T("Value1"));
    }

    TEST(DictionaryTest, ContainsKey_Given_ExistingKey_When_Checked_Then_ReturnsTrue) {
        // Given
        Dictionary<String, String> dict;
        dict.Add(_T("Key"), _T("Value"));

        // When & Then
        EXPECT_TRUE(dict.ContainsKey(_T("Key")));
        EXPECT_FALSE(dict.ContainsKey(_T("Missing")));
    }

    TEST(DictionaryTest, Remove_Given_ExistingKey_When_Called_Then_RemovesItem) {
        // Given
        Dictionary<int, int> dict;
        dict.Add(10, 100);

        // When
        bool removed = dict.Remove(10);

        // Then
        EXPECT_TRUE(removed);
        EXPECT_EQ(dict.GetCount(), 0);
    }

    TEST(DictionaryTest, RangeBasedFor_When_Used_Then_IteratesOverPairs) {
        // Given
        Dictionary<int, int> dict;
        dict.Add(1, 10);
        dict.Add(2, 20);
        int sumKeys = 0;
        int sumValues = 0;

        // When
        for (auto const& [key, val] : dict) {
            sumKeys += key;
            sumValues += val;
        }

        // Then
        EXPECT_EQ(sumKeys, 3);
        EXPECT_EQ(sumValues, 30);
    }
}
