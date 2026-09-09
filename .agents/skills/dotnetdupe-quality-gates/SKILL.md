---
name: dotnetdupe-quality-gates
description: Enforces, validates, and refactors code to strictly comply with the 11 DotNetDupe Quality Gate constraints defined in GEMINI.md.
---

# DotNetDupe Quality Gate Enforcement Skill

## Overview
This skill guides the validation and refactoring of DotNetDupe C++ code to guarantee 100% compliance with all 11 Quality Gate constraints defined in `GEMINI.md`. Zero errors and zero warnings are permitted.

## The 11 Quality Gate Constraints

| # | Metric | Limit | Rules & Resolution Strategy |
|---|--------|-------|-----------------------------|
| 1 | **Function LLOC** | $\le 15$ LLOC | Excludes blanks, standalone braces (`{`, `}`), and comments (`//`, `/*`, `*`, `///`). Extract sub-steps into static helper functions or private helpers. |
| 2 | **Class LLOC** | $\le 500$ LLOC | Split multi-responsibility classes into helper classes or private Pimpl components. |
| 3 | **File LLOC** | $\le 600$ LLOC | Split large `.cpp` translation units across logical module files (e.g. `String.cpp`, `StringFormatting.cpp`). |
| 4 | **Cyclomatic Complexity (CCN)** | $\le 10$ | Replace nested conditionals with lookup tables, guard clauses, or polymorphic handlers. |
| 5 | **Nesting Depth** | $\le 4$ levels | Use early returns/guard clauses and subroutines to flatten nested code blocks. |
| 6 | **DLL Export Macro** | Function-level only | Place `DOTNETDUPE_API` on individual methods and functions; **never** on `class DOTNETDUPE_API MyClass`. |
| 7 | **Exception Types** | Custom library only | Never throw `std::*` exceptions. Inherit from `DotNetDupe::System::Exception` or subclasses (`ArgumentException`, `InvalidOperationException`, `IOException`). |
| 8 | **Memory Management** | Zero raw ownership | Strict `DotNetDupe::System::SmartPointer<T>` and RAII. Raw `new`/`delete` or unmanaged pointer ownership is forbidden. |
| 9 | **Catch Block Integrity** | No swallowed catches | Catch blocks must rethrow, log, propagate, or include an explicit `(void)0;` suppression with rationale comment. |
| 10 | **ABI Stability** | Zero STL type leakage | Public `DOTNETDUPE_API` methods must not use STL types (`std::string`, `std::vector`, `std::map`). Use `String`, `List<T>`, `Dictionary<TKey, TValue>`, `Array<T>`. |
| 11 | **SmartPointer Hungarian Naming** | Mandatory `p`/`sp` prefix | Local/parameter: `pStream`, `spContext`; Member: `m_pLogger`, `m_spReader`; Static: `s_pFactory`, `s_spInstance`. |

## Running the Quality Gate Validator
Run the automated static analysis script from PowerShell:
```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\Check-QualityGates.ps1 -RootDir . -OutputDir CodeCoverage
```

## Refactoring Techniques for $\le 15$ LLOC & $\le 10$ CCN

### 1. Guard Clause Flattening
```cpp
// Bad (Deep nesting, counts toward LLOC and nesting depth):
if (pData != nullptr) {
    if (iLength > 0) {
        // ...
    }
}

// Good (Flat structure, early return):
if (pData == nullptr || iLength <= 0) {
    return false;
}
```

### 2. Static Subroutine Extraction
When a function performs multiple stages (validation, preprocessing, execution, postprocessing), extract each stage into a static file-scope helper function in the `.cpp` file. Keep each helper $\le 15$ LLOC and documented with `///`.

### 3. Logical Block Grouping
Separate logical blocks (guards, initialization, transformation, return) with single blank lines. Blank lines and `///` comments do not count toward LLOC in `Check-QualityGates.ps1`.
