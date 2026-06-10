# DotNetDupe 🚀

<!-- Badges (Example - replace with actual badges if available) -->
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](https://example.com/your-build-status)

Ever admired the elegance and developer-friendliness of .NET APIs? 🤔 While the C++ Standard Template Library (STL) is powerful, its learning curve can be steep. This project, **DotNetDupe**, bridges that gap! 🌉

Inspired by the clear and concise API design of C# .NET, DotNetDupe is a C++ library that brings a familiar, streamlined development experience to your C++ projects. ✨

> [!IMPORTANT]
> **Major Update (v3.0.0):** DotNetDupe is now fully cross-platform! 🌐 We've introduced official support for Linux (tested via Ubuntu on WSL) alongside Windows, applied C++ Core Coding Guidelines for better reliability, and introduced the `SmartPointer` utility for unified resource management.

We're starting with a foundational set of classes in the `System` and `IO` namespaces, offering a glimpse into the library's potential. Your contributions are highly welcome to expand its functionality! 🤝

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
  - [Cross-Platform Support 🌐](#cross-platform-support-)
    - [Building and Testing](#building-and-testing)
    - [Integration via NuGet](#integration-via-nuget)
  - [WSL Setup Guide (Windows) 🐧](#wsl-setup-guide-windows-)
    - [1. Install WSL](#1-install-wsl)
    - [2. Environment Provisioning](#2-environment-provisioning)
    - [3. Build & Test in WSL](#3-build--test-in-wsl)
  - [Building and Running with NuGet in WSL 🐧📦](#building-and-running-with-nuget-in-wsl-)
  - [Usage 💻](#usage-)
  - [STL vs DotNetDupe Comparison ⚖️](#stl-vs-dotnetdupe-comparison-️)
  - [API Reference 📖](#api-reference-)
  - [Project Status 🚧](#project-status-)
  - [Contributions 👋](#contributions-)
  - [CI/CD Pipeline 🚀](#cicd-pipeline-)
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
*   **Time and Time Zone Support:** Core classes like `DateTimeOffset`, `TimeSpan`, `TimeZone`, and `TimeZoneInfo` for accurate time management.
*   **Type System Enhancements:** (Add more specific features as they are implemented, e.g., `Object` base class, `IComparable`, `IClonable` etc.)

## Getting Started 🚀

### Prerequisites 📋

*   C++17 compatible compiler (e.g., MSVC, GCC, Clang)
*   CMake (for building, if applicable)
*   (Any other dependencies, e.g., Google Test for running tests)

### Installation ⬇️

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/sudheeshps/DotNetDupe.git
    cd DotNetDupe
    ```
2.  **Build the solution and generate NuGet package:**
    Open a Developer Command Prompt for Visual Studio and navigate to the project root.
    ```bash
    msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64
    nuget pack DotNetDupe.nuspec -OutputDirectory nuget_packages
    ```
    This will build the DotNetDupe library and automatically create a NuGet package (DotNetDupe.3.0.0.nupkg) in the nuget_packages directory at the solution root.

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


## Cross-Platform Support 🌐

DotNetDupe is designed for high portability and officially supports **Windows** (via MSVC/MSBuild) and **Linux** (via GCC/Clang/CMake).

### Building and Testing

| Platform | Build System | Build Command | Test Command |
| :--- | :--- | :--- | :--- |
| **Windows** | MSBuild | `msbuild DotNetDupe.sln /p:Configuration=Release` | `.\bin\x64\Release\DotNetDupeTests.exe` |
| **Linux / WSL** | CMake | `cmake -S . -B build && cmake --build build` | `cd build && ctest` |

### Integration via NuGet

DotNetDupe is distributed as a multi-platform NuGet package. It contains native binaries for:
- `win-x64` (`DotNetDupe.dll`)
- `linux-x64` (`libDotNetDupe.so`)

When you add the NuGet package to your project, the appropriate binary is automatically selected based on your target platform.

#### Note for Linux Users
On Linux, NuGet packages are typically managed via `dotnet` CLI or integrated into CMake projects using tools like `vcpkg` or by manually extracting the shared library (`.so`) and headers from the `.nupkg` (which is a ZIP file).

Example manual extraction:
```bash
unzip DotNetDupe.nupkg -d dotnetdupe_lib
# Use dotnetdupe_lib/include for headers
# Use dotnetdupe_lib/runtimes/linux-x64/native/libDotNetDupe.so for linking
```

## WSL Setup Guide (Windows) 🐧

For Windows developers who want to build and test for Linux locally, we recommend using the Windows Subsystem for Linux (WSL).

### 1. Install WSL
If you haven't already, install Ubuntu via PowerShell:
```powershell
wsl --install -d Ubuntu
```

### 2. Environment Provisioning
Inside your WSL terminal, install the C++ build chain:
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake
```

### 3. Build & Test in WSL
Navigate to your project root (e.g., `/mnt/d/Projects/DotNetDupe`) and run:
```bash
# Create build directory
mkdir -p build-wsl && cd build-wsl

# Configure and Build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Run Tests
ctest --output-on-failure
```

### 4. Running WSL Commands from PowerShell
You can also build and test for Linux directly from a Windows PowerShell terminal without manually entering the WSL shell:

```powershell
# Create build directory
wsl -d Ubuntu -- bash -c "mkdir -p build-wsl"

# Configure
wsl -d Ubuntu -- bash -c "cd build-wsl && cmake .. -DCMAKE_BUILD_TYPE=Release"

# Build
wsl -d Ubuntu -- bash -c "cd build-wsl && cmake --build ."

# Run Tests (via CTest)
wsl -d Ubuntu -- bash -c "cd build-wsl && ctest"

# Run Tests (direct execution)
wsl -d Ubuntu -- bash -c "cd build-wsl && ./DotNetDupeTests"

# Run Demo Application
wsl -d Ubuntu -- bash -c "cd build-wsl && ./DotNetDupeDemo"
```

## Building and Running with NuGet in WSL 🐧📦

This section describes how to use the pre-compiled NuGet package to build and run the `DotNetDupeDemo` application in a WSL environment.

### 1. Extract the NuGet Package
The `.nupkg` file is a ZIP archive. Extract it to a local directory (e.g., `DotNetDupe_NuGet`) using PowerShell:
```powershell
Expand-Archive -Path "nuget_packages\DotNetDupe.3.0.0.nupkg" -DestinationPath "DotNetDupe_NuGet" -Force
```

### 2. Configure and Build in WSL
Use CMake with the `USE_NUGET` option enabled and point `NUGET_PATH` to the extracted directory. WSL automatically handles the path mapping for Windows drives.

```powershell
# Configure
wsl cmake -S . -B build-wsl -DUSE_NUGET=ON -DNUGET_PATH="./DotNetDupe_NuGet"

# Build the Demo Application
wsl cmake --build build-wsl --target DotNetDupeDemo
```

### 3. Execute in WSL
Run the demo application directly from PowerShell via `wsl`:
```powershell
wsl ./build-wsl/DotNetDupeDemo
```

## Usage 💻

Here are some quick examples of how to use DotNetDupe:

```cpp
#include <iostream>
#include "DotNetDupe/String.h"
#include "DotNetDupe/Path.h"
#include "DotNetDupe/BasicException.h"
#include "DotNetDupe/TimeZoneInfo.h"

int main() {
    // String Example
    DotNetDupe::System::String greeting = _T("Hello");
    DotNetDupe::System::String name = _T("World");
    DotNetDupe::System::String message = greeting + _T(", ") + name + _T("!");
    std::wcout << message << std::endl; // Output: Hello, World!

    // Path Example
    DotNetDupe::System::String fullPath = DotNetDupe::System::IO::Path::Combine({_T("C:\\"), _T("Users"), _T("document.txt")});
    std::wcout << L"Combined Path: " << fullPath << std::endl;

    // TimeZone Example
    DotNetDupe::System::TimeZoneInfo localTz = DotNetDupe::System::TimeZoneInfo::Local();
    std::wcout << L"Local TimeZone: " << localTz.GetDisplayName() << std::endl;

    // Exception Example
    try {
        throw DotNetDupe::System::BasicException<TCHAR>(_T("Something went wrong!"));
    } catch (const DotNetDupe::System::BasicException<TCHAR>& e) {
        std::wcout << L"Caught exception: " << e.What() << std::endl;
    }

    return 0;
}
```

## STL vs DotNetDupe Comparison ⚖️

DotNetDupe is designed to be more intuitive and less verbose than the standard C++ STL.

*   **[General Comparison Guide](docs/Comparison.md)**: Covers Strings, Collections, Timing, etc.
*   **[Threading Comparison Guide](docs/ThreadingComparison.md)**: Detailed comparison of thread synchronization primitives.
*   **[Process Management Comparison Guide](docs/ProcessComparison.md)**: Comparison of process execution and management.

### Sample Client and Test Code 🧪

The repository includes `DotNetDupeDemo` (a sample console application) and `DotNetDupeTests` (unit tests) projects. These projects demonstrate how to integrate and use the `DotNetDupe` library. You can refer to their `.vcxproj` files for examples of how to configure your own projects to consume the `DotNetDupe` NuGet package.




## API Reference 📖

For detailed information on the available classes, methods, and their usage, please refer to the comprehensive API documentation for each class.

### Namespace: `DotNetDupe::System`

**Classes**

| Class | Description |
|---|---|
| [Array<T>](docs/Array.md) | Provides methods for creating, manipulating, searching, and sorting arrays. |
| [BitConverter](docs/BitConverter.md) | Converts base data types to an array of bytes, and an array of bytes to base data types. |
| [Buffer](docs/Buffer.md) | Manipulates arrays of primitive types. |
| [Char](docs/Char.md) | Represents a character. |
| [Console](docs/Console.md) | Provides methods for reading from and writing to the standard input, output, and error streams. |
| [Convert](docs/Convert.md) | Provides methods for converting a base data type to another base data type. |
| [DaylightTime](docs/DaylightTime.md) | Defines the period of daylight saving time. |
| [DateTimeOffset](docs/DateTimeOffset.md) | Represents a point in time, typically expressed as a date and time of day relative to UTC. |
| [Environment](docs/Environment.md) | Provides information about the current environment and platform. |
| [Guid](docs/Guid.md) | Represents a globally unique identifier (GUID). |
| [Object](docs/Object.md) | Supports all classes in the .NET class hierarchy and provides low-level services to derived classes. |
| [OperatingSystem](docs/OperatingSystem.md) | Represents information about an operating system, such as the version and platform identifier. |
| [Random](docs/Random.md) | Represents a pseudo-random number generator. |
| [SmartPointer<T>](docs/SmartPointer.md) | A unified smart pointer supporting both unique and shared ownership. |
| [String](docs/String.md) | Represents text as a sequence of character code units. |
| [TimeProvider](docs/TimeProvider.md) | Provides an abstraction for time. |
| [TimeSpan](docs/TimeSpan.md) | Represents a time interval. |
| [TimeZone](docs/TimeZone.md) | Represents a time zone. |
| [TimeZoneInfo](docs/TimeZoneInfo.md) | Represents any time zone in the world. |
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
| [FormatException](docs/Exceptions.md) | The exception that is thrown when the format of an argument is invalid. |
| [NotImplementedException](docs/Exceptions.md) | The exception that is thrown when a requested method or operation is not implemented. |
| [OverflowException](docs/Exceptions.md) | The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow. |

**Interfaces**

| Interface | Description |
|---|---|
| [IClonable](docs/Interfaces.md) | Defines a general-purpose mechanism for creating a new object that is a copy of the current object. |
| [IComparable](docs/Interfaces.md) | Defines a method that a value type or class implements to compare itself with another object of the same type. |
| [IComparable<T>](docs/Interfaces.md) | Defines a method that a value type or class implements to compare itself with another object of the same type. |
| [IFormatProvider<T>](docs/Interfaces.md) | Provides a mechanism for retrieving a formatting service for a specified type. |

### Namespace: `DotNetDupe::System::Collections::Generic`

**Classes**

| Class | Description |
|---|---|
| [Dictionary<TKey, TValue>](docs/Dictionary.md) | Represents a collection of keys and values. |
| [List<T>](docs/List.md) | Represents a strongly typed list of objects that can be accessed by index. |

### Namespace: `DotNetDupe::System::Diagnostics`

**Classes**

| Class | Description |
|---|---|
| [Process](docs/Process.md) | Provides access to local processes and enables you to start and stop local system processes. |
| [Stopwatch](docs/Stopwatch.md) | Provides a set of methods and properties that you can use to accurately measure elapsed time. |

### Namespace: `DotNetDupe::System::Text`

**Classes**

| Class | Description |
|---|---|
| [StringBuilder](docs/StringBuilder.md) | Represents a mutable string of characters. |
| [TextEncoding](docs/TextEncoding.md) | Represents a character encoding. |

### Namespace: `DotNetDupe::System::Text::Json`

**Classes**

| Class | Description |
|---|---|
| [JsonSerializer](docs/JsonSerializer.md) | Provides functionality to serialize objects to JSON and deserialize JSON to objects. |

### Namespace: `DotNetDupe::System::IO`

**Classes**

| Class | Description |
|---|---|
| [Path](docs/Path.md) | A class for path-related operations. |
| [File](docs/File.md) | Provides static methods for the creation, copying, deletion, moving, and opening of a single file. |
| [FileStream](docs/FileStream.md) | Provides a `Stream` for a file, supporting both synchronous and asynchronous read and write operations. |
| [Stream](docs/Stream.md) | Provides a generic view of a sequence of bytes. |
| [TextReader](docs/TextReader.md) | Represents a reader that can read a sequential series of characters. |
| [TextWriter](docs/TextWriter.md) | Represents a writer that can write a sequential series of characters. |
| [StringReader](docs/StringReader.md) | Implements a `TextReader` that reads from a string. |
| [StringWriter](docs/StringWriter.md) | Implements a `TextWriter` for writing information to a string. |

**Exceptions**

| Exception | Description |
|---|---|
| [IOException](docs/Exceptions.md) | The exception that is thrown when an I/O error occurs. |

**Interfaces**

| Interface | Description |
|---|---|
| [IDisposable](docs/Interfaces.md) | Provides a mechanism for releasing unmanaged resources. |

### Namespace: `DotNetDupe::System::Net::Sockets`

**Classes**

| Class | Description |
|---|---|
| [Socket](docs/Sockets.md) | A wrapper around standard native socket APIs. |
| [NetworkStream](docs/Sockets.md) | Provides the underlying stream of data for network access. |
| [TcpClient](docs/Sockets.md) | Provides client connections for TCP network services. |
| [TcpListener](docs/Sockets.md) | Listens for connections from TCP network clients. |
| [UdpClient](docs/Sockets.md) | Provides UDP datagram sockets. |

### Namespace: `DotNetDupe::System::Threading`

**Classes**

| Class | Description |
|---|---|
| [Thread](docs/Thread.md) | Creates and controls a thread, sets its priority, and gets its status. |
| [ThreadPool](docs/ThreadPool.md) | Provides a pool of threads that can be used to execute tasks, post work items, and more. |
| [Task](docs/Task.md) | Represents an asynchronous operation. |
| [WaitHandle](docs/WaitHandle.md) | Abstract base class for synchronization objects. |
| [EventWaitHandle](docs/EventWaitHandle.md) | Represents a thread synchronization event. |
| [ManualResetEvent](docs/ManualResetEvent.md) | Notifies one or more waiting threads that an event has occurred. |
| [AutoResetEvent](docs/AutoResetEvent.md) | Notifies a waiting thread that an event has occurred. |
| [Mutex](docs/Mutex.md) | A synchronization primitive that can also be used for inter-process synchronization. |
| [Semaphore](docs/Semaphore.md) | Limits the number of threads that can access a resource or pool of resources concurrently. |
| [SemaphoreSlim](docs/SemaphoreSlim.md) | A lightweight alternative to `Semaphore`. |
| [CriticalSection](docs/CriticalSection.md) | A wrapper around a recursive mutex for critical sections. |
| [Interlocked](docs/Interlocked.md) | Provides atomic operations for variables that are shared by multiple threads. |
| [Lock<T>](docs/Lock.md) | RAII mechanism for synchronization objects. |

**Exceptions**

| Exception | Description |
|---|---|
| [ThreadStateException](docs/Exceptions.md) | The exception that is thrown when a Thread is in an invalid ThreadState for the method call. |
| [ThreadInterruptedException](docs/Exceptions.md) | The exception that is thrown when a Thread is interrupted while it is in a waiting state. |
| [SynchronizationLockException](docs/Exceptions.md) | The exception that is thrown when a method requires the caller to own the lock on a given Monitor, and the method is invoked by a caller that does not own that lock. |
| [AbandonedMutexException](docs/Exceptions.md) | The exception that is thrown when one thread acquires a Mutex object that another thread has abandoned by exiting without releasing it. |
| [WaitHandleCannotBeOpenedException](docs/Exceptions.md) | The exception that is thrown when an attempt is made to open a system mutex, semaphore, or event wait handle that does not exist. |
| [SemaphoreFullException](docs/Exceptions.md) | The exception that is thrown when the Release method is called on a semaphore whose count is already at the maximum. |

## Project Status 🚧

DotNetDupe is currently **under active development**. We are continuously working on expanding the API coverage and improving stability.

## Contributions 👋

Contributions to the DotNetDupe project are highly welcome! Whether it's bug reports, feature requests, code contributions, or documentation improvements, your help is invaluable. Please refer to [GitHub's general contributing guidelines](https://docs.github.com/en/get-started/exploring-projects-on-github/contributing-to-a-project) for more information on how to get started.

## CI/CD Pipeline 🚀

This repository uses GitHub Actions to automate the build, test, and release process.

### Workflow Details
- **Build & Test**: Every push to `main` and all pull requests trigger a full build (Debug & Release) and execution of all unit tests.
- **NuGet Release**: Pushing a tag (e.g., `v1.0.0`) triggers the creation and publishing of the NuGet package to [nuget.org](https://www.nuget.org/).

### How to Release
1. Update the version in `DotNetDupe.nuspec`.
2. Commit and push the changes.
3. Create and push a new tag:
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```
   *Note: Requires `NUGET_API_KEY` to be set in GitHub repository secrets.*

## License 📄

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

## Generated Content 🤖

This project is a unique blend of manual craftsmanship and AI-powered development. While the initial core components were meticulously crafted by hand, the majority of the remaining code, including many classes, methods, and their corresponding unit tests, were generated with the assistance of Gemini, a large language model. This approach highlights the potential of generative AI in accelerating software development from scratch.

## Contact 📧

For questions or support, please open an issue on the [GitHub repository](https://github.com/sudheeshps/DotNetDupe/issues) or contact [sudheeshps@gmail.com](mailto:your-email@example.com).