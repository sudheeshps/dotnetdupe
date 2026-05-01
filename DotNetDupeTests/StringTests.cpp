#include "pch.h"

#include <memory>
#include <tchar.h>

#include "gtest/gtest.h"

#include "System/String.h"
#include "System/ArgumentException.h"

//<MethodName>_Should_<ExpectedBehavior>_When_<StateUnderTest>

using namespace DotNetDupe::System;
namespace SystemTests {
    namespace StringTestCases {
        TEST(StringTest, Constructor_Should_Throw_Exception_When_NullPointer_Specified) {
            ASSERT_THROW(String str(nullptr), ArgumentException);
        }
        TEST(StringTest, GetLength_Should_Return_ActualLength_When_A_GoodString_Specified) {
            String str(_T("Hello"));
            ASSERT_TRUE(str.GetLength() == 5);
        }
        TEST(StringTest, EqualToOperator_Should_Return_True_When_Same_Strings_Compared) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world"));
            ASSERT_TRUE(str1 == str2);
        }
        TEST(StringTest, EqualToOperator_Should_Return_False_When_Different_Strings_Compared) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world1"));
            ASSERT_FALSE(str1 == str2);
        }
        TEST(StringTest, Clone_Should_Return_A_Copy_Of_Same_String_When_Invoked) {
            String str1(_T("Hello world"));
            String str2 = str1.Clone();
            ASSERT_TRUE(str1 == str2);
        }
        TEST(StringTest, Compare_Should_Return_Zero_When_Same_Strings_Compared) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world"));
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 11, true) == 0);
        }
        TEST(StringTest, Compare_Should_Return_Greater_Than_Zero_When_Same_First_String_Is_Longer) {
            String str1(_T("Hello world"));
            String str2(_T("Hello worl"));
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 11, true) > 0);
        }
        TEST(StringTest, Compare_Should_Return_Less_Than_Zero_When_Same_Second_String_Is_Longer) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world2"));
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 12, true) < 0);
        }
        TEST(StringTest, Compare_Should_Return_Zero_When_Given_Length_Of_Characters_Matches) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world2"));
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 11, true) == 0);
        }
        TEST(StringTest, Compare_Should_Return_Zero_When_Given_Same_Range_Of_Characters_Matches) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world2"));
            ASSERT_TRUE(String::Compare(str1, 5, str2, 5, 5, true) == 0);
        }
        TEST(StringTest, Compare_Should_Return_Greater_Than_Zero_When_Second_String_Range_Is_Lexically_Greater) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world2"));
            ASSERT_TRUE(String::Compare(str1, 0, str2, 5, 5, true) > 0);
        }
        TEST(StringTest, Compare_Should_Return_Greater_Than_Zero_When_Second_String_Range_Is_Lexically_Greater2) {
            String str1(_T("Hello world"));
            String str2(_T("HELLO world2"));
            ASSERT_TRUE(String::Compare(str1, 0, str2, 0, 5, false) > 0);
        }
        TEST(StringTest, CompareTo_Should_Return_Zero_When_Same_Strings_Compared) {
            String str1(_T("Hello world"));
            String str2(_T("Hello world"));
            ASSERT_TRUE(str1.CompareTo(str2) == 0);
        }
        TEST(StringTest, CompareTo_Should_Return_Greater_Than_Zero_When_Second_String_Is_Lexically_Greater) {
            String str1(_T("Hello world"));
            String str2(_T("HELLO WORLD"));
            ASSERT_TRUE(str1.CompareTo(str2) > 0);
        }
        TEST(StringTest, EqualToOperator_Should_Return_True_When_Two_Added_Strings_Compared_To_Expected_String) {
            String str1(_T("Hello"));
            String str2(_T("World"));
            ASSERT_TRUE((str1 + str2) == (_T("HelloWorld")));
        }
        TEST(StringTest, EqualToOperator_Should_Return_True_When_Strings_Concatinated_Compared_To_Expected_String) {
            String str1(_T("Happy"));
            str1 = str1.Concat({ _T("New"), _T("Year"), _T("2024") });
            ASSERT_TRUE(str1 == _T("HappyNewYear2024"));
        }
        TEST(StringTest, EqualToOperator_Should_Return_False_When_Strings_Concatinated_Compared_To_A_Different_String) {
            String str1(_T("Happy"));
            str1 = str1.Concat({ _T("New"), _T("Year"), _T("2024") });
            ASSERT_FALSE(str1 == _T("HappyNewYear2023"));
        }
        TEST(StringTest, Contains_Should_Return_True_When_A_Contained_Character_Is_Searched) {
            String str1(_T("Happy"));
            ASSERT_TRUE(str1.Contains(_T('H')));
        }
        TEST(StringTest, Contains_Should_Return_False_When_A_Non_Contained_Character_Is_Searched) {
            String str1(_T("Happy"));
            ASSERT_FALSE(str1.Contains(_T('N')));
        }
        TEST(StringTest, Contains_Should_Return_True_When_A_Contained_Substring_Is_Searched) {
            String str1(_T("Happy New Year 2024"));
            ASSERT_TRUE(str1.Contains(_T("Happy")));
            ASSERT_TRUE(str1.Contains(_T("Year")));
        }
        TEST(StringTest, Contains_Should_Return_False_When_A_Non_Contained_Substring_Is_Searched) {
            String str1(_T("Happy New Year 2024"));
            ASSERT_TRUE(str1.Contains(_T("Happy")));
            ASSERT_TRUE(str1.Contains(_T("Year")));
            ASSERT_FALSE(str1.Contains(_T("2023")));
        }
        TEST(StringTest, CopyTo_Should_Copy_The_Substring_In_Range_When_A_Dest_Buffer_And_Range_Specified) {
            String str1(_T("Hello World"));
            TCHAR tszDest[12] = { 0 };
            str1.CopyTo(0, tszDest, 0, 12, 11);
            ASSERT_TRUE(str1 == String(tszDest));
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Dest_Buffer_Specified) {
            String str1(_T("Hello World"));
            ASSERT_THROW(str1.CopyTo(0, nullptr, 0, 12, 12), ArgumentException);
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Start_Index_Specified) {
            String str1(_T("Hello World"));
            TCHAR tszDest[12] = { 0 };
            ASSERT_THROW(str1.CopyTo(-1, tszDest, 0, 12, 12), ArgumentOutOfRangeException);
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Char_Count_Specified) {
            String str1(_T("Hello World"));
            TCHAR tszDest[12] = { 0 };
            ASSERT_THROW(str1.CopyTo(0, tszDest, 0, 12, 15), ArgumentOutOfRangeException);
        }
        TEST(StringTest, CopyTo_Should_Throw_Exception_When_Invalid_Dest_Buffer_Size_Specified) {
            String str1(_T("Hello World"));
            TCHAR tszDest[12] = { 0 };
            ASSERT_THROW(str1.CopyTo(0, tszDest, 0, 15, 12), ArgumentOutOfRangeException);
        }
        TEST(StringTest, EndsWith_Should_Return_True_When_A_Character_Searched_Is_Found) {
            String str1(_T("Hello World"));
            ASSERT_TRUE(str1.EndsWith(_T('d'), true));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_A_Character_Searched_Is_Not_Found) {
            String str1(_T("Hello World"));
            ASSERT_FALSE(str1.EndsWith(_T('D'), false));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_A_Character_Searched_Is_Not_Last_One) {
            String str1(_T("Hello World"));
            ASSERT_FALSE(str1.EndsWith(_T('l'), false));
        }
        TEST(StringTest, EndsWith_Should_Return_True_When_Substring_Searched_Ignoring_Case) {
            String str1(_T("Hello World"));
            ASSERT_TRUE(str1.EndsWith(_T("WORLD"), true));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_Substring_Searched_Considering_Case) {
            String str1(_T("Hello World"));
            ASSERT_FALSE(str1.EndsWith(_T("WORLD"), false));
        }
        TEST(StringTest, EndsWith_Should_Return_False_When_Substring_Searched_Is_Not_The_Ending_One) {
            String str1(_T("Hello World"));
            ASSERT_FALSE(str1.EndsWith(_T("Hello"), true));
        }
        TEST(StringTest, Equals_Should_Return_True_When_Same_Strings_Are_Compared) {
            String str1(_T("Hello World"));
            String str2(_T("Hello World"));
            ASSERT_TRUE(String::Equals(str1, str2));
        }
        TEST(StringTest, Equals_Should_Return_False_When_Different_Strings_Are_Compared) {
            String str1(_T("Hello World"));
            String str2(_T("Hello World2"));
            ASSERT_FALSE(String::Equals(str1, str2));
        }
        TEST(StringTest, Equals_Should_Return_True_When_Current_String_Compared_To_A_String_Of_Same_Content) {
            String str1(_T("Hello World"));
            String str2(_T("Hello World"));
            ASSERT_TRUE(str1.Equals(str2));
        }
        TEST(StringTest, Equals_Should_Return_False_When_Current_String_Compared_To_A_String_Of_Different_Content) {
            String str1(_T("Hello World"));
            String str2(_T("Hello World2"));
            ASSERT_FALSE(str1.Equals(str2));
        }
        TEST(StringTest, IndexOf_Should_Return_The_Index_When_An_Existing_Substring_Searched) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOf(_T("World")) == 6);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Non_Existing_Substring_Searched) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOf(_T("2024")) == -1);
        }
        TEST(StringTest, IndexOf_Should_Return_The_Index_When_An_Existing_Substring_Searched_From_A_Start_Index) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOf(_T("World"), 5, false) == 6);
        }
        TEST(StringTest, IndexOf_Should_Return_The_Index_When_An_Existing_Substring_Searched_Ignoring_Case_From_A_Start_Index) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOf(_T("WORLD"), 5, true) == 6);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Existing_Substring_Not_In_Range_Searched) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOf(_T("WORLD"), 7, true) == -1);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Existing_Substring_Searched_Considering_Case) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOf(_T("WORLD"), 5, false) == -1);
        }
        TEST(StringTest, IndexOf_Should_Return_Minus_One_When_An_Invalid_Start_Index_Specified) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.IndexOf(_T("WORLD"), -1, false), ArgumentOutOfRangeException);
        }
        TEST(StringTest, IndexOfAny_Should_Return_Index_Of_First_Char_When_Chars_Searched_From_Index) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOfAny(5, { _T('r'), _T('l'), _T('d') }) == 8);
        }
        TEST(StringTest, IndexOfAny_Should_Return_Index_Of_First_Char_When_Chars_Searched_From_Index2) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOfAny(5, { _T('H'), _T('e'), _T('d') }) == 10);
        }
        TEST(StringTest, IndexOfAny_Should_Return_Minus_One_When_Chars_Searched_Not_Found_After_An_Index) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.IndexOfAny(5, { _T('H'), _T('e'), _T('b') }) == -1);
        }
        TEST(StringTest, IndexOfAny_Should_Throw_Exception_When_Invalid_Start_Index_Specified) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.IndexOfAny(-1, { _T('H'), _T('e'), _T('b') }), ArgumentOutOfRangeException);
        }
        TEST(StringTest, IndexOfAny_Should_Throw_Exception_When_Invalid_Start_Index_Specified2) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.IndexOfAny(13, { _T('H'), _T('e'), _T('b') }), ArgumentOutOfRangeException);
        }

        TEST(StringTest, Insert_Should_Return_Combined_String_When_A_String_Inserted_At_Index_Zero) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Insert(0, _T("2024 ")) == _T("2024 Hello World"));
        }
        TEST(StringTest, Insert_Should_Returned_Combined_String_When_A_String_Inserted_At_Index_In_Between) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Insert(6, _T("2024 ")) == _T("Hello 2024 World"));
        }
        TEST(StringTest, Insert_Should_Returned_Combined_String_When_A_String_Inserted_At_Index_In_Between2) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Insert(5, _T("o 2024")) == _T("Helloo 2024 World"));
        }
        TEST(StringTest, Insert_Should_Throw_Exception_When_Invalid_Index_Specified) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.Insert(12, _T(" 2024")), ArgumentOutOfRangeException);
        }
        TEST(StringTest, Insert_Should_Throw_Exception_When_Invalid_Index_Specified2) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.Insert(-1, _T(" 2024")), ArgumentOutOfRangeException);
        }
        TEST(StringTest, IsEmpty_Should_Return_True_When_Current_String_Is_Empty) {
            String str(_T(""));
            ASSERT_TRUE(str.IsEmpty());
        }
        TEST(StringTest, IsEmpty_Should_Return_False_When_Current_String_Is_Not_Empty) {
            String str(_T(" "));
            ASSERT_FALSE(str.IsEmpty());
        }
        TEST(StringTest, Join_Should_Return_Combined_String_Separated_By_Specified_Char_When_Invoked) {
            String newStr = String::Join(_T(','), { _T("Apple"), _T("Orange"),_T("Grapes"),_T("Mango") });
            ASSERT_TRUE(newStr == _T("Apple,Orange,Grapes,Mango"));
            String newStr2 = String::Join(_T('|'), { _T("Apple"), _T("Orange") });
            ASSERT_TRUE(newStr2 == _T("Apple|Orange"));
            String newStr3 = String::Join(_T('|'), { _T("Apple") });
            ASSERT_TRUE(newStr3 == _T("Apple"));
        }
        TEST(StringTest, Join_Should_Return_Combined_String_Separated_By_Specified_Char_When_Invoked2) {
            String newStr = String::Join(_T(','), { _T("Apple"), _T("Orange"),_T("Grapes"),_T("Mango") },
                0, 4);
            ASSERT_TRUE(newStr == _T("Apple,Orange,Grapes,Mango"));
            String newStr2 = String::Join(_T('|'), { _T("Apple"), _T("Orange") }, 0, 2);
            ASSERT_TRUE(newStr2 == _T("Apple|Orange"));
            String newStr3 = String::Join(_T('|'), { _T("Apple") }, 0, 1);
            ASSERT_TRUE(newStr3 == _T("Apple"));
            String newStr4 = String::Join(_T(','), { _T("Apple"), _T("Orange"),_T("Grapes"),_T("Mango") },
                1, 3);
            ASSERT_TRUE(newStr4 == _T("Orange,Grapes,Mango"));
        }
        TEST(StringTest, Join_Should_Return_Combined_String_Separated_By_Specified_String_When_Invoked) {
            String newStr = String::Join(_T("##"), { _T("Apple"), _T("Orange"),_T("Grapes"),_T("Mango") });
            ASSERT_TRUE(newStr == _T("Apple##Orange##Grapes##Mango"));
            String newStr2 = String::Join(_T("|"), { _T("Apple"), _T("Orange") });
            ASSERT_TRUE(newStr2 == _T("Apple|Orange"));
            String newStr3 = String::Join(_T("|"), { _T("Apple") });
            ASSERT_TRUE(newStr3 == _T("Apple"));
        }
        TEST(StringTest, Join_Should_Return_Combined_String_When_Invoked_With_A_List_Of_Strings_And_Count) {
            String newStr = String::Join(_T("=="), { _T("Apple"), _T("Orange"),_T("Grapes"),_T("Mango") },
                0, 4);
            ASSERT_TRUE(newStr == _T("Apple==Orange==Grapes==Mango"));
            String newStr2 = String::Join(_T("****"), { _T("Apple"), _T("Orange") }, 0, 2);
            ASSERT_TRUE(newStr2 == _T("Apple****Orange"));
            String newStr3 = String::Join(_T("|"), { _T("Apple") }, 0, 1);
            ASSERT_TRUE(newStr3 == _T("Apple"));
            String newStr4 = String::Join(_T(","), { _T("Apple"), _T("Orange"),_T("Grapes"),_T("Mango") },
                1, 3);
            ASSERT_TRUE(newStr4 == _T("Orange,Grapes,Mango"));
        }
        TEST(StringTest, LastIndexOf_Should_Return_Index_When_Valid_Substring_Specified) {
            String str(_T("Hello World World 2024"));
            ASSERT_TRUE(str.LastIndexOf(_T("WORLD"), true) == 12);
            ASSERT_TRUE(str.LastIndexOf(_T("2024"), true) == 18);
        }
        TEST(StringTest, LastIndexOf_Should_Throw_Exception_When_Invoked_Considering_Case) {
            String str(_T("Hello World World 2024"));
            ASSERT_THROW(str.LastIndexOf(_T("WORLD"), false), NotImplementedException);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_With_A_Set_Of_Valid_Chars) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.LastIndexOfAny(0, { _T('O'), _T('l'), _T('d') }, false) == 9);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_With_A_Set_Of_Valid_Chars2) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.LastIndexOfAny(5, { _T('L'), _T('E'), _T('d') }, false) == 10);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Minus_One_When_Invoked_With_An_Invalid_Start_Index) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.LastIndexOfAny(5, { _T('H'), _T('e'), _T('b') }, false) == -1);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_Ignoring_Case) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.LastIndexOfAny(0, { _T('o'), _T('l'), _T('d') }, true) == 7);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Last_Index_When_Invoked_Ignoring_Case2) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.LastIndexOfAny(5, { _T('l'), _T('e'), _T('d') }, true) == 9);
        }
        TEST(StringTest, LastIndexOfAny_Should_Return_Minus_One_When_Invoked_With_A_Start_Index_That_Skips_Chars) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.LastIndexOfAny(5, { _T('H'), _T('e'), _T('b') }, true) == -1);
        }
        TEST(StringTest, LastIndexOfAny_Should_Throw_Exception_When_Invoked_With_Invalid_Start_Index) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.LastIndexOfAny(-1, { _T('H'), _T('e'), _T('b') }, true), ArgumentOutOfRangeException);
        }
        TEST(StringTest, LastIndexOfAny_Should_Throw_Exception_When_Invoked_With_Invalid_Start_Index2) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.LastIndexOfAny(13, { _T('H'), _T('e'), _T('b') }, true), ArgumentOutOfRangeException);
        }
        TEST(StringTest, PadLeft_Returns_Left_Padded_String_When_Invoked_With_A_Total_Width) {
            String str(_T("Hello World"));
            String strPadded = str.PadLeft(15);
            ASSERT_TRUE(strPadded == _T("    Hello World"));
        }
        TEST(StringTest, PadLeft_Returns_Same_String_When_Invoked_With_A_Total_Width_Which_Is_String_Len) {
            String str(_T("Hello World"));
            String strPadded = str.PadLeft(11);
            ASSERT_TRUE(strPadded == _T("Hello World"));
        }
        TEST(StringTest, PadLeft_Throws_Exception_When_Invoked_With_An_Invalid_Total_Width) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.PadLeft(-1), ArgumentException);
        }
        TEST(StringTest, PadLeft_Returns_String_Padded_Left_With_Specified_Char_When_Invoked_With_A_Char_And_Total_Width) {
            String str(_T("Hello World"));
            String strPadded = str.PadLeft(15, _T('#'));
            ASSERT_TRUE(strPadded == _T("####Hello World"));
        }
        TEST(StringTest, PadLeft_Returns_Same_String_When_Invoked_With_Char_And_Total_Width_Equals_To_String_Len) {
            String str(_T("Hello World"));
            String strPadded = str.PadLeft(11, _T('@'));
            ASSERT_TRUE(strPadded == _T("Hello World"));
        }
        TEST(StringTest, PadLeft_Throws_Exception_When_Invoked_With_Invalid_Total_Width) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.PadLeft(-1, _T('#')), ArgumentException);
        }
        TEST(StringTest, PadRight_Returns_Right_Padded_String_When_Invoked_With_A_Total_Width) {
            String str(_T("Hello World"));
            String strPadded = str.PadRight(15);
            ASSERT_TRUE(strPadded == _T("Hello World    "));
        }
        TEST(StringTest, PadRight_Returns_The_Same_String_When_Invoked_With_A_Total_Width_Equal_To_String_Len) {
            String str(_T("Hello World"));
            String strPadded = str.PadRight(11);
            ASSERT_TRUE(strPadded == _T("Hello World"));
        }
        TEST(StringTest, PadRight_Throws_Exception_When_Invoked_Invalid_Total_Width) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.PadRight(-1), ArgumentException);
        }
        TEST(StringTest, PadRight_Returns_A_String_Padded_Right_With_Specified_Char_When_Invoked_With_A_Char) {
            String str(_T("Hello World"));
            String strPadded = str.PadRight(15, _T('#'));
            ASSERT_TRUE(strPadded == _T("Hello World####"));
        }
        TEST(StringTest, PadRight_Returns_Same_String_When_Total_Width_Equal_To_String_Len) {
            String str(_T("Hello World"));
            String strPadded = str.PadRight(11, _T('@'));
            ASSERT_TRUE(strPadded == _T("Hello World"));
        }
        TEST(StringTest, PadRight_Throws_Exception_When_Invalid_Total_Width_Specified) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.PadRight(-1, _T('#')), ArgumentException);
        }
        TEST(StringTest, Remove_Throws_Exception_When_Invoked_With_An_Invalid_Index) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.Remove(-1), ArgumentOutOfRangeException);
        }
        TEST(StringTest, Remove_Throws_Exception_When_Invoked_With_An_Invalid_Index2) {
            String str(_T("Hello World"));
            ASSERT_THROW(str.Remove(12), ArgumentOutOfRangeException);
        }
        TEST(StringTest, Remove_Returns_String_Before_Index_When_Invoked_With_An_Index) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Remove(5) == _T("Hello"));
        }
        TEST(StringTest, Remove_Returns_Empty_String_Before_Index_When_Invoked_With_An_Index_Of_Zero) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Remove(0) == _T(""));
        }
        TEST(StringTest, Remove_Returns_A_String_After_Index_When_Invoked_With_An_Index_And_Count) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Remove(0, 5) == _T(" World"));
        }
        TEST(StringTest, Repalce_Returns_A_New_Replaced_String_When_Invoked_With_A_Char_To_Be_Replaced) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Replace(_T('l'), _T('*')) == _T("He**o Wor*d"));
        }
        TEST(StringTest, Repalce_Returns_Same_String_When_Char_To_Be_Replaced_Not_Found) {
            String str(_T("Hello World"));
            ASSERT_TRUE(str.Replace(_T('i'), _T('*')) == _T("Hello World"));
        }
        TEST(StringTest, Repalce_Returns_Replaced_New_String_When_String_To_Be_Replaced_Found) {
            String str(_T("Hello World World"));
            ASSERT_TRUE(str.Replace(_T("World"), _T("My World")) == _T("Hello My World My World"));
        }
    }
}