#include "pch.h"

#include <memory>
#include "System/Char.h"

#include "gtest/gtest.h"

#include "System/String.h"
#include "System/Utils/StringConvert.h"
#include "System/ArgumentException.h"
#include "System/FormatException.h"

//<MethodName>_Should_<ExpectedBehavior>_When_<StateUnderTest>

using namespace DotNetDupe::System;
namespace SystemTests {
    namespace StringTestCases {
        TEST(StringTest, Constructor_Should_Throw_Exception_When_NullPointer_Specified) {
            ASSERT_THROW(String str(nullptr), ArgumentException);
        }

        TEST(StringTest, AssignmentOperator_Should_AssignValue_When_ValidCharPointerPassed) {
            // Given
            String str("Initial");
            const char* pNewStr = "Updated String";

            // When
            str = pNewStr;

            // Then
            ASSERT_EQ(str, "Updated String");
        }

        TEST(StringTest, AssignmentOperator_Should_ThrowArgumentException_When_NullPointerPassed) {
            // Given
            String str("Initial");
            const char* pNullStr = nullptr;

            // When / Then
            ASSERT_THROW(str = pNullStr, ArgumentException);
        }
        TEST(StringTest, CopyConstructor_Should_CopyValue_When_ValidStringPassed) {
            // Given
            String original("Hello World");

            // When
            String copy(original);

            // Then
            ASSERT_EQ(copy, "Hello World");
            ASSERT_EQ(original, "Hello World");
        }

        TEST(StringTest, StartsWith_SingleArgument_ReturnsTrueWhenMatchingPrefix) {
            String str("Hello World");
            EXPECT_TRUE(str.StartsWith("Hello"));
            EXPECT_FALSE(str.StartsWith("world"));
        }

        TEST(StringTest, FromInt_ValueOf_And_StaticToString_ReturnFormattedString) {
            EXPECT_TRUE(String::FromInt(42).Equals("42"));
            EXPECT_TRUE(String::ValueOf(100).Equals("100"));
            EXPECT_TRUE(String::ValueOf(3.14).Equals("3.14"));
            EXPECT_TRUE(String::ToString(1234).Equals("1234"));
            EXPECT_TRUE(String::ToString(true).Equals("True"));
        }

        TEST(StringTest, CopyAssignmentOperator_Should_AssignValue_When_ValidStringPassed) {
            // Given
            String original("Hello World");
            String assigned("Initial");

            // When
            assigned = original;

            // Then
            ASSERT_EQ(assigned, "Hello World");
            ASSERT_EQ(original, "Hello World");
        }

