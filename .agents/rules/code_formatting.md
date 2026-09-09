# Code Formatting & Function Logical Block Grouping Rule

## Principle
All C++ functions in DotNetDupe must be clearly structured for readability and visual clarity by logically grouping related statements and separating distinct logical blocks with single blank lines.

## Guidelines
1. **Logical Block Separation**:
   - **Guard Clauses & Input Validation**: Separate early returns, null checks, and parameter validations from the rest of the function body with a single blank line.
   - **Initialization & Setup**: Group local variable declarations and state setup together, followed by a blank line.
   - **Core Logic & Transformation**: Separate loops, algorithm steps, or data transformations into distinct logical paragraphs separated by blank lines and commented with `///`.
   - **Output & Return**: Separate final result preparation, logging, and `return` statements from preceding operations with a blank line.

2. **No False Compression / Cramming**:
   - Do not cram multiple unrelated statements onto a single line or eliminate empty lines between logical blocks merely to make functions look physically shorter.
   - The 15 LLOC limit enforced by static analysis (`scripts/Check-QualityGates.ps1`) counts **Logical Lines of Code (LLOC)** (non-empty, non-comment, non-standalone-brace lines). Blank lines and comments have zero impact on LLOC.

3. **Standalone Braces**:
   - Standalone opening `{` and closing `}` on their own lines are encouraged where appropriate as they do not count toward LLOC and improve visual structure.

4. **Consistency**:
   - Apply this spacing and block grouping consistently across all `System`, `System::IO`, `System::Collections`, `System::Threading`, `System::Diagnostics`, `System::Net`, and `WebAppCore` modules.
