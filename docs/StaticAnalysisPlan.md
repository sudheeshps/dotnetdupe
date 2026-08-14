# DotNetDupe Static Analysis & Code Quality Roadmap

## 1. Executive Summary & Objectives

This document defines the comprehensive static analysis and code metrics architecture for the **DotNetDupe** C++20 Base Class Library. The goal is to enforce architectural consistency, memory safety, deterministic error handling, low cyclomatic complexity, and header STL decoupling across both local developer workflows and automated GitHub CI/CD pipelines.

---

## 2. Static Analysis Metric Dimensions & Parameters

### A. Code Complexity & Structure

| Metric | Description | Target Threshold | Quality Gate Action |
|---|---|---|---|
| **Cyclomatic Complexity (CCN)** | Counts independent decision paths in each function/method (`if`, `else if`, `for`, `while`, `case`, `&&`, `||`, `? :`). | $\le 10$ per function | Warning if $> 10$; Error if $> 15$ |
| **Cognitive Complexity** | Evaluates mental effort required to read control flow, heavily penalizing nested loops and unstructured breaks/returns. | $\le 15$ per function | Warning if $> 15$ |
| **Maximum Nesting Depth** | Detects deep indentation levels (e.g., nested `if`/`loop`/`try` blocks). | $\le 3\text{--}4$ levels | Warning if $> 4$ |
| **Function Length (LLOC)** | Logical lines of code per method/function (excluding blanks, comments, standalone braces). | $\le 15$ LLOC | Error if $> 15$ (per `GEMINI.md`) |
| **Parameter Count** | Number of parameters per method or constructor. | $\le 5$ arguments | Warning if $> 5$ |

---

### B. Size & Granularity Metrics

| Metric | Description | Target Threshold | Quality Gate Action |
|---|---|---|---|
| **Class Size (LLOC)** | Logical lines of code per class declaration/definition. | $\le 500$ LLOC | Error if $> 500$ (per `GEMINI.md`) |
| **Source File Size (LLOC)** | Logical lines of code per `.cpp` source implementation file. | $\le 600$ LLOC | Error if $> 600$ (per `GEMINI.md`) |
| **Comment-to-Code Ratio (CCR)** | Ratio of API documentation comments (`///`, `/** */`) to logical lines of code. | $15\%\text{--}30\%$ | Informational / Quality badge |

---

### C. Error Handling & Exception Safety

| Metric | Description | Target Threshold | Quality Gate Action |
|---|---|---|---|
| **Custom Exception Adherence** | Detects prohibited standard exceptions (`throw std::runtime_error`, `throw std::invalid_argument`) and ensures library exceptions (`ArgumentException`, `IOException`, `SystemException`) are thrown. | **0 standard exception throws** | **Strict Failure** (Exit code 1) |
| **Swallowed Exception Detection** | Detects empty `catch (...) {}` or `catch (const Exception&) {}` blocks that discard errors without logging or rethrowing. | **0 swallowed exceptions** | **Strict Failure** (Exit code 1) |
| **Diagnostic Message Check** | Verifies all exception throws pass a non-empty, descriptive diagnostic string. | **100% compliant** | Warning if empty string |
| **System Call Error Handling** | Verifies system API invocations (Win32 API, socket descriptors) capture error codes (`GetLastError()`) and format actionable error messages upon failure. | **100% compliant** | Warning / Review trigger |

---

### D. Memory Management & Safety

| Metric | Description | Target Threshold | Quality Gate Action |
|---|---|---|---|
| **Raw Pointer / RAII Violations** | Detects raw dynamic allocations (`new`, `delete`, `malloc`, `free`) to guarantee `SmartPointer<T>` and RAII factory methods (`NewShared()`, `NewUnique()`) are used exclusively. | **0 raw `new`/`delete` calls** | **Strict Failure** (Exit code 1) |
| **SmartPointer Hungarian `p` Prefix** | Validates that all `SmartPointer` variables follow naming standards: parameter/local (`pVar`), member (`m_pMember`), static (`s_pStatic`). | **100% compliance** | Warning / Error |
| **Handle / Descriptor RAII** | Checks that OS handles (`HANDLE`, socket descriptors) are encapsulated in RAII wrappers. | **100% compliance** | Warning / Error |