        TEST(StringTest, GetLength_Should_Return_ActualLength_When_A_GoodString_Specified) {
            String str("Hello");
            ASSERT_TRUE(str.GetLength() == 5);
        }
        TEST(StringTest, EqualToOperator_Should_Return_True_When_Same_Strings_Compared) {
            String str1("Hello world");
            String str2("Hello world");
            ASSERT_TRUE(str1 == str2);
        }
        TEST(StringTest, EqualToOperator_Should_Return_False_When_Different_Strings_Compared) {
            String str1("Hello world");
            String str2("Hello world1");
            ASSERT_FALSE(str1 == str2);
        }
        TEST(StringTest, Clone_Should_Return_A_Copy_Of_Same_String_When_Invoked) {
            String str1("Hello world");
            String str2 = str1.Clone();
            ASSERT_TRUE(str1 == str2);
        }
        TEST(StringTest, Compare_Should_Return_Zero_When_Same_Strings_Compared) {
            String str1("Hello world");
            String str2("Hello world");
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 11, true) == 0);
        }
        TEST(StringTest, Compare_Should_Return_Greater_Than_Zero_When_Same_First_String_Is_Longer) {
            String str1("Hello world");
            String str2("Hello worl");
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 11, true) > 0);
        }
        TEST(StringTest, Compare_Should_Return_Less_Than_Zero_When_Same_Second_String_Is_Longer) {
            String str1("Hello world");
            String str2("Hello world2");
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 12, true) < 0);
        }
        TEST(StringTest, Compare_Should_Return_Zero_When_Given_Length_Of_Characters_Matches) {
            String str1("Hello world");
            String str2("Hello world2");
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 11, true) == 0);
        }
        TEST(StringTest, Compare_Should_Return_Zero_When_Given_Same_Range_Of_Characters_Matches) {
            String str1("Hello world");
            String str2("Hello world2");
            ASSERT_TRUE(String::Compare(str1, 5, str2, 5, 5, true) == 0);
        }
        TEST(StringTest, Compare_Should_Return_Greater_Than_Zero_When_Second_String_Range_Is_Lexically_Greater) {
            String str1("Hello world");
            String str2("Hello world2");
            ASSERT_TRUE(String::Compare(str1, 0, str2, 5, 5, true) > 0);
        }
        TEST(StringTest, Compare_Should_Return_Greater_Than_Zero_When_Second_String_Range_Is_Lexically_Greater2) {
            String str1("Hello world");
            String str2("HELLO world2");
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 5, false) > 0);
        }
        TEST(StringTest, CompareTo_Should_Return_Zero_When_Same_Strings_Compared) {
            String str1("Hello world");
            String str2("Hello world");
            ASSERT_TRUE(str1.CompareTo(str2) == 0);
        }
        TEST(StringTest, CompareTo_Should_Return_Greater_Than_Zero_When_Second_String_Is_Lexically_Greater) {
            String str1("Hello world");
            String str2("HELLO WORLD");
            ASSERT_TRUE(str1.CompareTo(str2) > 0);
        }
        TEST(StringTest, EqualToOperator_Should_Return_True_When_Two_Added_Strings_Compared_To_Expected_String) {
            String str1("Hello");
            String str2("World");
            ASSERT_TRUE((str1 + str2) == ("HelloWorld"));
        }
        TEST(StringTest, EqualToOperator_Should_Return_True_When_Strings_Concatinated_Compared_To_Expected_String) {
            String str1("Happy");
            str1 = str1.Concat({ "New", "Year", "2024" });
            ASSERT_TRUE(str1 == "HappyNewYear2024");
        }
        TEST(StringTest, EqualToOperator_Should_Return_False_When_Strings_Concatinated_Compared_To_A_Different_String) {
            String str1("Happy");
            str1 = str1.Concat({ "New", "Year", "2024" });
            ASSERT_FALSE(str1 == "HappyNewYear2023");
        }
        TEST(StringTest, Contains_Should_Return_True_When_A_Contained_Character_Is_Searched) {
            String str1("Happy");
            ASSERT_TRUE(str1.Contains('H'));
        }
        TEST(StringTest, Contains_Should_Return_False_When_A_Non_Contained_Character_Is_Searched) {
            String str1("Happy");
            ASSERT_FALSE(str1.Contains('N'));
        }
        TEST(StringTest, Contains_Should_Return_True_When_A_Contained_Substring_Is_Searched) {
            String str1("Happy New Year 2024");
            ASSERT_TRUE(str1.Contains("Happy"));
            ASSERT_TRUE(str1.Contains("Year"));
        }
        TEST(StringTest, Contains_Should_Return_False_When_A_Non_Contained_Substring_Is_Searched) {
            String str1("Happy New Year 2024");
            ASSERT_TRUE(str1.Contains("Happy"));
            ASSERT_TRUE(str1.Contains("Year"));
            ASSERT_FALSE(str1.Contains("2023"));
        }
        TEST(StringTest, CopyTo_Should_Copy_The_Substring_In_Range_When_A_Dest_Buffer_And_Range_Specified) {
            String str1("Hello World");
            char tszDest[12] = { 0 };
            str1.CopyTo(0, tszDest, 0, 12, 11);
            ASSERT_TRUE(str1 == String(tszDest));
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Dest_Buffer_Specified) {
            String str1("Hello World");
            ASSERT_THROW(str1.CopyTo(0, nullptr, 0, 12, 12), ArgumentException);
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Start_Index_Specified) {
            String str1("Hello World");
            char tszDest[12] = { 0 };
            ASSERT_THROW(str1.CopyTo(-1, tszDest, 0, 12, 12), ArgumentOutOfRangeException);
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Char_Count_Specified) {
            String str1("Hello World");
            char tszDest[12] = { 0 };
            ASSERT_THROW(str1.CopyTo(0, tszDest, 0, 12, 15), ArgumentOutOfRangeException);
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Dest_Buffer_Size_Specified) {
            String str1("Hello World");
            char tszDest[12] = { 0 };
            ASSERT_THROW(str1.CopyTo(0, tszDest, 0, 15, 12), ArgumentOutOfRangeException);
        }
        TEST(StringTest, EndsWith_Should_Return_True_When_A_Character_Searched_Is_Found) {
            String str1("Hello World");
            ASSERT_TRUE(str1.EndsWith('d', true));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_A_Character_Searched_Is_Not_Found) {
            String str1("Hello World");
            ASSERT_FALSE(str1.EndsWith('D', false));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_A_Character_Searched_Is_Not_Last_One) {
            String str1("Hello World");
            ASSERT_FALSE(str1.EndsWith('l', false));
        }
        TEST(StringTest, EndsWith_Should_Return_True_When_Substring_Searched_Ignoring_Case) {
            String str1("Hello World");
            ASSERT_TRUE(str1.EndsWith("WORLD", true));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_Substring_Searched_Considering_Case) {
            String str1("Hello World");
            ASSERT_FALSE(str1.EndsWith("WORLD", false));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_Substring_Searched_Is_Not_The_Ending_One) {
            String str1("Hello World");
            ASSERT_FALSE(str1.EndsWith("Hello", true));
        }
        TEST(StringTest, Equals_Should_Return_True_When_Same_Strings_Are_Compared) {
            String str1("Hello World");
            String str2("Hello World");
            ASSERT_TRUE(String::Equals(str1, str2));
        }
        TEST(StringTest, Equals_Should_Return_False_When_Different_Strings_Are_Compared) {
            String str1("Hello World");
            String str2("Hello World2");
            ASSERT_FALSE(String::Equals(str1, str2));
        }
        TEST(StringTest, Equals_Should_Return_True_When_Current_String_Compared_To_A_String_Of_Same_Content) {
            String str1("Hello World");
            String str2("Hello World");
            ASSERT_TRUE(str1.Equals(str2));
        }
        TEST(StringTest, Equals_Should_Return_False_When_Current_String_Compared_To_A_String_Of_Different_Content) {
            String str1("Hello World");
            String str2("Hello World2");
            ASSERT_FALSE(str1.Equals(str2));
        }
        TEST(StringTest, IndexOf_Should_Return_The_Index_When_An_Existing_Substring_Searched) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOf("World") == 6);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Non_Existing_Substring_Searched) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOf("2024") == -1);
        }
        TEST(StringTest, IndexOf_Should_Return_The_Index_When_An_Existing_Substring_Searched_From_A_Start_Index) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOf("World", 5, false) == 6);
        }
        TEST(StringTest, IndexOf_Should_Return_The_Index_When_An_Existing_Substring_Searched_Ignoring_Case_From_A_Start_Index) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOf("WORLD", 5, true) == 6);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Existing_Substring_Not_In_Range_Searched) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOf("WORLD", 7, true) == -1);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Existing_Substring_Searched_Considering_Case) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOf("WORLD", 5, false) == -1);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Invalid_Start_Index_Specified) {
            String str("Hello World");
            ASSERT_THROW(str.IndexOf("WORLD", -1, false), ArgumentOutOfRangeException);
        }
        TEST(StringTest, IndexOfAny_Should_Return_Index_Of_First_Char_When_Chars_Searched_From_Index) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOfAny(5, { 'r', 'l', 'd' }) == 8);
        }
        TEST(StringTest, IndexOfAny_Should_Return_Index_Of_First_Char_When_Chars_Searched_From_Index2) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOfAny(5, { 'H', 'e', 'd' }) == 10);
        }
        TEST(StringTest, IndexOfAny_Should_Return_Minus_One_When_Chars_Searched_Not_Found_After_An_Index) {
            String str("Hello World");
            ASSERT_TRUE(str.IndexOfAny(5, { 'H', 'e', 'b' }) == -1);
        }
        TEST(StringTest, IndexOfAny_Should_Throw_Exception_When_Invalid_Start_Index_Specified) {
            String str("Hello World");
            ASSERT_THROW(str.IndexOfAny(-1, { 'H', 'e', 'b' }), ArgumentOutOfRangeException);
        }
        TEST(StringTest, IndexOfAny_Should_Throw_Exception_When_Invalid_Start_Index_Specified2) {
            String str("Hello World");
            ASSERT_THROW(str.IndexOfAny(13, { 'H', 'e', 'b' }), ArgumentOutOfRangeException);
        }

        TEST(StringTest, Insert_Should_Return_Combined_String_When_A_String_Inserted_At_Index_Zero) {
            String str("Hello World");
            ASSERT_TRUE(str.Insert(0, "2024 ") == "2024 Hello World");
        }
        TEST(StringTest, Insert_Should_Returned_Combined_String_When_A_String_Inserted_At_Index_In_Between) {
            String str("Hello World");
            ASSERT_TRUE(str.Insert(6, "2024 ") == "Hello 2024 World");
        }
        TEST(StringTest, Insert_Should_Returned_Combined_String_When_A_String_Inserted_At_Index_In_Between2) {
            String str("Hello World");
            ASSERT_TRUE(str.Insert(5, "o 2024") == "Helloo 2024 World");
        }
        TEST(StringTest, Insert_Should_Throw_Exception_When_Invalid_Index_Specified) {
            String str("Hello World");
            ASSERT_THROW(str.Insert(12, " 2024"), ArgumentOutOfRangeException);
        }
        TEST(StringTest, Insert_Should_Throw_Exception_When_Invalid_Index_Specified2) {
            String str("Hello World");
            ASSERT_THROW(str.Insert(-1, " 2024"), ArgumentOutOfRangeException);
        }
        TEST(StringTest, IsEmpty_Should_Return_True_When_Current_String_Is_Empty) {
            String str("");
            ASSERT_TRUE(str.IsEmpty());
        }
        TEST(StringTest, IsEmpty_Should_Return_False_When_Current_String_Is_Not_Empty) {
            String str(" ");
            ASSERT_FALSE(str.IsEmpty());
        }
        TEST(StringTest, Join_Should_Return_Combined_String_Separated_By_Specified_Char_When_Invoked) {
            String newStr = String::Join(',', { "Apple", "Orange","Grapes","Mango" });
            ASSERT_TRUE(newStr == "Apple,Orange,Grapes,Mango");
            String newStr2 = String::Join('|', { "Apple", "Orange" });
            ASSERT_TRUE(newStr2 == "Apple|Orange");
            String newStr3 = String::Join('|', { "Apple" });
            ASSERT_TRUE(newStr3 == "Apple");
        }
        TEST(StringTest, Join_Should_Return_Combined_String_Separated_By_Specified_Char_When_Invoked2) {
            String newStr = String::Join(',', { "Apple", "Orange","Grapes","Mango" },
                0, 4);
            ASSERT_TRUE(newStr == "Apple,Orange,Grapes,Mango");
            String newStr2 = String::Join('|', { "Apple", "Orange" }, 0, 2);
            ASSERT_TRUE(newStr2 == "Apple|Orange");
            String newStr3 = String::Join('|', { "Apple" }, 0, 1);
            ASSERT_TRUE(newStr3 == "Apple");
            String newStr4 = String::Join(',', { "Apple", "Orange","Grapes","Mango" },
                1, 3);
            ASSERT_TRUE(newStr4 == "Orange,Grapes,Mango");
        }
        TEST(StringTest, Join_Should_Return_Combined_String_Separated_By_Specified_String_When_Invoked) {
            String newStr = String::Join("##", { "Apple", "Orange","Grapes","Mango" });
            ASSERT_TRUE(newStr == "Apple##Orange##Grapes##Mango");
            String newStr2 = String::Join("|", { "Apple", "Orange" });
            ASSERT_TRUE(newStr2 == "Apple|Orange");
            String newStr3 = String::Join("|", { "Apple" });
            ASSERT_TRUE(newStr3 == "Apple");
        }
        TEST(StringTest, Join_Should_Return_Combined_String_When_Invoked_With_A_List_Of_Strings_And_Count) {
            String newStr = String::Join("==", { "Apple", "Orange","Grapes","Mango" },
                0, 4);
            ASSERT_TRUE(newStr == "Apple==Orange==Grapes==Mango");
            String newStr2 = String::Join("****", { "Apple", "Orange" }, 0, 2);
            ASSERT_TRUE(newStr2 == "Apple****Orange");
            String newStr3 = String::Join("|", { "Apple" }, 0, 1);
            ASSERT_TRUE(newStr3 == "Apple");
            String newStr4 = String::Join(",", { "Apple", "Orange","Grapes","Mango" },
                1, 3);
            ASSERT_TRUE(newStr4 == "Orange,Grapes,Mango");
        }
        TEST(StringTest, LastIndexOf_Should_Return_Index_When_Valid_Substring_Specified) {
            String str("Hello World World 2024");
            ASSERT_TRUE(str.LastIndexOf("WORLD", true) == 12);
            ASSERT_TRUE(str.LastIndexOf("2024", true) == 18);
        }
        TEST(StringTest, LastIndexOf_Should_Find_Correct_Index_When_Invoked_Considering_Case) {
            String str("Hello World World 2024");
            ASSERT_TRUE(str.LastIndexOf("WORLD", false) == -1);
            ASSERT_TRUE(str.LastIndexOf("World", false) == 12);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_With_A_Set_Of_Valid_Chars) {
            String str("Hello World");
            ASSERT_TRUE(str.LastIndexOfAny(0, { 'O', 'l', 'd' }, false) == 9);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_With_A_Set_Of_Valid_Chars2) {
            String str("Hello World");
            ASSERT_TRUE(str.LastIndexOfAny(5, { 'L', 'E', 'd' }, false) == 10);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Minus_One_When_Invoked_With_An_Invalid_Start_Index) {
            String str("Hello World");
            ASSERT_TRUE(str.LastIndexOfAny(5, { 'H', 'e', 'b' }, false) == -1);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_Ignoring_Case) {
            String str("Hello World");
            ASSERT_TRUE(str.LastIndexOfAny(0, { 'o', 'l', 'd' }, true) == 7);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_Ignoring_Case2) {
            String str("Hello World");
            ASSERT_TRUE(str.LastIndexOfAny(5, { 'l', 'e', 'd' }, true) == 9);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Minus_One_When_Invoked_With_A_Start_Index_That_Skips_Chars) {
            String str("Hello World");
            ASSERT_TRUE(str.LastIndexOfAny(5, { 'H', 'e', 'b' }, true) == -1);
        }
        TEST(StringTest, LastIndexOfAny_Should_Throw_Exception_When_Invoked_With_Invalid_Start_Index) {
            String str("Hello World");
            ASSERT_THROW(str.LastIndexOfAny(-1, { 'H', 'e', 'b' }, true), ArgumentOutOfRangeException);
        }
        TEST(StringTest, LastIndexOfAny_Should_Throw_Exception_When_Invoked_With_Invalid_Start_Index2) {
            String str("Hello World");
            ASSERT_THROW(str.LastIndexOfAny(13, { 'H', 'e', 'b' }, true), ArgumentOutOfRangeException);
        }
        TEST(StringTest, PadLeft_Returns_Left_Padded_String_When_Invoked_With_A_Total_Width) {
            String str("Hello World");
            String strPadded = str.PadLeft(15);
            ASSERT_TRUE(strPadded == "    Hello World");
        }
        TEST(StringTest, PadLeft_Returns_Same_String_When_Invoked_With_A_Total_Width_Which_Is_String_Len) {
            String str("Hello World");
            String strPadded = str.PadLeft(11);
            ASSERT_TRUE(strPadded == "Hello World");
        }
        TEST(StringTest, PadLeft_Throws_Exception_When_Invoked_With_An_Invalid_Total_Width) {
            String str("Hello World");
            ASSERT_THROW(str.PadLeft(-1), ArgumentException);
        }
        TEST(StringTest, PadLeft_Returns_String_Padded_Left_With_Specified_Char_When_Invoked_With_A_Char_And_Total_Width) {
            String str("Hello World");
            String strPadded = str.PadLeft(15, '#');
            ASSERT_TRUE(strPadded == "####Hello World");
        }
        TEST(StringTest, PadLeft_Returns_Same_String_When_Invoked_With_Char_And_Total_Width_Equals_To_String_Len) {
            String str("Hello World");
            String strPadded = str.PadLeft(11, '@');
            ASSERT_TRUE(strPadded == "Hello World");
        }
        TEST(StringTest, PadLeft_Throws_Exception_When_Invoked_With_Invalid_Total_Width) {
            String str("Hello World");
            ASSERT_THROW(str.PadLeft(-1, '#'), ArgumentException);
        }
        TEST(StringTest, PadRight_Returns_Right_Padded_String_When_Invoked_With_A_Total_Width) {
            String str("Hello World");
            String strPadded = str.PadRight(15);
            ASSERT_TRUE(strPadded == "Hello World    ");
        }
        TEST(StringTest, PadRight_Returns_The_Same_String_When_Invoked_With_A_Total_Width_Equal_To_String_Len) {
            String str("Hello World");
            String strPadded = str.PadRight(11);
            ASSERT_TRUE(strPadded == "Hello World");
        }
        TEST(StringTest, PadRight_Throws_Exception_When_Invoked_Invalid_Total_Width) {
            String str("Hello World");
            ASSERT_THROW(str.PadRight(-1), ArgumentException);
        }
        TEST(StringTest, PadRight_Returns_A_String_Padded_Right_With_Specified_Char_When_Invoked_With_A_Char) {
            String str("Hello World");
            String strPadded = str.PadRight(15, '#');
            ASSERT_TRUE(strPadded == "Hello World####");
        }
        TEST(StringTest, PadRight_Returns_Same_String_When_Total_Width_Equal_To_String_Len) {
            String str("Hello World");
            String strPadded = str.PadRight(11, '@');
            ASSERT_TRUE(strPadded == "Hello World");
        }
        TEST(StringTest, PadRight_Throws_Exception_When_Invalid_Total_Width_Specified) {
            String str("Hello World");
            ASSERT_THROW(str.PadRight(-1, '#'), ArgumentException);
        }
        TEST(StringTest, Remove_Throws_Exception_When_Invoked_With_An_Invalid_Index) {
            String str("Hello World");
            ASSERT_THROW(str.Remove(-1), ArgumentOutOfRangeException);
        }
        TEST(StringTest, Remove_Throws_Exception_When_Invoked_With_An_Invalid_Index2) {
            String str("Hello World");
            ASSERT_THROW(str.Remove(12), ArgumentOutOfRangeException);
        }
        TEST(StringTest, Remove_Returns_String_Before_Index_When_Invoked_With_An_Index) {
            String str("Hello World");
            ASSERT_TRUE(str.Remove(5) == "Hello");
        }
        TEST(StringTest, Remove_Returns_Empty_String_Before_Index_When_Invoked_With_An_Index_Of_Zero) {
            String str("Hello World");
            ASSERT_TRUE(str.Remove(0) == "");
        }
        TEST(StringTest, Remove_Returns_A_String_After_Index_When_Invoked_With_An_Index_And_Count) {
            String str("Hello World");
            ASSERT_TRUE(str.Remove(0, 5) == " World");
        }
        TEST(StringTest, Replace_Returns_A_New_Replaced_String_When_Invoked_With_A_Char_To_Be_Replaced) {
            String str("Hello World");
            ASSERT_TRUE(str.Replace('l', '*') == "He**o Wor*d");
        }
        TEST(StringTest, Replace_Returns_Same_String_When_Char_To_Be_Replaced_Not_Found) {
            String str("Hello World");
            ASSERT_TRUE(str.Replace('i', '*') == "Hello World");
        }
        TEST(StringTest, Replace_Returns_Replaced_New_String_When_String_To_Be_Replaced_Found) {
            String str("Hello World World");
            ASSERT_TRUE(str.Replace("World", "My World") == "Hello My World My World");
        }

        TEST(StringTest, Format_Should_ReturnFormattedString_When_SimplePlaceholdersSpecified) {
            // Given
            String formatStr = "Hello {0}, welcome to {1}!";
            
            // When
            String result = String::Format(formatStr, "John", 2024);
            
            // Then
            ASSERT_EQ(result, "Hello John, welcome to 2024!");
        }

        TEST(StringTest, Format_Should_ReturnOriginalString_When_NoPlaceholdersSpecified) {
            // Given
            String formatStr = "Hello World";
            
            // When
            String result = String::Format(formatStr);
            
            // Then
            ASSERT_EQ(result, "Hello World");
        }

        TEST(StringTest, Format_Should_HandleEscapedBraces_When_DoubleBracesSpecified) {
            // Given
            String formatStr = "{{hello}} {0} {{0}}";
            
            // When
            String result = String::Format(formatStr, "world");
            
            // Then
            ASSERT_EQ(result, "{hello} world {0}");
        }

        TEST(StringTest, Format_Should_FormatVariousTypes_When_VariousTypesPassed) {
            // Given
            String formatStr = "{0} - {1} - {2} - {3}";
            
            // When
            String result = String::Format(formatStr, true, 3.14, nullptr, "Alice");
            
            // Then
            ASSERT_EQ(result, "True - 3.14 -  - Alice");
        }

        TEST(StringTest, Format_Should_ThrowFormatException_When_PlaceholderIndexOutOfRange) {
            // Given
            String formatStr = "Hello {1}";
            
            // When/Then
            ASSERT_THROW(String::Format(formatStr, "world"), FormatException);
        }

        TEST(StringConvertTest, WCharToUtf8_Should_ConvertWideStringToUtf8_When_ValidWideStringPassed) {
            // Given
            const wchar_t* pwszInput = L"DotNetDupe Wide String";

            // When
            std::string sUtf8 = Utils::StringConvert::WCharToUtf8(pwszInput);

            // Then
            ASSERT_EQ(sUtf8, "DotNetDupe Wide String");
        }

        TEST(StringConvertTest, Utf8ToWChar_Should_ConvertUtf8ToWideString_When_ValidUtf8StringPassed) {
            // Given
            const char* pszInput = "DotNetDupe UTF8 String";

            // When
            std::wstring sWide = Utils::StringConvert::Utf8ToWChar(pszInput);

            // Then
            ASSERT_TRUE(sWide == L"DotNetDupe UTF8 String");
        }

        TEST(StringConvertTest, ConversionMethods_Should_ReturnEmptyString_When_NullPointerPassed) {
            // Given / When / Then
            ASSERT_EQ(Utils::StringConvert::WCharToUtf8(nullptr), "");
            ASSERT_TRUE(Utils::StringConvert::Utf8ToWChar(nullptr) == L"");
        }

        TEST(StringTest, Constructor_Should_ConvertWideString_When_WideCharPointerPassed) {
            // Given
            const wchar_t* pwszWide = L"Wide String Test";

            // When
            String str(pwszWide);

            // Then
            ASSERT_EQ(str, "Wide String Test");
        }

        TEST(StringTest, AssignmentOperator_Should_ConvertWideString_When_WideCharPointerPassed) {
            // Given
            String str;
            const wchar_t* pwszWide = L"Assigned Wide String";

            // When
            str = pwszWide;

            // Then
            ASSERT_EQ(str, "Assigned Wide String");
        }
    }
}