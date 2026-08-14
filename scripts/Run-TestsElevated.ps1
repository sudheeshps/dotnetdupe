<#
.SYNOPSIS
    Runs DotNetDupeTests with elevated administrator privileges by default.
.DESCRIPTION
    Self-elevates via UAC (Start-Process -Verb RunAs) if the current PowerShell session
    is not elevated, ensuring tests run with Administrator privileges by default.
#>

[CmdletBinding()]
param (
    [string]$Configuration = "Release",
    [string]$Platform = "x64",
    [string]$GTestFilter = "",
    [switch]$NoPause
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RootDir = Split-Path -Parent $ScriptDir
$TestExe = Join-Path $RootDir "bin\$Platform\$Configuration\DotNetDupeTests.exe"

if (-not (Test-Path $TestExe)) {
    Write-Error "Test executable not found at: $TestExe. Please build the solution first."
    exit 1
}

$CurrentIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
$Principal = [Security.Principal.WindowsPrincipal]$CurrentIdentity
$IsAdmin = $Principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

# Self-elevate if not running as Administrator
if (-not $IsAdmin) {
    Write-Host "Process is not elevated. Requesting Administrator elevation via UAC..." -ForegroundColor Cyan
    
    $scriptPath = $MyInvocation.MyCommand.Path
    $argList = @("-NoProfile", "-ExecutionPolicy", "Bypass", "-File", "`"$scriptPath`"", "-Configuration", "`"$Configuration`"", "-Platform", "`"$Platform`"")
    if ($GTestFilter) {
        $argList += @("-GTestFilter", "`"$GTestFilter`"")
    }
    if ($NoPause) {
        $argList += "-NoPause"
    }
    
    $process = Start-Process -FilePath "powershell.exe" -ArgumentList $argList -Verb RunAs -PassThru -Wait
    exit $process.ExitCode
}

# Running in elevated context
Write-Host "=================================================="
Write-Host " DotNetDupe Elevated Test Runner"
Write-Host " Running as Administrator: $IsAdmin"
Write-Host " User: $($CurrentIdentity.Name)"
Write-Host " Configuration: $Configuration | Platform: $Platform"
Write-Host "=================================================="

$testArgs = @()
if ($GTestFilter) {
    $testArgs += "--gtest_filter=$GTestFilter"
}

Write-Host "`nExecuting DotNetDupeTests.exe with elevated privileges..." -ForegroundColor Green
& $TestExe $testArgs
$ExitCode = $LASTEXITCODE

if (-not $NoPause -and ([Environment]::UserInteractive)) {
    Write-Host "`nPress any key to exit..." -ForegroundColor Gray
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
}

exit $ExitCode
