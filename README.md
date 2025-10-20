# DotNetDupe 🚀

<!-- Badges (Example - replace with actual badges if available) -->
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](https://example.com/your-build-status)

Ever admired the elegance and developer-friendliness of .NET APIs? 🤔 While the C++ Standard Template Library (STL) is powerful, its learning curve can be steep. This project, **DotNetDupe**, bridges that gap! 🌉

Inspired by the clear and concise API design of C# .NET, DotNetDupe is a C++ library that brings a familiar, streamlined development experience to your C++ projects. ✨ We're starting with a foundational set of classes in the `System` and `IO` namespaces, offering a glimpse into the library's potential. Your contributions are highly welcome to expand its functionality! 🤝

A fun fact: While I personally crafted core components like `Char`, `String`, and `Path` (along with their tests), a significant portion of the remaining code was generated with the help of Gemini Code Assist. 🤖 This project serves as a unique playground for exploring how generative AI can accelerate development from scratch. 🚀

DotNetDupe aims to simplify C++ development by providing C#-like interfaces for common tasks, making your code more concise, intuitive, and a joy to write. 💖

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

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-username/DotNetDupe.git
    cd DotNetDupe
    ```
2.  **Build the solution and generate NuGet package:**
    Open a Developer Command Prompt for Visual Studio and navigate to the project root.
    ```bash
    msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64
    ```
    This will build the `DotNetDupe` library and automatically create a NuGet package (`DotNetDupe.1.0.0.nupkg`) in the `nuget_packages` directory at the solution root.

3.  **Add local NuGet package source:**
    To use the locally generated NuGet package, add the `nuget_packages` directory as a local NuGet source:
    ```bash
    nuget sources Add -Name "DotNetDupeLocal" -Source "D:\Personal\Projects\C++\DotNetDupe\nuget_packages"
    ```
    (Replace `D:\Personal\Projects\C++\DotNetDupe` with your actual solution root path.)

4.  **Install the NuGet package in your project:**
    In your C++ project, you can now install the `DotNetDupe` package using the NuGet Package Manager or the command line:
    ```bash
    nuget install DotNetDupe -OutputDirectory <YourProjectDirectory> -Source DotNetDupeLocal
    ```
    (Replace `<YourProjectDirectory>` with the path to your project where you want to install the package.)

5.  **Integrate into your project:**
    Once installed, ensure your project's `.vcxproj` file is configured to link against the `DotNetDupe.lib` and include its headers. The NuGet package's `.targets` file should handle most of this automatically.


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

### Sample Client and Test Code 🧪

The repository includes `DotNetDupeClient` (a sample console application) and `DotNetDupeTests` (unit tests) projects. These projects demonstrate how to integrate and use the `DotNetDupe` library. You can refer to their `.vcxproj` files for examples of how to configure your own projects to consume the `DotNetDupe` NuGet package.




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

Contributions to the DotNetDupe project are highly welcome! Whether it's bug reports, feature requests, code contributions, or documentation improvements, your help is invaluable. Please refer to [GitHub's general contributing guidelines](https://docs.github.com/en/get-started/exploring-projects-on-github/contributing-to-a-project) for more information on how to get started.

## License 📄

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

## Generated Content 🤖

This project is a unique blend of manual craftsmanship and AI-powered development. While the initial core components were meticulously crafted by hand, the majority of the remaining code, including many classes, methods, and their corresponding unit tests, were generated with the assistance of Gemini, a large language model. This approach highlights the potential of generative AI in accelerating software development from scratch.

## Contact 📧

For questions or support, please open an issue on the [GitHub repository](https://github.com/your-username/DotNetDupe/issues) or contact [sudheeshps@gmail.com](mailto:your-email@example.com).