#include "pch.h"
#include "gtest/gtest.h"
#include "System/Console.h"
#include "System/String.h"
#include "System/ArgumentException.h"
#include "System/IO/StringWriter.h"
#include "System/IO/StringReader.h"
#include <limits>

#if defined(_WIN32)
#include <windows.h>
#endif

using namespace DotNetDupe::System;

namespace SystemTests {
	namespace ConsoleTestCases {

		bool IsConsoleAvailable() {
#if defined(_WIN32)
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi) != FALSE;
#else
			return isatty(STDOUT_FILENO);
#endif
		}

		// --- Positive Cases ---

		TEST(ConsoleTest, WriteLine_Should_OutputString_When_ValidStringPassed) {
			// Given
			Console::Clear();
			String message("Hello .NET");

			// When
			Console::WriteLine(message);
			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs.GetLength() == 1);
			ASSERT_TRUE(outputs [0] == message);
		}

		TEST(ConsoleTest, WriteLine_Should_FormatAndOutputString_When_CharPointerFormatPassed) {
			// Given
			Console::Clear();

			// When
			Console::WriteLine("Hello {0}, age {1}", "Alice", 30);

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_EQ(outputs.GetLength(), 1);
			ASSERT_EQ(outputs[0], "Hello Alice, age 30");
		}

		TEST(ConsoleTest, Write_Should_AccumulateOutput_When_MultipleCallsMade) {
			// Given
			Console::Clear();
			String part1("Hello");
			String part2(" World");

			// When
			Console::Write(part1);
			Console::Write(part2);
			Console::WriteLine();

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs.GetLength() == 1);
			ASSERT_TRUE(outputs [0] == "Hello World");
		}

		TEST(ConsoleTest, Write_Should_HandleBoolean_When_TruePassed) {
			// Given
			Console::Clear();

			// When
			Console::Write(true);
			Console::WriteLine();

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs [0] == "True");
		}

		TEST(ConsoleTest, Write_Should_HandleBoolean_When_FalsePassed) {
			// Given
			Console::Clear();

			// When
			Console::Write(false);
			Console::WriteLine();

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs [0] == "False");
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
			ASSERT_TRUE(outputs [0] == "42");
		}

		TEST(ConsoleTest, ReadLine_Should_ReturnInput_When_InputWasSet) {
			// Given
			Console::Clear();
			String input("Test Input");
			Console::SetIn(input);

			// When
			String result = Console::ReadLine();

			// Then
			ASSERT_TRUE(result == input);
		}

		TEST(ConsoleTest, SetTitle_Should_UpdateTitle_When_ValidStringPassed) {
			if (!IsConsoleAvailable()) return;
			// Given
			String newTitle("DotNetDupe Console");

			// When
			Console::SetTitle(newTitle);

			// Then
#if defined(_WIN32)
			ASSERT_TRUE(Console::GetTitle() == newTitle);
#endif
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
#if defined(_WIN32)
			ASSERT_TRUE(Console::GetForegroundColor() == fore);
			ASSERT_TRUE(Console::GetBackgroundColor() == back);
#endif

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
#if defined(_WIN32)
			ASSERT_TRUE(Console::GetCursorLeft() == left);
			ASSERT_TRUE(Console::GetCursorTop() == top);
#endif
		}

		TEST(ConsoleTest, CursorVisible_Should_Toggle_When_Set) {
			if (!IsConsoleAvailable()) return;
			// Given
			bool original = Console::GetCursorVisible();

			// When
			Console::SetCursorVisible(!original);
			bool updated = Console::GetCursorVisible();

			// Then
#if defined(_WIN32)
			ASSERT_TRUE(updated == !original);
#endif

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
#if defined(_WIN32)
				ASSERT_TRUE(updatedWidth == newWidth);
#endif

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
			Console::Write("");
			Console::WriteLine();

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs.GetLength() > 0);
			ASSERT_TRUE(outputs [0] == "");
		}

		TEST(ConsoleTest, Write_Should_HandleLargeString_When_LongStringPassed) {
			// Given
			Console::Clear();
			std::string longStr(5000, 'A');
			String message(longStr.c_str());

			// When
			Console::WriteLine(message);

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs [0].GetLength() == 5000);
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
			ASSERT_TRUE(outputs [0] == String(std::to_string(maxValue).c_str()));
		}
		TEST(ConsoleTest, WriteLine_Should_FormatString_When_InterpolatedArgumentsPassed) {
			// Given
			Console::Clear();

			// When
			Console::WriteLine("Hello {0}, your score is {1}!", "Alice", 100);

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs.GetLength() == 1);
			ASSERT_TRUE(outputs[0] == "Hello Alice, your score is 100!");
		}

		TEST(ConsoleTest, Write_Should_FormatString_When_InterpolatedArgumentsPassed) {
			// Given
			Console::Clear();

			// When
			Console::Write("Value: {0}", 42);
			Console::WriteLine();

			// Then
			auto outputs = Console::GetOutputs();
			ASSERT_TRUE(outputs.GetLength() == 1);
			ASSERT_TRUE(outputs[0] == "Value: 42");
		}

		TEST(ConsoleTest, SetOut_Should_RedirectOutput_When_StringWriterProvided) {
			// Given
			Console::Clear();
			SmartPointer<DotNetDupe::System::IO::StringWriter> writer(true);
			Console::SetOut(writer);

			// When
			Console::WriteLine("Redirected message");

			// Then
			ASSERT_TRUE(Console::Out() == writer);
			ASSERT_EQ(writer->ToString(), "Redirected message\r\n");

			// Cleanup
			Console::Clear();
		}

		TEST(ConsoleTest, SetIn_Should_RedirectInput_When_StringReaderProvided) {
			// Given
			Console::Clear();
			SmartPointer<DotNetDupe::System::IO::StringReader> reader(new DotNetDupe::System::IO::StringReader("Custom input line"));
			Console::SetIn(reader);

			// When
			String inputLine = Console::ReadLine();

			// Then
			ASSERT_TRUE(Console::In() == reader);
			ASSERT_EQ(inputLine, "Custom input line");

			// Cleanup
			Console::Clear();
		}

		TEST(ConsoleTest, SetError_Should_StoreErrorWriter_When_WriterProvided) {
			// Given
			Console::Clear();
			SmartPointer<DotNetDupe::System::IO::StringWriter> writer(true);

			// When
			Console::SetError(writer);

			// Then
			ASSERT_TRUE(Console::Error() == writer);

			// Cleanup
			Console::Clear();
		}
	}
}
