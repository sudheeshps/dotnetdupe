<#
.SYNOPSIS
    Generates Doxygen API Documentation for DotNetDupe.
.DESCRIPTION
    Locates doxygen, executes generation using the root Doxyfile, and outputs
    compiled HTML API reference to docs/html/. Also verifies docs/index.html.
.PARAMETER OpenBrowser
    Switch to automatically launch docs/index.html in default browser.
#>

[CmdletBinding()]
param (
    [string]$Version,
    [switch]$OpenBrowser
)

$ErrorActionPreference = "Stop"
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Resolve-Path (Join-Path $scriptDir "..")

Write-Host "==================================================" -ForegroundColor Cyan
Write-Host " DotNetDupe API Documentation Generator" -ForegroundColor Cyan
Write-Host "==================================================" -ForegroundColor Cyan

# Resolve dynamic package version if not explicitly passed
if ([string]::IsNullOrWhitespace($Version)) {
    # 1. Primary: Query the published version from nuget.org
    try {
        Write-Host "[INFO] Checking published version from nuget.org..." -ForegroundColor Gray
        $nugetMeta = Invoke-RestMethod -Uri "https://api.nuget.org/v3-flatcontainer/dotnetdupe/index.json" -TimeoutSec 5 -ErrorAction Stop
        if ($nugetMeta -and $nugetMeta.versions -and $nugetMeta.versions.Count -gt 0) {
            $Version = $nugetMeta.versions[-1].Trim()
            Write-Host "[INFO] Detected published NuGet version from nuget.org: $Version" -ForegroundColor Green
        }
    } catch {
        Write-Warning "Could not retrieve version from nuget.org: $_"
    }

    # 2. Fallback to DotNetDupe.nuspec if offline / nuget.org unreachable
    if ([string]::IsNullOrWhitespace($Version)) {
        $nuspecPath = Join-Path $rootDir "DotNetDupe.nuspec"
        if (Test-Path $nuspecPath) {
            try {
                [xml]$nuspec = Get-Content $nuspecPath
                if ($nuspec.package.metadata.version) {
                    $Version = $nuspec.package.metadata.version.Trim()
                }
            } catch {
                Write-Warning "Could not parse '$nuspecPath': $_"
            }
        }
    }

    # 3. Fallback to Include/Version.h
    if ([string]::IsNullOrWhitespace($Version)) {
        $versionHeader = Join-Path $rootDir "Include\Version.h"
        if (Test-Path $versionHeader) {
            $match = Select-String -Path $versionHeader -Pattern 'VER_FILEVERSION_STR\s+"([^\\"]+)'
            if ($match -and $match.Matches.Groups.Count -gt 1) {
                $rawVer = $match.Matches.Groups[1].Value.Trim()
                $Version = $rawVer -replace '\.0$', ''
            }
        }
    }

    # 4. Default fallback
    if ([string]::IsNullOrWhitespace($Version)) {
        $Version = "4.0.3"
    }
}

Write-Host "[INFO] Package Version: $Version" -ForegroundColor Green

# Synchronize Doxyfile PROJECT_NUMBER
$doxyfile = Join-Path $rootDir "Doxyfile"
if (-not (Test-Path $doxyfile)) {
    Write-Error "Doxyfile not found at '$doxyfile'."
}

$doxyfileContent = Get-Content $doxyfile -Raw
$updatedDoxyfile = $doxyfileContent -replace 'PROJECT_NUMBER\s*=\s*"[^"]*"', "PROJECT_NUMBER         = `"$Version`""
if ($updatedDoxyfile -ne $doxyfileContent) {
    Set-Content -Path $doxyfile -Value $updatedDoxyfile -Encoding UTF8
    Write-Host "[INFO] Synchronized Doxyfile PROJECT_NUMBER to '$Version'." -ForegroundColor Gray
}

# Synchronize docs/index.html header version badge
$portalHtml = Join-Path $rootDir "docs\index.html"
if (Test-Path $portalHtml) {
    $portalContent = Get-Content $portalHtml -Raw
    $updatedPortal = $portalContent -replace '🚀 Version [^&<]+', "🚀 Version $Version "
    if ($updatedPortal -ne $portalContent) {
        Set-Content -Path $portalHtml -Value $updatedPortal -Encoding UTF8
        Write-Host "[INFO] Synchronized docs/index.html version badge to '$Version'." -ForegroundColor Gray
    }
}

# Locate doxygen
$doxygenCmd = Get-Command doxygen -ErrorAction SilentlyContinue
$doxygenPath = ""

if ($doxygenCmd) {
    $doxygenPath = $doxygenCmd.Source
} elseif (Test-Path "C:\Program Files\doxygen\bin\doxygen.exe") {
    $doxygenPath = "C:\Program Files\doxygen\bin\doxygen.exe"
} else {
    Write-Error "Doxygen executable was not found in PATH or at 'C:\Program Files\doxygen\bin\doxygen.exe'. Please install Doxygen to proceed."
}

Write-Host "[INFO] Using Doxygen at: $doxygenPath" -ForegroundColor Gray

# Run Doxygen
Write-Host "[INFO] Generating API documentation from $doxyfile (Version: $Version)..." -ForegroundColor Yellow
$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()

Push-Location $rootDir
try {
    & $doxygenPath $doxyfile
} finally {
    Pop-Location
}

$stopwatch.Stop()
$elapsed = [math]::Round($stopwatch.Elapsed.TotalSeconds, 2)

$outputHtml = Join-Path $rootDir "docs\html\index.html"
$portalHtml = Join-Path $rootDir "docs\index.html"

if (Test-Path $outputHtml) {
    Write-Host "`n[SUCCESS] API Documentation generated in $elapsed seconds!" -ForegroundColor Green
    Write-Host "  -> API Reference: $outputHtml" -ForegroundColor Green
    Write-Host "  -> Docs Portal:   $portalHtml" -ForegroundColor Green

    if ($OpenBrowser -and (Test-Path $portalHtml)) {
        Start-Process $portalHtml
    }
} else {
    Write-Error "Failed to locate generated documentation at '$outputHtml'."
}