---

### E. Architectural Cleanliness & Decoupling

| Metric | Description | Target Threshold | Quality Gate Action |
|---|---|---|---|
| **Public Header STL Decoupling** | Verifies no standard container/string headers (`<string>`, `<vector>`, `<map>`, `<unordered_map>`, `<memory>`) are included in public headers under `Include/`. | **0 STL headers in `Include/`** | **Strict Failure** (Exit code 1) |
| **Export Macro Placement (`DOTNETDUPE_API`)** | Verifies `DOTNETDUPE_API` is applied strictly at the function/method level and never at the class level. | **0 class-level exports** | **Strict Failure** (Exit code 1) |
| **Namespace Conformance** | Ensures all exported types and functions are strictly scoped under the `DotNetDupe::*` root namespace. | **100% conformance** | Warning if missing |
| **Header Include Fan-Out** | Measures the number of `#include` directives per header to detect circular or excessive coupling. | $\le 12$ includes/header | Informational / Warning |

---

## 3. HTML Report Dashboard Architecture (`CodeCoverage/StaticAnalysis.html`)

The static analysis report will be generated alongside code coverage into the single unified `CodeCoverage/` folder:

1. **Summary Banner**: Overall Quality Gate Status (PASS / FAIL), scan timestamp, total files scanned, error count, warning count.
2. **Rule Cards & Badges**:
   - 🛡️ **Architectural Rules**: Header STL Isolation, Class-level `DOTNETDUPE_API` check.
   - ⚠️ **Error Handling Rules**: Standard exception throw checks, empty catch blocks.
   - 🧠 **Memory Safety Rules**: Raw `new`/`delete` scans, `SmartPointer` naming checks.
3. **Interactive File Metrics Table**:
   - Source File Path
   - LLOC (with progress/bar comparison against 600 LLOC limit)
   - Max Cyclomatic Complexity (CCN)
   - Nesting Depth
   - Violations / Status Badges (`PASS`, `WARN`, `FAIL`)
4. **Navigation**: Direct back-and-forth links to `index.html` (Code Coverage) and `StaticAnalysis.html`.

---

## 4. Phased Implementation Plan

### Phase 1: High-Priority Quality & Safety Gates (Immediate)
- [ ] Implement **Public Header STL Leakage Scanner** (`scripts/Check-QualityGates.ps1`).
- [ ] Implement **Prohibited Standard Exception Scanner** (`throw std::*`).
- [ ] Implement **Raw Memory Allocation Detector** (`new `, `delete `, `malloc`, `free`).
- [ ] Implement **Swallowed Exception Detector** (empty `catch` blocks).
- [ ] Output structured results directly to `CodeCoverage/StaticAnalysis.html`.

### Phase 2: Structural Complexity & Function Limits
- [ ] Implement **Function LLOC ($\le 15$) & Class LLOC ($\le 500$) Parser**.
- [ ] Implement **Cyclomatic Complexity (CCN) Calculator** per function.
- [ ] Implement **Maximum Nesting Depth Analyzer**.
- [ ] Implement **SmartPointer `p` Prefix Verifier**.

### Phase 3: CI/CD Enforcement & GitHub Badging
- [ ] Integrate full static analysis validation into GitHub Actions workflow (`.github/workflows/build-and-release.yml`).
- [ ] Enforce blocking build failures if any Tier-1 rule (STL leakage, raw memory allocation, standard exception, class-level export macro) is violated.
- [ ] Publish live combined Coverage + Static Analysis artifact to GitHub Pages.

---

## 5. Execution Commands

```powershell
# Run Static Analysis & Quality Gate Check
powershell -ExecutionPolicy Bypass -File .\scripts\Check-QualityGates.ps1 -OutputDir CodeCoverage

# Run Full Coverage + Static Analysis Pipeline locally
powershell -ExecutionPolicy Bypass -File .\scripts\Generate-CoverageReport.ps1 -Open
```
