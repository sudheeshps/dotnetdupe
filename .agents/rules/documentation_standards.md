# DotNetDupe Documentation Standards (Doxygen Format)

## Overview
All C++ header (`.h`) and source (`.cpp`) files across DotNetDupe must strictly adhere to the uniform **Doxygen (`///`)** documentation standard modeled after the SmartDicom project standard.
Code must be extensively documented with:
1. **Class Headers**: Architectural role, thread safety considerations, and specific .NET BCL / ECMA / RFC / ISO standard citations (e.g., ECMA-335 Common Language Infrastructure, RFC 4122 for GUIDs, RFC 4648 for Base64, IEEE 754 for BitConverter).
2. **Function & Method Headers**: Operation summary (`\brief`), parameter constraints (`\param`), return values (`\return`), exception guarantees (`\throws`), and algorithmic explanations (`\note`).
3. **Internal Logical Blocks**: Distinct logical blocks inside function bodies must feature a `///` comment explaining what each step is performing and why.
4. **Standard Part Citations**: Explicit references to the relevant BCL specifications and standards:
   - **ECMA-335**: Common Language Infrastructure (CLI) Base Class Library specifications.
   - **RFC 4122**: A Universally Unique Identifier (UUID) URN Namespace (Guid).
   - **RFC 4648**: The Base16, Base32, and Base64 Data Encodings (Convert).
   - **IEEE 754**: Standard for Floating-Point Arithmetic (BitConverter).
   - **RFC 7230-7235 / RFC 9110**: HTTP/1.1 and HTTP Semantics (HttpClient, HttpContext, WebAppCore).
   - **RFC 8259**: The JavaScript Object Notation (JSON) Data Interchange Format (JsonSerializer).

---

## 1. Class Header Format
Use triple-slash (`///`) comments preceding class declarations:
```cpp
/// \brief Represents the base class of all types in the DotNetDupe class hierarchy.
///
/// Serves as the ultimate base class for all DotNetDupe classes, providing core methods
/// including ToString(), GetHashCode(), Equals(), and GetType(). Provides high-performance
/// memory management via an internal aligned block pool allocator.
/// Thread-safe for const operations; derived classes must document their concurrency guarantees.
///
/// \note Conforms to ECMA-335 Partition I Section 8.9.1 (System.Object).
/// \see String, SmartPointer
class DOTNETDUPE_API Object {
```

## 2. Method / Function Header Format
Use triple-slash (`///`) comments with `\brief`, `\param`, `\return`, `\throws`, and `\note` tags:
```cpp
/// \brief Formats a composite format string using the specified argument array.
/// \param sFormat The composite format string containing zero or more format items (e.g. "{0}").
/// \param arrArgs An array of objects to format and insert into the format string.
/// \return A copy of format in which the format items have been replaced by the string representation of the corresponding objects.
/// \throws ArgumentNullException Thrown when sFormat is null.
/// \throws FormatException Thrown when the format string is malformed or an index is out of range.
/// \note Implements composite formatting state machine parsing per ECMA-335 Part IV.
DOTNETDUPE_API static String Format(const String& sFormat, const Array<SmartPointer<Object>>& arrArgs);
```

## 3. Inline Logical Block Commenting Format
Distinct logical blocks inside function bodies must feature a `///` comment explaining what each step is performing and why:
```cpp
SmartPointer<Array<byte>> Convert::FromBase64String(const String& sInput) {
    /// Guard: Ensure input string is not empty or malformed.
    if (sInput.IsEmpty()) {
        return SmartPointer<Array<byte>>::Create(0);
    }

    /// Calculate clean character count excluding whitespace and padding characters.
    int iCleanLen = CalculateBase64CleanLength(sInput);
    if ((iCleanLen % 4) != 0) {
        throw FormatException("The input is not a valid Base64 string as its length is not a multiple of 4.");
    }

    /// Allocate byte array sized for decoded 3-byte groups.
    int iTotalBytes = CalculateBase64DecodedSize(sInput, iCleanLen);
    auto spOutput = SmartPointer<Array<byte>>::Create(iTotalBytes);

    /// Stream and decode 4-character blocks into 3 output bytes using lookup table.
    DecodeBase64Blocks(sInput, *spOutput);

    /// Return the populated decoded byte array.
    return spOutput;
}
```

## 4. Quality Gate & Maintenance Rules
- **Mandatory Maintenance Rule**: Whenever adding a new class, interface, method, or updating existing code, full Doxygen documentation with exact standard citations (`ECMA-335`, `RFC`, etc.) must be added or updated synchronously.
- **No Numbering on Single Block Comments**: Do NOT use artificial numbers (e.g., `/// 1. Initialize controller without backend service handler.`). Use clean, natural descriptive statements (e.g., `/// Initialize controller without backend service handler.` or `/// Guard: Verify service handler is configured.`).
- **Triple-Slash (`///`) Format**: Recognized by `scripts/Check-QualityGates.ps1` as comments; does not count against the 15 LLOC limit.
- **Professional API Documentation Generation**: All public APIs must provide comprehensive descriptions compatible with automated Doxygen documentation generation (`Doxyfile` / `scripts/Generate-Docs.ps1`).
- **Single Source of Documentation Truth**: In-code Doxygen (`///`) comments generate the comprehensive interactive HTML documentation in `docs/html/` and the master portal `docs/index.html`, while architectural descriptions are maintained in `README.md`.
