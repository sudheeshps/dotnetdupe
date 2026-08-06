<#
.SYNOPSIS
    Full CI/CD pipeline for DotNetDupe: MSBuild (Release & Debug), CMake Windows (Release & Debug),
    CMake WSL (Debug), all tests, and NuGet packaging.

.DESCRIPTION
    Executes all build and test stages in sequence:
      [1] Update version from git tag
      [2] MSBuild x64 Release
      [3] Tests - MSBuild x64 Release
      [4] MSBuild x64 Debug
      [5] Tests - MSBuild x64 Debug
      [6] CMake Windows x64 Release
      [7] Tests - CMake Windows x64 Release
      [8] CMake Windows x64 Debug
      [9] Tests - CMake Windows x64 Debug
     [10] CMake WSL Debug
     [11] Tests - CMake WSL Debug
     [12] NuGet Pack (only if ALL preceding stages pass)

.PARAMETER SkipCMake
    Skip CMake Windows builds and tests.

.PARAMETER SkipWsl
    Skip CMake WSL build and tests.

.PARAMETER SkipTests
    Skip test execution stages.

.PARAMETER SkipNuGet
    Skip NuGet packaging stage.

.PARAMETER Clean
    Remove CMake build directories before configuring.

.PARAMETER NuGetOutputDir
    Output directory for the generated .nupkg file (default: nuget_packages).
#>
[CmdletBinding()]
param(
    [switch]$SkipCMake,
    [switch]$SkipWsl,
    [switch]$SkipTests,
    [switch]$SkipNuGet,
    [switch]$Clean,
    [string]$NuGetOutputDir = 'nuget_packages'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Continue'

$Script:RootDir    = $PSScriptRoot
$Script:StartTime  = [System.Diagnostics.Stopwatch]::StartNew()
$Script:Results    = [System.Collections.Specialized.OrderedDictionary]::new()
$Script:StepNumber = 0
$Script:LibVersion = '0.0.0.0'

function Write-Banner {
    param([string]$Text)
    $bar = '=' * 72
    Write-Host ''
    Write-Host $bar                 -ForegroundColor Cyan
    Write-Host "  $Text"            -ForegroundColor Cyan
    Write-Host $bar                 -ForegroundColor Cyan
}

function Write-StepHeader {
    param([string]$Label)
    $Script:StepNumber++
    Write-Host ''
    Write-Host "  [$($Script:StepNumber.ToString('D2'))] $Label" -ForegroundColor Yellow
    Write-Host "  $('-' * 64)"    -ForegroundColor DarkGray
}

function Write-Ok   { Write-Host "  [OK]   $args" -ForegroundColor Green  }
function Write-Fail { Write-Host "  [FAIL] $args" -ForegroundColor Red    }
function Write-Skip { Write-Host "  [SKIP] $args" -ForegroundColor DarkGray }
function Write-Info { Write-Host "    $args" -ForegroundColor Gray   }

function Record-Result {
    param([string]$Name, [bool]$Passed, [double]$Secs)
    $Script:Results[$Name] = [PSCustomObject]@{
        Passed = $Passed
        Secs   = [Math]::Round($Secs, 1)
    }
}

function All-Passed {
    foreach ($r in $Script:Results.Values) {
        if (-not $r.Passed) { return $false }
    }
    return $true
}

function Invoke-Step {
    param([string]$Name, [scriptblock]$Action)

    Write-StepHeader $Name
    $sw = [System.Diagnostics.Stopwatch]::StartNew()

    try {
        . $Action
        $code = $LASTEXITCODE
    } catch {
        Write-Fail "Unhandled exception: $_"
        $code = -1
    }

    $sw.Stop()
    $secs   = $sw.Elapsed.TotalSeconds
    $passed = ($code -eq 0)

    if ($passed) {
        Write-Ok "$Name  [$([Math]::Round($secs,1)) s]"
    } else {
        Write-Fail "$Name  exit=$code  [$([Math]::Round($secs,1)) s]"
    }

    Record-Result -Name $Name -Passed $passed -Secs $secs
    return $passed
}

function Find-CMake {
    $cmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    foreach ($p in @("$env:ProgramFiles\CMake\bin\cmake.exe",
                     "${env:ProgramFiles(x86)}\CMake\bin\cmake.exe")) {
        if (Test-Path $p) { return $p }
    }
    return $null
}

function Find-VSGenerator {
    param([string]$CMakePath)

    if ($CMakePath) {
        $help = & $CMakePath --help 2>$null
        foreach ($line in $help) {
            if ($line -match '^\*\s+(Visual Studio [^\=]+)') {
                return $Matches[1].Trim()
            }
        }
    }

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) {
        $vswhere = "$env:ProgramFiles\Microsoft Visual Studio\Installer\vswhere.exe"
    }
    if (-not (Test-Path $vswhere)) { return 'Visual Studio 17 2022' }

    $version = & $vswhere -latest -products * `
                   -requires Microsoft.Component.MSBuild `
                   -property installationVersion 2>$null
    if ($version -match '^(\d+)\.') {
        switch ($Matches[1]) {
            '18' { return 'Visual Studio 18 2026' }
            '17' { return 'Visual Studio 17 2022' }
            '16' { return 'Visual Studio 16 2019' }
            '15' { return 'Visual Studio 15 2017' }
        }
    }
    return 'Visual Studio 17 2022'
}

