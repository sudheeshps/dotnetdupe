# DotNetDupe Project Instructions

## Project Overview
DotNetDupe is a C++17 library designed to replicate the familiar and developer-friendly .NET Base Class Library (BCL) APIs in C++. It provides a set of classes in namespaces like `System`, `System::IO`, `System::Collections::Generic`, and `System::Threading`, aiming to bridge the gap between C++ performance and C# productivity.

### Core Technologies
- **Language**: C++17
- **Compiler**: MSVC (Visual Studio 2022 / v143 toolset preferred)
- **Build System**: MSBuild (`.sln`, `.vcxproj`)
- **Testing**: Google Test (gtest)
- **Platform**: Windows (Win32/x64)

## Project Structure
- `DotNetDupe/`: Source code (`.cpp`) for the main library.
- `Include/`: Public header files (`.h`), organized by namespace.
- `DotNetDupeTests/`: Unit tests using Google Test.
- `DotNetDupeDemo/`: A console application demonstrating library usage.
- `docs/`: Markdown documentation for various classes and comparisons.
- `bin/`: Output directory for compiled binaries.
- `obj/`: Intermediate directory for build artifacts.

## Building and Running

### Build via MSBuild
To build the solution from a Developer Command Prompt:
```powershell
msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64
```

### Running Tests
After building, the tests can be executed via the generated test runner:
```powershell
.\bin\x64\Release\DotNetDupeTests.exe
```

### NuGet Package Generation
The project is configured to generate a NuGet package on build (as specified in `README.md`). See the "Getting Started" section in `README.md` for local installation steps.

## Development Conventions

### Naming & Style
- **Namespaces**: Use `DotNetDupe::System` as the root.
- **Classes**: PascalCase (e.g., `String`, `DateTimeOffset`).
- **Methods**: PascalCase (e.g., `GetLength()`, `ToString()`).
- **Variables**: Follow [Microsoft C++ coding standards](https://learn.microsoft.com/en-us/visualstudio/ide/editorconfig-naming-conventions?view=vs-2022).
  - **Local Variables & Parameters**: Hungarian notation with type-based prefixes (e.g., `sSourceFileName`, `iIndex`, `pBuffer`).
  - **Private Members**: Prefixed with `m_` followed by type-based prefix (e.g., `m_nItems`, `m_hHandle`).
  - **Static Fields**: Prefixed with `s_` (e.g., `s_defaultInstance`).
- **Headers**: Use `#pragma once` and include files relative to the `Include/` directory.

### Code Patterns
- **Public APIs**: Avoid using STL types (e.g., `std::string`, `std::vector`) in public function arguments or return types. Use library types like `DotNetDupe::System::String` or `DotNetDupe::System::Collections::Generic::List` instead to maintain consistency and ABI stability.
- **Standards & Safety**:
  - Follow the [C++ Core Guidelines](https://learn.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=msvc-170) for all new code.
  - Use the [GSL (Guidelines Support Library)](https://github.com/microsoft/GSL) for runtime safety (e.g., `gsl::span`, `gsl::not_null`, `gsl::finally`).
  - Ensure portability across platforms (Windows, POSIX) when generating new code or updating existing code. Use conditional compilation (`#if defined(_WIN32)`) only when necessary.
- **DLL Exports**: Use the `DOTNETDUPE_API` macro (defined in `Include/Common.h`) for any classes or functions that need to be exported from the library DLL.
- **UTF-8 Portability**: Use UTF-8 character encoding for all string operations to ensure cross-platform portability across Windows, Linux, and other environments.
- **Precompiled Headers**: Implementation files in the `DotNetDupe/` directory should `#include "pch.h"`.
- **Memory Management**: **SmartPointer shall be used in all places and no direct pointer manipulation (e.g., raw `new`, `delete`, or raw pointers for ownership) is allowed.** Always follow RAII patterns. Many classes (like `FileStream`, `WaitHandle`) implement `.NET`-like patterns but rely on C++ destructors for resource cleanup.
- **Exceptions**: DotNetDupe classes shall not throw standard C++ exceptions (such as `std::runtime_error`, `std::invalid_argument`). Instead, throw the library's custom exception types inheriting from `BasicException` (such as `SystemException`, `ArgumentException`, `InvalidOperationException`, `JsonException`, `IOException`, `SocketException`) to match `.NET` behavior and ensure unified exception handling.

### Testing
- All new functionality should include corresponding unit tests in the `DotNetDupeTests/` project.
- Tests should follow the naming convention `ClassTests.cpp`.

## Class Generation Instructions
When generating a new class, the following instructions must be strictly followed:
1. **Strictly follow the development guidelines** defined in this document (naming, style, code patterns, etc.).
2. **Add tests in GivenWhenThen format** covering positive, negative, and edge cases to ensure robust functionality.
3. **Add documentation following documentation guidelines** in the `docs/` directory, referring to existing documentation (e.g., `docs/String.md`, `docs/Process.md`) for style and structure. **Each API documentation file must include clear, complete, and compilation-ready sample code usage showing real-world application of the API.**
4. **Link the documentation to README.md** under the appropriate namespace section to ensure discoverability.
5. **Add demo code** in the `DotNetDupeDemo` project showing clear, compile-ready usage of the new class, and call it from its `main()` entrypoint.
6. **Do not throw standard C++ exceptions** (e.g., `std::runtime_error`, `std::invalid_argument`). Instead, throw the library's custom exception types (e.g., `SystemException`, `ArgumentException`, `IOException`, `SocketException`) inheriting from `BasicException` to match .NET behavior and ensure unified exception handling.
7. **Do not use STL threading/timing utilities in demo code** (e.g., `std::thread`, `std::this_thread::sleep_for`). Use the library's own `Thread` class, `ThreadPool`, `Task`, or `Thread::Sleep()` to demonstrate library-centric API usage.

## Contextual Precedence
The instructions in this file are foundational. Always adhere to these patterns when extending or modifying the codebase to maintain architectural consistency.
