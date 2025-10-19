### template class `BasicException<CharT>`

Represents errors that occur during application execution.

**Note:** The `Exception` class is a typedef for `BasicException<TCHAR>`.

#### Usage

```cpp
#include <iostream>
#include "../DotNetDupe/Exception.h"

int main() {
    try {
        throw DotNetDupe::System::Exception(_T("Something went wrong!"));
    }
    catch (const DotNetDupe::System::Exception& e) {
        std::wcout << L"Caught exception: " << e.What() << std::endl;
    }
    return 0;
}
```

#### Methods

##### `BasicException(const CharT* pchMessage)`

Initializes a new instance of the `BasicException` class with a specified error message.

##### `CharT* What() const`

Returns a pointer to the error message.

---

### template class `BasicSystemException<CharT>`

The base class for all predefined exceptions in the System namespace.

**Note:** The `SystemException` class is a typedef for `BasicSystemException<TCHAR>`.

#### Methods

##### `BasicSystemException(const CharT* pchMessage)`

Initializes a new instance of the `BasicSystemException` class with a specified error message.

---

### template class `BasicArgumentException<CharT>`

Represents errors that occur during argument processing.

**Note:** The `ArgumentException` class is a typedef for `BasicArgumentException<TCHAR>`.

#### Methods

##### `BasicArgumentException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArgumentException` class with a specified error message.

---

### class `ArgumentNullException`

The exception that is thrown when a null reference is passed to a method that does not accept it as a valid argument.

#### Methods

##### `ArgumentNullException(const CharT* pchMessage)`

Initializes a new instance of the `ArgumentNullException` class with a specified error message.

---

### template class `BasicArgumentOutOfRangeException<CharT>`

Represents errors that occur when an argument is outside the allowable range of values.

**Note:** The `ArgumentOutOfRangeException` class is a typedef for `BasicArgumentOutOfRangeException<TCHAR>`.

#### Methods

##### `BasicArgumentOutOfRangeException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArgumentOutOfRangeException` class with a specified error message.

---

### template class `BasicArithmeticException<CharT>`

Represents errors in an arithmetic operation.

**Note:** The `ArithmeticException` class is a typedef for `BasicArithmeticException<TCHAR>`.

#### Methods

##### `BasicArithmeticException(const CharT* pchMessage)`

Initializes a new instance of the `BasicArithmeticException` class with a specified error message.

---

### template class `BasicIOException<CharT>`

The exception that is thrown when an I/O error occurs.

**Note:** The `IOException` class is a typedef for `BasicIOException<TCHAR>`.

#### Methods

##### `BasicIOException(const CharT* pchMessage)`

Initializes a new instance of the `BasicIOException` class with a specified error message.

---

### template class `BasicNotImplementedException<CharT>`

The exception that is thrown when a requested method or operation is not implemented.

**Note:** The `NotImplementedException` class is a typedef for `BasicNotImplementedException<TCHAR>`.

#### Methods

##### `BasicNotImplementedException(const CharT* pchMessage)`

Initializes a new instance of the `BasicNotImplementedException` class with a specified error message.

---

### template class `BasicOverflowException<CharT>`

The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow.

**Note:** The `OverflowException` class is a typedef for `BasicOverflowException<TCHAR>`.

#### Methods

##### `BasicOverflowException(const CharT* pchMessage)`

Initializes a new instance of the `BasicOverflowException` class with a specified error message.
