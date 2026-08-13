#include "pch.h"
#include "gtest/gtest.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/String.h"
#include "System/ArgumentException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

namespace DotNetDupeTests {

    TEST(DictionaryTest, Add_Given_KeyValue_When_Called_Then_CountIncreases) {
        Dictionary<String, int> dict;
        dict.Add("One", 1);
        dict.Add("Two", 2);
        EXPECT_EQ(dict.GetCount(), 2);
        EXPECT_EQ(dict["One"], 1);
    }

    TEST(DictionaryTest, TryGetValue_Given_ExistingKey_When_Called_Then_ReturnsTrueAndValue) {
        Dictionary<int, String> dict;
        dict.Add(1, "Value1");
        String val;
        bool found = dict.TryGetValue(1, val);
        EXPECT_TRUE(found);
        EXPECT_TRUE(val == "Value1");
    }

    TEST(DictionaryTest, ContainsKey_Given_ExistingKey_When_Checked_Then_ReturnsTrue) {
        Dictionary<String, String> dict;
        dict.Add("Key", "Value");
        EXPECT_TRUE(dict.ContainsKey("Key"));
        EXPECT_FALSE(dict.ContainsKey("Missing"));
    }

    TEST(DictionaryTest, Remove_Given_ExistingKey_When_Called_Then_RemovesItem) {
        Dictionary<int, int> dict;
        dict.Add(10, 100);
        bool removed = dict.Remove(10);
        EXPECT_TRUE(removed);
        EXPECT_EQ(dict.GetCount(), 0);
    }

    TEST(DictionaryTest, OperatorBracket_When_KeyNotExists_Then_AddsKey) {
        Dictionary<int, int> dict;
        dict[1] = 10;
        EXPECT_EQ(dict.GetCount(), 1);
        EXPECT_EQ(dict[1], 10);
    }

    TEST(DictionaryTest, Clear_When_Called_Then_RemovesAllItems) {
        Dictionary<int, int> dict;
        dict.Add(1, 1);
        dict.Add(2, 2);
        dict.Clear();
        EXPECT_EQ(dict.GetCount(), 0);
        EXPECT_FALSE(dict.ContainsKey(1));
    }

    TEST(DictionaryTest, Resizing_When_LoadExceedsCapacity_Then_ResizesProperly) {
        Dictionary<int, int> dict;
        for (int i = 0; i < 1000; i++) {
            dict.Add(i, i * 2);
        }
        EXPECT_EQ(dict.GetCount(), 1000);
        for (int i = 0; i < 1000; i++) {
            EXPECT_EQ(dict[i], i * 2);
        }
    }
    
    TEST(DictionaryTest, GetKeysValues_ReturnsArrays) {
        Dictionary<int, int> dict;
        dict.Add(1, 10);
        dict.Add(2, 20);
        auto keys = dict.GetKeys();
        auto values = dict.GetValues();
        EXPECT_EQ(keys.GetLength(), 2);
        EXPECT_EQ(values.GetLength(), 2);
    }

    TEST(DictionaryTest, Exceptions_When_KeyNotFound_Throws) {
        Dictionary<int, int> dict;
        const auto& c_dict = dict;
        EXPECT_THROW(c_dict[1], DotNetDupe::System::ArgumentException);
    }
}
