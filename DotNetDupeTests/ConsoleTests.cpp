#include "pch.h"
#include "gtest/gtest.h"
#include "System/Console.h"
#include "System/String.h"
#include "System/ArgumentException.h"
#include <limits>

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace ConsoleTestCases {

        bool IsConsoleAvailable() {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi) != FALSE;
        }

        // --- Positive Cases ---

        TEST(ConsoleTest, WriteLine_Should_OutputString_When_ValidStringPassed) {
            // Given
            Console::Clear();
            String message(_T("Hello .NET"));

            // When
            Console::WriteLine(message);

            // Then
            auto outputs = Console::GetOutputs();
            ASSERT_TRUE(outputs.size() == 1);
            ASSERT_TRUE(outputs[0] == message);
        }

        TEST(ConsoleTest, Write_Should_AccumulateOutput_When_MultipleCallsMade) {
            // Given
            Console::Clear();
            String part1(_T("Hello"));
            String part2(_T(" World"));

            // When
            Console::Write(part1);
            Console::Write(part2);
            Console::WriteLine();

            // Then
            auto outputs = Console::GetOutputs();
            ASSERT_TRUE(outputs.size() == 1);
            ASSERT_TRUE(outputs[0] == _T("Hello World"));
        }

        TEST(ConsoleTest, Write_Should_HandleBoolean_When_TruePassed) {
            // Given
            Console::Clear();

            // When
            Console::Write(true);
            Console::WriteLine();

            // Then
            auto outputs = Console::GetOutputs();
            ASSERT_TRUE(outputs[0] == _T("True"));
        }

        TEST(ConsoleTest, Write_Should_HandleBoolean_When_FalsePassed) {
            // Given
            Console::Clear();

            // When
            Console::Write(false);
            Console::WriteLine();

            // Then
            auto outputs = Console::GetOutputs();
            ASSERT_TRUE(outputs[0] == _T("False"));
        }

        TEST(ConsoleTest, Write_Should_HandleInteger_When_PositiveValuePassed) {
            // Given
            Console::Clear();
            int value = 42;

            // When
            Console::Write(value);
            Console::WriteLine();

            // Then
            auto outputs = Console::GetOutputs();
            ASSERT_TRUE(outputs[0] == _T("42"));
        }

        TEST(ConsoleTest, ReadLine_Should_ReturnInput_When_InputWasSet) {
            // Given
            Console::Clear();
            String input(_T("Test Input"));
            Console::SetIn(input);

            // When
            String result = Console::ReadLine();

            // Then
            ASSERT_TRUE(result == input);
        }

        TEST(ConsoleTest, SetTitle_Should_UpdateTitle_When_ValidStringPassed) {
            if (!IsConsoleAvailable()) return;
            // Given
            String newTitle(_T("DotNetDupe Console"));

            // When
            Console::SetTitle(newTitle);

            // Then
            ASSERT_TRUE(Console::GetTitle() == newTitle);
        }

        TEST(ConsoleTest, Colors_Should_BeConsistent_When_SetAndGet) {
            if (!IsConsoleAvailable()) return;
            // Given
            ConsoleColor fore = ConsoleColor::Red;
            ConsoleColor back = ConsoleColor::Yellow;

            // When
            Console::SetForegroundColor(fore);
            Console::SetBackgroundColor(back);

            // Then
            ASSERT_TRUE(Console::GetForegroundColor() == fore);
            ASSERT_TRUE(Console::GetBackgroundColor() == back);

            // Cleanup
            Console::ResetColor();
        }

        TEST(ConsoleTest, CursorPosition_Should_Update_When_Set) {
            if (!IsConsoleAvailable()) return;
            // Given
            int left = 10;
            int top = 5;

            // When
            Console::SetCursorPosition(left, top);

            // Then
            ASSERT_TRUE(Console::GetCursorLeft() == left);
            ASSERT_TRUE(Console::GetCursorTop() == top);
        }

        TEST(ConsoleTest, CursorVisible_Should_Toggle_When_Set) {
            if (!IsConsoleAvailable()) return;
            // Given
            bool original = Console::GetCursorVisible();

            // When
            Console::SetCursorVisible(!original);
            bool updated = Console::GetCursorVisible();

            // Then
            ASSERT_TRUE(updated == !original);

            // Cleanup
            Console::SetCursorVisible(original);
        }

        TEST(ConsoleTest, WindowSize_Should_Update_When_Set) {
            if (!IsConsoleAvailable()) return;
            // Given
            int originalWidth = Console::GetWindowWidth();
            int newWidth = originalWidth - 1; // Try to shrink slightly

            // When
            if (newWidth > 0) {
                Console::SetWindowWidth(newWidth);
                int updatedWidth = Console::GetWindowWidth();

                // Then
                ASSERT_TRUE(updatedWidth == newWidth);

                // Cleanup
                Console::SetWindowWidth(originalWidth);
            }
        }

        // --- Negative Cases ---

        TEST(ConsoleTest, SetIn_Should_ThrowException_When_NullPointerPassed) {
            // Given
            Console::Clear();

            // When & Then
            ASSERT_THROW(Console::SetIn(String(nullptr)), ArgumentException);
        }

        // --- Edge Cases ---

        TEST(ConsoleTest, Write_Should_HandleEmptyString_When_EmptyPassed) {
            // Given
            Console::Clear();

            // When
            Console::Write(_T(""));
            Console::WriteLine();

            // Then
            auto outputs = Console::GetOutputs();
            ASSERT_TRUE(outputs.size() > 0);
            ASSERT_TRUE(outputs[0] == _T(""));
        }

        TEST(ConsoleTest, Write_Should_HandleLargeString_When_LongStringPassed) {
            // Given
            Console::Clear();
            std::basic_string<TCHAR> longStr(5000, _T('A'));
            String message(longStr.c_str());

            // When
            Console::WriteLine(message);

            // Then
            auto outputs = Console::GetOutputs();
            ASSERT_TRUE(outputs[0].GetLength() == 5000);
        }

        TEST(ConsoleTest, Write_Should_HandleNumericLimits_When_MaxIntPassed) {
            // Given
            Console::Clear();
            int maxValue = (std::numeric_limits<int>::max)();

            // When
            Console::Write(maxValue);
            Console::WriteLine();

            // Then
            auto outputs = Console::GetOutputs();
#ifdef UNICODE
            ASSERT_TRUE(outputs[0] == String(std::to_wstring(maxValue).c_str()));
#else
            ASSERT_TRUE(outputs[0] == String(std::to_string(maxValue).c_str()));
#endif
        }
    }
}
