# DotNetDupe 🚀

<!-- Dynamic Repository Badges -->
[![Build Status](https://img.shields.io/github/actions/workflow/status/sudheeshps/DotNetDupe/build-and-release.yml?branch=main&style=flat-square&logo=github)](https://github.com/sudheeshps/DotNetDupe/actions)
[![Latest Tag](https://img.shields.io/github/v/tag/sudheeshps/DotNetDupe?include_prereleases=true&style=flat-square&logo=git&color=blue)](https://github.com/sudheeshps/DotNetDupe/tags)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17%2F20-blue?style=flat-square&logo=cplusplus)](https://en.cppreference.com/w/cpp/20)
[![Platforms](https://img.shields.io/badge/Platforms-Windows%20%7C%20Linux-lightgrey?style=flat-square&logo=linux)](https://github.com/sudheeshps/DotNetDupe#cross-platform-support-)
[![Code Coverage](https://img.shields.io/badge/Coverage-82.2%25-brightgreen?style=flat-square&logo=codecov)](https://github.com/sudheeshps/DotNetDupe/actions)
[![License](https://img.shields.io/github/license/sudheeshps/DotNetDupe?style=flat-square&color=orange)](LICENSE)

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
    Run the automated build script from PowerShell:
    ```powershell
    .\BuildAndPack.ps1
    ```
    This script will update the resource build timestamp, compile the x64 and x86 Release binaries, and output the NuGet package (`DotNetDupe.3.0.0.nupkg`) into the `nuget_packages` directory.

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
This updates the build timestamp, compiles both x64 and x86 Release binaries, and outputs `DotNetDupe.3.0.0.nupkg` inside the `nuget_packages/` directory.

#### B. Consuming NuGet Package in Visual Studio (Windows)
1. Add the local `nuget_packages` folder as a NuGet Package Source:
   ```bash
   nuget sources Add -Name "DotNetDupeLocal" -Source "D:\Personal\Projects\C++\DotNetDupe\nuget_packages"
   ```
2. In Visual Studio, right-click your project -> **Manage NuGet Packages** -> Select `DotNetDupeLocal` -> Install `DotNetDupe`.

#### C. Consuming NuGet Package on Linux / CMake (WSL)
1. Extract `DotNetDupe.3.0.0.nupkg` (ZIP format) to a local directory:
   ```powershell
   Expand-Archive -Path "nuget_packages\DotNetDupe.3.0.0.nupkg" -DestinationPath "DotNetDupe_NuGet" -Force
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
#include "WebAppCore/Builder/WebApplication.h"
#include "WebAppCore/Builder/WebApplicationBuilder.h"
#include "WebAppCore/Server/WebAppServer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::WebAppCore::Builder;
using namespace DotNetDupe::WebAppCore::Server;
using namespace DotNetDupe::WebAppCore::Http;

int main() {
    Console::WriteLine("=============================================");
    Console::WriteLine(" Starting DotNetDupe Cross-Platform Web Server");
    Console::WriteLine("=============================================");

    String webRoot = "wwwroot";

    // 1. Initialize WebApplication Host
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
    } catch (const BasicException<char>& ex) {
        Console::Write("Database Error: ");
        Console::WriteLine(ex.What());
    }
}
```

## STL vs DotNetDupe Comparison ⚖️

DotNetDupe is designed to be more intuitive and less verbose than the standard C++ STL.

*   **[General Comparison Guide](docs/Comparison.md)**: Covers Strings, Collections, Timing, etc.
*   **[Threading Comparison Guide](docs/ThreadingComparison.md)**: Detailed comparison of thread synchronization primitives.
*   **[Process Management Comparison Guide](docs/ProcessComparison.md)**: Comparison of process execution and management.
*   **[Authentication & Authorization Guide](docs/Auth.md)**: Covers client-side and server-side JWT authentication and authorization.

### Sample Client and Test Code 🧪

The repository includes `DotNetDupeDemo` (a sample console application) and `DotNetDupeTests` (unit tests) projects. These projects demonstrate how to integrate and use the `DotNetDupe` library. You can refer to their `.vcxproj` files for examples of how to configure your own projects to consume the `DotNetDupe` NuGet package.




## API Reference 📖

For detailed information on the available classes, methods, and their usage, please refer to the comprehensive API documentation for each class.

### Namespace: `DotNetDupe::WebAppCore::Builder`

**Classes**

| Class | Description |
|---|---|
| [WebApplicationBuilder](docs/WebApplication.md) | Configures services and builds the web application host. |
| [WebApplication](docs/WebApplication.md) | Configures routing endpoints and hosts the HTTP listener. |
| [WebAppServer](docs/WebAppServer.md) | Web server hosting static website content (HTML, CSS, images) and REST APIs. |

### Namespace: `DotNetDupe::WebAppCore::Http`

**Classes**

| Class | Description |
|---|---|
| [HttpContext](docs/WebApplication.md) | Encapsulates all HTTP-specific information about an individual HTTP request. |
| [HttpRequest](docs/WebApplication.md) | Represents the incoming HTTP request. |
| [HttpResponse](docs/WebApplication.md) | Represents the outgoing HTTP response. |
| [Push Notifications (SSE & WebSockets)](docs/PushNotifications.md) | Server-Sent Events (SSE) streaming and WebSocket push notification support. |

### Namespace: `DotNetDupe::WebAppCore::Controllers`

**Classes**

| Class | Description |
|---|---|
| [ControllerBase](docs/WebApplication.md) | Base class for MVC/Web API style controllers. |
| [ControllerRouteBuilder<T>](docs/WebApplication.md) | Maps controller actions and automates JSON payload serialization. |

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
| [IServiceProvider](docs/DependencyInjection.md) | Defines a mechanism for retrieving a service object (type-safe dependency resolution). |

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
| [EventLog](docs/EventLog.md) | Provides interaction with system event logs and writing diagnostic entries. |
| [EtwLogReader](docs/EtwLogReader.md) | Provides Event Tracing for Windows (ETW) and Linux Syslog channel enumeration, reading, and subscription listening. |
| [SystemMetrics](docs/RealtimeTelemetry.md) | Queries system hardware telemetry metrics including CPU %, Memory load, Disk %, Network Mbps, and top processes. |
| [ActiveUserSession](docs/RealtimeTelemetry.md) | Enumerates active and terminal user sessions across the system. |
| [TerminalSession](docs/TerminalSession.md) | Enumerates active, disconnected, and remote desktop (RDP) Terminal Services sessions. |
| [Process](docs/Process.md) | Provides access to local processes and enables you to start and stop local system processes. |
| [Stopwatch](docs/Stopwatch.md) | Provides a set of methods and properties that you can use to accurately measure elapsed time. |

### Namespace: `DotNetDupe::System::Text`

**Classes**

| Class | Description |
|---|---|
| [StringBuilder](docs/StringBuilder.md) | Represents a mutable string of characters. |
| [TextEncoding](docs/TextEncoding.md) | Represents a character encoding. |

### Namespace: `DotNetDupe::System::Utils`

**Classes**

| Class | Description |
|---|---|
| [StringConvert](docs/StringConvert.md) | Provides static utility methods for converting strings between UTF-8 and UTF-16 representations. |

### Namespace: `DotNetDupe::System::Collections::Generic`

**Classes**

| Class | Description |
|---|---|
| [List](docs/GenericCollections.md) | Represents a strongly typed list of objects that can be accessed by index. |
| [Dictionary](docs/GenericCollections.md) | Represents a collection of keys and values. |
| [Queue](docs/GenericCollections.md) | Represents a First-In-First-Out (FIFO) collection of instances. |
| [Stack](docs/GenericCollections.md) | Represents a Last-In-First-Out (LIFO) collection of instances. |
| [HashSet](docs/GenericCollections.md) | Represents a set of unique values using a hash table. |
| [SortedDictionary](docs/GenericCollections.md) | Represents a collection of key/value pairs that are sorted on the key. |
| [SortedSet](docs/GenericCollections.md) | Represents a collection of objects that is maintained in sorted order. |
| [LinkedList](docs/GenericCollections.md) | Represents a doubly linked list. |
| [PriorityQueue](docs/GenericCollections.md) | Represents a collection of items that have a value and a priority. |

### Namespace: `DotNetDupe::System::Collections::Concurrent`

**Classes**

| Class | Description |
|---|---|
| [ConcurrentDictionary](docs/ConcurrentCollections.md) | Thread-safe key-value collection for concurrent operations. |
| [ConcurrentQueue](docs/ConcurrentCollections.md) | Thread-safe First-In-First-Out (FIFO) collection. |
| [ConcurrentStack](docs/ConcurrentCollections.md) | Thread-safe Last-In-First-Out (LIFO) collection. |
| [ConcurrentBag](docs/ConcurrentCollections.md) | Thread-safe unordered collection of objects. |
| [BlockingCollection](docs/ConcurrentCollections.md) | Thread-safe collection for blocking producer-consumer workflows. |

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

### Namespace: `DotNetDupe::System::Net`

**Classes**

| Class | Description |
|---|---|
| [Dns](docs/HttpClient.md) | Provides simple domain name resolution functionality. |

### Namespace: `DotNetDupe::System::Net::Sockets`

**Classes**

| Class | Description |
|---|---|
| [Socket](docs/Sockets.md) | A wrapper around standard native socket APIs. |
| [NetworkStream](docs/Sockets.md) | Provides the underlying stream of data for network access. |
| [TcpClient](docs/Sockets.md) | Provides client connections for TCP network services. |
| [TcpListener](docs/Sockets.md) | Listens for connections from TCP network clients. |
| [UdpClient](docs/Sockets.md) | Provides UDP datagram sockets. |

### Namespace: `DotNetDupe::System::Net::Security`

**Classes**

| Class | Description |
|---|---|
| [SslStream](docs/SslStream.md) | Provides a stream that uses the SSL/TLS security protocol to secure communication. |

### Namespace: `DotNetDupe::System::Net::Http`

**Classes**

| Class | Description |
|---|---|
| [HttpClient](docs/HttpClient.md) | Sends HTTP requests and receives HTTP responses from a resource identified by a URI. |
| [RestClient](docs/RestClient.md) | Represents a strongly-typed REST client that simplifies resource-oriented Web API interactions by automatically serializing/deserializing payloads to and from C++ structures. |
| [HttpRequestMessage](docs/HttpClient.md) | Represents an HTTP request message. |
| [HttpResponseMessage](docs/HttpClient.md) | Represents an HTTP response message including the status code and data. |
| [HttpContent](docs/HttpClient.md) | Abstract class representing an HTTP entity body and content headers. |
| [StringContent](docs/HttpClient.md) | Provides HTTP content based on a string. |
| [ByteArrayContent](docs/HttpClient.md) | Provides HTTP content based on a byte array. |
| [HttpMethod](docs/HttpClient.md) | Represents an HTTP method. |

### Namespace: `DotNetDupe::System::Security::Principal`

**Classes**

| Class | Description |
|---|---|
| [UserPrincipal](docs/UserPrincipal.md) | Enumerates system user accounts (Windows/Linux) and inspects user classification, groups, and security permissions. |

### Namespace: `DotNetDupe::System::Security::Cryptography`

**Classes**

| Class | Description |
|---|---|
| [HMACSHA256](docs/JWTToken.md) | Computes a Hash-based Message Authentication Code (HMAC) using the SHA256 hash function. |

### Namespace: `DotNetDupe::System::Security::Cryptography::X509Certificates`

**Classes**

| Class | Description |
|---|---|
| [X509Certificate2](docs/X509Certificate2.md) | Represents an X.509 certificate and private key loaded from PEM files. |

### Namespace: `DotNetDupe::System::IdentityModel::Tokens::Jwt`

**Classes**

| Class | Description |
|---|---|
| [JWTToken](docs/JWTToken.md) | Represents a JSON Web Token containing a header block, claims, and signature. |

### Namespace: `DotNetDupe::System::Data::SqlClient`

**Classes**

| Class | Description |
|---|---|
| [SqlConnection](docs/Database.md) | Represents an open connection to a database. |
| [SqlCommand](docs/Database.md) | Represents an SQL statement to execute against a database. |
| [SqlDataReader](docs/Database.md) | Provides a way of reading a forward-only stream of rows from a database. |
| [SqlParameter](docs/Database.md) | Represents a parameter for an SQL command. |

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

### Namespace: `DotNetDupe::Extensions::DependencyInjection`

**Classes**

| Class | Description |
|---|---|
| [ServiceCollection](docs/DependencyInjection.md) | Accumulates service descriptors and builds the service provider. |
| [ServiceProvider](docs/DependencyInjection.md) | The concrete implementation of the dependency injection container. |
| [ServiceScope](docs/DependencyInjection.md) | Represents a lifetime scope for scoped services. |
| [ServiceScopeFactory](docs/DependencyInjection.md) | Creates instances of `ServiceScope`, which are used to resolve scoped dependencies. |

**Interfaces**

| Interface | Description |
|---|---|
| [IServiceCollection](docs/DependencyInjection.md) | A collection of service descriptors. |
| [IServiceScope](docs/DependencyInjection.md) | Represents the lifetime of a scoped service. |
| [IServiceScopeFactory](docs/DependencyInjection.md) | Defines a factory for creating service scopes. |

### Namespace: `DotNetDupe::Extensions::Logging`

**Classes**

| Class | Description |
|---|---|
| [LoggerFactory](docs/Logging.md) | The concrete implementation of the logging factory. |
| [Logger<T>](docs/Logging.md) | A generic template wrapper for class-specific category logging. |
| [ConsoleLoggerProvider](docs/Logging.md) | Renders structured console outputs (Plain or JSON format). |
| [FileLoggerProvider](docs/Logging.md) | Writes structured log records thread-safely to a file. |

**Interfaces**

| Interface | Description |
|---|---|
| [ILogger](docs/Logging.md) | Defines the entry interface for logging messages. |
| [ILoggerOf<T>](docs/Logging.md) | Represents a template logging category interface. |
| [ILoggerProvider](docs/Logging.md) | Defines a provider factory for creating loggers. |
| [ILoggerFactory](docs/Logging.md) | Manages logging providers and creates category loggers. |

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