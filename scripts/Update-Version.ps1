param (
    [string]$Tag = $env:GITHUB_REF_NAME,
    [string]$VersionHeaderPath = "Include/Version.h"
)

# If no tag parameter or GITHUB_REF_NAME environment variable, try git describe or default to 1.0.0.0
if ([string]::IsNullOrWhitespace($Tag)) {
    try {
        $Tag = (git describe --tags --abbrev=0 2>$null)
    } catch {
        $Tag = "1.0.0.0"
    }
}

if ([string]::IsNullOrWhitespace($Tag)) {
    $Tag = "1.0.0.0"
}

# Clean version tag (strip leading 'v' or 'V')
$CleanVersion = $Tag -replace '^[vV]', ''

# Split into numeric components
$Parts = $CleanVersion.Split('.')

$Major    = if ($Parts.Length -gt 0 -and $Parts[0] -match '^\d+$') { [int]$Parts[0] } else { 1 }
$Minor    = if ($Parts.Length -gt 1 -and $Parts[1] -match '^\d+$') { [int]$Parts[1] } else { 0 }
$Build    = if ($Parts.Length -gt 2 -and $Parts[2] -match '^\d+$') { [int]$Parts[2] } else { 0 }
$Revision = if ($Parts.Length -gt 3 -and $Parts[3] -match '^\d+$') { [int]$Parts[3] } else { 
    if ($env:GITHUB_RUN_NUMBER) { [int]$env:GITHUB_RUN_NUMBER } else { 0 }
}

$VersionComma = "$Major,$Minor,$Build,$Revision"
$VersionStr   = "$Major.$Minor.$Build.$Revision"

Write-Host "Updating version header ($VersionHeaderPath) to: $VersionStr ($VersionComma)"

$HeaderContent = @"
#pragma once

#define VER_FILEVERSION             $VersionComma
#define VER_FILEVERSION_STR         "$VersionStr\0"

#define VER_PRODUCTVERSION          $VersionComma
#define VER_PRODUCTVERSION_STR      "$VersionStr\0"
"@

Set-Content -Path $VersionHeaderPath -Value $HeaderContent -Encoding UTF8
