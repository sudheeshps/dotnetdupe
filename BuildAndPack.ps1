# BuildAndPack.ps1 - Automated Build, Timestamp Update, and Package Script for DotNetDupe
param(
    [string]$Configuration = "Release",
    [string]$OutputDirectory = "nuget_packages"
)

$ErrorActionPreference = "Stop"

Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host " Building & Packaging DotNetDupe Library" -ForegroundColor Cyan
Write-Host " Configuration: $Configuration" -ForegroundColor Cyan
Write-Host " Output Directory: $OutputDirectory" -ForegroundColor Cyan
Write-Host "==========================================================" -ForegroundColor Cyan

# 1. Update timestamp in DotNetDupe.rc before build
$projectDir = Join-Path $PSScriptRoot "DotNetDupe"
$rcPath = Join-Path $projectDir "DotNetDupe.rc"

if (Test-Path $rcPath) {
    Write-Host "[1/4] Updating build timestamp in DotNetDupe.rc..." -ForegroundColor Yellow
    $utc = [DateTime]::UtcNow.ToString('yyyy-MM-dd HH:mm:ssZ')
    $local = [DateTime]::Now.ToString('yyyy-MM-dd HH:mm:ss K')
    $rcContent = Get-Content $rcPath -Raw
    $updatedRc = $rcContent -replace 'VALUE "BuildTime",\s*".*"', ("VALUE `"BuildTime`", `"" + $utc + " (Local: " + $local + ")\0`"")
    Set-Content -Path $rcPath -Value $updatedRc -NoNewline
    Write-Host "   Updated timestamp: UTC $utc | Local $local" -ForegroundColor Green
} else {
    Write-Warning "DotNetDupe.rc not found at $rcPath"
}

# 2. Build x64 Solution
Write-Host "[2/4] Building solution (x64 $Configuration)..." -ForegroundColor Yellow
& msbuild DotNetDupe.sln /p:Configuration=$Configuration /p:Platform=x64 /v:m
if ($LASTEXITCODE -ne 0) {
    Throw "MSBuild x64 build failed with exit code $LASTEXITCODE"
}
Write-Host "   x64 Build successful." -ForegroundColor Green

# 3. Build Win32 (x86) Solution
Write-Host "[3/4] Building solution (x86 $Configuration)..." -ForegroundColor Yellow
& msbuild DotNetDupe.sln /p:Configuration=$Configuration /p:Platform=x86 /v:m
if ($LASTEXITCODE -ne 0) {
    Throw "MSBuild x86 build failed with exit code $LASTEXITCODE"
}
Write-Host "   x86 Build successful." -ForegroundColor Green

# 4. Pack NuGet Package
Write-Host "[4/4] Creating NuGet Package using DotNetDupe.nuspec..." -ForegroundColor Yellow
$checkQualityGates = Join-Path $PSScriptRoot "scripts\Check-QualityGates.ps1"
if (Test-Path $checkQualityGates) {
    & $checkQualityGates
    if ($LASTEXITCODE -ne 0) {
        Throw "Check-QualityGates failed with exit code $LASTEXITCODE"
    }
}

if (-not (Test-Path $OutputDirectory)) {
    New-Item -ItemType Directory -Path $OutputDirectory -Force | Out-Null
}

$nuspecPath = Join-Path $PSScriptRoot "DotNetDupe.nuspec"
& nuget pack $nuspecPath -OutputDirectory $OutputDirectory
if ($LASTEXITCODE -ne 0) {
    Throw "NuGet pack failed with exit code $LASTEXITCODE"
}

Write-Host "==========================================================" -ForegroundColor Green
Write-Host " SUCCESS: DotNetDupe package created in '$OutputDirectory'" -ForegroundColor Green
Write-Host "==========================================================" -ForegroundColor Green