function Find-NuGet {
    $cmd = Get-Command nuget -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    $local = Join-Path $Script:RootDir '.nuget\nuget.exe'
    if (Test-Path $local) { return $local }
    return $null
}

function Test-WslAvailable {
    if (-not (Get-Command wsl -ErrorAction SilentlyContinue)) { return $false }
    $distros = (wsl --list --quiet 2>$null) | Where-Object { $_ -match '\S' }
    if ($null -eq $distros -or @($distros).Count -eq 0) { return $false }

    $hasCmake = wsl -e sh -c "command -v cmake" 2>$null
    return ($LASTEXITCODE -eq 0 -and $hasCmake)
}

function ConvertTo-WslPath {
    param([string]$WindowsPath)
    $p = $WindowsPath -replace '\\', '/'
    if ($p -match '^([A-Za-z]):(.*)') {
        return "/mnt/$($Matches[1].ToLower())$($Matches[2])"
    }
    return $p
}

function Update-RcTimestamp {
    $rc = Join-Path $Script:RootDir 'DotNetDupe\DotNetDupe.rc'
    if (-not (Test-Path $rc)) { return }
    $utc   = [DateTime]::UtcNow.ToString('yyyy-MM-dd HH:mm:ssZ')
    $local = [DateTime]::Now.ToString('yyyy-MM-dd HH:mm:ss K')
    $text  = Get-Content $rc -Raw
    $text  = $text -replace 'VALUE "BuildTime",\s*".*"',
                    "VALUE `"BuildTime`", `"$utc (Local: $local)\0`""
    Set-Content $rc $text -NoNewline
    Write-Info "RC timestamp: $utc"
}

function Update-LibVersion {
    $updateScript = Join-Path $Script:RootDir 'scripts\Update-Version.ps1'
    if (Test-Path $updateScript) {
        $versionHeader = Join-Path $Script:RootDir 'Include\Version.h'
        & $updateScript -VersionHeaderPath $versionHeader
        Write-Info "Version.h refreshed"
    }

    $verHeader = Join-Path $Script:RootDir 'Include\Version.h'
    if (Test-Path $verHeader) {
        $m = Select-String 'VER_FILEVERSION_STR\s+"([0-9.]+)' $verHeader
        if ($m) {
            $Script:LibVersion = $m.Matches[0].Groups[1].Value
            Write-Info "Library version: $Script:LibVersion"
        }
    }
}

