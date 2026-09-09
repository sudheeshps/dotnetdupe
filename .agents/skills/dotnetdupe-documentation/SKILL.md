---
name: dotnetdupe-documentation
description: Guides creation of Doxygen (///) comments, internal algorithmic block comments, and HTML documentation generation for DotNetDupe modeled after SmartDicom standards.
---

# DotNetDupe Documentation & Doxygen Standards Skill

## Overview
This skill guides the authoring and maintenance of Doxygen API documentation and internal function logic comments across DotNetDupe, conforming to the `SmartDicom` documentation standard and `GEMINI.md`.

## Doxygen Commenting Standard (`///`)

### 1. Class Header Documentation
Document all classes and structs with architectural role, thread safety, and standard specifications:
```cpp
/// \brief Represents an arbitrarily expandable array of elements conforming to ECMA-335.
///
/// Implements a dynamically sized contiguous buffer with amortized O(1) append operations.
/// Thread-safe for multiple concurrent readers; writes require external synchronization.
///
/// \note Conforms to ECMA-335 Partition IV Section 5.3 (System.Collections.Generic.List`1).
/// \see Array, SmartPointer
template <typename T>
class List : public DotNetDupe::System::Object {
```

### 2. Method & Function Documentation
Document all public methods, functions, and constructors using standard Doxygen tags:
```cpp
/// \brief Searches for an element that matches the conditions defined by the specified predicate.
/// \param pMatch A delegate predicate defining the conditions of the element to search for.
/// \return The first element that matches the predicate; throws if not found.
/// \throws ArgumentNullException Thrown when pMatch is null.
/// \throws InvalidOperationException Thrown when no matching element is found.
/// \note Performs a linear O(N) traversal from index 0 to Count - 1.
DOTNETDUPE_API T Find(Predicate<T> pMatch) const;
```

### 3. Inline Logical Block Commenting
Every multi-stage function must include descriptive `///` comments explaining the purpose of distinct logical steps. Do NOT use artificial numbers (e.g. `/// 1. ...`).
```cpp
SmartPointer<Array<byte>> File::ReadAllBytes(const String& sPath) {
    /// Guard: Ensure file path is not null or empty.
    if (sPath.IsEmpty()) {
        throw ArgumentException("Path cannot be empty.");
    }

    /// Open binary file stream for sequential reading.
    FileStream stream(sPath, FileMode::Open, FileAccess::Read);
    long long llLength = stream.GetLength();

    /// Allocate byte array sized to file length.
    auto spBytes = SmartPointer<Array<byte>>::Create(static_cast<int>(llLength));

    /// Read all bytes from the stream into the destination array.
    stream.Read(*spBytes, 0, static_cast<int>(llLength));

    /// Return the populated byte array.
    return spBytes;
}
```

## Standard Specifications & Citations Table

| Technology / Component | Standard Citation |
|------------------------|-------------------|
| Common Types & BCL | **ECMA-335**: Common Language Infrastructure (CLI) |
| Guid Generation & Parsing | **RFC 4122**: Universally Unique Identifier (UUID) URN Namespace |
| Base64 Encoding/Decoding | **RFC 4648**: Base16, Base32, and Base64 Data Encodings |
| Floating-Point Conversion | **IEEE 754**: Standard for Floating-Point Arithmetic |
| HTTP & WebAppCore | **RFC 9110 / RFC 7230-7235**: HTTP Semantics and Architecture |
| JSON Serialization | **RFC 8259**: JavaScript Object Notation (JSON) Data Interchange Format |

## Documentation Generation Workflow

1. **Verify Doxygen Comments**: Ensure all comments use `///` format (recognized by `Check-QualityGates.ps1` and ignored for LLOC counts).
2. **Compile HTML Documentation**:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\scripts\Generate-Docs.ps1
   ```
   This compiles Doxygen into `docs/html/index.html`.
3. **Inspect Output**:
   To automatically launch the generated portal in your default browser:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\scripts\Generate-Docs.ps1 -OpenBrowser
   ```
4. **Master Portal**: `docs/index.html` provides the interactive root entry point linking Doxygen HTML and markdown guides.
