<#
.SYNOPSIS
    Quality Gate Check & Static Analysis Report Generator for DotNetDupe.
.DESCRIPTION
    Checks LLOC (Logical Lines of Code) limits, verifies export macro conventions,
    and optionally generates a standalone HTML static analysis report.
#>

[CmdletBinding()]
param (
    [string]$RootDir = ".",
    [string]$OutputDir = ""
)

$ErrorCount = 0
$CppStats = [System.Collections.Generic.List[PSCustomObject]]::new()
$HeaderViolations = [System.Collections.Generic.List[PSCustomObject]]::new()

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

    $isPassed = ($lloc -le 600)
    if (-not $isPassed) {
        Write-RuleError "File '$($file.Name)' exceeds limit: $lloc LLOC (Max allowed: 600 LLOC)"
    }

    $resolvedRoot = (Resolve-Path $RootDir).Path
    $relativeFilePath = $file.FullName.Replace($resolvedRoot, "").TrimStart("\", "/")
    $CppStats.Add([PSCustomObject]@{
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
# Check 2: Public API Export Macro Verification
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
            Write-RuleError "Class level export macro detected in '$($file.Name):$lineNum': '$trimmed'"
            $HeaderViolations.Add([PSCustomObject]@{
                FileName   = $file.Name
                LineNumber = $lineNum
                Content    = $trimmed
            })
        }
    }
}

if ($HeaderViolations.Count -eq 0) {
    Write-RulePass "All $checkedHeaders header files comply with function-level DOTNETDUPE_API export conventions."
}

# ------------------------------------------------------------------
# Generate Static Analysis HTML Report (if OutputDir is specified)
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
    [void]$sb.AppendLine('    <title>DotNetDupe Static Analysis Report</title>')
    [void]$sb.AppendLine('    <style>')
    [void]$sb.AppendLine('        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background: #f8fafc; color: #0f172a; margin: 0; padding: 24px; }')
    [void]$sb.AppendLine('        .container { max-width: 1000px; margin: 0 auto; background: #ffffff; border-radius: 8px; box-shadow: 0 4px 6px -1px rgb(0 0 0 / 0.1); padding: 32px; }')
    [void]$sb.AppendLine('        h1 { margin-top: 0; color: #1e293b; border-bottom: 2px solid #e2e8f0; padding-bottom: 12px; font-size: 24px; }')
    [void]$sb.AppendLine('        h2 { color: #334155; margin-top: 28px; font-size: 18px; }')
    [void]$sb.AppendLine('        .nav { margin-bottom: 20px; font-size: 14px; }')
    [void]$sb.AppendLine('        .nav a { color: #2563eb; text-decoration: none; font-weight: 600; margin-right: 16px; }')
    [void]$sb.AppendLine('        .nav a:hover { text-decoration: underline; }')
    [void]$sb.AppendLine('        .meta { color: #64748b; font-size: 13px; margin-bottom: 24px; }')
    [void]$sb.AppendLine('        .alert { padding: 12px 16px; border-radius: 6px; margin-bottom: 20px; font-size: 14px; }')
    [void]$sb.AppendLine('        .alert.pass { background: #dcfce7; color: #166534; border: 1px solid #bbf7d0; }')
    [void]$sb.AppendLine('        .alert.fail { background: #fee2e2; color: #991b1b; border: 1px solid #fecaca; }')
    [void]$sb.AppendLine('        table { width: 100%; border-collapse: collapse; margin-top: 12px; font-size: 13px; }')
    [void]$sb.AppendLine('        th, td { text-align: left; padding: 8px 12px; border-bottom: 1px solid #e2e8f0; }')
    [void]$sb.AppendLine('        th { background: #f1f5f9; color: #475569; font-weight: 600; }')
    [void]$sb.AppendLine('        tr:hover { background: #f8fafc; }')
    [void]$sb.AppendLine('        .badge-pass { background: #10b981; color: white; padding: 2px 8px; border-radius: 12px; font-size: 11px; font-weight: bold; }')
    [void]$sb.AppendLine('        .badge-fail { background: #ef4444; color: white; padding: 2px 8px; border-radius: 12px; font-size: 11px; font-weight: bold; }')
    [void]$sb.AppendLine('        code { font-family: "Consolas", "Courier New", monospace; background: #f1f5f9; padding: 2px 4px; border-radius: 4px; font-size: 12px; }')
    [void]$sb.AppendLine('    </style>')
    [void]$sb.AppendLine('</head>')
    [void]$sb.AppendLine('<body>')
    [void]$sb.AppendLine('    <div class="container">')
    [void]$sb.AppendLine('        <div class="nav"><a href="index.html">&larr; Back to Code Coverage Report</a></div>')
    [void]$sb.AppendLine('        <h1>DotNetDupe Static Analysis & Quality Gate Report</h1>')
    
    $timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
    $statusText = if ($ErrorCount -eq 0) { "PASSED" } else { "FAILED" }
    [void]$sb.AppendLine("        <div class='meta'>Generated: $timestamp | Overall Quality Status: $statusText</div>")
    
    [void]$sb.AppendLine('        <h2>1. Header Export Macro Compliance (DOTNETDUPE_API)</h2>')
    if ($HeaderViolations.Count -eq 0) {
        [void]$sb.AppendLine("        <div class='alert pass'>&#10004; <strong>All $checkedHeaders headers passed:</strong> Zero class-level <code>DOTNETDUPE_API</code> violations found.</div>")
    } else {
        [void]$sb.AppendLine("        <div class='alert fail'>&#10008; <strong>$($HeaderViolations.Count) violation(s) found:</strong> Class-level <code>DOTNETDUPE_API</code> detected.</div>")
    }

    [void]$sb.AppendLine('        <h2>2. Implementation File Length Limit (Max 600 LLOC)</h2>')
    $cppAlertClass = if ($ErrorCount -eq 0) { "pass" } else { "fail" }
    [void]$sb.AppendLine("        <div class='alert $cppAlertClass'>Checked <strong>$($cppFiles.Count)</strong> C++ source files. Max allowed per file is <strong>600 LLOC</strong>.</div>")
    [void]$sb.AppendLine('        <table>')
    [void]$sb.AppendLine('            <thead><tr><th>Source File</th><th>LLOC</th><th>Limit</th><th>Status</th></tr></thead>')
    [void]$sb.AppendLine('            <tbody>')

    foreach ($stat in ($CppStats | Sort-Object -Property LLOC -Descending)) {
        $badgeClass = if ($stat.Status -eq "PASS") { "badge-pass" } else { "badge-fail" }
        [void]$sb.AppendLine("                <tr><td><code>$($stat.RelativePath)</code></td><td>$($stat.LLOC)</td><td>$($stat.MaxAllowed)</td><td><span class='$badgeClass'>$($stat.Status)</span></td></tr>")
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
