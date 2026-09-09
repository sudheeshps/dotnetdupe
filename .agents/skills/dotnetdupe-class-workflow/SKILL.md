---
name: dotnetdupe-class-workflow
description: Step-by-step workflow for implementing, extending, or modifying DotNetDupe classes following the 10-step checklist in GEMINI.md.
---

# DotNetDupe Class Generation & Modification Workflow Skill

## Overview
This skill provides the mandatory 10-step sequence for creating new classes or modifying existing classes in DotNetDupe, in accordance with `GEMINI.md`.

## The 10-Step Class Workflow Checklist

### Step 1: Apply Quality Gate Constraints
- **Function LLOC**: $\le 15$ LLOC per method/constructor/destructor/lambda.
- **Class LLOC**: $\le 500$ LLOC.
- **File LLOC**: $\le 600$ LLOC.
- **Cyclomatic Complexity**: $\text{CCN} \le 10$.
- **Nesting Depth**: $\le 4$ levels.
- **DLL Export**: `DOTNETDUPE_API` on individual methods only; never on `class`.
- **Exceptions**: Custom library exceptions only (derive from `DotNetDupe::System::Exception`).
- **Memory**: `SmartPointer<T>` and RAII; no raw `new`/`delete`.
- **Catch Blocks**: Meaningful error handling or explicit `(void)0;` with rationale.
- **ABI Stability**: Zero STL types in public signatures (use `String`, `Array<T>`, `List<T>`, etc.).
- **Hungarian Naming**: Strict `p`/`sp`/`m_p`/`s_p` for `SmartPointer` variables.

### Step 2: Add 100% Unit Test Coverage
- Add unit tests in `DotNetDupeTests/<Class>Tests.cpp`.
- Follow the `GivenWhenThen` naming convention:
  ```cpp
  TEST(StringTests, GivenEmptyString_WhenLengthChecked_ThenReturnsZero) {
      // Given
      String sText;
      // When
      int iLen = sText.GetLength();
      // Then
      EXPECT_EQ(iLen, 0);
  }
  ```
- Cover positive paths, negative paths, null/empty parameters, boundary conditions, custom exceptions, and stream/interface interactions.

### Step 3: Document Class in README.md
- Add the class and its architectural description in `README.md` under the appropriate namespace table.

### Step 5: Add Demo Code in DotNetDupeDemo
- Add a compile-ready demo file in `DotNetDupeDemo/` (e.g., `Demo<Class>.cpp`).
- Invoke the demo method from `DotNetDupeDemo/DotNetDupeDemo.cpp`.

### Step 6: No STL Threading/Timing in Demos
- Always use library concurrency types in demos:
  - `DotNetDupe::System::Threading::Thread`
  - `DotNetDupe::System::Threading::ThreadPool`
  - `DotNetDupe::System::Threading::Tasks::Task`
  - `DotNetDupe::System::Threading::Thread::Sleep()`

### Step 7: Update Natvis Debugger Visualizer (`DotNetDupe.natvis`)
- When creating or modifying a class (especially classes using Pimpl, internal buffers, or OS handles), update `DotNetDupe.natvis` with clean `<Type>` definitions for Visual Studio debugging.

### Step 8: Verify Solution Build
- Build the solution via MSBuild:
  ```powershell
  msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64
  ```
- Ensure 0 errors and 0 warnings.

### Step 9: Verify Unit Tests
- Execute the test runner:
  ```powershell
  .\bin\x64\Release\DotNetDupeTests.exe
  ```
- Ensure 100% test pass rate with zero failures.

### Step 10: Verify Quality Gates
- Execute the quality gate static analyzer:
  ```powershell
  powershell -ExecutionPolicy Bypass -File .\scripts\Check-QualityGates.ps1 -RootDir . -OutputDir CodeCoverage
  ```
- Ensure 0 errors and 0 warnings.
