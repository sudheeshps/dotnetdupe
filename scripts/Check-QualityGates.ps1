<#
.SYNOPSIS
    Quality Gate Check Script for DotNetDupe.
.DESCRIPTION
    Checks LLOC (Logical Lines of Code) limits and verifies source file metrics.
#>

[CmdletBinding()]
param (
    [string]$RootDir = "."
)

$ErrorCount = 0

function Write-RuleError {
    param ([string]$Message)
    Write-Host "::error::$Message" -ForegroundColor Red
    $script:ErrorCount++
}

function Write-RulePass {
    param ([string]$Message)
    Write-Host "[PASS] $Message" -ForegroundColor Green
}

Write-Host "=================================================="
Write-Host " Running DotNetDupe Code Quality Gate Checks"
Write-Host "=================================================="

# ------------------------------------------------------------------
# Check 1: File Length Limit (Max 600 LLOC for .cpp implementation files)
# ------------------------------------------------------------------
Write-Host "`n--- Check 1: Implementation File Length Limits (Max 600 LLOC) ---"
$cppFiles = Get-ChildItem -Path (Join-Path $RootDir "DotNetDupe") -Recurse -Filter "*.cpp"

foreach ($file in $cppFiles) {
    $lines = Get-Content $file.FullName
    $lloc = 0
    foreach ($line in $lines) {
        $trimmed = $line.Trim()
        if ($trimmed -eq "" -or $trimmed -eq "{" -or $trimmed -eq "}" -or $trimmed.StartsWith("//") -or $trimmed.StartsWith("/*") -or $trimmed.StartsWith("*")) {
            continue
        }
        $lloc++
    }

    if ($lloc -gt 600) {
        Write-RuleError "File '$($file.Name)' exceeds limit: $lloc LLOC (Max allowed: 600 LLOC)"
    }
}

if ($ErrorCount -eq 0) {
    Write-RulePass "All implementation source files comply with the 600 LLOC limit."
}

# ------------------------------------------------------------------
# Check 2: Public API Export Macro Verification
# ------------------------------------------------------------------
Write-Host "`n--- Check 2: Class-Level Export Macro Compliance ---"
$headerFiles = Get-ChildItem -Path (Join-Path $RootDir "Include") -Recurse -Filter "*.h"

foreach ($file in $headerFiles) {
    $lines = Get-Content $file.FullName
    $lineNum = 0
    foreach ($line in $lines) {
        $lineNum++
        $trimmed = $line.Trim()
        # Per project conventions: DOTNETDUPE_API shall strictly be applied at function/method level, NOT class level.
        if ($trimmed -match 'class\s+DOTNETDUPE_API\s+') {
            Write-RuleError "Class level export macro detected in '$($file.Name):$lineNum': '$trimmed'"
        }
    }
}

if ($ErrorCount -eq 0) {
    Write-RulePass "No class-level DOTNETDUPE_API export violations found."
}

# ------------------------------------------------------------------
# Summary
# ------------------------------------------------------------------
Write-Host "`n=================================================="
if ($ErrorCount -gt 0) {
    Write-Host "Quality Gate Check FAILED with $ErrorCount violation(s)." -ForegroundColor Red
    exit 1
} else {
    Write-Host "All Quality Gate Checks PASSED successfully!" -ForegroundColor Green
    exit 0
}
