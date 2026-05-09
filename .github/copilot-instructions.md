# Copilot instructions for DotNetDupe

This file gives concise, actionable guidance for future Copilot sessions working in this repository.

---

## 1) Build, test, and (no) lint commands

- Restore NuGet packages (required for tests):
  - nuget restore DotNetDupe.sln

- Build the full solution (Release x64 — also produces NuGet package):
  - msbuild DotNetDupe.sln /p:Configuration=Release /p:Platform=x64

- Build Debug x64 (common for running tests locally):
  - msbuild DotNetDupe.sln /p:Configuration=Debug /p:Platform=x64

- Run all unit tests (after building Debug|x64):
  - .\x64\Debug\DotNetDupeTests.exe

- Run a single GoogleTest test by name or pattern:
  - .\x64\Debug\DotNetDupeTests.exe --gtest_filter=TestSuite.TestName
  - Example: .\x64\Debug\DotNetDupeTests.exe --gtest_filter=PathTests.*

Notes:
- The tests rely on a NuGet package: Microsoft.googletest.v140.*; ensure nuget restore succeeded.
- The DotNetDupe project contains an MSBuild target (PackNuget) that creates a NuGet package to `nuget_packages/` for Release x64.

---

## 2) High-level architecture (big picture)

- Solution (DotNetDupe.sln) contains three projects:
  - DotNetDupe — native C++ dynamic library (the library itself).
  - DotNetDupeTests — GoogleTest-based test runner that references the library.
  - DotNetDupeClient — sample console application demonstrating usage.

- Public API surface is headers under `Include/` organized by pseudo-namespaces `System` and `System::IO` (e.g., `Include/System/String.h`, `Include/System/IO/Path.h`).

- Build outputs (binaries & test executables) live in arch/config folders such as `x64\Debug` and `x64\Release`.

- Packaging: DotNetDupe.vcxproj contains an MSBuild `PackNuget` target that uses `DotNetDupe.nuspec` to assemble a NuGet package (headers + binaries + targets) into `nuget_packages/`.

---

## 3) Key repository conventions and patterns

- Header layout and intended consumption:
  - Public headers are under `Include\` and are consumed by projects with relative includes (e.g., `$(ProjectDir)..\Include` or `$(SolutionDir)Include`).

- Precompiled headers are used throughout (`pch.h` / `pch.cpp`); many source files assume PCH usage.

- Platform/toolset behaviour:
  - Solution supports Debug/Release for Win32 and x64. Default CI/developer flow uses x64 where possible.
  - Project files reference different platform toolsets (v143/v145) — prefer using Developer Command Prompt matching installed VS toolset.

- Tests:
  - GoogleTest is included as a NuGet package (see `DotNetDupeTests.vcxproj` imports). The test binary is a standalone executable; use gtest flags for filtering, output formats (e.g., `--gtest_output=xml:results.xml`), etc.

- NuGet packaging:
  - The nuspec (`DotNetDupe.nuspec`) defines what goes into the package (headers, docs, binaries). The Release x64 build triggers packing via the `PackNuget` MSBuild target.

- No repository-level linting/format files detected (e.g., no `.clang-format` or cpplint configs).

---

## 4) Existing AI assistant configs

- No CLAUDE.md, AGENTS.md, .cursorrules, CONVENTIONS.md, AIDER_CONVENTIONS.md, .windsurfrules, or .clinerules were found in the repo root. If present, add or link important rules here.

---

If this file already exists and you want changes applied instead of replacement, mention which sections to preserve.

