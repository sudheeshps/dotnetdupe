#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Char.h"
#include "System/Array.h"
#include "System/SmartPointer.h"
#include "System/IO/TextWriter.h"
#include "System/IO/TextReader.h"
#include <memory>

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class TextWriter;
            class TextReader;
        }

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
            DOTNETDUPE_API static void Write(bool bValue);
            DOTNETDUPE_API static void Write(Char chValue);
            DOTNETDUPE_API static void Write(double dValue);
            DOTNETDUPE_API static void Write(int iValue);
            DOTNETDUPE_API static void Write(long iValue);
            DOTNETDUPE_API static void Write(long long llValue);
            DOTNETDUPE_API static void Write(float fValue);
            DOTNETDUPE_API static void Write(const String& sValue);
            DOTNETDUPE_API static void Write(const char* sValue);

            template <class... Args>
            static void Write(const String& sFormat, const Args&... args) {
                Write(String::Format(sFormat, args...));
            }

            template <class... Args>
            static void Write(const char* sFormat, const Args&... args) {
                Write(String::Format(sFormat, args...));
            }

            // WriteLine methods
            DOTNETDUPE_API static void WriteLine();
            DOTNETDUPE_API static void WriteLine(bool bValue);
            DOTNETDUPE_API static void WriteLine(Char chValue);
            DOTNETDUPE_API static void WriteLine(double dValue);
            DOTNETDUPE_API static void WriteLine(int iValue);
            DOTNETDUPE_API static void WriteLine(long iValue);
            DOTNETDUPE_API static void WriteLine(long long llValue);
            DOTNETDUPE_API static void WriteLine(float fValue);
            DOTNETDUPE_API static void WriteLine(const String& sValue);
            DOTNETDUPE_API static void WriteLine(const char* sValue);

            template <class... Args>
            static void WriteLine(const String& sFormat, const Args&... args) {
                WriteLine(String::Format(sFormat, args...));
            }

            template <class... Args>
            static void WriteLine(const char* sFormat, const Args&... args) {
                WriteLine(String::Format(sFormat, args...));
            }

            // Reading methods
            DOTNETDUPE_API static int Read();
            DOTNETDUPE_API static String ReadLine();

            // Properties
            DOTNETDUPE_API static int GetCursorLeft();
            DOTNETDUPE_API static void SetCursorLeft(int iLeft);
            DOTNETDUPE_API static int GetCursorTop();
            DOTNETDUPE_API static void SetCursorTop(int iTop);
            DOTNETDUPE_API static void SetCursorPosition(int iLeft, int iTop);

            DOTNETDUPE_API static bool GetCursorVisible();
            DOTNETDUPE_API static void SetCursorVisible(bool bVisible);

            DOTNETDUPE_API static int GetWindowHeight();
            DOTNETDUPE_API static void SetWindowHeight(int iHeight);
            DOTNETDUPE_API static int GetWindowWidth();
            DOTNETDUPE_API static void SetWindowWidth(int iWidth);

            DOTNETDUPE_API static bool GetCapsLock();
            DOTNETDUPE_API static bool GetNumberLock();

            DOTNETDUPE_API static ConsoleColor GetBackgroundColor();
            DOTNETDUPE_API static void SetBackgroundColor(ConsoleColor enumColor);
            DOTNETDUPE_API static ConsoleColor GetForegroundColor();
            DOTNETDUPE_API static void SetForegroundColor(ConsoleColor enumColor);
            DOTNETDUPE_API static void ResetColor();

            DOTNETDUPE_API static String GetTitle();
            DOTNETDUPE_API static void SetTitle(const String& sTitle);

            DOTNETDUPE_API static void Beep();
            DOTNETDUPE_API static void Clear();

            // Standard I/O redirection APIs
            DOTNETDUPE_API static void SetOut(const SmartPointer<IO::TextWriter>& pOutWriter);
            DOTNETDUPE_API static void SetError(const SmartPointer<IO::TextWriter>& pErrorWriter);
            DOTNETDUPE_API static void SetIn(const SmartPointer<IO::TextReader>& pInReader);
            DOTNETDUPE_API static SmartPointer<IO::TextWriter> Out();
            DOTNETDUPE_API static SmartPointer<IO::TextWriter> Error();
            DOTNETDUPE_API static SmartPointer<IO::TextReader> In();

            // Test helpers
            DOTNETDUPE_API static void SetIn(const String& sValue);
            DOTNETDUPE_API static void ClearInputs();
            DOTNETDUPE_API static Array<String> GetOutputs();
        };
    }
}
