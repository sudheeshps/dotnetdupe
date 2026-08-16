# Console

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Console.h"`

Provides static methods for reading from and writing to standard input, standard output, and standard error streams with color controls, cursor positioning, window sizing, and stream redirection via `SmartPointer<TextWriter>` and `SmartPointer<TextReader>`.

---

## Syntax

```cpp
enum class ConsoleColor {
    Black, DarkBlue, DarkGreen, DarkCyan, DarkRed, DarkMagenta, DarkYellow, Gray,
    DarkGray, Blue, Green, Cyan, Red, Magenta, Yellow, White
};

class Console : public Object;
```

---

## Static Output Methods

### `static void Write(...)`
Writes the text representation of the specified value to the standard output stream.

Overloads:
- `static void Write(bool bValue)`
- `static void Write(Char chValue)`
- `static void Write(int iValue)`
- `static void Write(long iValue)`
- `static void Write(long long llValue)`
- `static void Write(float fValue)`
- `static void Write(double dValue)`
- `static void Write(const String& sValue)`
- `static void Write(const char* sValue)`
- `template <class... Args> static void Write(const String& sFormat, const Args&... args)`
- `template <class... Args> static void Write(const char* sFormat, const Args&... args)`

### `static void WriteLine(...)`
Writes the text representation of the specified value, followed by the current line terminator, to the standard output stream.

Overloads:
- `static void WriteLine()`
- `static void WriteLine(bool bValue)`
- `static void WriteLine(Char chValue)`
- `static void WriteLine(int iValue)`
- `static void WriteLine(long iValue)`
- `static void WriteLine(long long llValue)`
- `static void WriteLine(float fValue)`
- `static void WriteLine(double dValue)`
- `static void WriteLine(const String& sValue)`
- `static void WriteLine(const char* sValue)`
- `template <class... Args> static void WriteLine(const String& sFormat, const Args&... args)`
- `template <class... Args> static void WriteLine(const char* sFormat, const Args&... args)`

---

## Static Input Methods

### `static int Read()`
Reads the next character from standard input. Returns `-1` if no more characters are available.

### `static String ReadLine()`
Reads the next line of characters from the standard input stream.

---

## Colors, Cursor & Window Properties

### `static ConsoleColor GetForegroundColor()` / `static void SetForegroundColor(ConsoleColor enumColor)`
Gets or sets the foreground color of the console.

### `static ConsoleColor GetBackgroundColor()` / `static void SetBackgroundColor(ConsoleColor enumColor)`
Gets or sets the background color of the console.

### `static void ResetColor()`
Sets the foreground and background console colors to their defaults.

### `static int GetCursorLeft()` / `static void SetCursorLeft(int iLeft)`
Gets or sets the column position of the cursor within the buffer area.

### `static int GetCursorTop()` / `static void SetCursorTop(int iTop)`
Gets or sets the row position of the cursor within the buffer area.

### `static void SetCursorPosition(int iLeft, int iTop)`
Sets the position of the cursor.

### `static bool GetCursorVisible()` / `static void SetCursorVisible(bool bVisible)`
Gets or sets a value indicating whether the cursor is visible.

### `static int GetWindowWidth()` / `static void SetWindowWidth(int iWidth)`
Gets or sets the width of the console window.

### `static int GetWindowHeight()` / `static void SetWindowHeight(int iHeight)`
Gets or sets the height of the console window.

### `static bool GetCapsLock()` / `static bool GetNumberLock()`
Gets a value indicating whether the CAPS LOCK or NUM LOCK keyboard toggle is turned on.

### `static String GetTitle()` / `static void SetTitle(const String& sTitle)`
Gets or sets the title to display in the console title bar.

### `static void Beep()`
Plays the sound of a beep through the console speaker.

### `static void Clear()`
Clears the console buffer and corresponding console window of display information.

---

## Standard Stream Redirection & Accessors

### `static SmartPointer<IO::TextWriter> Out()`
Gets the standard output stream.

### `static SmartPointer<IO::TextWriter> Error()`
Gets the standard error output stream.

### `static SmartPointer<IO::TextReader> In()`
Gets the standard input stream.

### `static void SetOut(const SmartPointer<IO::TextWriter>& pOutWriter)`
Redirects the standard output stream to a custom `TextWriter`.

### `static void SetError(const SmartPointer<IO::TextWriter>& pErrorWriter)`
Redirects standard error stream to a custom `TextWriter`.

### `static void SetIn(const SmartPointer<IO::TextReader>& pInReader)`
Redirects standard input stream to a custom `TextReader`.

### `static void SetIn(const String& sValue)`
Simulates standard input by buffering the specified string (useful for testing).

### `static void ClearInputs()`
Clears buffered test inputs.

### `static Array<String> GetOutputs()`
Retrieves captured test outputs.

---

## Example

```cpp
#include "System/Console.h"
#include "System/String.h"

using namespace DotNetDupe::System;

int main() {
    Console::SetForegroundColor(ConsoleColor::Cyan);
    Console::WriteLine("=== System Console Test ===");
    Console::ResetColor();

    Console::Write("Enter your username: ");
    String sUser = Console::ReadLine();

    Console::SetForegroundColor(ConsoleColor::Yellow);
    Console::WriteLine("Hello, {0}!", sUser);
    Console::ResetColor();

    return 0;
}
```
