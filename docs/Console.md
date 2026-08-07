### class `Console`

Provides methods for reading from and writing to the standard input, output, and error streams.

#### Methods

##### `void static Write(...)`

Writes the text representation of the specified value or formatted string to the standard output stream. Overloads are available for primitives as well as parameter interpolation using `{0}`, `{1}`, etc.

**Parameters:**
- `sFormat` / `value`: The format string or value to write.
- `args...`: Variadic arguments to format into placeholders `{0}`, `{1}`, etc.

**Usage:**
```cpp
Console::Write("The answer is ");
Console::Write(42);
Console::Write("Items: {0}, Total: {1}", 5, 99.99);
```

##### `void static WriteLine(...)`

Writes the text representation of the specified value or formatted string, followed by the current line terminator, to the standard output stream. Overloads are available for primitives as well as parameter interpolation using `{0}`, `{1}`, etc.

**Parameters:**
- `sFormat` / `value`: (Optional) The format string or value to write.
- `args...`: Variadic arguments to format into placeholders `{0}`, `{1}`, etc.

**Usage:**
```cpp
Console::WriteLine("Hello World");
Console::WriteLine("Hello {0}, your score is {1}!", "Alice", 100);
Console::WriteLine(true);
```

##### `int static Read()`

Reads the next character from the standard input stream.

**Returns:**
- The next character from the input stream, or -1 if no more characters are available.

**Usage:**
```cpp
int c = Console::Read();
```

##### `String static ReadLine()`

Reads the next line of characters from the standard input stream.

**Returns:**
- The next line of characters from the input stream, or an empty string if the end of the input stream has been reached.

**Usage:**
```cpp
String line = Console::ReadLine();
```

##### `void static Clear()`

Clears the console buffer and corresponding console window of display information.

**Usage:**
```cpp
Console::Clear();
```

##### `void static Beep()`

Plays the sound of a beep through the console speaker.

**Usage:**
```cpp
Console::Beep();
```

##### `void static ResetColor()`

Sets the foreground and background console colors to their defaults.

**Usage:**
```cpp
Console::ResetColor();
```

#### Properties

##### `ConsoleColor GetBackgroundColor() / SetBackgroundColor(ConsoleColor color)`

Gets or sets the background color of the console.

**Usage:**
```cpp
Console::SetBackgroundColor(ConsoleColor::DarkBlue);
ConsoleColor current = Console::GetBackgroundColor();
```

##### `ConsoleColor GetForegroundColor() / SetForegroundColor(ConsoleColor color)`

Gets or sets the foreground color of the console.

**Usage:**
```cpp
Console::SetForegroundColor(ConsoleColor::Green);
ConsoleColor current = Console::GetForegroundColor();
```

##### `String GetTitle() / SetTitle(const String& title)`

Gets or sets the title to display in the console title bar.

**Usage:**
```cpp
Console::SetTitle("My App");
String title = Console::GetTitle();
```

##### `int GetCursorLeft() / SetCursorLeft(int left)`

Gets or sets the column position of the cursor within the buffer area.

##### `int GetCursorTop() / SetCursorTop(int top)`

Gets or sets the row position of the cursor within the buffer area.

##### `void static SetCursorPosition(int left, int top)`

Sets the position of the cursor.

##### `bool GetCursorVisible() / SetCursorVisible(bool visible)`

Gets or sets a value indicating whether the cursor is visible.

##### `int GetWindowHeight() / SetWindowHeight(int height)`

Gets or sets the height of the console window area.

##### `int GetWindowWidth() / SetWindowWidth(int width)`

Gets or sets the width of the console window area.

##### `bool GetCapsLock()`

Gets a value indicating whether the CAPS LOCK keyboard light is on.

##### `bool GetNumberLock()`

Gets a value indicating whether the NUM LOCK keyboard light is on.

#### Stream Redirection APIs

##### `void static SetOut(const SmartPointer<IO::TextWriter>& pOutWriter) / SmartPointer<IO::TextWriter> static Out()`

Sets or gets the standard output writer stream for log output redirection.

**Usage:**
```cpp
SmartPointer<StringWriter> writer(true);
Console::SetOut(writer);
Console::WriteLine("Output redirected!");
auto currentOut = Console::Out();
```

##### `void static SetError(const SmartPointer<IO::TextWriter>& pErrorWriter) / SmartPointer<IO::TextWriter> static Error()`

Sets or gets the standard error writer stream for error log output redirection.

**Usage:**
```cpp
SmartPointer<StringWriter> errorWriter(true);
Console::SetError(errorWriter);
auto currentError = Console::Error();
```

##### `void static SetIn(const SmartPointer<IO::TextReader>& pInReader) / SmartPointer<IO::TextReader> static In()`

Sets or gets the standard input reader stream for input redirection.

**Usage:**
```cpp
SmartPointer<StringReader> reader(new StringReader("Redirection Line"));
Console::SetIn(reader);
String line = Console::ReadLine();
auto currentIn = Console::In();
```

#### Test Helpers

##### `void static SetIn(const String& value)`

Adds a string to the internal input buffer for testing purposes.

##### `static Array<String> GetOutputs()`

Returns the accumulated outputs (from `WriteLine` calls) since the last `Clear`.

## Code Example

Below is a complete, compile-ready example demonstrating the usage of `Console` and console redirection APIs.

```cpp
#include "System/Console.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/IO/StringWriter.h"
#include "System/IO/StringReader.h"
#include "System/Exception.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

int main() {
    try {
        // Set colors and write lines
        Console::SetForegroundColor(ConsoleColor::Cyan);
        Console::WriteLine("--- Console API Demonstration ---");
        Console::ResetColor();

        // Standard Output Redirection
        SmartPointer<StringWriter> customWriter(true);
        Console::SetOut(customWriter);
        Console::WriteLine("This message is redirected to StringWriter.");

        // Standard Input Redirection
        SmartPointer<StringReader> customReader(new StringReader("Redirected Input Data"));
        Console::SetIn(customReader);
        String inputData = Console::ReadLine();

        // Restore / Reset
        Console::Clear();

    } catch (const Exception& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
        return 1;
    }
    return 0;
}
```


