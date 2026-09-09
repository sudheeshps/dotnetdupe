# DotNetDupe Documentation Revamp & Doxygen API Architecture Plan

Revamp the documentation ecosystem for **DotNetDupe** by introducing comprehensive Doxygen-compatible API comments across the library's classes, documenting internal algorithms and execution logic, establishing an automated Doxygen HTML generation pipeline targeting `docs/html/`, building a modern documentation portal at `docs/index.html`, and integrating API documentation into the main `README.md`.

---

## User Review Required

> [!IMPORTANT]
> **Codebase Scale & Phased Execution**: DotNetDupe contains 193 header files in `Include/` and 106 implementation files in `DotNetDupe/` covering over 1,500 methods and 100+ classes. To maintain absolute Quality Gate compliance (11 rules: $\le 15$ LLOC per function, $\le 500$ LLOC per class, $\le 600$ LLOC per file, $\text{CCN} \le 10$, Hungarian naming, etc.), changes are sequenced into structured phases so each tier remains cleanly compilable and verified.

> [!NOTE]
> **Doxygen 1.18.0** is already installed and verified at `C:\Program Files\doxygen\bin\doxygen.exe`. Graphviz `dot` is not present in `PATH`, so Doxygen will be configured to generate clean HTML documentation without requiring Graphviz dot diagrams, or with graceful fallback.

---

## Proposed Architecture & Design

### 1. Doxygen Comment Standards

All header files will adopt clean Javadoc/Qt-style Doxygen comments:
- **File-level comment**:
  ```cpp
  /**
   * @file String.h
   * @brief High-performance UTF-8 / UTF-16 string manipulation class mirroring .NET System.String.
   * @namespace DotNetDupe::System
   */
  ```
- **Class-level comment**:
  ```cpp
  /**
   * @class String
   * @brief Represents text as a sequence of UTF-8 code units with culture-invariant operations.
   * 
   * Provides .NET-compliant string operations including formatting, splitting, trimming,
   * searching, case transformations, and cross-encoding transcoding.
   * 
   * @note Implemented using the Pimpl idiom to preserve ABI stability across library boundaries.
   * @see DotNetDupe::System::StringBuilder
   */
  ```
- **Method-level comment**:
  ```cpp
  /**
   * @brief Splits a string into substrings based on specified delimiter characters and options.
   * @param arrSeparators Array of separator characters.
   * @param options Options specifying whether to trim entries or discard empty substrings.
   * @return Array of substrings parsed from the source string.
   * @throws ArgumentNullException If separator array is empty.
   */
  ```

### 2. Algorithmic Guiding Comments in Implementations

In `.cpp` files and template header implementations, inline guiding comments will detail the underlying logic, invariants, and algorithmic complexity:
- **`String`**: Multi-delimiter scan algorithm, Boyer-Moore / Knuth-Morris-Pratt substring search principles, UTF-8 state machine transcoding.
- **`SmartPointer<T>`**: Lock-free atomic reference counting control block semantics (`fetch_add`/`fetch_sub`), strong vs weak reference lifecycle, zero-allocation aliasing.
- **`PriorityQueue<TElement, TPriority>`**: Binary min-heap invariants, sift-up ($O(\log N)$) and sift-down ($O(\log N)$) array restructuring.
- **`ConcurrentQueue<T>` / `ConcurrentStack<T>`**: Michael-Scott lock-free queue, ABA prevention, lock-free atomic `compare_exchange_weak` (CAS) retry loops.
- **`Process` & `ProcessStreamer`**: Win32 Toolhelp32 snapshot enumeration, dual-buffer progressive telemetry streaming, token-based cancellation.
- **`DateTime` & `TimeZoneInfo`**: Astronomical Gregorian calendar epoch calculations, proleptic tick conversions, Daylight Saving Time adjustment rule resolution.

> [!TIP]
> DotNetDupe Quality Gate Check 1 and Checks 7–10 in `Check-QualityGates.ps1` explicitly skip comment lines starting with `//`, `/*`, or `*`. Therefore, comprehensive algorithmic and Doxygen comments will not increase LLOC or CCN metrics.

### 3. Documentation Portal (`docs/index.html`)

A modern documentation portal will be built at `docs/index.html` featuring:
- **Visual Design**: Sleek dark/light theme, modern typography (Inter), glassmorphism cards, responsive layout.
- **Master Directory**: Interactive namespace grid grouping all classes (`System`, `System::IO`, `System::Collections::Generic`, `System::Collections::Concurrent`, `System::Threading`, `System::Net`, `System::Diagnostics`, `System::Data`, `WebAppCore`).
- **One-Click Links**:
  - Direct entry point to Doxygen API Reference at `html/index.html`, `html/annotated.html`, and `html/classes.html`.
  - Links to all 107 conceptual topic guides (e.g. `Comparison.html`, `ThreadingComparison.html`, `Auth.html`).
