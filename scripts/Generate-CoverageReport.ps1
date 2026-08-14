# Generate-CoverageReport.ps1 - Builds, runs OpenCppCoverage, and generates static analysis reports into CodeCoverage/
[CmdletBinding()]
param(
    [string]$Configuration = "Release",
    [string]$OutputDir = "CodeCoverage",
    [switch]$Open
)

$ErrorActionPreference = "Stop"
$RootDir = Split-Path $PSScriptRoot -Parent

Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host " Generating Code Coverage & Static Analysis Reports" -ForegroundColor Cyan
Write-Host " Output Directory: $OutputDir" -ForegroundColor Cyan
Write-Host "==========================================================" -ForegroundColor Cyan

# 1. Locate OpenCppCoverage
$exePath = "C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe"
if (-not (Test-Path $exePath)) {
    $cmd = Get-Command OpenCppCoverage -ErrorAction SilentlyContinue
    if ($cmd) {
        $exePath = $cmd.Source
    } else {
        Throw "OpenCppCoverage.exe not found! Please install via 'choco install opencppcoverage -y' or add to PATH."
    }
}

# 2. Build Release x64 solution if tests binary does not exist
$testsExe = Join-Path $RootDir "bin\x64\$Configuration\DotNetDupeTests.exe"
if (-not (Test-Path $testsExe)) {
    Write-Host "[1/3] Building solution ($Configuration x64)..." -ForegroundColor Yellow
    & msbuild (Join-Path $RootDir "DotNetDupe.sln") /p:Configuration=$Configuration /p:Platform=x64 /v:m /nologo
    if ($LASTEXITCODE -ne 0) {
        Throw "MSBuild failed with exit code $LASTEXITCODE"
    }
}

# 3. Ensure Output Directory exists
$targetOutDir = if ([System.IO.Path]::IsPathRooted($OutputDir)) { $OutputDir } else { Join-Path $RootDir $OutputDir }
if (-not (Test-Path $targetOutDir)) {
    New-Item -ItemType Directory -Path $targetOutDir -Force | Out-Null
}

# 4. Run OpenCppCoverage with stripped date directory
Write-Host "[2/3] Running OpenCppCoverage against tests..." -ForegroundColor Yellow
Push-Location $RootDir
try {
    & $exePath --modules DotNetDupe.dll --sources DotNetDupe --excluded_sources DotNetDupeTests --export_type "html:$OutputDir" -- $testsExe
    if ($LASTEXITCODE -ne 0) {
        Write-Warning "Test runner exited with code $LASTEXITCODE"
    }
} finally {
    Pop-Location
}

# 5. Run Static Analysis / Quality Gates Report
Write-Host "[3/3] Generating Static Analysis Report..." -ForegroundColor Yellow
$qualityGateScript = Join-Path $PSScriptRoot "Check-QualityGates.ps1"
& $qualityGateScript -RootDir $RootDir -OutputDir $targetOutDir

Write-Host "==========================================================" -ForegroundColor Green
Write-Host " Code Coverage & Static Analysis Reports Generated in '$OutputDir'!" -ForegroundColor Green
Write-Host " Index: $(Join-Path $targetOutDir 'index.html')" -ForegroundColor Green
Write-Host " Static Analysis: $(Join-Path $targetOutDir 'StaticAnalysis.html')" -ForegroundColor Green
Write-Host "==========================================================" -ForegroundColor Green

if ($Open) {
    Start-Process (Join-Path $targetOutDir "index.html")
}
