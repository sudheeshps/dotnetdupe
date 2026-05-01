### template class `BasicIOException<CharT>`

The exception that is thrown when an I/O error occurs.

#### Methods

##### `BasicIOException(const CharT* pchMessage)`

Initializes a new instance of the `BasicIOException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw BasicIOException<wchar_t>(L"An I/O error occurred.");
```