function Invoke-MSBuild {
    param([string]$Configuration)

    Invoke-Step "MSBuild x64 $Configuration" {
        Push-Location $Script:RootDir
        try {
            Update-RcTimestamp
            & msbuild DotNetDupe.sln `
                /p:Configuration=$Configuration `
                /p:Platform=x64 `
                /v:m /nologo
        } finally {
            Pop-Location
        }
    }
}

function Invoke-MSBuildTests {
    param([string]$Configuration)

    Invoke-Step "Tests - MSBuild x64 $Configuration" {
        $exe = Join-Path $Script:RootDir "bin\x64\$Configuration\DotNetDupeTests.exe"
        if (-not (Test-Path $exe)) {
            Write-Fail "Binary not found: $exe"
            cmd /c "exit 1"
            return
        }
        & $exe
    }
}

function Invoke-CMakeWindows {
    param([string]$BuildType, [string]$CMakePath, [string]$Generator)

    $buildDir = Join-Path $Script:RootDir "out\build\x64-$BuildType"

    Invoke-Step "CMake Windows x64 $BuildType" {
        $cacheFile = Join-Path $buildDir "CMakeCache.txt"
        if ($Clean -or (Test-Path $cacheFile)) {
            # Check generator mismatch in cache
            if (Test-Path $cacheFile) {
                $cacheText = Get-Content $cacheFile -Raw
                if ($cacheText -notmatch [regex]::Escape("CMAKE_GENERATOR:INTERNAL=$Generator")) {
                    Write-Info "Generator mismatch in $buildDir; cleaning cache..."
                    Remove-Item $buildDir -Recurse -Force -ErrorAction SilentlyContinue
                }
            }
        }
        if (-not (Test-Path $buildDir)) {
            New-Item $buildDir -ItemType Directory -Force | Out-Null
        }
        Write-Info "Generator : $Generator"
        Write-Info "Build dir : $buildDir"

        Push-Location $buildDir
        try {
            & $CMakePath $Script:RootDir -G $Generator -A x64
            if ($LASTEXITCODE -ne 0) { return }
            & $CMakePath --build . --config $BuildType --parallel
        } finally {
            Pop-Location
        }
    }
}

function Invoke-CMakeWindowsTests {
    param([string]$BuildType, [string]$CTestPath)

    $buildDir = Join-Path $Script:RootDir "out\build\x64-$BuildType"

    Invoke-Step "Tests - CMake Windows x64 $BuildType" {
        if (-not (Test-Path $buildDir)) {
            Write-Fail "Build dir not found: $buildDir"
            cmd /c "exit 1"
            return
        }
        Push-Location $buildDir
        try {
            & $CTestPath --output-on-failure --build-config $BuildType --parallel 4
        } finally {
            Pop-Location
        }
    }
}

function Invoke-CMakeWsl {
    $wslRoot     = ConvertTo-WslPath $Script:RootDir
    $wslBuildDir = "$wslRoot/build-wsl"
    $cleanCmd    = if ($Clean) { "rm -rf '$wslBuildDir' && " } else { '' }

    $shScript = "set -e; ${cleanCmd}mkdir -p '$wslBuildDir'; cd '$wslBuildDir'; cmake .. -DCMAKE_BUILD_TYPE=Debug; cmake --build . --parallel 4"

    Invoke-Step "CMake WSL Debug" {
        Write-Info "WSL path: $wslRoot"
        wsl -e sh -c $shScript
    }
}

function Invoke-CMakeWslTests {
    $wslRoot     = ConvertTo-WslPath $Script:RootDir
    $wslBuildDir = "$wslRoot/build-wsl"

    Invoke-Step "Tests - CMake WSL Debug" {
        wsl -e sh -c "cd '$wslBuildDir' && ctest --output-on-failure --parallel 4"
    }
}

function Invoke-NuGetPack {
    param([string]$NuGetPath)

    $outDir  = Join-Path $Script:RootDir $NuGetOutputDir
    $nuspec  = Join-Path $Script:RootDir 'DotNetDupe.nuspec'
    $version = $Script:LibVersion

    Invoke-Step "NuGet Pack (v$version)" {
        if (-not (Test-Path $outDir)) {
            New-Item $outDir -ItemType Directory -Force | Out-Null
        }
        Write-Info "Output : $outDir"
        Write-Info "Version: $version"

        Push-Location $Script:RootDir
        try {
            & $NuGetPath pack $nuspec `
                -Version $version `
                -OutputDirectory $outDir `
                -NonInteractive
        } finally {
            Pop-Location
        }
    }
}