- **Live Search & Filter**: Real-time JavaScript search to locate any class, method, or topic guide instantly.
- **Feature Matrix & Quick Start**: Side-by-side C# vs DotNetDupe code comparison.

### 4. Doxygen Output Directory (`docs/html/`)

- Configured in `Doxyfile`:
  ```ini
  PROJECT_NAME           = "DotNetDupe"
  PROJECT_BRIEF          = "C++17/20 Implementation of the .NET Base Class Library"
  OUTPUT_DIRECTORY       = docs
  HTML_OUTPUT            = html
  GENERATE_HTML          = YES
  GENERATE_LATEX         = NO
  EXTRACT_ALL            = YES
  EXTRACT_PRIVATE        = NO
  EXTRACT_STATIC         = YES
  JAVADOC_AUTOBRIEF      = YES
  RECURSIVE              = YES
  INPUT                  = Include docs DotNetDupe
  FILE_PATTERNS          = *.h *.hpp *.cpp *.md
  USE_MDFILE_AS_MAINPAGE = docs/index.md (or custom mainpage)
  HTML_COLORSTYLE_HUE    = 220
  HTML_COLORSTYLE_SAT    = 100
  HTML_COLORSTYLE_GAMMA  = 80
  GENERATE_TREEVIEW      = YES
  DISABLE_INDEX          = NO
  SEARCHENGINE           = YES
  ```

### 5. Documentation Generation Script (`scripts/Generate-Docs.ps1`)

Automates:
1. Detecting or locating `doxygen.exe`.
2. Validating `Doxyfile` configuration.
3. Generating Doxygen HTML into `docs/html/`.
4. Validating output links and verifying that `docs/index.html` connects seamlessly to `docs/html/index.html`.

---

## Proposed Changes

### Phase 1: Tooling, Infrastructure & Documentation Portal

#### [NEW] [Doxyfile](file:///d:/Personal/Projects/C++/dotnetdupe/Doxyfile)
- Complete Doxygen configuration tuned for C++17, DotNetDupe namespaces, markdown guides integration, and output to `docs/html/`.

#### [NEW] [Generate-Docs.ps1](file:///d:/Personal/Projects/C++/dotnetdupe/scripts/Generate-Docs.ps1)
- PowerShell automation script to run Doxygen, check warnings, and generate documentation.

#### [NEW] [index.html](file:///d:/Personal/Projects/C++/dotnetdupe/docs/index.html)
- Modern documentation landing portal linking to `html/index.html` (API Reference) and all guides in `docs/`.

#### [MODIFY] [README.md](file:///d:/Personal/Projects/C++/dotnetdupe/README.md)
- Add documentation badge linking to `docs/index.html` / `docs/html/index.html`.
- Add section explaining how to browse and build the API documentation.
- Update API Reference table headers with links to generated HTML classes.

---

### Phase 2: Core Primitives, Memory, Delegates & Exceptions

