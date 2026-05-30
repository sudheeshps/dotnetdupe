### class `Console`

Provides methods for reading from and writing to the standard input, output, and error streams.

#### Methods

##### `void static Write(...)`

Writes the text representation of the specified value to the standard output stream. Overloads are available for various types.

**Parameters:**
- `value`: The value to write (bool, Char, double, int, long, long long, float, String, or const TCHAR*).

**Usage:**
```cpp
Console::Write("The answer is ");
Console::Write(42);
```

##### `void static WriteLine(...)`

Writes the text representation of the specified value, followed by the current line terminator, to the standard output stream. Overloads are available for various types.

**Parameters:**
- `value`: (Optional) The value to write (bool, Char, double, int, long, float, String, or const TCHAR*).

**Usage:**
```cpp
Console::WriteLine("Hello World");
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

#### Test Helpers

##### `void static SetIn(const String& value)`

Adds a string to the internal input buffer for testing purposes.

##### `static Array<String> GetOutputs()`

Returns the accumulated outputs (from `WriteLine` calls) since the last `Clear`.