function Show-Summary {
    $total   = $Script:StartTime.Elapsed.TotalSeconds
    $allOk   = All-Passed
    $passed  = @($Script:Results.Values | Where-Object Passed).Count
    $count   = $Script:Results.Count

    Write-Host ''
    Write-Host ('=' * 72)                                     -ForegroundColor Cyan
    Write-Host '  PIPELINE SUMMARY'                            -ForegroundColor Cyan
    Write-Host ('=' * 72)                                      -ForegroundColor Cyan
    Write-Host ("  {0,-50} {1,-6} {2,8}" -f 'Stage', 'Result', 'Secs') `
        -ForegroundColor DarkGray
    Write-Host "  $('-' * 68)"                                 -ForegroundColor DarkGray

    foreach ($key in $Script:Results.Keys) {
        $r      = $Script:Results[$key]
        $symbol = if ($r.Passed) { 'PASS' } else { 'FAIL' }
        $colour = if ($r.Passed) { 'Green' } else { 'Red' }
        Write-Host ("  {0,-50} {1,-6} {2,8}" -f $key, $symbol, $r.Secs) -ForegroundColor $colour
    }

    Write-Host "  $('-' * 68)"                                 -ForegroundColor DarkGray
    Write-Host ''

    $statusColour = if ($allOk) { 'Green' } else { 'Red' }
    $statusText   = if ($allOk) { 'ALL STAGES PASSED' } else { "$($count - $passed) STAGE(S) FAILED" }

    Write-Host ("  {0,-30}  {1} / {2} passed    Total: {3:N1} s" -f `
        $statusText, $passed, $count, $total) -ForegroundColor $statusColour
    Write-Host ('=' * 72)                                      -ForegroundColor Cyan
    Write-Host ''

    return $allOk
}

# =============================================================================
#   MAIN PIPELINE
# =============================================================================
Write-Banner 'DotNetDupe - Full Build, Test & Packaging Pipeline'
Write-Info "Root   : $Script:RootDir"
Write-Info "Start  : $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
Write-Info "Options: SkipCMake=$SkipCMake  SkipWsl=$SkipWsl  SkipTests=$SkipTests  SkipNuGet=$SkipNuGet  Clean=$Clean"

# ── Tool detection ─────────────────────────────────────────────────────────────
Write-Host ''
Write-Host '  Detecting tools...' -ForegroundColor DarkGray

$cmakePath    = Find-CMake
$vsGenerator  = Find-VSGenerator -CMakePath $cmakePath
$ctestPath    = if ($cmakePath) { Join-Path (Split-Path $cmakePath) 'ctest.exe' } else { 'ctest' }
$nugetPath    = Find-NuGet
$wslAvailable = Test-WslAvailable

if ($cmakePath) {
    $cmakeVer = (& $cmakePath --version 2>$null | Select-Object -First 1)
    Write-Info "cmake  : $cmakePath  ($cmakeVer)"
} else {
    Write-Info "cmake  : NOT FOUND - CMake stages will be skipped"
    $SkipCMake = $true
    $SkipWsl   = $true
}

Write-Info "VSgen  : $vsGenerator"

if ($nugetPath) {
    Write-Info "nuget  : $nugetPath"
} else {
    Write-Info "nuget  : NOT FOUND - NuGet stage will be skipped"
    $SkipNuGet = $true
}

if ($wslAvailable) {
    Write-Info "WSL    : available (with cmake)"
} else {
    Write-Info "WSL    : not available or cmake not installed in WSL - WSL stage will be skipped"
    $SkipWsl = $true
}

