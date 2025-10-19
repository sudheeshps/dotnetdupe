#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/Char.h"

using namespace DotNetDupe::System;
namespace SystemTests {
    namespace CharTestCases {
        TEST(CharTest, CharOperatorEqualsTest) {
            Char c = 'A';

            ASSERT_TRUE(c == 'A');
            Char wc = 'B';
            ASSERT_TRUE(wc == 'B');
        }
        TEST(CharTest, CharEqualsTest) {
            Char c = 'A';
            ASSERT_TRUE(c.Equals('A'));
            Char wc = 'B';
            ASSERT_TRUE(wc.Equals('B'));
        }
        TEST(CharTest, GetNumericValueTest) {
            ASSERT_TRUE(Char::GetNumericValue('1') == 1.0);
            ASSERT_TRUE(Char::GetNumericValue('0') == 0.0);
            ASSERT_TRUE(Char::GetNumericValue('A') == -1.0);
        }

        TEST(CharTest, IsAsciiTest) {
            ASSERT_TRUE(Char::IsAscii('+') == true);
            ASSERT_TRUE(Char::IsAscii('A') == true);
            ASSERT_TRUE(Char::IsAscii('0') == true);
        }
        TEST(CharTest, IsAsciiDigit) {
            ASSERT_TRUE(Char::IsAsciiDigit('0'));
            ASSERT_FALSE(Char::IsAsciiDigit('a'));
            ASSERT_TRUE(Char::IsAsciiDigit('9'));
            ASSERT_FALSE(Char::IsAsciiDigit('z'));
        }
        TEST(CharTest, IsAsciiHexDigit) {
            ASSERT_TRUE(Char::IsAsciiHexDigit('0'));
            ASSERT_FALSE(Char::IsAsciiHexDigit('S'));
            ASSERT_TRUE(Char::IsAsciiHexDigit('F'));
            ASSERT_FALSE(Char::IsAsciiHexDigit('V'));
            ASSERT_TRUE(Char::IsAsciiHexDigit('A'));
        }
        TEST(CharTest, IsAsciiHexDigitLower) {
            ASSERT_TRUE(Char::IsAsciiHexDigitLower('0'));
            ASSERT_FALSE(Char::IsAsciiHexDigitLower('A'));
            ASSERT_TRUE(Char::IsAsciiHexDigitLower('a'));
            ASSERT_FALSE(Char::IsAsciiHexDigitLower('V'));
            ASSERT_TRUE(Char::IsAsciiHexDigitLower('b'));
        }
        TEST(CharTest, IsAsciiHexDigitUpper) {
            ASSERT_TRUE(Char::IsAsciiHexDigitUpper('0'));
            ASSERT_FALSE(Char::IsAsciiHexDigitUpper('a'));
            ASSERT_TRUE(Char::IsAsciiHexDigitUpper('A'));
            ASSERT_FALSE(Char::IsAsciiHexDigitUpper('V'));
            ASSERT_TRUE(Char::IsAsciiHexDigitUpper('B'));
        }
        TEST(CharTest, IsAsciiLetter) {
            ASSERT_TRUE(Char::IsAsciiLetter('a'));
            ASSERT_TRUE(Char::IsAsciiLetter('A'));
            ASSERT_FALSE(Char::IsAsciiLetter('+'));
            ASSERT_FALSE(Char::IsAsciiLetter('0'));
        }
        TEST(CharTest, IsAsciiLetterLower) {
            ASSERT_TRUE(Char::IsAsciiLetterLower('a'));
            ASSERT_FALSE(Char::IsAsciiLetterLower('A'));
            ASSERT_TRUE(Char::IsAsciiLetterLower('z'));
            ASSERT_FALSE(Char::IsAsciiLetterLower('0'));
        }
        TEST(CharTest, IsAsciiLetterOrDigit) {
            ASSERT_TRUE(Char::IsAsciiLetterOrDigit('a'));
            ASSERT_TRUE(Char::IsAsciiLetterOrDigit('5'));
            ASSERT_TRUE(Char::IsAsciiLetterOrDigit('z'));
            ASSERT_FALSE(Char::IsAsciiLetterOrDigit('+'));
        }
        TEST(CharTest, IsAsciiLetterUpper) {
            ASSERT_TRUE(Char::IsAsciiLetterUpper('A'));
            ASSERT_FALSE(Char::IsAsciiLetterUpper('a'));
            ASSERT_TRUE(Char::IsAsciiLetterUpper('Z'));
            ASSERT_FALSE(Char::IsAsciiLetterUpper('0'));
        }
        TEST(CharTest, IsBetween) {
            ASSERT_TRUE(Char::IsBetween('A', 'A', 'Z'));
            ASSERT_FALSE(Char::IsBetween('a', 'A', 'Z'));
            ASSERT_TRUE(Char::IsBetween('Z', 'A', 'Z'));
            ASSERT_FALSE(Char::IsBetween('0', 'A', 'Z'));
        }
        TEST(CharTest, IsControl) {
            ASSERT_TRUE(Char::IsControl(0x0006));
            ASSERT_FALSE(Char::IsControl('A'));
            ASSERT_TRUE(Char::IsControl(0x009F));
        }
        TEST(CharTest, IsLowerTest) {
            ASSERT_TRUE(Char::IsLower('a'));
            ASSERT_FALSE(Char::IsLower('A'));
        }
        TEST(CharTest, ToLowerTest) {
            ASSERT_TRUE(Char::ToLower('A') == 'a');
            ASSERT_TRUE(Char::ToLower('0') == '0');
            ASSERT_FALSE(Char::ToLower('A') == 'A');
        }
        TEST(CharTest, GetChar) {
            ASSERT_TRUE(Char('A').GetChar() == 'A');
            ASSERT_TRUE(Char('0').GetChar() == '0');
            ASSERT_FALSE(Char('A').GetChar() == 'B');
        }
    }
}