Add Doxygen comments and algorithmic guiding notes to foundational classes:
- [Include/System/Object.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Object.h) & [DotNetDupe/Object.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Object.cpp)
- [Include/System/SmartPointer.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/SmartPointer.h)
- [Include/System/String.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/String.h) & [DotNetDupe/String.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/String.cpp)
- [Include/System/Char.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Char.h) & [DotNetDupe/Char.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Char.cpp)
- [Include/System/Array.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Array.h)
- [Include/System/Buffer.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Buffer.h) & [DotNetDupe/Buffer.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Buffer.cpp)
- [Include/System/BitConverter.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/BitConverter.h) & [DotNetDupe/BitConverter.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/BitConverter.cpp)
- [Include/System/Console.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Console.h) & [DotNetDupe/Console.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Console.cpp)
- [Include/System/Convert.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Convert.h) & [DotNetDupe/Convert.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Convert.cpp)
- [Include/System/DateTime.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/DateTime.h) & [DotNetDupe/DateTime.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/DateTime.cpp)
- [Include/System/DateTimeOffset.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/DateTimeOffset.h) & [DotNetDupe/DateTimeOffset.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/DateTimeOffset.cpp)
- [Include/System/TimeSpan.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/TimeSpan.h) & [DotNetDupe/TimeSpan.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/TimeSpan.cpp)
- [Include/System/TimeZoneInfo.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/TimeZoneInfo.h) & [DotNetDupe/TimeZoneInfo.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/TimeZoneInfo.cpp)
- [Include/System/Guid.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Guid.h) & [DotNetDupe/Guid.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Guid.cpp)
- [Include/System/Environment.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Environment.h) & [DotNetDupe/Environment.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Environment.cpp)
- [Include/System/OperatingSystem.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/OperatingSystem.h) & [DotNetDupe/OperatingSystem.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/OperatingSystem.cpp)
- [Include/System/Random.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Random.h) & [DotNetDupe/Random.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Random.cpp)
- [Include/System/Version.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Version.h) & [DotNetDupe/Version.cpp](file:///d:/Personal/Projects/C++/dotnetdupe/DotNetDupe/Version.cpp)
- [Include/System/EventHandler.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/EventHandler.h) & [Include/System/EventArgs.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/EventArgs.h)
- [Include/System/Action.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Action.h), [Include/System/Func.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Func.h), [Include/System/Predicate.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Predicate.h)
- Custom Exceptions in [Include/System/Exception.h](file:///d:/Personal/Projects/C++/dotnetdupe/Include/System/Exception.h), `ArgumentException.h`, `ArgumentNullException.h`, `ArgumentOutOfRangeException.h`, `InvalidOperationException.h`, `FormatException.h`, `OverflowException.h`, `NotImplementedException.h`, etc.

---

### Phase 3: Generic & Concurrent Collections

- Generic Collections:
  - `List.h`, `Dictionary.h`, `HashSet.h`, `Queue.h`, `Stack.h`, `PriorityQueue.h`, `SortedDictionary.h`, `SortedSet.h`, `LinkedList.h`
- Concurrent Collections:
  - `ConcurrentDictionary.h`, `ConcurrentQueue.h`, `ConcurrentStack.h`, `ConcurrentBag.h`, `BlockingCollection.h`

---

### Phase 4: I/O, File System, Text, and Networking

- File System & Streams:
  - `File.h`, `Directory.h`, `Path.h`, `Stream.h`, `FileStream.h`, `MemoryStream.h`, `BinaryReader.h`, `BinaryWriter.h`, `TextReader.h`, `TextWriter.h`, `StreamReader.h`, `StreamWriter.h`, `StringReader.h`, `StringWriter.h`
- Text Encoding:
  - `Encoding.h`, `UTF8Encoding.h`, `ASCIIEncoding.h`, `StringBuilder.h`
- Networking & HTTP:
  - `Socket.h`, `TcpClient.h`, `TcpListener.h`, `UdpClient.h`, `IPAddress.h`, `IPEndPoint.h`, `Dns.h`, `NetworkStream.h`
  - `HttpClient.h`, `HttpRequestMessage.h`, `HttpResponseMessage.h`, `HttpContent.h`, `StringContent.h`, `ByteArrayContent.h`, `RestClient.h`

---

### Phase 5: Threading, Diagnostics, Logging, Database & WebAppCore

- Threading Primitives:
  - `Thread.h`, `ThreadPool.h`, `Task.h`, `Monitor.h`, `Mutex.h`, `Semaphore.h`, `SemaphoreSlim.h`, `AutoResetEvent.h`, `ManualResetEvent.h`, `Interlocked.h`, `Lock.h`
- Diagnostics:
  - `Process.h`, `ProcessStreamer.h`, `Stopwatch.h`, `EventLog.h`, `EtwLogReader.h`, `SystemMetrics.h`, `TerminalSession.h`, `ActiveUserSession.h`
- Database:
  - `SqlConnection.h`, `SqlCommand.h`, `SqlDataReader.h`, `SqlParameter.h`
- WebAppCore:
  - `WebApplication.h`, `WebAppServer.h`, `HttpContext.h`, `HttpRequest.h`, `HttpResponse.h`, `ControllerBase.h`, `ServiceCollection.h`, `ServiceProvider.h`, `JsonSerializer.h`, `JwtTokenHandler.h`

---

### Phase 6: Build, Documentation Generation & Validation

- Execute `scripts/Generate-Docs.ps1` to produce Doxygen API documentation under `docs/html/`.
- Validate that `docs/index.html` loads and links correctly to `docs/html/index.html`.
- Run `scripts/Check-QualityGates.ps1` to confirm 100% Quality Gate compliance (0 errors, 0 warnings).
- Run full solution build via `msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64`.
- Execute test runner `bin/x64/Release/DotNetDupeTests.exe` to verify 100% test pass rate.

---

## Verification Plan

### Automated Checks
1. **Documentation Generation**:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\scripts\Generate-Docs.ps1
   ```
   *Expected*: `docs/html/index.html` generated with zero fatal Doxygen errors.
2. **Quality Gate Static Analysis**:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\scripts\Check-QualityGates.ps1 -RootDir . -OutputDir CodeCoverage
   ```
   *Expected*: 0 Errors, 100% compliance across all 11 Quality Gates.
3. **Build & Test Verification**:
   ```powershell
   msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64
   .\bin\x64\Release\DotNetDupeTests.exe
   ```
   *Expected*: Zero build errors/warnings, 100% tests passing.

### Visual & Interactive Inspection
- Inspect `docs/index.html` in browser to verify modern responsive aesthetics, dark/light theme, search functionality, namespace browsing, and seamless navigation into `docs/html/index.html`.
- Inspect `docs/html/index.html` generated by Doxygen to confirm proper formatting of class descriptions, parameters, returns, and cross-references.