# ── [1] Version stamp ──────────────────────────────────────────────────────────
Write-StepHeader 'Version stamp from git tag'
try {
    Update-LibVersion
    Write-Ok "Version: $Script:LibVersion"
    Record-Result 'Version Stamp' $true 0
} catch {
    Write-Info "Version update skipped: $_"
    Record-Result 'Version Stamp' $true 0
}

# ── [2-3] MSBuild Release ──────────────────────────────────────────────────────
$msBuildRelOk = Invoke-MSBuild -Configuration Release

if (-not $SkipTests) {
    if ($msBuildRelOk) {
        Invoke-MSBuildTests -Configuration Release
    } else {
        Write-StepHeader 'Tests - MSBuild x64 Release'
        Write-Skip 'Skipped - MSBuild Release failed'
        Record-Result 'Tests - MSBuild x64 Release' $false 0
    }
}

# ── [4-5] MSBuild Debug ────────────────────────────────────────────────────────
$msBuildDbgOk = Invoke-MSBuild -Configuration Debug

if (-not $SkipTests) {
    if ($msBuildDbgOk) {
        Invoke-MSBuildTests -Configuration Debug
    } else {
        Write-StepHeader 'Tests - MSBuild x64 Debug'
        Write-Skip 'Skipped - MSBuild Debug failed'
        Record-Result 'Tests - MSBuild x64 Debug' $false 0
    }
}

# ── [6-7] CMake Windows Release ───────────────────────────────────────────────
if (-not $SkipCMake) {
    $cmakeWinRelOk = Invoke-CMakeWindows Release $cmakePath $vsGenerator

    if (-not $SkipTests) {
        if ($cmakeWinRelOk) {
            Invoke-CMakeWindowsTests Release $ctestPath
        } else {
            Write-StepHeader 'Tests - CMake Windows x64 Release'
            Write-Skip 'Skipped - CMake Windows Release failed'
            Record-Result 'Tests - CMake Windows x64 Release' $false 0
        }
    }
} else {
    Write-Skip 'CMake Windows Release - skipped (-SkipCMake)'
}

# ── [8-9] CMake Windows Debug ─────────────────────────────────────────────────
if (-not $SkipCMake) {
    $cmakeWinDbgOk = Invoke-CMakeWindows Debug $cmakePath $vsGenerator

    if (-not $SkipTests) {
        if ($cmakeWinDbgOk) {
            Invoke-CMakeWindowsTests Debug $ctestPath
        } else {
            Write-StepHeader 'Tests - CMake Windows x64 Debug'
            Write-Skip 'Skipped - CMake Windows Debug failed'
            Record-Result 'Tests - CMake Windows x64 Debug' $false 0
        }
    }
} else {
    Write-Skip 'CMake Windows Debug - skipped (-SkipCMake)'
}

# ── [10-11] CMake WSL ─────────────────────────────────────────────────────────
if (-not $SkipWsl) {
    $cmakeWslOk = Invoke-CMakeWsl

    if (-not $SkipTests) {
        if ($cmakeWslOk) {
            Invoke-CMakeWslTests
        } else {
            Write-StepHeader 'Tests - CMake WSL Debug'
            Write-Skip 'Skipped - CMake WSL build failed'
            Record-Result 'Tests - CMake WSL Debug' $false 0
        }
    }
} else {
    Write-Skip 'CMake WSL - skipped (-SkipWsl)'
}

# ── [12] NuGet packaging ──────────────────────────────────────────────────────
if (-not $SkipNuGet) {
    if (All-Passed) {
        Invoke-NuGetPack -NuGetPath $nugetPath
    } else {
        Write-StepHeader 'NuGet Pack'
        Write-Skip 'Skipped - one or more earlier stages failed'
        Record-Result 'NuGet Pack' $false 0
    }
} else {
    Write-Skip 'NuGet Pack - skipped (-SkipNuGet)'
}

# ── Final summary ──────────────────────────────────────────────────────────────
$allPassed = Show-Summary
exit $(if ($allPassed) { 0 } else { 1 })
