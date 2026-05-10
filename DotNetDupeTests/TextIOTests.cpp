#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/StringReader.h"
#include "System/IO/StringWriter.h"
#include <tchar.h>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

namespace SystemTests {
    namespace TextIOTestCases {

        TEST(StringWriterTest, Write_Should_AppendText_When_Invoked) {
            // Given: A StringWriter
            StringWriter sw;
            // When: Writing strings
            sw.Write(_T("Hello"));
            sw.Write(_T(" "));
            sw.Write(_T("World"));
            // Then: The resulting string should be correct
            ASSERT_EQ(sw.ToString(), _T("Hello World"));
        }

        TEST(StringWriterTest, WriteLine_Should_AppendTextAndNewline_When_Invoked) {
            // Given: A StringWriter
            StringWriter sw;
            // When: Writing a line
            sw.WriteLine(_T("Line 1"));
            // Then: It should contain the newline
            ASSERT_EQ(sw.ToString(), _T("Line 1\r\n"));
        }

        TEST(StringReaderTest, ReadLine_Should_ReturnLines_When_Invoked) {
            // Given: A StringReader with multiple lines
            String content(_T("Line 1\r\nLine 2\nLine 3"));
            StringReader sr(content);
            // When: Reading lines
            // Then:
            ASSERT_EQ(sr.ReadLine(), _T("Line 1"));
            ASSERT_EQ(sr.ReadLine(), _T("Line 2"));
            ASSERT_EQ(sr.ReadLine(), _T("Line 3"));
            ASSERT_TRUE(sr.ReadLine().IsEmpty());
        }

        TEST(StringReaderTest, ReadToEnd_Should_ReturnAllRemainingText_When_Invoked) {
            // Given: A StringReader
            String content(_T("Some text to read"));
            StringReader sr(content);
            // When: Reading one char then reading to end
            sr.Read();
            String remaining = sr.ReadToEnd();
            // Then:
            ASSERT_EQ(remaining, _T("ome text to read"));
        }

        TEST(StringReaderTest, Peek_Should_ReturnNextCharWithoutConsuming_When_Invoked) {
            // Given: A StringReader
            StringReader sr(_T("ABC"));
            // When: Peeking
            int p1 = sr.Peek();
            int p2 = sr.Peek();
            int r1 = sr.Read();
            // Then:
            ASSERT_EQ((wchar_t)p1, L'A');
            ASSERT_EQ((wchar_t)p2, L'A');
            ASSERT_EQ((wchar_t)r1, L'A');
        }

        TEST(TextIOTest, NegativeCase_ReadBeyondEnd_Should_ReturnMinusOne) {
            // Given: A StringReader at the end
            StringReader sr(_T("A"));
            sr.Read();
            // When: Reading again
            int ch = sr.Read();
            int peek = sr.Peek();
            // Then:
            ASSERT_EQ(ch, -1);
            ASSERT_EQ(peek, -1);
        }
        
        TEST(TextIOTest, EdgeCase_EmptyString_Should_BeHandledCorrectly) {
            // Given: Empty reader and writer
            StringReader sr(_T(""));
            StringWriter sw;
            
            // When/Then:
            ASSERT_EQ(sr.Read(), -1);
            ASSERT_TRUE(sr.ReadLine().IsEmpty());
            ASSERT_TRUE(sr.ReadToEnd().IsEmpty());
            
            sw.Write(_T(""));
            ASSERT_TRUE(sw.ToString().IsEmpty());
        }
    }
}
