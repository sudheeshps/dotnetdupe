### template class `BasicException<CharT>`

Represents errors that occur during application execution.

**Note:** The `Exception` class is a typedef for `BasicException<TCHAR>`.

#### Methods

##### `BasicException(const CharT* pchMessage)`

Initializes a new instance of the `BasicException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw BasicException<wchar_t>(L"An error occurred.");
```

##### `CharT* What() const`

Returns a pointer to the error message.

**Returns:**
- A pointer to the error message string.

**Usage:**
```cpp
try {
    // ...
} catch (const Exception& e) {
    const TCHAR* msg = e.What();
}
```

---

### template class `BasicSystemException<CharT>`

The base class for all predefined exceptions in the System namespace.

**Note:** The `SystemException` class is a typedef for `BasicSystemException<TCHAR>`.

#### Methods

##### `BasicSystemException(const CharT* pchMessage)`

Initializes a new instance of the `BasicSystemException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw SystemException(_T("A system error occurred."));
```

---

### template class `BasicArgumentException<CharT>`

Represents errors that occur during argument processing.

**Note:** The `ArgumentException` class is a typedef for `BasicArgumentException<TCHAR>`.

#### Methods

##### `BasicArgumentException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArgumentException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw ArgumentException(_T("Invalid argument."));
```

---

### class `ArgumentNullException`

The exception that is thrown when a null reference is passed to a method that does not accept it as a valid argument.

#### Methods

##### `ArgumentNullException(const TCHAR* message)`

Initializes a new instance of the `ArgumentNullException` class with a specified error message.

**Parameters:**
- `message`: A pointer to the error message string.

**Usage:**
```cpp
if (arg == nullptr) {
    throw ArgumentNullException(_T("arg cannot be null."));
}
```

---

### template class `BasicArgumentOutOfRangeException<CharT>`

Represents errors that occur when an argument is outside the allowable range of values.

**Note:** The `ArgumentOutOfRangeException` class is a typedef for `BasicArgumentOutOfRangeException<TCHAR>`.

#### Methods

##### `BasicArgumentOutOfRangeException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArgumentOutOfRangeException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw ArgumentOutOfRangeException(_T("Index was out of range."));
```

---

### template class `BasicArithmeticException<CharT>`

Represents errors in an arithmetic operation.

**Note:** The `ArithmeticException` class is a typedef for `BasicArithmeticException<TCHAR>`.

#### Methods

##### `BasicArithmeticException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArithmeticException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw ArithmeticException(_T("Arithmetic error."));
```

---

### template class `BasicIOException<CharT>`

The exception that is thrown when an I/O error occurs.

**Note:** The `IOException` class is a typedef for `BasicIOException<TCHAR>`.

#### Methods

##### `BasicIOException(const CharT* pchMessage)`

Initializes a new instance of the `BasicIOException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw IOException(_T("An I/O error occurred."));
```

---

### template class `BasicNotImplementedException<CharT>`

The exception that is thrown when a requested method or operation is not implemented.

**Note:** The `NotImplementedException` class is a typedef for `BasicNotImplementedException<TCHAR>`.

#### Methods

##### `BasicNotImplementedException(const CharT* pchMessage)`

Initializes a new instance of the `BasicNotImplementedException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw NotImplementedException(_T("The method is not implemented."));
```

---

### template class `BasicOverflowException<CharT>`

The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow.

**Note:** The `OverflowException` class is a typedef for `BasicOverflowException<TCHAR>`.

#### Methods

##### `BasicOverflowException(const CharT* pchMessage)`

Initializes a new instance of the `BasicOverflowException` class with a specified error message.

**Parameters:**
- `pchMessage`: A pointer to the error message string.

**Usage:**
```cpp
throw OverflowException(_T("Arithmetic overflow."));
```
