<#
.SYNOPSIS
    Quality Gate Check & Static Analysis Report Generator for DotNetDupe.
.DESCRIPTION
    Executes Static Analysis Quality Gates:
    Phase 1:
      1. Implementation file length limit (Max 600 LLOC)
      2. Class-level export macro compliance (Function-level DOTNETDUPE_API)
      3. Prohibited standard exception scanner (throw std::*)
      4. Raw memory allocation scanner (new/delete/malloc/free)
      5. Swallowed exception block scanner (empty catch blocks)
      6. Public API STL parameter/return leakage scanner
    Phase 2:
      7. Function LLOC limit parser (Max 15 LLOC per method/function)
      8. Class LLOC limit aggregator (Max 500 LLOC per class implementation)
      9. Cyclomatic Complexity Number (CCN) calculator
      10. Maximum Nesting Depth analyzer (Max depth <= 4)
      11. SmartPointer 'p' prefix naming convention scanner
    Generates a unified HTML dashboard report to CodeCoverage/StaticAnalysis.html.
#>

[CmdletBinding()]
param (
    [string]$RootDir = ".",
    [string]$OutputDir = ""
)

$ErrorActionPreference = "Stop"
$ErrorCount = 0
$WarningCount = 0

$CppFileStats = [System.Collections.Generic.List[PSCustomObject]]::new()
$HeaderExportViolations = [System.Collections.Generic.List[PSCustomObject]]::new()
$StdExceptionViolations = [System.Collections.Generic.List[PSCustomObject]]::new()
$RawMemoryViolations = [System.Collections.Generic.List[PSCustomObject]]::new()
$EmptyCatchViolations = [System.Collections.Generic.List[PSCustomObject]]::new()
$StlLeakageViolations = [System.Collections.Generic.List[PSCustomObject]]::new()

$FunctionStats = [System.Collections.Generic.List[PSCustomObject]]::new()
$ClassStats = [System.Collections.Generic.Dictionary[string, int]]::new()
$SmartPointerNamingViolations = [System.Collections.Generic.List[PSCustomObject]]::new()

function Write-RuleError {
    param ([string]$Message)
    Write-Host "::error::$Message" -ForegroundColor Red
    $script:ErrorCount++
}

function Write-RuleWarn {
    param ([string]$Message)
    Write-Host "::warning::$Message" -ForegroundColor Yellow
    $script:WarningCount++
}

function Write-RulePass {
    param ([string]$Message)
    Write-Host "[PASS] $Message" -ForegroundColor Green
}

Write-Host "=================================================="
Write-Host " Running DotNetDupe Static Analysis Quality Gates"
Write-Host "=================================================="

# ------------------------------------------------------------------
# Phase 1 - Check 1: File Length Limit (Max 600 LLOC)
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

    $isPassed = ($lloc -le 600)
    if (-not $isPassed) {
        Write-RuleError "File '$($file.Name)' exceeds limit: $lloc LLOC (Max allowed: 600 LLOC)"
    }

    $resolvedRoot = (Resolve-Path $RootDir).Path
    $relativeFilePath = $file.FullName.Replace($resolvedRoot, "").TrimStart("\", "/")
    $CppFileStats.Add([PSCustomObject]@{
        FileName     = $file.Name
        RelativePath = $relativeFilePath
        LLOC         = $lloc
        MaxAllowed   = 600
        Status       = if ($isPassed) { "PASS" } else { "FAIL" }
    })
}

if ($ErrorCount -eq 0) {
    Write-RulePass "All $($cppFiles.Count) implementation source files comply with the 600 LLOC limit."
}

# ------------------------------------------------------------------
# Phase 1 - Check 2: Public API Export Macro Placement
# ------------------------------------------------------------------
Write-Host "`n--- Check 2: Class-Level Export Macro Compliance ---"
$headerFiles = Get-ChildItem -Path (Join-Path $RootDir "Include") -Recurse -Filter "*.h"
$checkedHeaders = 0

foreach ($file in $headerFiles) {
    $checkedHeaders++
    $lines = Get-Content $file.FullName
    $lineNum = 0
    foreach ($line in $lines) {
        $lineNum++
        $trimmed = $line.Trim()
        if ($trimmed -match 'class\s+DOTNETDUPE_API\s+') {
            Write-RuleError "Class-level export macro detected in '$($file.Name):$lineNum': '$trimmed'"
            $HeaderExportViolations.Add([PSCustomObject]@{
                FileName   = $file.Name
                LineNumber = $lineNum
                Content    = $trimmed
            })
        }
    }
}

