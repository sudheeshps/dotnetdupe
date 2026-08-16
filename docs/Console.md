# Console

**Namespace:** `DotNetDupe::System`  
**Header:** `#include "System/Console.h"`

Provides static methods for reading from and writing to standard input, standard output, and standard error streams with color controls, cursor positioning, and stream redirection via `SmartPointer<TextWriter>` and `SmartPointer<TextReader>`.

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

### `static void Write(const String& sValue)` / `int` / `double` / `bool` / `char`
Writes the text representation of the specified value to the standard output stream.

### `template <typename... Args> static void Write(const char* sFormat, const Args&... args)`
Writes the formatted string to the standard output stream using composite formatting.

### `static void WriteLine()`
Writes the current line terminator to the standard output stream.

### `static void WriteLine(const String& sValue)` / `int` / `double` / `bool`
Writes the text representation of the specified value, followed by the current line terminator, to the standard output stream.

### `template <typename... Args> static void WriteLine(const char* sFormat, const Args&... args)`
Writes the formatted string and newline to standard output.

```cpp
Console::WriteLine("Server status: {0} on port {1}", "Active", 8080);
```

---

## Static Input Methods

### `static int Read()`
Reads the next character from standard input. Returns `-1` if no more characters are available.

### `static String ReadLine()`
Reads the next line of characters from the standard input stream.

```cpp
String sInput = Console::ReadLine();
```

---

## Colors & Cursor

### `static void SetForegroundColor(ConsoleColor enumColor)` / `GetForegroundColor()`
Sets or gets the foreground color of the console.

### `static void SetBackgroundColor(ConsoleColor enumColor)` / `GetBackgroundColor()`
Sets or gets the background color of the console.

### `static void ResetColor()`
Sets the foreground and background console colors to their defaults.

```cpp
Console::SetForegroundColor(ConsoleColor::Green);
Console::WriteLine("Success!");
Console::ResetColor();
```

### `static void SetCursorPosition(int iLeft, int iTop)`
Sets the position of the console cursor.

### `static void Clear()`
Clears the console buffer and corresponding console window of display information.

---

## Standard Stream Redirection

### `static void SetOut(const SmartPointer<IO::TextWriter>& pOutWriter)`
Redirects the standard output stream to a custom `TextWriter` (e.g. `StringWriter`, `FileStream`, or `LoggerTextWriter`).

```cpp
auto spWriter = SmartPointer<StringWriter>::NewShared();
Console::SetOut(spWriter);
Console::WriteLine("Redirected message");
String output = spWriter->ToString();
```

### `static void SetError(const SmartPointer<IO::TextWriter>& pErrorWriter)`
Redirects standard error stream.

### `static void SetIn(const SmartPointer<IO::TextReader>& pInReader)`
Redirects standard input stream.

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
    String user = Console::ReadLine();

    Console::SetForegroundColor(ConsoleColor::Yellow);
    Console::WriteLine("Hello, {0}!", user);
    Console::ResetColor();

    return 0;
}
```
