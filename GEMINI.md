# DotNetDupe Project Instructions

## Project Overview
DotNetDupe is a C++17 library designed to replicate the familiar and developer-friendly .NET Base Class Library (BCL) APIs in C++. It provides a set of classes in namespaces like `System`, `System::IO`, `System::Collections::Generic`, and `System::Threading`, aiming to bridge the gap between C++ performance and C# productivity.

### Core Technologies
- **Language**: C++17
- **Compiler**: MSVC (Visual Studio 2022 / v143 toolset preferred), GCC/Clang on Linux
- **Build System**: MSBuild (`.sln`, `.vcxproj`) on Windows, CMake on Linux
- **Testing**: Google Test (gtest)
- **Platform**: Windows (Win32/x64) and Linux (POSIX)

## Project Structure
- `DotNetDupe/`: Source code (`.cpp`) for the main library.
- `Include/`: Public header files (`.h`), organized by namespace.
- `DotNetDupeTests/`: Unit tests using Google Test.
- `DotNetDupeDemo/`: A console application demonstrating library usage.
- `docs/`: Markdown documentation for various classes and comparisons.
- `scripts/`: Quality Gate and CI/CD validation scripts (`Check-QualityGates.ps1`, `Run-TestsElevated.ps1`).
- `bin/`: Output directory for compiled binaries.
- `obj/`: Intermediate directory for build artifacts.

## Building and Running

### Build via MSBuild (Windows)
To build the solution from a Developer Command Prompt:
```powershell
msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64
```

### Running Tests
After building, the tests can be executed via the generated test runner:
```powershell
.\bin\x64\Release\DotNetDupeTests.exe
```

### Running Quality Gate Static Analysis
To execute the quality gate static analyzer and generate the compliance report:
```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\Check-QualityGates.ps1 -RootDir . -OutputDir CodeCoverage
```

### NuGet Package Generation
The project is configured to generate a NuGet package on build (as specified in `README.md`). See the "Getting Started" section in `README.md` for local installation steps.

## Quality Gate Constraints & Enforcement

All generated, added, or modified code must strictly adhere to the following **11 Quality Gate constraints**. 100% compliance is mandatory with **zero errors and zero warnings** permitted:

| # | Quality Gate Metric | Constraint | Enforcement Description |
|---|---------------------|------------|-------------------------|
| **1** | **Function LLOC Limit** | **$\le 15$ LLOC** | No member method, free function, constructor, destructor, static helper, or lambda can exceed 15 logical lines of code (excluding blank lines, standalone single-line braces `{` / `}`, and standalone comment lines). Decompose complex routines into focused static helpers or subroutines. |
| **2** | **Class LLOC Limit** | **$\le 500$ LLOC** | No single class declaration or implementation can exceed 500 logical lines of code. Multi-responsibility classes must be split across smaller helper components. |
| **3** | **File LLOC Limit** | **$\le 600$ LLOC** | No single implementation file (`.cpp`) can exceed 600 logical lines of code. Split large translation units across logical module files. |
| **4** | **Cyclomatic Complexity (CCN)** | **$\text{CCN} \le 10$** | Cyclomatic complexity per function must not exceed 10. Avoid deeply branchy control flows, nested switch/case chains, or excessive compound conditions. |
| **5** | **Maximum Nesting Depth** | **$\le 4$ Levels** | Code block nesting depth (nested `if`, `for`, `while`, `try`, `switch`) must not exceed 4 levels. Use guard clauses, early returns, and helper functions to flatten control flow. |
| **6** | **DLL Export Macro Placement** | **Function-Level Only** | The `DOTNETDUPE_API` export macro must strictly be placed on individual member methods and free functions, **never at the class level** (`class DOTNETDUPE_API MyClass` is strictly prohibited). |
| **7** | **Exception Types** | **Custom Library Exceptions Only** | DotNetDupe code must **never** throw standard C++ exceptions (`throw std::runtime_error`, `throw std::invalid_argument`, etc.). All exceptions must inherit from `DotNetDupe::System::BasicException` (e.g., `SystemException`, `ArgumentException`, `InvalidOperationException`, `IOException`, `SocketException`, `JsonException`). |
| **8** | **Memory Management & Ownership** | **SmartPointer & RAII (Zero Raw Ownership)** | Raw `new` and `delete` or unmanaged pointer ownership are strictly forbidden. Use `DotNetDupe::System::SmartPointer<T>` and RAII patterns for all dynamic objects. Define alias types (e.g., `using HttpResponseMessagePtr = SmartPointer<HttpResponseMessage>;`). |
| **9** | **Catch Block Integrity** | **No Empty / Swallowed Catches** | Empty catch blocks (`catch (...) {}` or `catch (const std::exception&) {}`) are strictly forbidden. Every catch block must contain meaningful handling, error propagation, or explicit suppressed ignore statements (e.g., `(void)0;` with rationale comments). |
| **10** | **Public API ABI Stability** | **Zero STL Type Leakage** | Public `DOTNETDUPE_API` functions and methods must never use STL types (`std::string`, `std::vector`, `std::map`, etc.) in parameter lists or return types. Use library types (`DotNetDupe::System::String`, `DotNetDupe::System::Collections::Generic::List`, `Dictionary`, etc.). |
| **11** | **SmartPointer Hungarian Naming** | **Mandatory `p` / `sp` / `m_p` / `s_p` Prefix** | All `SmartPointer` variables and parameters must strictly follow Hungarian pointer naming: local/parameter (`pStream`, `spContext`), member (`m_pLogger`, `m_spReader`), static (`s_pFactory`, `s_spInstance`). |