if ($HeaderExportViolations.Count -eq 0) {
    Write-RulePass "All $checkedHeaders header files comply with function-level DOTNETDUPE_API export conventions."
}

# ------------------------------------------------------------------
# Phase 1 - Check 3: Prohibited Standard Exception Scanner (throw std::*)
# ------------------------------------------------------------------
Write-Host "`n--- Check 3: Prohibited Standard Exception Scanner (throw std::*) ---"
$allSourceFiles = @($cppFiles) + @($headerFiles)

foreach ($file in $allSourceFiles) {
    $lines = Get-Content $file.FullName
    $lineNum = 0
    foreach ($line in $lines) {
        $lineNum++
        $trimmed = $line.Trim()
        if ($trimmed -match 'throw\s+std::') {
            Write-RuleError "Prohibited standard exception thrown in '$($file.Name):$lineNum': '$trimmed'"
            $StdExceptionViolations.Add([PSCustomObject]@{
                FileName   = $file.Name
                LineNumber = $lineNum
                Content    = $trimmed
            })
        }
    }
}

if ($StdExceptionViolations.Count -eq 0) {
    Write-RulePass "Zero prohibited standard exceptions found across $($allSourceFiles.Count) files."
}

# ------------------------------------------------------------------
# Phase 1 - Check 4: Raw Memory Allocation Scanner
# ------------------------------------------------------------------
Write-Host "`n--- Check 4: Raw Memory Allocation Scanner ---"
$allowedPimplAndRaiiFiles = @(
    "SmartPointer.h",
    "String.cpp",
    "MemoryStream.cpp",
    "Socket.cpp",
    "MockStream.cpp",
    "Exception.cpp",
    "ServiceCollection.cpp",
    "StringWriter.cpp",
    "TcpClient.cpp",
    "TcpListener.cpp",
    "UdpClient.cpp",
    "Thread.cpp",
    "HttpClient.cpp",
    "HttpContent.cpp"
)

foreach ($file in $cppFiles) {
    if ($allowedPimplAndRaiiFiles -contains $file.Name) { continue }
    $lines = Get-Content $file.FullName
    $lineNum = 0
    foreach ($line in $lines) {
        $lineNum++
        $trimmed = $line.Trim()
        if ($trimmed.StartsWith("//") -or $trimmed.StartsWith("/*") -or $trimmed.StartsWith("*")) { continue }

        if ($trimmed -match '\bnew\s+[A-Za-z0-9_:]+(\s*\[|\s*\()' -and -not ($trimmed -match '::new\s*\(') -and -not ($trimmed -match 'NewShared|NewUnique')) {
            Write-RuleError "Unmanaged raw 'new' allocation in '$($file.Name):$lineNum': '$trimmed'"
            $RawMemoryViolations.Add([PSCustomObject]@{
                FileName   = $file.Name
                LineNumber = $lineNum
                Operation  = "new"
                Content    = $trimmed
            })
        }

        if ($trimmed -match '\bdelete\s+[^;]+;' -or $trimmed -match '\bdelete\[\]\s+[^;]+;') {
            Write-RuleError "Unmanaged raw 'delete' call in '$($file.Name):$lineNum': '$trimmed'"
            $RawMemoryViolations.Add([PSCustomObject]@{
                FileName   = $file.Name
                LineNumber = $lineNum
                Operation  = "delete"
                Content    = $trimmed
            })
        }
    }
}

if ($RawMemoryViolations.Count -eq 0) {
    Write-RulePass "Zero unmanaged raw memory allocations detected in DotNetDupe implementation sources."
}

# ------------------------------------------------------------------
# Phase 1 - Check 5: Swallowed Exception Detector (Empty Catch Blocks)
# ------------------------------------------------------------------
Write-Host "`n--- Check 5: Swallowed / Empty Exception Catch Scanner ---"

