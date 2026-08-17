# DotNetDupe 🚀

<!-- Dynamic Repository Badges -->
[![Build Status](https://img.shields.io/github/actions/workflow/status/sudheeshps/DotNetDupe/build-and-release.yml?branch=main&style=flat-square&logo=github)](https://github.com/sudheeshps/DotNetDupe/actions/workflows/build-and-release.yml)
[![CodeQL Security](https://img.shields.io/github/actions/workflow/status/sudheeshps/DotNetDupe/codeql.yml?branch=main&style=flat-square&logo=github&label=CodeQL)](https://github.com/sudheeshps/DotNetDupe/actions/workflows/codeql.yml)
[![Coverage](https://img.shields.io/badge/Coverage-Report-brightgreen?style=flat-square&logo=googlechrome)](CodeCoverage/index.html)
[![NuGet Version](https://img.shields.io/nuget/v/DotNetDupe?style=flat-square&logo=nuget&color=blue)](https://www.nuget.org/packages/DotNetDupe)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17%2F20-blue?style=flat-square&logo=cplusplus)](https://en.cppreference.com/w/cpp/20)
[![Platforms](https://img.shields.io/badge/Platforms-Windows%20%7C%20Linux-lightgrey?style=flat-square&logo=linux)](https://github.com/sudheeshps/DotNetDupe#cross-platform-support-)
[![License](https://img.shields.io/github/license/sudheeshps/DotNetDupe?style=flat-square&color=orange)](LICENSE)

Ever admired the elegance and developer-friendliness of .NET APIs? 🤔 While the C++ Standard Template Library (STL) is powerful, its learning curve can be steep. This project, **DotNetDupe**, bridges that gap! 🌉

Inspired by the clear and concise API design of C# .NET, DotNetDupe is a C++ library that brings a familiar, streamlined development experience to your C++ projects. ✨

> [!IMPORTANT]
> **Latest Published Version ([![NuGet Version](https://img.shields.io/nuget/v/DotNetDupe?style=flat-square&logo=nuget&color=blue&label=version)](https://www.nuget.org/packages/DotNetDupe)):** Comprehensive documentation, API reference updates, Pimpl ABI stability, and refined packaging! 🌐 Key highlights include:
> - 🛡️ **Zero Header STL Dependencies:** Completely refactored public headers to eliminate STL dependencies from public interfaces, ensuring clean ABI boundaries and library-centric types across `String`, `Collections`, `IO`, `Net`, `Logging`, and `Data`.
> - 📦 **Core Data Structures & Collections Overhaul:** Pure library implementations for `List<T>`, `Dictionary<K, V>`, `HashSet<T>`, `Queue<T>`, `Stack<T>`, `PriorityQueue<T>`, `SortedDictionary<K, V>`, `SortedSet<T>`, and `LinkedList<T>`.
> - ⚡ **Thread-Safe Concurrent Collections (`System::Collections::Concurrent`):** Lock-free/fine-grained thread-safe data structures including `ConcurrentDictionary`, `ConcurrentQueue`, `ConcurrentStack`, `ConcurrentBag`, and `BlockingCollection`.
> - 📊 **Real-Time Telemetry & System Metrics (`System::Diagnostics::SystemMetrics`):** Real-time monitoring of system hardware metrics (CPU load %, memory usage, disk utilization %, network I/O Mbps, and active processes).
> - 📜 **ETW & Enterprise Event Logging (`System::Diagnostics::EtwLogReader` & `EventLog`):** High-performance Event Tracing for Windows (ETW) and Linux Syslog channel enumeration, querying, and live subscription listening.
> - 🖥️ **Terminal & User Sessions (`System::Diagnostics::TerminalSession` & `ActiveUserSession`):** Enumerate local, disconnected, and remote desktop (RDP) Terminal Services user sessions.
> - 📄 **Console I/O Redirection:** Standard stream redirection via `Console::SetOut`, `Console::SetError`, and `Console::SetIn` using `SmartPointer`.
> - 🖊️ **LoggerTextWriter Log Redirector:** Bridge standard `TextWriter` stream calls directly into `LogManager` logging providers without per-call lookup overhead.
> - 🏷️ **Global LogManager:** Thread-safe category logger caching and static factory via `LogManager::GetLogger("Category")` and `LogManager::GetLogger<T>()`.
> - 📥 **FileDownloader Utility:** High-level download utility with pause/resume support, speed tracking, and prompt file handle release.
> - 🌲 **Recursive Directory Creation:** Overloaded `Directory::CreateDirectory(path, recursive)` to automatically construct missing parent directory structures.
> - 📂 **Resilient File Logging:** Enhanced `FileLoggerProvider` to resolve relative log paths to full paths and auto-create missing log directories.
> - 🧩 **ServiceCollection DI Enhancements:** Improved lifetime management and container registration.

This project is a living example of how persistent human effort during weekends and late evenings can build a system from scratch using AI. DotNetDupe has grown far beyond a foundational set of classes into a comprehensive, multi-platform C++ Base Class Library offering extensive modern capabilities across System, IO, Collections, Net, Threading, Logging, Data, and Hosting.
If anyone wants to join hands, you are most welcome in the form of PRs, issues or comments 🙏

A fun fact: While I personally crafted core components like `Char`, `String`, and `Path` (along with their tests), the rest of the system was collaboratively designed, built, and expanded by myself and my peer Antigravity. 🤖 This project serves as a unique playground for exploring how generative AI can accelerate development from scratch. 🚀

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
  - [Developing Cross-Platform Applications 🌐💻](#developing-cross-platform-applications-)
    - [1. Cross-Platform Project Architecture](#1-cross-platform-project-architecture)
    - [2. Integrating via NuGet Package](#2-integrating-via-nuget-package)
    - [3. Building a Web Application with Static Files & REST APIs](#3-building-a-web-application-with-static-files--rest-apis)
  - [Usage 💻](#usage-)
  - [Web API & Database Integration Guide 🌐🗄️](#web-api--database-integration-guide-️)
    - [1. Hosting REST API Controllers](#1-hosting-rest-api-controllers)
    - [2. Consuming REST APIs](#2-consuming-rest-apis)
    - [3. Using the Database Layer](#3-using-the-database-layer)
  - [STL vs DotNetDupe Comparison ⚖️](#stl-vs-dotnetdupe-comparison-️)
  - [API Reference 📖](#api-reference-)
  - [Project Status 🚧](#project-status-)
  - [Contributions 👋](#contributions-)
  - [CI/CD Pipeline 🚀](#cicd-pipeline-)
    - [Code Coverage & Static Analysis 📊](#code-coverage--static-analysis-)
  - [License 📄](#license-)
  - [Generated Content 🤖](#generated-content-)
  - [Contact 📧](#contact-)

## Project Overview 💡

The core objective of DotNetDupe is to bridge the gap between the power and performance of C++ and the ease of use and productivity offered by C# APIs. By providing C#-like interfaces for common programming tasks, DotNetDupe aims to:

*   **Simplify C++ Development:** Reduce the boilerplate and complexity often associated with STL, making C++ more approachable for developers accustomed to higher-level languages.
*   **Enhance Readability:** Promote cleaner and more readable code by adopting well-known C# API patterns.
*   **Boost Productivity:** Accelerate development cycles by offering intuitive and efficient tools for common operations.

## Features & Library Capabilities ✨

DotNetDupe has evolved into a feature-rich, multi-platform C++20 Base Class Library (BCL) offering an extensive suite of modern capabilities:

- 🧠 **Memory & Object Management (`System`)**:
  - `SmartPointer<T>`: Exception-safe RAII reference-counted smart pointer with `NewShared()` and `NewUnique()` factory helpers.
  - Base `Object` type system with `ToString()`, `GetType()`, `Equals()`, and `GetHashCode()`.

- 🔤 **String Manipulation & Utilities (`System::String`)**:
  - Full-featured `String` & `WString` with UTF-8 / UTF-16 cross-transcoding.
  - Methods: `Format()`, `Split()`, `Join()`, `Replace()`, `Contains()`, `StartsWith()`, `EndsWith()`, `Trim()`, `PadLeft()`, `PadRight()`.

- 📦 **Generic Collections (`System::Collections::Generic`)**:
  - Type-safe container wrappers: `List<T>`, `Dictionary<K, V>`, `Queue<T>`, `Stack<T>`, `HashSet<T>`, and `KeyValuePair<K, V>`.

- 📁 **File I/O & System Services (`System::IO`)**:
  - High-level static primitives: `File` (`ReadAllText`, `WriteAllText`, `AppendAllText`, `Exists`, `Delete`) and `Directory` (`CreateDirectory(path, recursive)`, `Exists`, `EnumerateFiles`).
  - Cross-platform path calculations: `Path` (`Combine`, `GetFullPath`, `GetDirectoryName`, `GetFileName`, `GetExtension`).
  - Stream & Reader/Writer hierarchy: `FileStream`, `MemoryStream`, `BufferedStream`, `StreamReader`, `StreamWriter`, `BinaryReader`, `BinaryWriter`, `StringReader`, `StringWriter`.

- 📟 **Console & Stream Redirection (`System::Console`)**:
  - Rich console text & background color controls (`SetForegroundColor`, `SetBackgroundColor`, `ResetColor`).
  - Full stream redirection via `SetOut()`, `SetError()`, and `SetIn()` accepting `SmartPointer<TextWriter>` and `SmartPointer<TextReader>`.

- 🌲 **Enterprise Logging & Diagnostics (`Extensions::Logging`)**:
  - Structured & Plaintext logging (`ILogger`, `ILoggerProvider`, `LoggerFactory`).
  - Formats: Custom plaintext layout templates & JSON structured payloads with key-value metadata properties.
  - Providers: `ConsoleLoggerProvider` and `FileLoggerProvider` with automatic relative path resolution & recursive parent directory creation.
  - `LogManager`: Category logger caching factory (`GetLogger("Category")`, `GetLogger<T>()`, `GetConsoleLogger`, `GetFileLogger`).
  - `LoggerTextWriter`: High-performance stream redirector bridging `TextWriter` output into `LogManager`.

- 🧵 **Multi-Threading & Process Management (`System::Threading` / `System::Diagnostics`)**:
  - Thread lifecycle (`Thread`), high-throughput `ThreadPool`, and task parallelism (`Task`, `Task<T>`).
  - Synchronization primitives: `Mutex`, `AutoResetEvent`, `ManualResetEvent`, `EventWaitHandle`, `CriticalSection`, `ReaderWriterLockSlim`.
  - Process orchestration: `Process`, `ProcessStartInfo` for executing sub-processes with stdin/stdout/stderr piping.

- 🌐 **Networking, HTTP & Web App Hosting (`System::Net` / `Extensions::Hosting`)**:
  - Low-level socket abstractions (`TcpClient`, `TcpListener`, `UdpClient`, `Socket`).
  - High-level HTTP client: `HttpClient`, `HttpRequestMessage`, `HttpResponseMessage`, `HttpContent`, `StringContent`, `ByteArrayContent`.
  - SSL/TLS security: `SslStream` backed by OpenSSL.
  - Embedded Web Server & Hosting: `WebAppServer` and `HostBuilder` for serving static Web UI files and REST API controllers.

- 💉 **Dependency Injection (`Extensions::DependencyInjection`)**:
  - Full-featured DI container (`ServiceCollection`, `ServiceProvider`, `ServiceDescriptor`).
  - Lifetimes: `Transient`, `Scoped`, `Singleton`.

- 🔐 **Security & Principal (`System::Security::Principal`)**:
  - `UserPrincipal` and `WindowsIdentity` for cross-platform OS user enumeration and privilege inspection.

- ⚡ **Database Data Access (`System::Data`)**:
  - Abstract ADO.NET-style data provider interfaces (`IDbConnection`, `IDbCommand`, `IDataReader`, `IDataParameterCollection`).

## Getting Started 🚀

### Prerequisites 📋

*   C++17 / C++20 compatible compiler (e.g., MSVC v143, GCC 11+, Clang 13+)
*   CMake 3.15+ (for building on Linux / WSL)
*   **OpenSSL / SSL Runtime Dependencies**:
    *   **Windows**: The NuGet package bundles pre-built OpenSSL runtime binaries (`libssl-4-x64.dll`, `libcrypto-4-x64.dll` for x64, and `libssl-4.dll`, `libcrypto-4.dll` for x86) which are automatically copied into the target build output directory via MSBuild `.targets`.
    *   **Linux**: Requires system OpenSSL 3.x / 1.1.x runtime libraries (`libssl.so`, `libcrypto.so`). Install via `sudo apt-get install -y libssl-dev` (Ubuntu/Debian) or `sudo dnf install -y openssl-devel` (Fedora/RHEL).

### Installation ⬇️

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/sudheeshps/DotNetDupe.git
    cd DotNetDupe
    ```
2.  **Build the solution and generate NuGet package:**
    Run the automated build script from PowerShell:
    ```powershell
    .\BuildAndPack.ps1
    ```
    This script will update the resource build timestamp, compile the x64 and x86 Release binaries, and output the NuGet package (`DotNetDupe.4.0.2.nupkg`) into the `nuget_packages` directory.

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
- `win-x64` (`DotNetDupe.dll`, `libssl-4-x64.dll`, `libcrypto-4-x64.dll`)
- `win-x86` (`DotNetDupe.dll`, `libssl-4.dll`, `libcrypto-4.dll`)
- `linux-x64` (`libDotNetDupe.so`)

When you add the NuGet package to your project, the appropriate binary and dependencies are automatically selected based on your target platform.

#### SSL Runtime Dependencies
* **Windows**: Dynamic OpenSSL dependencies (`libssl-4-x64.dll` & `libcrypto-4-x64.dll`) are packaged directly inside the NuGet package and deployed next to `DotNetDupe.dll` at build time.
* **Linux**: Dynamically links against standard host OpenSSL libraries (`libssl.so.3` / `libssl.so.1.1`). Ensure OpenSSL is installed on target runtime hosts (`apt-get install libssl3` or `libssl1.1`).

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
Inside your WSL terminal, install the C++ build chain and OpenSSL development headers:
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libssl-dev pkg-config
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

## Developing Cross-Platform Applications 🌐💻

DotNetDupe enables unified, cross-platform C++20 development across Windows (MSVC / MSBuild) and Linux (GCC/Clang via CMake or WSL).

---

### 1. Cross-Platform Project Architecture

When building a cross-platform application with DotNetDupe:
- Use **UTF-8 character encoding** for cross-platform portability.
- Rely on **`DotNetDupe::System::SmartPointer<T>`** for memory and resource cleanup.
- Avoid platform-specific raw syscalls; use DotNetDupe abstractions like `System::IO::Path`, `System::IO::File`, `System::Threading::Thread`, and `System::Net::Sockets::TcpClient`.

---

### 2. Integrating via NuGet Package

You can package and consume DotNetDupe via NuGet across Windows and Linux (WSL / CMake) projects.

#### A. Generating the NuGet Package
Run the automated build script from PowerShell:
```powershell
.\BuildAndPack.ps1
```
This updates the build timestamp, compiles both x64 and x86 Release binaries, and outputs `DotNetDupe.4.0.2.nupkg` inside the `nuget_packages/` directory.

#### B. Consuming NuGet Package in Visual Studio (Windows)
1. Add the local `nuget_packages` folder as a NuGet Package Source:
   ```bash
   nuget sources Add -Name "DotNetDupeLocal" -Source "D:\Personal\Projects\C++\DotNetDupe\nuget_packages"
   ```
2. In Visual Studio, right-click your project -> **Manage NuGet Packages** -> Select `DotNetDupeLocal` -> Install `DotNetDupe`.

#### C. Consuming NuGet Package on Linux / CMake (WSL)
1. Extract `DotNetDupe.4.0.2.nupkg` (ZIP format) to a local directory:
   ```powershell
   Expand-Archive -Path "nuget_packages\DotNetDupe.4.0.2.nupkg" -DestinationPath "DotNetDupe_NuGet" -Force
   ```
2. Configure CMake pointing `NUGET_PATH` to the extracted package folder:
   ```bash
   cmake -S . -B build -DUSE_NUGET=ON -DNUGET_PATH="./DotNetDupe_NuGet"
   cmake --build build
   ```

---

### 3. Building a Web Application with Static Files & REST APIs

The **`WebAppServer`** class serves static website assets (`index.html`, CSS, JavaScript, images) alongside mapped REST endpoints.

#### Project Directory Layout
```text
MyWebApp/
├── wwwroot/
│   ├── index.html
│   └── site.css
└── main.cpp
```

#### Step 1: Create `wwwroot/index.html`
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>DotNetDupe Web App</title>
    <link rel="stylesheet" href="site.css">
</head>
<body>
    <h1>DotNetDupe Web Application Server</h1>
    <p>Serving static assets and REST API endpoints simultaneously!</p>
    <button onclick="fetchWelcomeMessage()">Hit Welcome Endpoint</button>
    <p id="welcome-output"></p>

    <script>
        async function fetchWelcomeMessage() {
            const response = await fetch('/api/welcome?name=Developer');
            const data = await response.json();
            document.getElementById('welcome-output').innerText = data.message;
        }
    </script>
</body>
</html>
```

#### Step 2: C++ Application (`main.cpp`)
Here is a complete, compile-ready web application hosting `index.html` and responding with a welcome message on hitting `/api/welcome`:

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/IO/Directory.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Server/WebAppServer.h"
#include "WebAppCore/Http/HttpContext.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Server;
using namespace DotNetDupe::WebAppCore::Http;

int main() {
    Console::WriteLine("=============================================");
    Console::WriteLine(" Starting DotNetDupe Cross-Platform Web Server");
    Console::WriteLine("=============================================");

    String webRoot = "wwwroot";
    if (!IO::Directory::Exists(webRoot)) {
        IO::Directory::CreateDirectory(webRoot, true);
    }

    // 1. Initialize WebApplication Host Builder
    auto builder = WebApplication::CreateBuilder();
    auto app = builder->Build();

    // 2. Map REST Endpoint (/api/welcome?name=...)
    app->MapGet("/api/welcome", [](SmartPointer<HttpContext> ctx) -> String {
        String name = "Guest";
        if (ctx->GetRequest()->GetQuery().ContainsKey("name")) {
            name = ctx->GetRequest()->GetQuery()["name"];
        }

        ctx->GetResponse()->SetContentType("application/json");
        return String("{\"status\":\"Success\",\"message\":\"Welcome to DotNetDupe Web Server, ") + name + "!\"}";
    });

    // 3. Initialize WebAppServer to serve website content (index.html & assets)
    WebAppServer server(app, webRoot);
    server.EnableStaticFiles("index.html");

    Console::WriteLine("Server running at: http://localhost:8080/index.html");
    Console::WriteLine("Welcome Endpoint at: http://localhost:8080/api/welcome?name=Developer");

    // 4. Start Server
    server.Run("http://localhost:8080/index.html");

    return 0;
}
```

---

## Usage 💻

Here are some quick examples of how to use DotNetDupe:

### 1. Strings, Dates & Console

```cpp
#include "System/Console.h"
#include "System/String.h"
#include "System/DateTime.h"
#include "System/TimeSpan.h"
#include "System/IO/Path.h"

using namespace DotNetDupe::System;

void DemonstrateBasics() {
    // String interpolation / formatting & manipulation
    String sGreeting = "Hello";
    String sName = "Developer";
    String sMessage = String::Format("{0}, {1}! Welcome to DotNetDupe.", sGreeting, sName);
    Console::WriteLine(sMessage);

    // Cross-platform Path manipulation
    String sDocPath = IO::Path::Combine("C:\\Projects", "DotNetDupe", "README.md");
    Console::WriteLine("Combined Path: {0}", sDocPath);
    Console::WriteLine("Extension: {0}", IO::Path::GetExtension(sDocPath));

    // DateTime and TimeSpan operations
    DateTime dtNow = DateTime::Now();
    DateTime dtTomorrow = dtNow.AddDays(1);
    TimeSpan tsDiff = dtTomorrow - dtNow;
    Console::WriteLine("Current Time: {0}", dtNow.ToString());
    Console::WriteLine("Hours until tomorrow: {0}", tsDiff.GetTotalHours());
}
```

### 2. Multi-Threading & Asynchronous Tasks

```cpp
#include "System/Console.h"
#include "System/Threading/Thread.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Tasks/Task.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Threading;
using namespace DotNetDupe::System::Threading::Tasks;

void DemonstrateConcurrency() {
    // Thread lifecycle
    Thread workerThread([]() {
        Console::WriteLine("Worker thread running...");
        Thread::Sleep(100);
    });
    workerThread.Start();
    workerThread.Join();

    // High-throughput ThreadPool queue
    ThreadPool::QueueUserWorkItem([]() {
        Console::WriteLine("ThreadPool work item executed.");
    });

    // Async Task Parallelism
    auto spTask = Task<int>::Run([]() -> int {
        Thread::Sleep(50);
        return 42 * 2;
    });

    spTask->Wait();
    Console::WriteLine("Task result: {0}", spTask->GetResult());
}
```

### 3. Thread-Safe Concurrent Collections

```cpp
#include "System/Console.h"
#include "System/Collections/Concurrent/ConcurrentDictionary.h"
#include "System/Collections/Concurrent/ConcurrentQueue.h"
#include "System/Collections/Concurrent/BlockingCollection.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Concurrent;

void DemonstrateConcurrentCollections() {
    // Thread-safe ConcurrentDictionary
    ConcurrentDictionary<String, int> dict;
    dict.TryAdd("CPU", 95);
    dict.TryAdd("Memory", 60);
    dict.GetOrAdd("Disk", 40);

    int iUsage = 0;
    if (dict.TryGetValue("CPU", iUsage)) {
        Console::WriteLine("CPU Usage: {0}%", iUsage);
    }

    // Thread-safe Producer-Consumer BlockingCollection
    BlockingCollection<String> pipeline(100);
    pipeline.Add("Packet #1");
    pipeline.Add("Packet #2");

    String sItem;
    if (pipeline.TryTake(sItem)) {
        Console::WriteLine("Processed item from pipeline: {0}", sItem);
    }
}
```

## Web API & Database Integration Guide 🌐🗄️

This section provides comprehensive examples and recommended practices for hosting web services, consuming them, and using the database storage layer.

### 1. Hosting REST API Controllers

DotNetDupe provides an ASP.NET-like hosting model with dependency injection and controller-based routing in the `WebAppCore` namespaces.

#### Defining and Hosting a Controller

1. Declare your data structure.
2. Specialize the `JsonConverter<T>` for the structure to enable automatic serialization and deserialization.
3. Inherit from `ControllerBase` and define your action methods.
4. Bind routes using `ControllerRouteBuilder` and map them in the application host.

```cpp
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Controllers/ControllerBase.h"
#include "System/Collections/Generic/List.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Controllers;

// 1. Declare the data model
struct ProductItem {
    String Name;
    int Price = 0;
};

// 2. Specialize JsonConverter for serialization/deserialization
namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<ProductItem> {
                    static JsonElement Write(const ProductItem& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("name", JsonElement(value.Name));
                        obj.SetProperty("price", JsonElement(static_cast<double>(value.Price)));
                        return obj;
                    }
                    static ProductItem Read(const JsonElement& element) {
                        ProductItem p;
                        JsonElement prop;
                        if (element.TryGetProperty("name", prop)) p.Name = prop.GetString();
                        if (element.TryGetProperty("price", prop)) p.Price = prop.GetInt32();
                        return p;
                    }
                };
            }
        }
    }
}

// 3. Define the controller
class ProductsController : public ControllerBase {
public:
    // GET /api/products
    Collections::Generic::List<ProductItem> GetProducts() {
        Collections::Generic::List<ProductItem> list;
        list.Add(ProductItem{"Espresso Machine", 299});
        list.Add(ProductItem{"Coffee Grinder", 89});
        return list;
    }

    // POST /api/products
    String CreateProduct(const ProductItem& product) {
        return Created(String("Added ") + product.Name);
    }
};

int main() {
    auto builder = WebApplication::CreateBuilder();

    // 4. Register and configure routes on the controller
    builder->AddController<ProductsController>("/api/products")
        .MapGet("", &ProductsController::GetProducts)
        .MapPost("", &ProductsController::CreateProduct);

    auto app = builder->Build();
    app->MapControllers(); // Applies the registered mappings

    app->Run("http://127.0.0.1:5000");
}
```

---

### 2. Consuming REST APIs

DotNetDupe makes client-side web requests simple. You can query endpoints using a low-level HTTP client or a strongly-typed REST resource client.

#### Option A: Low-level HTTP Client (`HttpClient`)

Best for general, raw request handling, sending custom headers, or performing authentication (like setting Bearer tokens).

```cpp
#include "System/Net/Http/HttpClient.h"
#include "System/Console.h"

void GetRawData() {
    using namespace DotNetDupe::System::Net::Http;
    
    HttpClient client;
    
    // Add default headers (e.g. JWT Auth token)
    client.GetDefaultRequestHeaders().Add("Authorization", "Bearer your_token_here");
    
    auto response = client.Get("http://127.0.0.1:5000/api/products");
    if (response->GetStatusCode() == 200) {
        DotNetDupe::System::String json = response->GetContent()->ReadAsString();
        DotNetDupe::System::Console::WriteLine(json);
    }
}
```

#### Option B: Strongly-Typed Client (`RestClient<T>`) - *Recommended*

Best for standard RESTful resources. It handles payload serialization/deserialization to your model structures automatically.

```cpp
#include "System/Net/Http/RestClient.h"
#include "System/Console.h"

void SyncProducts() {
    using namespace DotNetDupe::System::Net::Http;
    
    // Point the RestClient to your resource endpoint
    RestClient<ProductItem> client("http://127.0.0.1:5000/api/products");
    
    // GET all resources automatically parsed into List<T>
    auto products = client.GetAll();
    
    // POST new resource automatically serialized to JSON
    ProductItem newProduct{"Milk Frother", 35};
    DotNetDupe::System::String reply = client.Post(newProduct);
}
```

---

### 3. Using the Database Layer

DotNetDupe emulates C# ADO.NET (`SqlConnection`, `SqlCommand`, `SqlDataReader`) for SQL execution.

#### Recommended Usage Patterns

1. **Routing and Engine Fallback via Connection String**:
   * **In-Memory Emulation (Default)**: Use `Engine=InMemory;` for unit testing or when you don't want server/file dependencies. This runs an in-memory SQL parsing engine.
   * **Persistent SQLite**: Use `Engine=SQLite; Data Source=my_db.db;` when you need actual SQLite file persistence. *Note: Requires compilation with the `DOTNETDUPE_USE_SQLITE` flag defined.*
2. **Always Parameterize Queries**: Use `AddWithValue` parameters to protect queries against parsing issues and SQL syntax injections.
3. **Use RAII and Smart Pointers**: Wrap database commands and readers in smart pointers to guarantee resource cleanup.

```cpp
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Console.h"

void AccessDatabase() {
    using namespace DotNetDupe::System;
    using namespace DotNetDupe::System::Data::SqlClient;

    try {
        // Connect. Uses default In-Memory emulation engine if SQLite is not compiled.
        SqlConnection conn("Data Source=InventoryDb;Engine=InMemory;");
        conn.Open();

        // 1. Create table schema
        auto cmdCreate = conn.CreateCommand();
        cmdCreate->SetCommandText("CREATE TABLE Items (Id INT, Name VARCHAR, Price INT)");
        cmdCreate->ExecuteNonQuery();

        // 2. Parameterized Insert (Recommended)
        auto cmdInsert = conn.CreateCommand();
        cmdInsert->SetCommandText("INSERT INTO Items (Id, Name, Price) VALUES (@id, @name, @price)");
        cmdInsert->GetParameters()->AddWithValue("@id", 101);
        cmdInsert->GetParameters()->AddWithValue("@name", "Bean Grinder");
        cmdInsert->GetParameters()->AddWithValue("@price", 75);
        cmdInsert->ExecuteNonQuery();

        // 3. Querying rows
        auto cmdSelect = conn.CreateCommand();
        cmdSelect->SetCommandText("SELECT Id, Name, Price FROM Items WHERE Price < @maxPrice");
        cmdSelect->GetParameters()->AddWithValue("@maxPrice", 100);

        auto reader = cmdSelect->ExecuteReader();
        while (reader->Read()) {
            int id = reader->GetInt32(0);
            String name = reader->GetString(1);
            int price = reader->GetInt32(2);

            Console::Write("Item: ");
            Console::Write(name);
            Console::Write(" costs $");
            Console::WriteLine(price);
        }

        conn.Close();
    } catch (const Exception& ex) {
        Console::Write("Database Error: ");
        Console::WriteLine(ex.What());
    }
}
```

## STL vs DotNetDupe Comparison ⚖️

DotNetDupe is designed to be more intuitive and less verbose than the standard C++ STL.

*   **[General Comparison Guide](https://sudheeshps.github.io/dotnetdupe/docs/Comparison.html)**: Covers Strings, Collections, Timing, etc.
*   **[Threading Comparison Guide](https://sudheeshps.github.io/dotnetdupe/docs/ThreadingComparison.html)**: Detailed comparison of thread synchronization primitives.
*   **[Process Management Comparison Guide](https://sudheeshps.github.io/dotnetdupe/docs/ProcessComparison.html)**: Comparison of process execution and management.
*   **[Authentication & Authorization Guide](https://sudheeshps.github.io/dotnetdupe/docs/Auth.html)**: Covers client-side and server-side JWT authentication and authorization.

### Sample Client and Test Code 🧪

The repository includes `DotNetDupeDemo` (a sample console application) and `DotNetDupeTests` (unit tests) projects. These projects demonstrate how to integrate and use the `DotNetDupe` library. You can refer to their `.vcxproj` files for examples of how to configure your own projects to consume the `DotNetDupe` NuGet package.

## API Reference 📖

For detailed information on the available classes, methods, and their usage, please refer to the comprehensive API documentation for each class:

### Namespace: `DotNetDupe::System`

**Core Primitives & Base Classes**

| Class | Description |
|---|---|
| [Object](docs/Object.md) | Supports all classes in the .NET class hierarchy and provides low-level services to derived classes. |
| [SmartPointer&lt;T&gt;](docs/SmartPointer.md) | Unified smart pointer supporting RAII unique and shared reference-counted ownership. |
| [Char](docs/Char.md) | Represents character code points and provides Unicode classification and transformation methods. |
| [String](docs/String.md) | Represents immutable sequences of UTF-8 and UTF-16 characters (`String` and `WString`). |
| [Array&lt;T&gt;](docs/Array.md) | Provides methods for creating, manipulating, searching, and sorting arrays. |
| [BitConverter](docs/BitConverter.md) | Converts base data types to arrays of bytes, and arrays of bytes to base data types. |
| [Buffer](docs/Buffer.md) | Manipulates arrays of primitive types efficiently. |
| [Console](docs/Console.md) | Reads and writes to standard I/O streams with full color control and stream redirection. |
| [Convert](docs/Convert.md) | Converts base data types and hexadecimal strings. |
| [DateTime](docs/DateTime.md) | Represents an instant in time, typically expressed as a date and time of day. |
| [DateTimeOffset](docs/DateTimeOffset.md) | Represents a point in time relative to UTC with time zone offset. |
| [TimeSpan](docs/TimeSpan.md) | Represents a time interval. |
| [TimeZone](docs/TimeZone.md) | Represents a time zone. |
| [TimeZoneInfo](docs/TimeZoneInfo.md) | Represents any time zone in the world with Daylight Saving adjustments. |
| [DaylightTime](docs/DaylightTime.md) | Defines the period of daylight saving time. |
| [TimeProvider](docs/TimeProvider.md) | Provides a testable abstraction for date and time. |
| [Guid](docs/Guid.md) | Represents a globally unique identifier (GUID). |
| [Environment](docs/Environment.md) | Provides environment variables, machine info, and platform properties. |
| [OperatingSystem](docs/OperatingSystem.md) | Represents operating system platform identifiers and version metadata. |
| [Random](docs/Random.md) | Represents a pseudo-random number generator. |
| [Uri](docs/Uri.md) | Provides an object representation of Uniform Resource Identifiers (URI). |
| [UriBuilder](docs/UriBuilder.md) | Provides convenient mutation of URI components. |
| [UriComponents](docs/UriComponents.md) | Specifies parts of a URI. |
| [UriFormat](docs/UriFormat.md) | Controls how URI information is escaped. |
| [UriParser](docs/UriParser.md) | Parses and validates URI schemes. |
| [GenericUriParser](docs/GenericUriParser.md) | Customizable parser for hierarchical URI schemes. |
| [Version](docs/Version.md) | Represents version numbers (`major.minor.build.revision`). |

**Core Interfaces**

| Interface | Description |
|---|---|
| [IDisposable](docs/Interfaces.md) | Defines a mechanism for deterministic release of unmanaged resources. |
| [IClonable](docs/Interfaces.md) | Defines mechanisms for deep or shallow object cloning. |
| [IComparable](docs/Interfaces.md) | Defines comparison method for sorting and ordering. |
| [IComparable&lt;T&gt;](docs/Interfaces.md) | Defines strongly-typed comparison method for sorting and ordering. |
| [IFormatProvider&lt;T&gt;](docs/Interfaces.md) | Provides custom type-formatting services. |
| [IServiceProvider](docs/DependencyInjection.md) | Defines service object resolution mechanism for dependency injection. |

**Exceptions**

| Exception | Description |
|---|---|
| [Exception](docs/Exceptions.md) | Root exception class for all DotNetDupe library errors. |
| [SystemException](docs/Exceptions.md) | Base class for system-level runtime exceptions. |
| [ArgumentException](docs/Exceptions.md) | Thrown when an argument passed to a method is invalid. |
| [ArgumentNullException](docs/Exceptions.md) | Thrown when a null argument is passed to a non-null parameter. |
| [ArgumentOutOfRangeException](docs/Exceptions.md) | Thrown when an argument falls outside allowable boundary limits. |
| [ArithmeticException](docs/Exceptions.md) | Thrown for errors in mathematical or arithmetic operations. |
| [FormatException](docs/Exceptions.md) | Thrown when string or argument formatting is invalid. |
| [NotImplementedException](docs/Exceptions.md) | Thrown when a requested method or feature is not implemented. |
| [OverflowException](docs/Exceptions.md) | Thrown on arithmetic or conversion overflow. |

---

### Namespace: `DotNetDupe::System::Collections::Generic`

**Classes**

| Class | Description |
|---|---|
| [List&lt;T&gt;](docs/List.md) | Strongly-typed dynamic array list accessible by index. |
| [Dictionary&lt;TKey, TValue&gt;](docs/Dictionary.md) | Key/value hash map collection. |
| [HashSet&lt;T&gt;](docs/HashSet.md) | Set of unique elements backed by a hash table. |
| [Queue&lt;T&gt;](docs/Queue.md) | First-In-First-Out (FIFO) queue collection. |
| [Stack&lt;T&gt;](docs/Stack.md) | Last-In-First-Out (LIFO) stack collection. |
| [PriorityQueue&lt;TElement, TPriority&gt;](docs/PriorityQueue.md) | Min-heap collection of prioritized items. |
| [SortedDictionary&lt;TKey, TValue&gt;](docs/SortedDictionary.md) | Key/value collection sorted by key. |
| [SortedSet&lt;T&gt;](docs/SortedSet.md) | Ordered unique collection maintained in sorted order. |
| [LinkedList&lt;T&gt;](docs/LinkedList.md) | Doubly-linked list collection. |
| [Generic Collections Overview](docs/GenericCollections.md) | Comprehensive guide and comparison of generic collection types. |

---

### Namespace: `DotNetDupe::System::Collections::Concurrent`

**Classes**

| Class | Description |
|---|---|
| [ConcurrentDictionary&lt;TKey, TValue&gt;](docs/ConcurrentDictionary.md) | Thread-safe key/value collection for concurrent multi-threaded access. |
| [ConcurrentQueue&lt;T&gt;](docs/ConcurrentQueue.md) | Lock-free thread-safe First-In-First-Out (FIFO) queue. |
| [ConcurrentStack&lt;T&gt;](docs/ConcurrentStack.md) | Lock-free thread-safe Last-In-First-Out (LIFO) stack. |
| [ConcurrentBag&lt;T&gt;](docs/ConcurrentBag.md) | Thread-safe unordered object container with thread-local storage. |
| [BlockingCollection&lt;T&gt;](docs/BlockingCollection.md) | Thread-safe collection providing blocking producer-consumer capabilities. |
| [Concurrent Collections Overview](docs/ConcurrentCollections.md) | Comprehensive guide and architecture of lock-free and thread-safe collections. |

---

### Namespace: `DotNetDupe::System::IO`

**Classes**

| Class | Description |
|---|---|
| [File](docs/File.md) | Static helper methods for file creation, reading, writing, moving, and deletion. |
| [Directory](docs/Directory.md) | Static helper methods for creating (including recursive creation), moving, deleting, and enumerating directories. |
| [Path](docs/Path.md) | Performs cross-platform directory and file path string operations. |
| [Stream](docs/Stream.md) | Abstract base class for byte sequence streams. |
| [FileStream](docs/FileStream.md) | Provides a byte stream for files supporting synchronous read/write. |
| [MemoryStream](docs/Stream.md) | Creates a stream whose backing store is memory. |
| [TextReader](docs/TextReader.md) | Abstract reader for sequential character input. |
| [TextWriter](docs/TextWriter.md) | Abstract writer for sequential character output. |
| [StringReader](docs/StringReader.md) | Implements `TextReader` reading from a `String`. |
| [StringWriter](docs/StringWriter.md) | Implements `TextWriter` writing characters into a string buffer. |

**Exceptions**

| Exception | Description |
|---|---|
| [IOException](docs/Exceptions.md) | Thrown when an I/O or file system error occurs. |
| [FileNotFoundException](docs/Exceptions.md) | Thrown when an attempt to access a file that does not exist on disk fails. |
| [DirectoryNotFoundException](docs/Exceptions.md) | Thrown when part of a file or directory path cannot be found. |
| [EndOfStreamException](docs/Exceptions.md) | Thrown when reading is attempted past the end of a stream. |

---

### Namespace: `DotNetDupe::System::Threading` & `System::Threading::Tasks`

**Classes**

| Class | Description |
|---|---|
| [Thread](docs/Thread.md) | Creates, configures, and controls OS threads. |
| [ThreadPool](docs/ThreadPool.md) | High-throughput worker thread pool managing parallel task execution. |
| [Task](docs/Task.md) | Represents asynchronous operations with continuation support. |
| [Task&lt;T&gt;](docs/Task.md) | Represents asynchronous operations returning a result value. |
| [WaitHandle](docs/WaitHandle.md) | Abstract base class for thread synchronization handles. |
| [EventWaitHandle](docs/EventWaitHandle.md) | Manages cross-thread and system synchronization event signals. |
| [AutoResetEvent](docs/AutoResetEvent.md) | Notifies waiting threads and automatically resets to non-signaled state. |
| [ManualResetEvent](docs/ManualResetEvent.md) | Notifies waiting threads and remains signaled until manually reset. |
| [Mutex](docs/Mutex.md) | Mutual exclusion synchronization primitive (supports named inter-process mutexes). |
| [Semaphore](docs/Semaphore.md) | Limits concurrent thread access to a bounded resource pool. |
| [SemaphoreSlim](docs/SemaphoreSlim.md) | Lightweight alternative to `Semaphore` avoiding kernel transitions for fast locking. |
| [CriticalSection](docs/CriticalSection.md) | Low-overhead recursive mutex primitive for intra-process synchronization. |
| [Interlocked](docs/Interlocked.md) | Provides atomic hardware operations (`Increment`, `Decrement`, `Exchange`, `CompareExchange`). |
| [Lock&lt;T&gt;](docs/Lock.md) | Exception-safe RAII lock wrapper for synchronization primitives. |

**Exceptions**

| Exception | Description |
|---|---|
| [ThreadStateException](docs/Exceptions.md) | Thrown when a thread is in an invalid state for the requested operation. |
| [ThreadInterruptedException](docs/Exceptions.md) | Thrown when a thread is interrupted while waiting. |
| [SynchronizationLockException](docs/Exceptions.md) | Thrown when unlocking a synchronization object not owned by the caller. |
| [AbandonedMutexException](docs/Exceptions.md) | Thrown when a thread acquires a mutex abandoned by another terminating thread. |
| [WaitHandleCannotBeOpenedException](docs/Exceptions.md) | Thrown when attempting to open a non-existent named system sync handle. |
| [SemaphoreFullException](docs/Exceptions.md) | Thrown when releasing a semaphore whose count is already at maximum capacity. |
| [TaskCanceledException](docs/Exceptions.md) | Thrown when a task execution is canceled. |

---

### Namespace: `DotNetDupe::System::Diagnostics`

**Classes**

| Class | Description |
|---|---|
| [Process](docs/Process.md) | Starts, manages, monitors, and redirects stdin/stdout/stderr for child processes. |
| [Stopwatch](docs/Stopwatch.md) | High-resolution performance timer for measuring elapsed time. |
| [EventLog](docs/EventLog.md) | Interacts with OS diagnostic event logs and writes operational entries. |
| [EtwLogReader](docs/EtwLogReader.md) | Queries Event Tracing for Windows (ETW) channels and Linux syslog files with live event subscription listening. |
| [SystemMetrics](docs/SystemMetrics.md) | Queries system hardware telemetry metrics including CPU %, Memory load, Disk %, Network Mbps, and top processes. |
| [RealtimeTelemetry](docs/RealtimeTelemetry.md) | High-frequency telemetry metrics streaming and real-time dashboard endpoint. |
| [ActiveUserSession](docs/ActiveUserSession.md) | Enumerates active and terminal user sessions across the system. |
| [TerminalSession](docs/TerminalSession.md) | Enumerates active, disconnected, and remote desktop (RDP) Terminal Services sessions. |

---

### Namespace: `DotNetDupe::System::Net`, `System::Net::Sockets` & `System::Net::Security`

**Classes**

| Class | Description |
|---|---|
| [Dns](docs/HttpClient.md) | Provides domain name resolution and IP address lookup. |
| [Socket](docs/Sockets.md) | Low-level cross-platform BSD/WinSock socket abstraction. |
| [NetworkStream](docs/Sockets.md) | Implements `Stream` backed by a network socket. |
| [TcpClient](docs/Sockets.md) | Client connection wrapper for TCP network services. |
| [TcpListener](docs/Sockets.md) | TCP listener for accepting incoming network connections. |
| [UdpClient](docs/Sockets.md) | User Datagram Protocol (UDP) client for datagram transmission. |
| [SslStream](docs/SslStream.md) | TLS/SSL secure stream wrapper built on OpenSSL. |

---

### Namespace: `DotNetDupe::System::Net::Http`

**Classes**

| Class | Description |
|---|---|
| [HttpClient](docs/HttpClient.md) | Sends HTTP/HTTPS requests and receives responses from URI endpoints. |
| [RestClient&lt;T&gt;](docs/RestClient.md) | Strongly-typed REST client with automated C++ structure JSON serialization/deserialization. |
| [FileDownloader](docs/FileDownloader.md) | High-level HTTP/HTTPS file downloader with pause/resume, speed metrics, and prompt resource disposal. |
| [HttpRequestMessage](docs/HttpClient.md) | Represents an outgoing HTTP request with headers, method, and payload. |
| [HttpResponseMessage](docs/HttpClient.md) | Represents an HTTP response with status code, response headers, and content stream. |
| [HttpContent](docs/HttpClient.md) | Base class for HTTP entity bodies and content headers. |
| [StringContent](docs/HttpClient.md) | HTTP content wrapper for text and JSON payloads. |
| [ByteArrayContent](docs/HttpClient.md) | HTTP content wrapper for raw byte arrays and binary payloads. |
| [HttpMethod](docs/HttpClient.md) | Represents standard HTTP request methods (GET, POST, PUT, DELETE, etc.). |

---

### Namespace: `DotNetDupe::System::Text` & `System::Text::Json`

**Classes**

| Class | Description |
|---|---|
| [StringBuilder](docs/StringBuilder.md) | Mutable string buffer for high-performance string concatenation. |
| [TextEncoding](docs/TextEncoding.md) | Represents character encodings (UTF-8, ASCII, UTF-16). |
| [JsonSerializer](docs/JsonSerializer.md) | Serializes objects to JSON strings and deserializes JSON to C++ types. |

---

### Namespace: `DotNetDupe::System::Utils`

**Classes**

| Class | Description |
|---|---|
| [StringConvert](docs/StringConvert.md) | Static helper methods for UTF-8 / UTF-16 conversions and type transformations. |

---

### Namespace: `DotNetDupe::System::Security`

**Classes**

| Class | Description |
|---|---|
| [UserPrincipal](docs/UserPrincipal.md) | Cross-platform user account enumeration, group memberships, and administrative privilege inspection. |
| [HMACSHA256](docs/JWTToken.md) | Computes SHA256 Hash-based Message Authentication Codes. |
| [X509Certificate2](docs/X509Certificate2.md) | Loads and inspects X.509 SSL/TLS certificates and private keys. |
| [JWTToken](docs/JWTToken.md) | Encodes, parses, and validates JSON Web Tokens with HMAC-SHA256 signature verification. |

---

### Namespace: `DotNetDupe::System::Data::SqlClient`

**Classes**

| Class | Description |
|---|---|
| [SqlConnection](docs/Database.md) | Represents an open connection to a database (supports In-Memory emulation and SQLite persistence). |
| [SqlCommand](docs/Database.md) | Represents SQL statements and queries to execute against a database. |
| [SqlDataReader](docs/Database.md) | Forward-only cursor for reading result rows from SQL queries. |
| [SqlParameter](docs/Database.md) | Parameter for parameterized SQL commands protecting against SQL injection. |

---

### Namespace: `DotNetDupe::Extensions::DependencyInjection`

**Classes & Interfaces**

| Type | Description |
|---|---|
| [ServiceCollection](docs/DependencyInjection.md) | Accumulates service descriptors with Transient, Scoped, and Singleton lifetimes. |
| [ServiceProvider](docs/DependencyInjection.md) | Dependency injection container resolving registered service dependencies. |
| [ServiceScope](docs/DependencyInjection.md) | Represents a lifetime scope for resolving scoped service instances. |
| [ServiceScopeFactory](docs/DependencyInjection.md) | Factory for creating scoped service containers. |
| [IServiceCollection](docs/DependencyInjection.md) | Contract for service collection builders. |
| [IServiceScope](docs/DependencyInjection.md) | Contract for lifetime scopes. |
| [IServiceScopeFactory](docs/DependencyInjection.md) | Contract for service scope factories. |

---

### Namespace: `DotNetDupe::Extensions::Logging`

**Classes & Interfaces**

| Type | Description |
|---|---|
| [LogManager](docs/LogManager.md) | Global thread-safe static factory and cache for category loggers and file/console providers. |
| [LoggerTextWriter](docs/LoggerTextWriter.md) | High-performance stream redirector bridging `TextWriter` output into `LogManager`. |
| [LoggerFactory](docs/Logging.md) | Configures providers and generates category loggers. |
| [Logger&lt;T&gt;](docs/Logging.md) | Generic category logger for class-specific logging. |
| [ConsoleLoggerProvider](docs/Logging.md) | Renders structured console log records (Plain text and JSON format). |
| [FileLoggerProvider](docs/Logging.md) | Thread-safe file logging provider with auto directory creation and relative path resolution. |
| [ILogger](docs/Logging.md) | Core interface for emitting structured diagnostic log events. |
| [ILoggerOf&lt;T&gt;](docs/Logging.md) | Generic category logger interface. |
| [ILoggerProvider](docs/Logging.md) | Provider factory interface for creating loggers. |
| [ILoggerFactory](docs/Logging.md) | Logging factory interface. |

---

### Namespace: `DotNetDupe::WebAppCore`

**Builder, Controllers & Server**

| Class | Description |
|---|---|
| [WebApplicationBuilder](docs/WebApplication.md) | Configures services, dependency injection, and builds the `WebApplication` host. |
| [WebApplication](docs/WebApplication.md) | Configures routing endpoints and executes the HTTP server listener. |
| [WebAppServer](docs/WebAppServer.md) | Web server hosting static website content (`index.html`, CSS, JS) and REST APIs simultaneously. |
| [ControllerBase](docs/ControllerBase.md) | Base class for ASP.NET MVC / Web API style controllers (`Ok`, `Created`, `NotFound`, `BadRequest`). |
| [ControllerRouteBuilder&lt;T&gt;](docs/WebApplication.md) | Maps controller actions and automates JSON payload serialization and deserialization. |
| [HttpContext](docs/HttpContext.md) | Encapsulates HTTP request and response context for individual HTTP transactions. |
| [HttpRequest](docs/HttpContext.md) | Represents incoming HTTP request headers, query parameters, and body. |
| [HttpResponse](docs/HttpContext.md) | Represents outgoing HTTP response status codes, headers, and body. |
| [Push Notifications (SSE & WebSockets)](docs/PushNotifications.md) | Real-time push notifications via Server-Sent Events (SSE) and full-duplex WebSockets. |

---

## Project Status 🚧

DotNetDupe is currently **under active development**. I am continuously working on expanding the API coverage and improving stability.

## Contributions 👋

Contributions to the DotNetDupe project are highly welcome! Whether it's bug reports, feature requests, code contributions, or documentation improvements, your help is invaluable. Please refer to [GitHub's general contributing guidelines](https://docs.github.com/en/get-started/exploring-projects-on-github/contributing-to-a-project) for more information on how to get started.

## CI/CD Pipeline 🚀

This repository uses GitHub Actions to automate the build, test, and release process.

### Workflow Details
- **Build & Test Matrix**: Every push to `main` and all pull requests trigger a full build (Debug & Release) across Windows (MSBuild) and Linux (CMake/GCC), running all 660 Google Tests.
- **GitHub CodeQL Analysis**: Continuous semantic security vulnerability scanning on every push, pull request, and weekly schedule via `github/codeql-action`.
- **Quality Gates & Static Analysis**: Automated enforcement of 11 Quality Gate metrics (LLOC &le; 15, CCN &le; 10, Nesting depth &le; 4, Hungarian SmartPointers, zero empty catch blocks, zero standard `throw std::*` exceptions, zero raw pointer ownership).
- **Code Coverage**: Automated code coverage measurement via OpenCppCoverage (> 80% line coverage required) and static analysis HTML report generation.
- **NuGet Release**: Pushing a tag (e.g., `v1.0.0`) triggers the creation and publishing of the NuGet package to [nuget.org](https://www.nuget.org/).

### Code Coverage & Static Analysis 📊
- **Online Reports:**
  - 📈 [OpenCppCoverage HTML Report](https://sudheeshps.github.io/dotnetdupe/CodeCoverage/index.html)
  - 🔍 [Static Analysis & Quality Gate Report](https://sudheeshps.github.io/dotnetdupe/CodeCoverage/StaticAnalysis.html)
- **Live Documentation Portal:** [https://sudheeshps.github.io/dotnetdupe/](https://sudheeshps.github.io/dotnetdupe/)
- **Local Generation:** Run `powershell .\scripts\Generate-CoverageReport.ps1` to execute tests under OpenCppCoverage and refresh the `CodeCoverage/` folder.

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

This project is a unique blend of manual craftsmanship and AI-powered development, built collaboratively by myself and my peer Antigravity. While the initial core components were meticulously crafted by hand, the remaining code, including many classes, methods, and their corresponding unit tests, were developed and expanded with Antigravity. This project stands as a testimonial on how persistent human effort—spanning weekends and late evenings—combined with generative AI can build an entire, robust system from scratch.

## Contact 📧

For questions or support, please open an issue on the [GitHub repository](https://github.com/sudheeshps/DotNetDupe/issues) or contact [sudheeshps@gmail.com](mailto:sudheeshps@gmail.com).