## Development Conventions

### Naming & Style
- **Namespaces**: Use `DotNetDupe::System` as the root namespace hierarchy.
- **Classes & Structs**: PascalCase (e.g., `String`, `DateTimeOffset`, `ProcessInfo`).
- **Methods & Functions**: PascalCase (e.g., `GetLength()`, `ToString()`, `ReadAndWriteData()`).
- **Variables**: Follow Microsoft C++ coding standards with Hungarian notation:
  - **Local Variables & Parameters**: Type-based prefixes (e.g., `sSourceFileName`, `iIndex`, `arrBuffer`, `llTotalBytes`).
  - **SmartPointer Variables**: Must be prefixed with `p` or `sp` (e.g., `pLogger`, `spStream`, `spContext`).
  - **Private Members**: Prefixed with `m_` followed by type-based prefix (e.g., `m_nItems`, `m_hHandle`, `m_pStream`, `m_spClient`).
  - **Static Fields**: Prefixed with `s_` followed by type-based prefix (e.g., `s_defaultInstance`, `s_pFactory`, `s_mtxLock`).
- **Headers**: Use `#pragma once` and include paths relative to the `Include/` directory.

### Code Patterns & Safety
- **Public APIs**: Always expose library types (`String`, `List<T>`, `Dictionary<TKey, TValue>`, `Array<T>`) to guarantee ABI stability and consistency.
- **Standards & Safety**:
  - Adhere to the [C++ Core Guidelines](https://learn.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=msvc-170).
  - Use GSL ([Guidelines Support Library](https://github.com/microsoft/GSL)) for safety checks where applicable (`gsl::span`, `gsl::not_null`, `gsl::finally`).
  - Ensure portability across Windows (Win32/x64) and Linux (POSIX). Use `#if defined(_WIN32)` / `#else` / `#endif` blocks for platform-specific implementations.
- **UTF-8 Portability**: Use UTF-8 character encoding across all string operations for cross-platform portability.
- **Precompiled Headers**: All implementation files in `DotNetDupe/` must `#include "pch.h"`.
- **Code Readability & Grouping**: Add blank lines inside function bodies to cleanly separate logical blocks (parameter validation, initialization, main execution, and return preparation).
- **Avoid Unnecessary `String()` Wrappers**: Rely on `DotNetDupe::System::String`'s implicit converting constructor (`String(const char*)`) rather than creating redundant `String("...")` or `String(pStr)` wrappers.
- **Error Handling**: Do not swallow exceptions or ignore system API failures. Retrieve platform error codes (e.g., `GetLastError()` or `errno`) and throw descriptive library exceptions.

### Testing Guidelines
- All functionality must include corresponding unit tests in the `DotNetDupeTests/` project.
- Test files must follow the naming convention `<Class>Tests.cpp`.
- **100% Test Coverage**: Every added or modified class and API must have 100% unit test coverage in `GivenWhenThen` format covering positive paths, negative paths, null/empty parameters, boundary conditions, custom exceptions, and mocked streams/interfaces.
- **Test Integrity**: Existing unit tests must never be weakened, modified, or deleted during refactoring. All tests must continue to pass cleanly.

## Class Generation & Modification Checklist
When generating a new class or modifying existing code, you must strictly follow this checklist in sequence:
1. **Apply Quality Gate Constraints**: Verify function LLOC $\le 15$, class LLOC $\le 500$, file LLOC $\le 600$, $\text{CCN} \le 10$, nesting depth $\le 4$, Hungarian `SmartPointer` naming, no `std::*` exceptions, no empty catches, no STL public leakage, and function-level `DOTNETDUPE_API` macros.
2. **Add 100% Unit Test Coverage**: Add unit tests in `DotNetDupeTests/<Class>Tests.cpp` in `GivenWhenThen` format covering all execution branches, error conditions, and boundary values.
3. **Add Comprehensive API Documentation**: Add markdown documentation in `docs/<Class>.md` with clear, compile-ready sample code showing real-world application usage.
4. **Link Documentation in README.md**: Link the documentation in `README.md` under the appropriate namespace section.
5. **Add Demo Code in DotNetDupeDemo**: Add a compile-ready demo file in `DotNetDupeDemo/` and invoke it from `DotNetDupeDemo/DotNetDupeDemo.cpp`.
6. **No STL Threading/Timing in Demos**: Use library types (`Thread`, `ThreadPool`, `Task`, `Thread::Sleep()`) rather than STL concurrency primitives.
7. **Verify Solution Build**: Run `msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64` to verify 0 errors and 0 warnings.
8. **Verify Unit Tests**: Run `.\bin\x64\Release\DotNetDupeTests.exe` to verify 100% test pass rate.
9. **Verify Quality Gates**: Run `powershell -ExecutionPolicy Bypass -File .\scripts\Check-QualityGates.ps1 -RootDir . -OutputDir CodeCoverage` to verify 0 errors and 0 warnings.

## Contextual Precedence
The instructions and quality gate rules in this file are foundational and take precedence for all code generation, refactoring, and static analysis verification across the DotNetDupe codebase.