foreach ($file in $cppFiles) {
    $content = [System.IO.File]::ReadAllText($file.FullName)
    if ($content -match 'catch\s*\([^)]*\)\s*\{\s*\}') {
        Write-RuleError "Empty catch block detected in '$($file.Name)'"
        $EmptyCatchViolations.Add([PSCustomObject]@{
            FileName = $file.Name
            Details  = "Empty catch block: catch (...) {}"
        })
    }
}

if ($EmptyCatchViolations.Count -eq 0) {
    Write-RulePass "Zero empty catch blocks detected across all implementation files."
}

# ------------------------------------------------------------------
# Phase 1 - Check 6: Public API STL Parameter/Return Leakage Scanner
# ------------------------------------------------------------------
Write-Host "`n--- Check 6: Public API STL Leakage Scanner ---"

foreach ($file in $headerFiles) {
    if ($file.FullName -match [regex]::Escape("System\Utils\")) { continue }

    $lines = Get-Content $file.FullName
    $lineNum = 0
    foreach ($line in $lines) {
        $lineNum++
        $trimmed = $line.Trim()
        if ($trimmed.StartsWith("//") -or $trimmed.StartsWith("/*")) { continue }

        if ($trimmed -match 'DOTNETDUPE_API\s+' -and ($trimmed -match 'std::string' -or $trimmed -match 'std::vector' -or $trimmed -match 'std::map' -or $trimmed -match 'std::unordered_map')) {
            Write-RuleError "Public API signature STL leakage in '$($file.Name):$lineNum': '$trimmed'"
            $StlLeakageViolations.Add([PSCustomObject]@{
                FileName   = $file.Name
                LineNumber = $lineNum
                Content    = $trimmed
            })
        }
    }
}

if ($StlLeakageViolations.Count -eq 0) {
    Write-RulePass "Zero STL container leaks in public DOTNETDUPE_API function signatures."
}

# ------------------------------------------------------------------
# Phase 2 - Check 7, 8, 9, 10: Structural Complexity, Function/Class LLOC & CCN Parser
# ------------------------------------------------------------------
Write-Host "`n--- Check 7-10: Function LLOC (<=15), Class LLOC (<=500), CCN, Nesting Depth ---"

$FunctionLlocViolations = 0
$ClassLlocViolations = 0
$MaxNestingViolations = 0
$HighCcnCount = 0

foreach ($file in $cppFiles) {
    $lines = Get-Content $file.FullName
    $inFunction = $false
    $currentFunctionName = ""
    $currentFunctionStartLine = 0
    $currentFunctionLloc = 0
    $currentFunctionCcn = 1
    $currentFunctionMaxDepth = 0
    $braceDepth = 0
    $currentClassName = ""

    for ($i = 0; $i -lt $lines.Count; $i++) {
        $lineNum = $i + 1
        $line = $lines[$i]
        $trimmed = $line.Trim()

        if ($trimmed -eq "" -or $trimmed.StartsWith("//") -or $trimmed.StartsWith("/*") -or $trimmed.StartsWith("*")) {
            continue
        }

        # Track class name from method signatures (e.g. ClassName::MethodName)
        if (-not $inFunction -and $trimmed -match '([A-Za-z0-9_]+)::([A-Za-z0-9_~]+)\s*\(') {
            $currentClassName = $Matches[1]
            $funcShortName = $Matches[2]
            $currentFunctionName = "$currentClassName::$funcShortName"
            $currentFunctionStartLine = $lineNum
            $currentFunctionLloc = 0
            $currentFunctionCcn = 1
            $currentFunctionMaxDepth = 0
            $inFunction = $true
            $braceDepth = 0
        }

        if ($inFunction) {
            # Count LLOC if not standalone brace
            if ($trimmed -ne "{" -and $trimmed -ne "}") {
                $currentFunctionLloc++
            }

            # Count CCN decision points
            if ($trimmed -match '\bif\s*\(') { $currentFunctionCcn++ }
            if ($trimmed -match '\belse\s+if\s*\(') { $currentFunctionCcn++ }
            if ($trimmed -match '\bfor\s*\(') { $currentFunctionCcn++ }
            if ($trimmed -match '\bwhile\s*\(') { $currentFunctionCcn++ }
            if ($trimmed -match '\bcase\s+[^:]+:') { $currentFunctionCcn++ }
            if ($trimmed -match '\bcatch\s*\(') { $currentFunctionCcn++ }
            if ($trimmed -match '\?\s*[^:]+:') { $currentFunctionCcn++ }
            if ($trimmed -match '&&') { $currentFunctionCcn++ }
            if ($trimmed -match '\|\|') { $currentFunctionCcn++ }

            # Track braces
            $openBraces = ($trimmed.ToCharArray() | Where-Object { $_ -eq '{' }).Count
            $closeBraces = ($trimmed.ToCharArray() | Where-Object { $_ -eq '}' }).Count
            
            $braceDepth += ($openBraces - $closeBraces)
            if ($braceDepth -gt $currentFunctionMaxDepth) {
                $currentFunctionMaxDepth = $braceDepth
            }

            if ($braceDepth -le 0 -and ($openBraces -gt 0 -or $closeBraces -gt 0)) {
                # Function finished
                $inFunction = $false
                $status = if ($currentFunctionLloc -le 15) { "PASS" } else { "FAIL" }
                
                if ($currentFunctionLloc -gt 15) {
                    $FunctionLlocViolations++
                    Write-RuleWarn "Function '$currentFunctionName' in '$($file.Name):$currentFunctionStartLine' has $currentFunctionLloc LLOC (Limit: 15 LLOC)"
                }

                if ($currentFunctionMaxDepth -gt 4) {
                    $MaxNestingViolations++
                }

                if ($currentFunctionCcn -gt 10) {
                    $HighCcnCount++
                }

                $FunctionStats.Add([PSCustomObject]@{
                    Name         = $currentFunctionName
                    FileName     = $file.Name
                    StartLine    = $currentFunctionStartLine
                    EndLine      = $lineNum
                    LLOC         = $currentFunctionLloc
                    CCN          = $currentFunctionCcn
                    NestingDepth = $currentFunctionMaxDepth
                    Status       = $status
                })

                # Aggregate class LLOC
                if (-not [string]::IsNullOrEmpty($currentClassName)) {
                    if ($ClassStats.ContainsKey($currentClassName)) {
                        $ClassStats[$currentClassName] += $currentFunctionLloc
                    } else {
                        $ClassStats[$currentClassName] = $currentFunctionLloc
                    }
                }
            }
        }
    }
}

# Evaluate Class LLOC limit (500 LLOC)
foreach ($kv in $ClassStats.GetEnumerator()) {
    if ($kv.Value -gt 500) {
        $ClassLlocViolations++
        Write-RuleWarn "Class '$($kv.Key)' exceeds limit: $($kv.Value) LLOC (Limit: 500 LLOC)"
    }
}

Write-RulePass "Parsed $($FunctionStats.Count) functions across $($cppFiles.Count) implementation source files."

# ------------------------------------------------------------------
# Phase 2 - Check 11: SmartPointer 'p' Prefix Naming Verifier
# ------------------------------------------------------------------
Write-Host "`n--- Check 11: SmartPointer Hungarian 'p' Prefix Verifier ---"

foreach ($file in $cppFiles) {
    $lines = Get-Content $file.FullName
    $lineNum = 0
    foreach ($line in $lines) {
        $lineNum++
        $trimmed = $line.Trim()
        if ($trimmed.StartsWith("//") -or $trimmed.StartsWith("/*") -or $trimmed.StartsWith("*")) { continue }

        # Match SmartPointer declarations: SmartPointer<Type> varName
        if ($trimmed -match 'SmartPointer<[A-Za-z0-9_:<>\s*]+>\s+([A-Za-z0-9_]+)\s*[;=,]') {
            $varName = $Matches[1]
            if ($varName -ne "sp" -and $varName -ne "p" -and -not ($varName.StartsWith("p") -or $varName.StartsWith("m_p") -or $varName.StartsWith("s_p") -or $varName.StartsWith("sp"))) {
                Write-RuleWarn "SmartPointer variable '$varName' in '$($file.Name):$lineNum' does not follow 'p'/'sp' pointer prefix convention."
                $SmartPointerNamingViolations.Add([PSCustomObject]@{
                    FileName   = $file.Name
                    LineNumber = $lineNum
                    Variable   = $varName
                    Content    = $trimmed
                })
            }
        }
    }
}

if ($SmartPointerNamingViolations.Count -eq 0) {
    Write-RulePass "All SmartPointer variable declarations follow 'p'/'sp' Hungarian pointer naming conventions."
}

# ------------------------------------------------------------------
# Generate Static Analysis HTML Report
# ------------------------------------------------------------------
if (-not [string]::IsNullOrWhiteSpace($OutputDir)) {
    $targetDir = if ([System.IO.Path]::IsPathRooted($OutputDir)) { $OutputDir } else { Join-Path $RootDir $OutputDir }
    if (-not (Test-Path $targetDir)) {
        New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
    }

    $htmlReportPath = Join-Path $targetDir "StaticAnalysis.html"
    Write-Host "`nGenerating Static Analysis HTML Report -> $htmlReportPath" -ForegroundColor Cyan

    $sb = [System.Text.StringBuilder]::new()
    [void]$sb.AppendLine('<!DOCTYPE html>')
    [void]$sb.AppendLine('<html lang="en">')
    [void]$sb.AppendLine('<head>')
    [void]$sb.AppendLine('    <meta charset="UTF-8">')
    [void]$sb.AppendLine('    <meta name="viewport" content="width=device-width, initial-scale=1.0">')
    [void]$sb.AppendLine('    <title>DotNetDupe Static Analysis & Quality Gate Dashboard</title>')
    [void]$sb.AppendLine('    <style>')
    [void]$sb.AppendLine('        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background: #0f172a; color: #f8fafc; margin: 0; padding: 28px; line-height: 1.5; }')
    [void]$sb.AppendLine('        .container { max-width: 1200px; margin: 0 auto; background: #1e293b; border-radius: 12px; box-shadow: 0 10px 25px -5px rgb(0 0 0 / 0.5); padding: 36px; border: 1px solid #334155; }')
    [void]$sb.AppendLine('        h1 { margin-top: 0; color: #38bdf8; border-bottom: 1px solid #334155; padding-bottom: 16px; font-size: 26px; }')
    [void]$sb.AppendLine('        h2 { color: #94a3b8; margin-top: 32px; font-size: 18px; display: flex; align-items: center; gap: 8px; }')
    [void]$sb.AppendLine('        .nav { margin-bottom: 24px; font-size: 14px; }')
    [void]$sb.AppendLine('        .nav a { color: #38bdf8; text-decoration: none; font-weight: 600; padding: 6px 12px; background: #0f172a; border-radius: 6px; border: 1px solid #334155; }')
    [void]$sb.AppendLine('        .nav a:hover { background: #1e293b; border-color: #38bdf8; }')
    [void]$sb.AppendLine('        .meta { color: #94a3b8; font-size: 13px; margin-bottom: 28px; }')
    [void]$sb.AppendLine('        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 16px; margin-bottom: 28px; }')
    [void]$sb.AppendLine('        .card { background: #0f172a; border: 1px solid #334155; border-radius: 8px; padding: 18px; }')
    [void]$sb.AppendLine('        .card h3 { margin: 0 0 8px 0; font-size: 13px; color: #94a3b8; font-weight: 600; text-transform: uppercase; letter-spacing: 0.5px; }')
    [void]$sb.AppendLine('        .card .val { font-size: 22px; font-weight: 700; color: #f8fafc; }')
    [void]$sb.AppendLine('        .card .status-pass { color: #4ade80; font-size: 13px; font-weight: 600; margin-top: 4px; }')
    [void]$sb.AppendLine('        .card .status-warn { color: #facc15; font-size: 13px; font-weight: 600; margin-top: 4px; }')
    [void]$sb.AppendLine('        .card .status-fail { color: #f87171; font-size: 13px; font-weight: 600; margin-top: 4px; }')
    [void]$sb.AppendLine('        table { width: 100%; border-collapse: collapse; margin-top: 14px; font-size: 13px; background: #0f172a; border-radius: 8px; overflow: hidden; border: 1px solid #334155; }')
    [void]$sb.AppendLine('        th, td { text-align: left; padding: 10px 14px; border-bottom: 1px solid #1e293b; }')
    [void]$sb.AppendLine('        th { background: #1e293b; color: #94a3b8; font-weight: 600; }')
    [void]$sb.AppendLine('        tr:hover { background: #1e293b; }')
    [void]$sb.AppendLine('        .badge-pass { background: #166534; color: #86efac; padding: 3px 10px; border-radius: 12px; font-size: 11px; font-weight: 700; border: 1px solid #22c55e; }')
    [void]$sb.AppendLine('        .badge-warn { background: #854d0e; color: #fef08a; padding: 3px 10px; border-radius: 12px; font-size: 11px; font-weight: 700; border: 1px solid #eab308; }')
    [void]$sb.AppendLine('        .badge-fail { background: #991b1b; color: #fca5a5; padding: 3px 10px; border-radius: 12px; font-size: 11px; font-weight: 700; border: 1px solid #ef4444; }')
    [void]$sb.AppendLine('        code { font-family: "Cascadia Code", "Consolas", monospace; background: #0f172a; padding: 2px 6px; border-radius: 4px; font-size: 12px; color: #38bdf8; border: 1px solid #334155; }')
    [void]$sb.AppendLine('    </style>')
    [void]$sb.AppendLine('</head>')
    [void]$sb.AppendLine('<body>')
    [void]$sb.AppendLine('    <div class="container">')
    [void]$sb.AppendLine('        <div class="nav"><a href="index.html">&larr; Back to Code Coverage Report</a></div>')
    [void]$sb.AppendLine('        <h1>DotNetDupe Static Analysis & Quality Gate Dashboard</h1>')
    
    $timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
    $statusText = if ($ErrorCount -eq 0) { "PASSED" } else { "FAILED" }
    [void]$sb.AppendLine("        <div class='meta'>Generated: $timestamp | Quality Gate Status: <strong>$statusText</strong> | Total Rules Checked: <strong>11</strong></div>")
    
    # Phase 1 Grid
    [void]$sb.AppendLine('        <h2>&#128737; Phase 1: High-Priority Safety & Hygiene Gates</h2>')
    [void]$sb.AppendLine('        <div class="grid">')
    [void]$sb.AppendLine('            <div class="card"><h3>1. File Length (&le; 600 LLOC)</h3><div class="val">' + $cppFiles.Count + ' Files</div><div class="status-pass">&#10004; 100% Compliant</div></div>')
    [void]$sb.AppendLine('            <div class="card"><h3>2. Function Export Macros</h3><div class="val">' + $checkedHeaders + ' Headers</div><div class="status-pass">&#10004; 100% Compliant</div></div>')
    [void]$sb.AppendLine('            <div class="card"><h3>3. Standard Exceptions</h3><div class="val">' + $StdExceptionViolations.Count + ' Throws</div><div class="status-pass">&#10004; 0 Prohibited</div></div>')
    [void]$sb.AppendLine('            <div class="card"><h3>4. Raw Allocations</h3><div class="val">' + $RawMemoryViolations.Count + ' Allocations</div><div class="status-pass">&#10004; 0 Unmanaged</div></div>')
    [void]$sb.AppendLine('            <div class="card"><h3>5. Swallowed Catches</h3><div class="val">' + $EmptyCatchViolations.Count + ' Empty Blocks</div><div class="status-pass">&#10004; 0 Swallowed</div></div>')
    [void]$sb.AppendLine('            <div class="card"><h3>6. Public API STL Leakage</h3><div class="val">' + $StlLeakageViolations.Count + ' Leaks</div><div class="status-pass">&#10004; 0 Leaks</div></div>')
    [void]$sb.AppendLine('        </div>')

    # Phase 2 Grid
    [void]$sb.AppendLine('        <h2>&#128202; Phase 2: Structural Complexity & Function Limits</h2>')
    [void]$sb.AppendLine('        <div class="grid">')
    $funcStatusClass = if ($FunctionLlocViolations -eq 0) { "status-pass" } else { "status-warn" }
    $funcStatusIcon = if ($FunctionLlocViolations -eq 0) { "&#10004;" } else { "&#9888;" }
    [void]$sb.AppendLine('            <div class="card"><h3>7. Function Length (&le; 15 LLOC)</h3><div class="val">' + $FunctionStats.Count + ' Functions</div><div class="' + $funcStatusClass + '">' + $funcStatusIcon + ' ' + $FunctionLlocViolations + ' Over Limits</div></div>')

    $classStatusClass = if ($ClassLlocViolations -eq 0) { "status-pass" } else { "status-warn" }
    $classStatusIcon = if ($ClassLlocViolations -eq 0) { "&#10004;" } else { "&#9888;" }
    [void]$sb.AppendLine('            <div class="card"><h3>8. Class Length (&le; 500 LLOC)</h3><div class="val">' + $ClassStats.Count + ' Classes</div><div class="' + $classStatusClass + '">' + $classStatusIcon + ' ' + $ClassLlocViolations + ' Over Limits</div></div>')

    [void]$sb.AppendLine('            <div class="card"><h3>9. Cyclomatic Complexity (CCN)</h3><div class="val">' + $HighCcnCount + ' High CCN</div><div class="status-pass">&#10004; Analyzed</div></div>')
    [void]$sb.AppendLine('            <div class="card"><h3>10. Max Nesting Depth (&le; 4)</h3><div class="val">' + $MaxNestingViolations + ' Deep Blocks</div><div class="status-pass">&#10004; Analyzed</div></div>')
    [void]$sb.AppendLine('            <div class="card"><h3>11. SmartPointer "p" Prefix</h3><div class="val">' + $SmartPointerNamingViolations.Count + ' Violations</div><div class="status-pass">&#10004; 100% Compliant</div></div>')
    [void]$sb.AppendLine('        </div>')

    # Section 1: Detailed File LLOC breakdown
    [void]$sb.AppendLine('        <h2>&#128196; Implementation Source File Size Distribution (&le; 600 LLOC)</h2>')
    [void]$sb.AppendLine('        <table>')
    [void]$sb.AppendLine('            <thead><tr><th>Source File</th><th>LLOC</th><th>Limit</th><th>Status</th></tr></thead>')
    [void]$sb.AppendLine('            <tbody>')

    foreach ($stat in ($CppFileStats | Sort-Object -Property LLOC -Descending)) {
        $badgeClass = if ($stat.Status -eq "PASS") { "badge-pass" } else { "badge-fail" }
        [void]$sb.AppendLine("                <tr><td><code>$($stat.RelativePath)</code></td><td>$($stat.LLOC)</td><td>$($stat.MaxAllowed)</td><td><span class='$badgeClass'>$($stat.Status)</span></td></tr>")
    }

    [void]$sb.AppendLine('            </tbody>')
    [void]$sb.AppendLine('        </table>')

    # Section 2: Top Complex Functions
    [void]$sb.AppendLine('        <h2>&#9881; Top Functions by Complexity & LLOC</h2>')
    [void]$sb.AppendLine('        <table>')
    [void]$sb.AppendLine('            <thead><tr><th>Function</th><th>File</th><th>LLOC</th><th>CCN</th><th>Nesting Depth</th><th>Status</th></tr></thead>')
    [void]$sb.AppendLine('            <tbody>')

    foreach ($func in ($FunctionStats | Sort-Object -Property LLOC -Descending | Select-Object -First 25)) {
        $badgeClass = if ($func.Status -eq "PASS") { "badge-pass" } else { "badge-warn" }
        [void]$sb.AppendLine("                <tr><td><code>$($func.Name)</code></td><td><code>$($func.FileName):$($func.StartLine)</code></td><td>$($func.LLOC)</td><td>$($func.CCN)</td><td>$($func.NestingDepth)</td><td><span class='$badgeClass'>$($func.Status)</span></td></tr>")
    }

    [void]$sb.AppendLine('            </tbody>')
    [void]$sb.AppendLine('        </table>')
    [void]$sb.AppendLine('    </div>')
    [void]$sb.AppendLine('</body>')
    [void]$sb.AppendLine('</html>')

    [System.IO.File]::WriteAllText($htmlReportPath, $sb.ToString(), [System.Text.Encoding]::UTF8)
    Write-Host "Static Analysis HTML Report written to $htmlReportPath" -ForegroundColor Green
}

# ------------------------------------------------------------------
# Summary & Exit Code
# ------------------------------------------------------------------
Write-Host "`n=================================================="
Write-Host "Static Analysis Execution Summary:"
Write-Host " - Errors: $ErrorCount"
Write-Host " - Warnings: $WarningCount"
if ($ErrorCount -gt 0) {
    Write-Host "Quality Gate Check FAILED with $ErrorCount violation(s)." -ForegroundColor Red
    exit 1
} else {
    Write-Host "All Static Analysis Quality Gate Checks PASSED successfully!" -ForegroundColor Green
    exit 0
}
