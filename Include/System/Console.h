#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Char.h"
#include <vector>

namespace DotNetDupe {
    namespace System {

        enum class ConsoleColor {
            Black = 0,
            DarkBlue = 1,
            DarkGreen = 2,
            DarkCyan = 3,
            DarkRed = 4,
            DarkMagenta = 5,
            DarkYellow = 6,
            Gray = 7,
            DarkGray = 8,
            Blue = 9,
            Green = 10,
            Cyan = 11,
            Red = 12,
            Magenta = 13,
            Yellow = 14,
            White = 15
        };

        class Console : public Object {
        public:
            // Write methods
            DOTNETDUPE_API static void Write(bool value);
            DOTNETDUPE_API static void Write(Char value);
            DOTNETDUPE_API static void Write(double value);
            DOTNETDUPE_API static void Write(int value);
            DOTNETDUPE_API static void Write(long value);
            DOTNETDUPE_API static void Write(long long value);
            DOTNETDUPE_API static void Write(float value);
            DOTNETDUPE_API static void Write(const String& value);
            DOTNETDUPE_API static void Write(const char* value);

            // WriteLine methods
            DOTNETDUPE_API static void WriteLine();
            DOTNETDUPE_API static void WriteLine(bool value);
            DOTNETDUPE_API static void WriteLine(Char value);
            DOTNETDUPE_API static void WriteLine(double value);
            DOTNETDUPE_API static void WriteLine(int value);
            DOTNETDUPE_API static void WriteLine(long value);
            DOTNETDUPE_API static void WriteLine(long long value);
            DOTNETDUPE_API static void WriteLine(float value);
            DOTNETDUPE_API static void WriteLine(const String& value);
            DOTNETDUPE_API static void WriteLine(const char* value);

            // Reading methods
            DOTNETDUPE_API static int Read();
            DOTNETDUPE_API static String ReadLine();

            // Properties
            DOTNETDUPE_API static int GetCursorLeft();
            DOTNETDUPE_API static void SetCursorLeft(int left);
            DOTNETDUPE_API static int GetCursorTop();
            DOTNETDUPE_API static void SetCursorTop(int top);
            DOTNETDUPE_API static void SetCursorPosition(int left, int top);

            DOTNETDUPE_API static bool GetCursorVisible();
            DOTNETDUPE_API static void SetCursorVisible(bool visible);

            DOTNETDUPE_API static int GetWindowHeight();
            DOTNETDUPE_API static void SetWindowHeight(int height);
            DOTNETDUPE_API static int GetWindowWidth();
            DOTNETDUPE_API static void SetWindowWidth(int width);

            DOTNETDUPE_API static bool GetCapsLock();
            DOTNETDUPE_API static bool GetNumberLock();

            DOTNETDUPE_API static ConsoleColor GetBackgroundColor();
            DOTNETDUPE_API static void SetBackgroundColor(ConsoleColor color);
            DOTNETDUPE_API static ConsoleColor GetForegroundColor();
            DOTNETDUPE_API static void SetForegroundColor(ConsoleColor color);
            DOTNETDUPE_API static void ResetColor();

            DOTNETDUPE_API static String GetTitle();
            DOTNETDUPE_API static void SetTitle(const String& title);

            DOTNETDUPE_API static void Beep();
            DOTNETDUPE_API static void Clear();

            // Test helpers
            DOTNETDUPE_API static void SetIn(const String& value);
            DOTNETDUPE_API static void ClearInputs();
            DOTNETDUPE_API static std::vector<String> GetOutputs();
        };
    }
}
