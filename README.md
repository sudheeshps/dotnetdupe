# DotNetDupe 🚀

<!-- Badges (Example - replace with actual badges if available) -->
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](https://example.com/your-build-status)

Hey, have you everbeen fascinated by the simplicity of .NET APIs? How do you feel when you compare with STL? They are 2 different worlds altogether, right? :). Though STL is vast and comprehensive, it is very difficult to use. Incontrast C# .NET being a modern framework designed by clear emphasis on API design, it is the more developer friendly. This project is inspired by .NET APIs and built referring the C# namespaces and classes. Just to start with only few classes in System and IO namespaces are implemented. It will give a gist on the intention of the library. Any PRs are welcome and we can add more and more classes to it. 

By the way, though I started implementation by coding myself from scratch (most of Char, String and  Path along with tests), I leveraged the boon of Gemini Code Assist to generate rest of the code. In that way, it can be treated as our playground for trying out implementing something from scratch using Gen AI.

DotNetDupe is a C++ library designed to provide a familiar and streamlined development experience by mimicking common C# APIs. It offers an alternative to the often cumbersome C++ Standard Template Library (STL) for certain functionalities, allowing developers to write more concise and intuitive C++ code.

## Table of Contents 📚

- [DotNetDupe 🚀](#dotnetdupe-)
  - [Table of Contents 📚](#table-of-contents-)
  - [Project Overview 💡](#project-overview-)
  - [Features ✨](#features-)
  - [Getting Started 🚀](#getting-started-)
    - [Prerequisites 📋](#prerequisites-)
    - [Installation ⬇️](#installation-️)
  - [Usage 💻](#usage-)
  - [API Reference 📖](#api-reference-)
    - [Namespace: `DotNetDupe::System`](#namespace-dotnetdupesystem)
    - [Namespace: `DotNetDupe::System::IO`](#namespace-dotnetdupesystemio)
  - [Project Status 🚧](#project-status-)
  - [Contributions 👋](#contributions-)
  - [License 📄](#license-)
  - [Generated Content 🤖](#generated-content-)
  - [Contact 📧](#contact-)

## Project Overview 💡

The core objective of DotNetDupe is to bridge the gap between the power and performance of C++ and the ease of use and productivity offered by C# APIs. By providing C#-like interfaces for common programming tasks, DotNetDupe aims to:

*   **Simplify C++ Development:** Reduce the boilerplate and complexity often associated with STL, making C++ more approachable for developers accustomed to higher-level languages.
*   **Enhance Readability:** Promote cleaner and more readable code by adopting well-known C# API patterns.
*   **Boost Productivity:** Accelerate development cycles by offering intuitive and efficient tools for common operations.

## Features ✨

*   **C#-like API Design:** Intuitive and familiar interfaces for common programming constructs.
*   **Exception Handling:** Robust exception classes mimicking .NET's exception hierarchy.
*   **String Manipulation:** Powerful `String` class with comprehensive methods for text processing.
*   **Path Utilities:** Convenient functions for file system path operations.
*   **Type System Enhancements:** (Add more specific features as they are implemented, e.g., `Object` base class, `IComparable`, `IClonable` etc.)

## Getting Started 🚀

### Prerequisites 📋

*   C++17 compatible compiler (e.g., MSVC, GCC, Clang)
*   CMake (for building, if applicable)
*   (Any other dependencies, e.g., Google Test for running tests)

### Installation ⬇️

(Provide detailed installation instructions here. Example for a typical C++ project with CMake)

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-username/DotNetDupe.git
    cd DotNetDupe
    ```
2.  **Build with CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```
3.  **Integrate into your project:**
    (Instructions on how users can link DotNetDupe to their own C++ projects, e.g., via CMake `find_package` or by including headers and linking libraries.)

## Usage 💻

Here are some quick examples of how to use DotNetDupe:

```cpp
#include <iostream>
#include "DotNetDupe/String.h"
#include "DotNetDupe/Path.h"
#include "DotNetDupe/BasicException.h"

int main() {
    // String Example
    DotNetDupe::System::String greeting = _T("Hello");
    DotNetDupe::System::String name = _T("World");
    DotNetDupe::System::String message = greeting + _T(", ") + name + _T("!");
    std::wcout << message.GetRawString() << std::endl; // Output: Hello, World!

    // Path Example
    DotNetDupe::System::String fullPath = DotNetDupe::System::IO::Path::Combine({_T("C:\\"), _T("Users"), _T("document.txt")});
    std::wcout << L"Combined Path: " << fullPath.GetRawString() << std::endl;

    // Exception Example
    try {
        throw DotNetDupe::System::BasicException<TCHAR>(_T("Something went wrong!"));
    } catch (const DotNetDupe::System::BasicException<TCHAR>& e) {
        std::wcout << L"Caught exception: " << e.What() << std::endl;
    }

    return 0;
}
```



## API Reference 📖

For detailed information on the available classes, methods, and their usage, please refer to the comprehensive API documentation for each class.

### Namespace: `DotNetDupe::System`

**Classes**

| Class | Description |
|---|---|
| [Array<T>](docs/Array.md) | Provides methods for creating, manipulating, searching, and sorting arrays. |
| [Buffer](docs/Buffer.md) | Manipulates arrays of primitive types. |
| [Char](docs/Char.md) | Represents a character. |
| [Environment](docs/Environment.md) | Provides information about the current environment and platform. |
| [Object](docs/Object.md) | Supports all classes in the .NET class hierarchy and provides low-level services to derived classes. |
| [OperatingSystem](docs/OperatingSystem.md) | Represents information about an operating system, such as the version and platform identifier. |
| [Random](docs/Random.md) | Represents a pseudo-random number generator. |
| [String](docs/String.md) | Represents text as a sequence of UTF-16 code units. |
| [Uri](docs/Uri.md) | Provides an object representation of a uniform resource identifier (URI) and easy access to the parts of the URI. |
| [UriBuilder](docs/UriBuilder.md) | Provides a convenient way to modify the contents of a `Uri` instance. |
| [UriComponents](docs/UriComponents.md) | Specifies the parts of a URI. |
| [UriFormat](docs/UriFormat.md) | Controls how URI information is escaped. |
| [UriParser](docs/UriParser.md) | Parses a new URI scheme. |
| [GenericUriParser](docs/GenericUriParser.md) | A customizable parser for a hierarchical URI. |
| [Version](docs/Version.md) | Represents the version number of an assembly, operating system, or the common language runtime. |

**Exceptions**

| Exception | Description |
|---|---|
| [Exception](docs/Exceptions.md) | Represents errors that occur during application execution. |
| [SystemException](docs/Exceptions.md) | The base class for all predefined exceptions in the System namespace. |
| [ArgumentException](docs/Exceptions.md) | Represents errors that occur during argument processing. |
| [ArgumentNullException](docs/Exceptions.md) | The exception that is thrown when a null reference is passed to a method that does not accept it as a valid argument. |
| [ArgumentOutOfRangeException](docs/Exceptions.md) | Represents errors that occur when an argument is outside the allowable range of values. |
| [ArithmeticException](docs/Exceptions.md) | Represents errors in an arithmetic operation. |
| [NotImplementedException](docs/Exceptions.md) | The exception that is thrown when a requested method or operation is not implemented. |
| [OverflowException](docs/Exceptions.md) | The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow. |

**Interfaces**

| Interface | Description |
|---|---|
| [IClonable](docs/Interfaces.md) | Defines a general-purpose mechanism for creating a new object that is a copy of the current object. |
| [IComparable](docs/Interfaces.md) | Defines a method that a value type or class implements to compare itself with another object of the same type. |
| [IComparable<T>](docs/Interfaces.md) | Defines a method that a value type or class implements to compare itself with another object of the same type. |
| [IFormatProvider<T>](docs/Interfaces.md) | Provides a mechanism for retrieving a formatting service for a specified type. |

### Namespace: `DotNetDupe::System::IO`

**Classes**

| Class | Description |
|---|---|
| [Path](docs/Path.md) | A class for path-related operations. |
| [File](docs/File.md) | Provides static methods for the creation, copying, deletion, moving, and opening of a single file. |
| [FileStream](docs/FileStream.md) | Provides a `Stream` for a file, supporting both synchronous and asynchronous read and write operations. |
| [Stream](docs/Stream.md) | Provides a generic view of a sequence of bytes. |

**Exceptions**

| Exception | Description |
|---|---|
| [IOException](docs/Exceptions.md) | The exception that is thrown when an I/O error occurs. |

**Interfaces**

| Interface | Description |
|---|---|
| [IDisposable](docs/Interfaces.md) | Provides a mechanism for releasing unmanaged resources. |

## Project Status 🚧

DotNetDupe is currently **under active development**. We are continuously working on expanding the API coverage and improving stability.

## Contributions 👋

Contributions to the DotNetDupe project are highly welcome! Whether it's bug reports, feature requests, code contributions, or documentation improvements, your help is invaluable. (Please refer to our Contributing Guidelines for more information.)

## License 📄

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details. (Replace MIT with actual license and link to LICENSE file)

## Generated Content 🤖

This `README.md` file was generated with the assistance of Gemini, a large language model. Most of the code in the present form was coded manually. However, the last two methods in the `Path` class and their corresponding tests were added with the help of Gemini as a coding experiment.

## Contact 📧

For questions or support, please open an issue on the [GitHub repository](https://github.com/your-username/DotNetDupe/issues) or contact [sudheeshps@gmail.com](mailto:your-email@example.com). (Replace with actual contact info)