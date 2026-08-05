param(
    [string]$OpensslConf,
    [string]$FipsModule
)

Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

# === Path resolution ===
function Resolve-AnyPath {
    param([string]$path)
    
    if ([string]::IsNullOrWhiteSpace($path)) { return $path }
    try {
        if ([System.IO.Path]::IsPathRooted($path)) {
            return [System.IO.Path]::GetFullPath($path)
        }
        return [System.IO.Path]::GetFullPath((Join-Path (Get-Location).Path $path))
    } catch {
        return $path
    }
}

# Depends on:
# - $script:scriptDir
# - $script:opensslExe
function Find-DefaultOpensslConf {
    # 1. OPENSSL_CONF environment variable (respects user/system overrides)
    if ($env:OPENSSL_CONF) {
        $candidate = Resolve-AnyPath $env:OPENSSL_CONF
        if (Test-Path -LiteralPath $candidate) { return $candidate }
    }

    # 2. Relative: two levels up from script dir, then ssl\openssl.cnf
    #    (matches the original assumption about script placement)
    $candidate = Resolve-AnyPath (Join-Path $script:scriptDir '..\..\ssl\openssl.cnf')
    if (Test-Path -LiteralPath $candidate) { return $candidate }

    # 3. Fallback to baked in `OPENSSL_DIR`
    if (Test-Path -LiteralPath $script:opensslExe) {
        try {
            $output = & $script:opensslExe version -d 2>&1
            if ($LASTEXITCODE -eq 0) {
                $candidate = output.split('"')[1]
                if (Test-Path -LiteralPath $candidate) { return $candidate }
            }
        } catch {
        }
    }

    # 4. Known FireDaemon SSL 4 location under Common Files
    $commonFiles = $env:CommonProgramFiles   # Handles both 32/64-bit correctly
    if ($commonFiles) {
        return Resolve-AnyPath (Join-Path $commonFiles 'FireDaemon SSL 4\openssl.cnf')
    }

    # 5. Absolute last resort: return the relative path (will not exist)
    return Resolve-AnyPath (Join-Path $scriptDir '..\..\ssl\openssl.cnf')
}

$scriptDir = if ($PSCommandPath) { Split-Path -Parent $PSCommandPath } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$script:opensslExe = Resolve-AnyPath (Join-Path $scriptDir 'openssl.exe')

# unused, just for completeness. Path is determined by Find-DefaultOpensslConf
$defaultOpensslConf = Resolve-AnyPath (Join-Path $scriptDir '..\..\ssl\openssl.cnf')
$defaultFipsModule  = Resolve-AnyPath (Join-Path $scriptDir '..\lib\ossl-modules\fips.dll')

$script:opensslConfFromCli = -not [string]::IsNullOrWhiteSpace($OpensslConf)
$script:fipsModuleFromCli  = -not [string]::IsNullOrWhiteSpace($FipsModule)

if ($script:opensslConfFromCli) {
    $initialOpensslConfPath = Resolve-AnyPath $OpensslConf
} else {
    $initialOpensslConfPath = Find-DefaultOpensslConf
}

$fipsModuleAutoFound = Test-Path -LiteralPath $defaultFipsModule
if ($script:fipsModuleFromCli) {
    $initialFipsDllPath = Resolve-AnyPath $FipsModule
} elseif ($fipsModuleAutoFound) {
    $initialFipsDllPath = $defaultFipsModule
} else {
    $initialFipsDllPath = ''
}

$script:opensslConfLocked = $script:opensslConfFromCli
$script:fipsDllLocked     = $script:fipsModuleFromCli -or ($script:opensslConfFromCli -and $fipsModuleAutoFound)

$script:suppressRegen = $false

# === Form ===
$form = New-Object System.Windows.Forms.Form
$form.Text = "OpenSSL Configuration Editor"
$form.Size = New-Object System.Drawing.Size(900,680)
$form.StartPosition = "CenterScreen"
$form.FormBorderStyle = "FixedDialog"
$form.MaximizeBox = $false

# === Tab Control ===
$tabControl = New-Object System.Windows.Forms.TabControl
$tabControl.Location = New-Object System.Drawing.Point(10,10)
$tabControl.Size = New-Object System.Drawing.Size(860,590)

# ==================== TAB 1: FIPS Configuration ====================
$tabFips = New-Object System.Windows.Forms.TabPage
$tabFips.Text = "FIPS Configuration"

# --- Files group ---
$groupFiles = New-Object System.Windows.Forms.GroupBox
$groupFiles.Text = "Files"
$groupFiles.Location = New-Object System.Drawing.Point(10,10)
$groupFiles.Size = New-Object System.Drawing.Size(820,90)

$lblOpensslConf = New-Object System.Windows.Forms.Label
$lblOpensslConf.Text = "OpenSSL Config (openssl.cnf):"
$lblOpensslConf.Location = New-Object System.Drawing.Point(20,25)
$lblOpensslConf.Size = New-Object System.Drawing.Size(200,20)

$txtOpensslConfPath = New-Object System.Windows.Forms.TextBox
$txtOpensslConfPath.Location = New-Object System.Drawing.Point(220,23)
$txtOpensslConfPath.Size = New-Object System.Drawing.Size(460,20)
$txtOpensslConfPath.Text = $initialOpensslConfPath
if ($script:opensslConfLocked) {
    $txtOpensslConfPath.ReadOnly = $true
    $txtOpensslConfPath.BackColor = [System.Drawing.SystemColors]::Control
}

$btnBrowseOpensslConf = New-Object System.Windows.Forms.Button
$btnBrowseOpensslConf.Text = "Browse..."
$btnBrowseOpensslConf.Location = New-Object System.Drawing.Point(690,22)
$btnBrowseOpensslConf.Size = New-Object System.Drawing.Size(100,25)
$btnBrowseOpensslConf.Add_Click({ Browse-OpensslConf })
$btnBrowseOpensslConf.Visible = -not $script:opensslConfLocked

$lblFipsDll = New-Object System.Windows.Forms.Label
$lblFipsDll.Text = "FIPS Provider DLL (fips.dll):"
$lblFipsDll.Location = New-Object System.Drawing.Point(20,55)
$lblFipsDll.Size = New-Object System.Drawing.Size(200,20)

$txtFipsDllPath = New-Object System.Windows.Forms.TextBox
$txtFipsDllPath.Location = New-Object System.Drawing.Point(220,53)
$txtFipsDllPath.Size = New-Object System.Drawing.Size(460,20)
$txtFipsDllPath.Text = $initialFipsDllPath
if ($script:fipsDllLocked) {
    $txtFipsDllPath.ReadOnly = $true
    $txtFipsDllPath.BackColor = [System.Drawing.SystemColors]::Control
}

$btnBrowseFipsDll = New-Object System.Windows.Forms.Button
$btnBrowseFipsDll.Text = "Browse..."
$btnBrowseFipsDll.Location = New-Object System.Drawing.Point(690,52)
$btnBrowseFipsDll.Size = New-Object System.Drawing.Size(100,25)
$btnBrowseFipsDll.Add_Click({ Browse-FipsDll })
$btnBrowseFipsDll.Visible = -not $script:fipsDllLocked

$groupFiles.Controls.AddRange(@($lblOpensslConf, $txtOpensslConfPath, $btnBrowseOpensslConf, $lblFipsDll, $txtFipsDllPath, $btnBrowseFipsDll))

# --- FIPS Mode group ---
$groupFipsMode = New-Object System.Windows.Forms.GroupBox
$groupFipsMode.Text = "FIPS Mode"
$groupFipsMode.Location = New-Object System.Drawing.Point(10,110)
$groupFipsMode.Size = New-Object System.Drawing.Size(820,100)

$radioStrictFips = New-Object System.Windows.Forms.RadioButton
$radioStrictFips.Text = "Strict FIPS Mode (FIPS + Base providers only)"
$radioStrictFips.Location = New-Object System.Drawing.Point(20,25)
$radioStrictFips.Size = New-Object System.Drawing.Size(500,20)

$radioFlexible = New-Object System.Windows.Forms.RadioButton
$radioFlexible.Text = "Flexible Mode (FIPS + Base + Default providers)"
$radioFlexible.Location = New-Object System.Drawing.Point(20,50)
$radioFlexible.Size = New-Object System.Drawing.Size(500,20)

$radioNormal = New-Object System.Windows.Forms.RadioButton
$radioNormal.Text = "Normal Mode (Default provider only)"
$radioNormal.Location = New-Object System.Drawing.Point(20,75)
$radioNormal.Size = New-Object System.Drawing.Size(500,20)
$radioNormal.Checked = $true

$radioHandler = {
    Update-CheckboxDisplay
    if ($script:suppressRegen) { return }
    Regenerate-Preview
}
$radioStrictFips.Add_CheckedChanged($radioHandler)
$radioFlexible.Add_CheckedChanged($radioHandler)
$radioNormal.Add_CheckedChanged($radioHandler)

$groupFipsMode.Controls.AddRange(@($radioStrictFips, $radioFlexible, $radioNormal))

# --- Active Providers group (informational) ---
$groupProviders = New-Object System.Windows.Forms.GroupBox
$groupProviders.Text = "Active Providers (informational)"
$groupProviders.Location = New-Object System.Drawing.Point(10,220)
$groupProviders.Size = New-Object System.Drawing.Size(820,100)

$chkFipsProvider = New-Object System.Windows.Forms.CheckBox
$chkFipsProvider.Text = "FIPS Provider (FIPS-approved algorithms only)"
$chkFipsProvider.Location = New-Object System.Drawing.Point(20,25)
$chkFipsProvider.Size = New-Object System.Drawing.Size(400,20)
$chkFipsProvider.Enabled = $false
$chkFipsProvider.AutoCheck = $false

$chkBaseProvider = New-Object System.Windows.Forms.CheckBox
$chkBaseProvider.Text = "Base Provider (Encoding/decoding operations)"
$chkBaseProvider.Location = New-Object System.Drawing.Point(20,50)
$chkBaseProvider.Size = New-Object System.Drawing.Size(400,20)
$chkBaseProvider.Enabled = $false
$chkBaseProvider.AutoCheck = $false

$chkDefaultProvider = New-Object System.Windows.Forms.CheckBox
$chkDefaultProvider.Text = "Default Provider (All standard algorithms)"
$chkDefaultProvider.Location = New-Object System.Drawing.Point(20,75)
$chkDefaultProvider.Size = New-Object System.Drawing.Size(400,20)
$chkDefaultProvider.Checked = $true
$chkDefaultProvider.Enabled = $false
$chkDefaultProvider.AutoCheck = $false

$groupProviders.Controls.AddRange(@($chkFipsProvider, $chkBaseProvider, $chkDefaultProvider))

# --- Status ---
$lblStatus = New-Object System.Windows.Forms.Label
$lblStatus.Text = "Status: No configuration loaded"
$lblStatus.Location = New-Object System.Drawing.Point(10,335)
$lblStatus.Size = New-Object System.Drawing.Size(820,150)
$lblStatus.BorderStyle = "FixedSingle"
$lblStatus.BackColor = [System.Drawing.Color]::LightYellow
$lblStatus.Padding = New-Object System.Windows.Forms.Padding(5)

$tabFips.Controls.AddRange(@($groupFiles, $groupFipsMode, $groupProviders, $lblStatus))

# ==================== TAB 2: Advanced Editor ====================
$tabAdvanced = New-Object System.Windows.Forms.TabPage
$tabAdvanced.Text = "Advanced Editor"

$lblAdvanced = New-Object System.Windows.Forms.Label
$lblAdvanced.Text = "Direct configuration file editing (saved as-is on Save):"
$lblAdvanced.Location = New-Object System.Drawing.Point(10,10)
$lblAdvanced.Size = New-Object System.Drawing.Size(500,20)

$txtConfigEditor = New-Object System.Windows.Forms.TextBox
$txtConfigEditor.Multiline = $true
$txtConfigEditor.ScrollBars = "Both"
$txtConfigEditor.WordWrap = $false
$txtConfigEditor.AcceptsReturn = $true
$txtConfigEditor.AcceptsTab = $true
$txtConfigEditor.Location = New-Object System.Drawing.Point(10,35)
$txtConfigEditor.Size = New-Object System.Drawing.Size(830,520)
$txtConfigEditor.Font = New-Object System.Drawing.Font("Consolas",10)

$tabAdvanced.Controls.AddRange(@($lblAdvanced, $txtConfigEditor))

# ==================== TAB 3: Help ====================
$tabHelp = New-Object System.Windows.Forms.TabPage
$tabHelp.Text = "Help"

$txtHelp = New-Object System.Windows.Forms.RichTextBox
$txtHelp.Location = New-Object System.Drawing.Point(10,10)
$txtHelp.Size = New-Object System.Drawing.Size(830,545)
$txtHelp.ReadOnly = $true
$txtHelp.Font = New-Object System.Drawing.Font("Segoe UI",10)
$txtHelp.Text = @"
OpenSSL Configuration Editor - Help

USAGE:

1. The OpenSSL Config and FIPS Provider DLL paths are pre-filled
   from the script's installation. Adjust if needed (when running
   the script standalone, outside a deployed configuration).

2. Choose a FIPS mode.

3. Click Save. This will:
   - For Strict / Flexible modes: regenerate fipsmodule.cnf alongside
     openssl.cnf using 'openssl fipsinstall'.
   - Write the updated openssl.cnf to disk.

The original openssl.cnf is preserved verbatim except for the
provider-related sections, which are stripped and replaced based on
the chosen mode. Manual edits in the Advanced Editor tab are honored
on Save (they are saved as-is).

FIPS MODES:

Strict FIPS Mode:
- Only FIPS-approved cryptographic algorithms are available
- Activates: FIPS provider + Base provider
- MD5, RC4, and other non-approved algorithms are blocked
- Use this for FIPS 140-3 compliance

Flexible Mode:
- Both FIPS and standard algorithms available
- Activates: FIPS provider + Base provider + Default provider
- Applications can choose which provider to use
- Not strictly FIPS compliant
- Note: Default provider must be explicitly activated when any other
  provider is activated, otherwise applications depending on OpenSSL
  may not work correctly.

Normal Mode:
- Standard OpenSSL operation
- Activates: Default provider only
- All algorithms available
- No FIPS restrictions

PROVIDERS:

FIPS Provider:
- Contains only FIPS 140-3 validated cryptographic algorithms
- Examples: AES, SHA-256, RSA, ECDSA, HMAC
- Excludes: MD5, RC4, DES, and other non-approved algorithms

Base Provider:
- Non-cryptographic support operations
- Examples: PEM/DER encoding, certificate parsing, Base64
- Required for FIPS mode to handle data formats

Default Provider:
- All standard OpenSSL algorithms (FIPS + legacy)
- Includes weak/deprecated algorithms for compatibility

VALIDATION:

Test your configuration with:
  openssl list -providers

For strict FIPS mode, MD5 should fail:
  openssl dgst -md5 test.txt

FIPS-approved algorithms should work:
  openssl sha256 test.txt
"@

$tabHelp.Controls.Add($txtHelp)

# Add tabs
$tabControl.TabPages.AddRange(@($tabFips, $tabAdvanced, $tabHelp))
$form.Controls.Add($tabControl)

# === Bottom buttons ===
$btnSave = New-Object System.Windows.Forms.Button
$btnSave.Text = "Save"
$btnSave.Location = New-Object System.Drawing.Point(700,610)
$btnSave.Size = New-Object System.Drawing.Size(80,30)
$btnSave.Add_Click({ Save-AllConfig })

$btnClose = New-Object System.Windows.Forms.Button
$btnClose.Text = "Close"
$btnClose.Location = New-Object System.Drawing.Point(790,610)
$btnClose.Size = New-Object System.Drawing.Size(80,30)
$btnClose.Add_Click({ $form.Close() })

$form.Controls.AddRange(@($btnSave, $btnClose))

# ============================================================
# Functions
# ============================================================

function Get-CurrentMode {
    if ($radioStrictFips.Checked) { return 'strict' }
    if ($radioFlexible.Checked)   { return 'flexible' }
    return 'normal'
}

function Get-FipsmoduleCnfPath {
    $opensslConfPath = $txtOpensslConfPath.Text
    if ([string]::IsNullOrWhiteSpace($opensslConfPath)) { return $null }
    $dir = Split-Path -Parent $opensslConfPath
    if (-not $dir) { $dir = '.' }
    return Join-Path $dir 'fipsmodule.cnf'
}

function Update-CheckboxDisplay {
    if ($radioStrictFips.Checked) {
        $chkFipsProvider.Checked    = $true
        $chkBaseProvider.Checked    = $true
        $chkDefaultProvider.Checked = $false
    } elseif ($radioFlexible.Checked) {
        $chkFipsProvider.Checked    = $true
        $chkBaseProvider.Checked    = $true
        $chkDefaultProvider.Checked = $true
    } else {
        $chkFipsProvider.Checked    = $false
        $chkBaseProvider.Checked    = $false
        $chkDefaultProvider.Checked = $true
    }
}

function Browse-OpensslConf {
    $dialog = New-Object System.Windows.Forms.OpenFileDialog
    $dialog.Filter = "Config files (*.cnf)|*.cnf|All files (*.*)|*.*"
    $dialog.Title = "Select openssl.cnf"
    $dialog.FileName = "openssl.cnf"
    if ($dialog.ShowDialog() -eq "OK") {
        $txtOpensslConfPath.Text = $dialog.FileName
        Load-Config $dialog.FileName
    }
}

function Browse-FipsDll {
    $dialog = New-Object System.Windows.Forms.OpenFileDialog
    $dialog.Filter = "DLL files (*.dll)|*.dll|All files (*.*)|*.*"
    $dialog.Title = "Select fips.dll"
    $dialog.FileName = "fips.dll"
    if ($dialog.ShowDialog() -eq "OK") {
        $txtFipsDllPath.Text = $dialog.FileName
    }
}

function Load-Config {
    param([string]$path)

    if (-not (Test-Path -LiteralPath $path)) {
        $script:suppressRegen = $true
        $txtConfigEditor.Text = ''
        $radioNormal.Checked = $true
        $script:suppressRegen = $false
        Update-CheckboxDisplay
        $lblStatus.Text = "Status: $path does not exist (will be created on Save)"
        $lblStatus.BackColor = [System.Drawing.Color]::LightYellow
        return
    }

    try {
        $content = [System.IO.File]::ReadAllText($path)
        $content = $content -replace "`r`n", "`n" -replace "`n", "`r`n"

        $script:suppressRegen = $true
        $txtConfigEditor.Text = $content
        $mode = Parse-Mode $content
        switch ($mode) {
            'strict'   { $radioStrictFips.Checked = $true }
            'flexible' { $radioFlexible.Checked   = $true }
            default    { $radioNormal.Checked     = $true }
        }
        $script:suppressRegen = $false
        Update-CheckboxDisplay

        $lblStatus.Text = "Status: Loaded $path (mode detected: $mode)"
        $lblStatus.BackColor = [System.Drawing.Color]::LightGreen
    } catch {
        $lblStatus.Text = "Status: Error loading file: $_"
        $lblStatus.BackColor = [System.Drawing.Color]::LightPink
    }
}

function Regenerate-Preview {
    $current = $txtConfigEditor.Text
    $mode = Get-CurrentMode
    $fipsmoduleCnfPath = Get-FipsmoduleCnfPath
    $newContent = Regenerate-FipsBlock $current $mode $fipsmoduleCnfPath
    $txtConfigEditor.Text = $newContent
}

function Parse-Mode {
    param([string]$content)

    $lines = $content -split "`r?`n"

    # FIPS is active when openssl_conf points to an init section that lists fips
    # in its providers section. The .include directive is optional (the FIPS
    # sections may be inlined in openssl.cnf rather than pulled from fipsmodule.cnf).

    # Find init section name
    $initSection = $null
    $currentSection = $null
    foreach ($line in $lines) {
        if ($line -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') { $currentSection = $matches[1]; continue }
        if ($null -eq $currentSection -and $line -match '^\s*openssl_conf\s*=\s*(\S+)') {
            $initSection = $matches[1]
        }
    }
    if (-not $initSection) { return 'normal' }

    # Find providers section name
    $providersSection = $null
    $currentSection = $null
    foreach ($line in $lines) {
        if ($line -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') { $currentSection = $matches[1]; continue }
        if ($currentSection -eq $initSection -and $line -match '^\s*providers\s*=\s*(\S+)') {
            $providersSection = $matches[1]
        }
    }
    if (-not $providersSection) { return 'normal' }

    # Inspect [providersSection] for fips and default activation
    $hasFips = $false
    $hasDefault = $false
    $currentSection = $null
    foreach ($line in $lines) {
        if ($line -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') { $currentSection = $matches[1]; continue }
        if ($currentSection -eq $providersSection) {
            if ($line -match '^\s*fips\s*=')    { $hasFips = $true }
            if ($line -match '^\s*default\s*=') { $hasDefault = $true }
        }
    }

    if ($hasFips -and $hasDefault) { return 'flexible' }
    if ($hasFips)                  { return 'strict' }
    return 'normal'
}

function Strip-FipsBlock {
    param([string]$content)

    $lines = $content -split "`r?`n"

    # Phase 1: discover section names
    $initSection = $null
    $providersSection = $null
    $providerSubSections = @()

    $currentSection = $null
    foreach ($line in $lines) {
        if ($line -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') { $currentSection = $matches[1]; continue }
        if ($null -eq $currentSection -and $line -match '^\s*openssl_conf\s*=\s*(\S+)') {
            $initSection = $matches[1]
        }
    }

    if ($initSection) {
        $currentSection = $null
        foreach ($line in $lines) {
            if ($line -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') { $currentSection = $matches[1]; continue }
            if ($currentSection -eq $initSection -and $line -match '^\s*providers\s*=\s*(\S+)') {
                $providersSection = $matches[1]
            }
        }
    }

    if ($providersSection) {
        $currentSection = $null
        foreach ($line in $lines) {
            if ($line -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') { $currentSection = $matches[1]; continue }
            if ($currentSection -eq $providersSection) {
                if ($line -match '^\s*[A-Za-z_][A-Za-z0-9_]*\s*=\s*(\S+)') {
                    $sub = $matches[1]
                    if ($sub -and ($providerSubSections -notcontains $sub)) {
                        $providerSubSections += $sub
                    }
                }
            }
        }
    }

    $sectionsToStrip = @()
    if ($initSection)      { $sectionsToStrip += $initSection }
    if ($providersSection) { $sectionsToStrip += $providersSection }
    foreach ($sub in $providerSubSections) { $sectionsToStrip += $sub }

    # Phase 2: emit, dropping FIPS lines
    $out = @()
    $currentSection = $null
    $skipping = $false
    foreach ($line in $lines) {
        if ($line -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') {
            $currentSection = $matches[1]
            $skipping = $sectionsToStrip -contains $currentSection
            if (-not $skipping) { $out += $line }
            continue
        }
        if ($skipping) { continue }
        # Drop fipsmodule.cnf .include lines wherever they appear (uncommented or stock-commented).
        # Path uses .* (not \S*) so paths with spaces like "C:\Program Files\..." are matched.
        if ($line -match '^\s*#?\s*\.include\s+.*[Ff]ipsmodule\.cnf\b') { continue }
        # openssl_conf is only meaningful in the default section, so only strip there.
        if ($null -eq $currentSection -and $line -match '^\s*#?\s*openssl_conf\s*=') { continue }
        $out += $line
    }

    # Trim trailing empty lines (find first non-blank from end, then slice once)
    $lastNonBlank = $out.Count
    while ($lastNonBlank -gt 0 -and [string]::IsNullOrWhiteSpace($out[$lastNonBlank - 1])) {
        $lastNonBlank--
    }
    if ($lastNonBlank -eq 0) {
        $out = @()
    } elseif ($lastNonBlank -lt $out.Count) {
        $out = @($out[0..($lastNonBlank - 1)])
    }

    return ($out -join "`r`n")
}

function Regenerate-FipsBlock {
    param(
        [string]$content,
        [string]$mode,
        [string]$fipsmoduleCnfPath
    )

    $stripped = Strip-FipsBlock $content
    $lines = @($stripped -split "`r?`n")

    # Build top-level directives (go into default section, before first [name])
    $topDirectives = @("openssl_conf = openssl_init")
    if ($mode -eq 'strict' -or $mode -eq 'flexible') {
        if (-not [string]::IsNullOrWhiteSpace($fipsmoduleCnfPath)) {
            $fipsCnfFwd = $fipsmoduleCnfPath -replace '\\', '/'
            $topDirectives += ".include $fipsCnfFwd"
        }
    }

    # Find first [section] header
    $firstSectionIdx = -1
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i] -match '^\s*\[\s*([^\]]+?)\s*\]\s*$') { $firstSectionIdx = $i; break }
    }

    if ($firstSectionIdx -eq -1) {
        # No named sections in source; directives followed by named sections
        if ($lines.Count -gt 0 -and -not [string]::IsNullOrWhiteSpace($lines[$lines.Count - 1])) {
            $lines = $lines + @('')
        }
        $lines = $lines + $topDirectives
    } elseif ($firstSectionIdx -eq 0) {
        # File starts with a section; prepend directives
        $lines = $topDirectives + @('') + $lines
    } else {
        $before = @($lines[0..($firstSectionIdx - 1)])
        $after  = @($lines[$firstSectionIdx..($lines.Count - 1)])
        # Strip trailing blanks from $before so we don't double-up
        $lastNonBlank = $before.Count
        while ($lastNonBlank -gt 0 -and [string]::IsNullOrWhiteSpace($before[$lastNonBlank - 1])) {
            $lastNonBlank--
        }
        if ($lastNonBlank -eq 0) {
            $before = @()
        } elseif ($lastNonBlank -lt $before.Count) {
            $before = @($before[0..($lastNonBlank - 1)])
        }
        $lines = $before + @('') + $topDirectives + @('') + $after
    }

    # Build named sections at EOF
    $named = @()
    $named += ''
    $named += '[openssl_init]'
    $named += 'providers = provider_sect'
    $named += ''
    if ($mode -eq 'strict') {
        $named += '[provider_sect]'
        $named += 'fips = fips_sect'
        $named += 'base = base_sect'
        $named += ''
        $named += '[fips_sect]'
        $named += 'activate = 1'
        $named += ''
        $named += '[base_sect]'
        $named += 'activate = 1'
    } elseif ($mode -eq 'flexible') {
        $named += '[provider_sect]'
        $named += 'fips = fips_sect'
        $named += 'base = base_sect'
        $named += 'default = default_sect'
        $named += ''
        $named += '[fips_sect]'
        $named += 'activate = 1'
        $named += ''
        $named += '[base_sect]'
        $named += 'activate = 1'
        $named += ''
        $named += '[default_sect]'
        $named += 'activate = 1'
    } else {
        $named += '[provider_sect]'
        $named += 'default = default_sect'
        $named += ''
        $named += '[default_sect]'
        $named += 'activate = 1'
    }
    $named += ''

    return ($lines + $named) -join "`r`n"
}

function Save-AllConfig {
    $opensslConfPath = $txtOpensslConfPath.Text
    if ([string]::IsNullOrWhiteSpace($opensslConfPath)) {
        [System.Windows.Forms.MessageBox]::Show("Please specify the openssl.cnf path", "Error", "OK", "Error") | Out-Null
        return
    }

    $mode = Get-CurrentMode
    $needsFips = ($mode -ne 'normal')
    $fipsmoduleCnfPath = $null

    if ($needsFips) {
        $fipsDllPath = $txtFipsDllPath.Text
        if ([string]::IsNullOrWhiteSpace($fipsDllPath)) {
            [System.Windows.Forms.MessageBox]::Show("Please specify the fips.dll path for FIPS modes", "Error", "OK", "Error") | Out-Null
            return
        }
        if (-not (Test-Path -LiteralPath $fipsDllPath)) {
            [System.Windows.Forms.MessageBox]::Show("FIPS DLL not found:`r`n$fipsDllPath", "Error", "OK", "Error") | Out-Null
            return
        }
        if (-not (Test-Path -LiteralPath $script:opensslExe)) {
            [System.Windows.Forms.MessageBox]::Show("openssl.exe not found at:`r`n$($script:opensslExe)`r`n`r`nNeeded to generate fipsmodule.cnf.", "Error", "OK", "Error") | Out-Null
            return
        }

        $fipsmoduleCnfPath = Get-FipsmoduleCnfPath
        try {
            $output = & $script:opensslExe fipsinstall -out $fipsmoduleCnfPath -module $fipsDllPath 2>&1
            if ($LASTEXITCODE -ne 0) {
                [System.Windows.Forms.MessageBox]::Show("openssl fipsinstall failed (exit $LASTEXITCODE):`r`n`r`n$output", "Error", "OK", "Error") | Out-Null
                return
            }
        } catch {
            [System.Windows.Forms.MessageBox]::Show("Error running openssl fipsinstall:`r`n$_", "Error", "OK", "Error") | Out-Null
            return
        }
    }

    try {
        $content = $txtConfigEditor.Text -replace "`r`n", "`n" -replace "`n", "`r`n"
        $utf8NoBom = New-Object System.Text.UTF8Encoding $false
        [System.IO.File]::WriteAllText($opensslConfPath, $content, $utf8NoBom)

        if ($needsFips) {
            $lblStatus.Text = "Status: Saved $opensslConfPath`r`n        and $fipsmoduleCnfPath"
        } else {
            $lblStatus.Text = "Status: Saved $opensslConfPath"
        }
        $lblStatus.BackColor = [System.Drawing.Color]::LightGreen
    } catch {
        [System.Windows.Forms.MessageBox]::Show("Error saving openssl.cnf:`r`n$_", "Error", "OK", "Error") | Out-Null
    }
}

# === Initial load ===
if (Test-Path -LiteralPath $initialOpensslConfPath) {
    Load-Config $initialOpensslConfPath
} else {
    $script:suppressRegen = $true
    $radioNormal.Checked = $true
    $script:suppressRegen = $false
    Update-CheckboxDisplay
    $lblStatus.Text = "Status: $initialOpensslConfPath does not exist (will be created on Save)"
    $lblStatus.BackColor = [System.Drawing.Color]::LightYellow
}

[void]$form.ShowDialog()

# SIG # Begin signature block
# MIIQmAYJKoZIhvcNAQcCoIIQiTCCEIUCAQExCzAJBgUrDgMCGgUAMGkGCisGAQQB
# gjcCAQSgWzBZMDQGCisGAQQBgjcCAR4wJgIDAQAABBAfzDtgWUsITrck0sYpfvNR
# AgEAAgEAAgEAAgEAAgEAMCEwCQYFKw4DAhoFAAQU4hx1LgcyK6+hmMksNGd89cNt
# wJigggz2MIIGHDCCBASgAwIBAgIQM9cIqJFAUxnipbvTObmtbjANBgkqhkiG9w0B
# AQwFADBWMQswCQYDVQQGEwJHQjEYMBYGA1UEChMPU2VjdGlnbyBMaW1pdGVkMS0w
# KwYDVQQDEyRTZWN0aWdvIFB1YmxpYyBDb2RlIFNpZ25pbmcgUm9vdCBSNDYwHhcN
# MjEwMzIyMDAwMDAwWhcNMzYwMzIxMjM1OTU5WjBXMQswCQYDVQQGEwJHQjEYMBYG
# A1UEChMPU2VjdGlnbyBMaW1pdGVkMS4wLAYDVQQDEyVTZWN0aWdvIFB1YmxpYyBD
# b2RlIFNpZ25pbmcgQ0EgRVYgUjM2MIIBojANBgkqhkiG9w0BAQEFAAOCAY8AMIIB
# igKCAYEAu9H+HrdCW3j1kKeuLIPxjSHTMIaFe9/TzdkWS6yFxbsBz+KMKBFyBHYs
# gcWrEnpASsUQ6IEUORtfTwf2MDAwfzUl5cBzPUAJlOio+Os5C1XVtgyLHif43j4i
# wb/vZe5z7mXdKN27H32bMn+3mVUXqrJJqDwQajrDIbKZqEPXO4KoGWG1PmpaXbi8
# nhPQCp71W49pOGjqpR9byiPuC+280B5DQ26wU4zCcypEMW6+j7jGAva7ggQVeQxS
# IOiYJ3Fh7y/k+AL7M1m19MNV59/2CCKuttEJWewBn3OJt0NP1fLZvVZZCd23F/bE
# dIC6h0asBtvbBA3VTrrujAk0GZUb5nATBCXfj7jXhDOMbKYM62i6lU98ROjUaY0l
# ecMh8TV3+E+2ElWV0FboGALV7nnIhqFp8RtOlBNqB2Lw0GuZpZdQnhwzoR7uYYsF
# aByO9e4mkIPW/nGFp5ryDRQ+NrUSrXd1esznRjZqkFPLxpRx3gc6IfnWMmfgnG5U
# hqBkoIPLAgMBAAGjggFjMIIBXzAfBgNVHSMEGDAWgBQy65Ka/zWWSC8oQEJwIDaR
# XBeF5jAdBgNVHQ4EFgQUgTKSQSsozUbIxKLGKjkS7EipPxQwDgYDVR0PAQH/BAQD
# AgGGMBIGA1UdEwEB/wQIMAYBAf8CAQAwEwYDVR0lBAwwCgYIKwYBBQUHAwMwGgYD
# VR0gBBMwETAGBgRVHSAAMAcGBWeBDAEDMEsGA1UdHwREMEIwQKA+oDyGOmh0dHA6
# Ly9jcmwuc2VjdGlnby5jb20vU2VjdGlnb1B1YmxpY0NvZGVTaWduaW5nUm9vdFI0
# Ni5jcmwwewYIKwYBBQUHAQEEbzBtMEYGCCsGAQUFBzAChjpodHRwOi8vY3J0LnNl
# Y3RpZ28uY29tL1NlY3RpZ29QdWJsaWNDb2RlU2lnbmluZ1Jvb3RSNDYucDdjMCMG
# CCsGAQUFBzABhhdodHRwOi8vb2NzcC5zZWN0aWdvLmNvbTANBgkqhkiG9w0BAQwF
# AAOCAgEAXzas+/n2cloUt/ALHd7Y/ZcB0v0B7pkthuj2t/A5/9aBSlqnQkoKLRWd
# 5pT9xWlKstdL8RYSTPa+kGZliy101KsI92oRAwh3fL5p4bDbnySJA9beXKTgsta0
# z+M41bltzCfWzmQR6BBydtP54OksielJ07OXlgYK4fYKyEGakV2B2DZ3mMqAQZeo
# +JE/Y5+qzVRUS4Dq9Rdm05Rx/Z79RzHj6RqGHdO+INI/sVJfspO9jJUJmHKPlQH0
# mEOlSvsUJqqdNr9ysPzcvYQN7O00qF6VKzgWYwV12fYxLhVr4pSyKtJ0NbWYmqP+
# +CsvthdLJ2xa5rl2XtqG3atk1mrqgxiIGzGC9YizlCXAIS8IaQLjTLtMKhEw64F5
# BuFBlSrUIPYLk+R8dgydHSZrX4QB9iqZza/ex/DkGKJOmy8qDGamknUmvtlANRNv
# rqY3GnrorRxRYwcqVgZs7X4Y9uPsZHOmbQg2i68Pma51axcrwk1qw1FGQVbpj8KN
# /xNxm9rtntOfq+VFphLFFFpSQZejBgAIxeYc6ieCPDvb5kbE7y0ANRPNNn2d5aon
# CAXMzsA2DksZT9Bjmm2/xSlTMSLbdVB3htDy+GruawYbPoUjK5fIfnqZQQzdWH8O
# qMMSPTo1m+CdLIwXgVREqHodmJ2Wf1lYplRl/1FCC/hH68/45b8wggbSMIIFOqAD
# AgECAhByqh02acEKYX4S44bsYvf3MA0GCSqGSIb3DQEBCwUAMFcxCzAJBgNVBAYT
# AkdCMRgwFgYDVQQKEw9TZWN0aWdvIExpbWl0ZWQxLjAsBgNVBAMTJVNlY3RpZ28g
# UHVibGljIENvZGUgU2lnbmluZyBDQSBFViBSMzYwHhcNMjQwNDAzMDAwMDAwWhcN
# MjcwNDAzMjM1OTU5WjCBujERMA8GA1UEBRMIMDU5MDE2MDExEzARBgsrBgEEAYI3
# PAIBAxMCR0IxHTAbBgNVBA8TFFByaXZhdGUgT3JnYW5pemF0aW9uMQswCQYDVQQG
# EwJHQjEQMA4GA1UECAwHU3VmZm9sazEoMCYGA1UECgwfRmlyZURhZW1vbiBUZWNo
# bm9sb2dpZXMgTGltaXRlZDEoMCYGA1UEAwwfRmlyZURhZW1vbiBUZWNobm9sb2dp
# ZXMgTGltaXRlZDCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAKjEZ3j3
# 2WJ2ywV9TW0QgfMKFSgA97Q8s+DlbmkQa9mNNWKwtl6IzxAfq7LjKUhL0fiXMD+i
# FcaNMJl7GSwB3hog0h+SuR9aujCcdGmOcaWxPq9gi86cVf9OZUs7ERNor/pGE+x6
# Dz83zqYCP5UBlzKu0xLo7UY2Q2ulOxl5avjsEKje4gUzCx/6bjddT8omxfYJTqoA
# hyCKhLHIg5F3tNIiwAtT47W4T6EY+8Q5c3OLj2cu1/NjzL3ihPXPo2PGpIoMQ7S2
# bqjcXqCoF8BoGFBjTkrqgamNocUKY0SqQrLPNNO9X4Qqi2PwUEheclS79BjfQUI7
# UpinYjx4ZCbH83b/s5iSaGXBg3dzh/CiUKL4SlYfD8U8zeUs3vlVhwXU4+hHzcqa
# ZQ1HUaLskwGHLVJoN8lA0ETleOhMLo6xbn79OfCglOB1pi0i5vbBF9xHm2MGnIL3
# xQr8HP5CvqK4eDQjP0rKB407Z4zttDrJdBuBxIw5sQq+QPaCBDer6NnT6fBOET0+
# z4r7jWPU6I+lEy6eDRtb4dBSALAgASBLkGV5JPQ6BD2l2yDqlSzGn4zx50FbFBwT
# DJXiwq2WgnAykv4KUT1VbKS3TCYHqXb5umCMaE1EiTZzb6iAZzgoyaVip+dd44v3
# nrNntyhfFs72wGNxm6UY9tKgU3mjz7hj2tq9AgMBAAGjggG0MIIBsDAfBgNVHSME
# GDAWgBSBMpJBKyjNRsjEosYqORLsSKk/FDAdBgNVHQ4EFgQUXtFzIzPXC6jpL140
# +vC67UdOsa0wDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwEwYDVR0lBAww
# CgYIKwYBBQUHAwMwSQYDVR0gBEIwQDA1BgwrBgEEAbIxAQIBBgEwJTAjBggrBgEF
# BQcCARYXaHR0cHM6Ly9zZWN0aWdvLmNvbS9DUFMwBwYFZ4EMAQMwSwYDVR0fBEQw
# QjBAoD6gPIY6aHR0cDovL2NybC5zZWN0aWdvLmNvbS9TZWN0aWdvUHVibGljQ29k
# ZVNpZ25pbmdDQUVWUjM2LmNybDB7BggrBgEFBQcBAQRvMG0wRgYIKwYBBQUHMAKG
# Omh0dHA6Ly9jcnQuc2VjdGlnby5jb20vU2VjdGlnb1B1YmxpY0NvZGVTaWduaW5n
# Q0FFVlIzNi5jcnQwIwYIKwYBBQUHMAGGF2h0dHA6Ly9vY3NwLnNlY3RpZ28uY29t
# MCYGA1UdEQQfMB2gGwYIKwYBBQUHCAOgDzANDAtHQi0wNTkwMTYwMTANBgkqhkiG
# 9w0BAQsFAAOCAYEANIljaPe3Kia/VSSVn0sKCo+RzbMH0ytSI4GFq3Mx77yfVT+f
# S10HPnHAqcOaqS0Gn/QLo1df7FvwS8/WA8O3G4gklqhLyUSmYg+FAj7o09JTnzv2
# 7ll8gtm7Y+/A4huWkuGt5P5q1m8/xNqJEYTZ2JndI7zkeI4sgbZTrKebiJA1xiLf
# 8WIah1Y6lT4AaEEBjtkAZn+bnZ6LrF6SRskbu2W1nHBocvblWchzVohM+96cngJK
# HL/HvHcv9de8kq7oU4Z5iCalKAzc7Oia4Db4+jGAP27lAxHUaLt4fo747yQk4LjF
# gbw0ceCF5chPhmGusqyRo1E3DfCcSD4Wepef+CF1aEzj/dufLLjRSsHqYF1RTAmI
# XOmNNI1MtFBZwNy1IjPCYhexW8B8PjlMpV2pIad8UFfaAjngUjealGsWkX1jduT0
# JiRRWraVDi4uHm6uR5uNzpNPjJIny2hH7Kd7IdOdKDJI+2d8alOabB0bxuiZejxG
# OsABHHJMboW0yukjMYIDDDCCAwgCAQEwazBXMQswCQYDVQQGEwJHQjEYMBYGA1UE
# ChMPU2VjdGlnbyBMaW1pdGVkMS4wLAYDVQQDEyVTZWN0aWdvIFB1YmxpYyBDb2Rl
# IFNpZ25pbmcgQ0EgRVYgUjM2AhByqh02acEKYX4S44bsYvf3MAkGBSsOAwIaBQCg
# eDAYBgorBgEEAYI3AgEMMQowCKACgAChAoAAMBkGCSqGSIb3DQEJAzEMBgorBgEE
# AYI3AgEEMBwGCisGAQQBgjcCAQsxDjAMBgorBgEEAYI3AgEVMCMGCSqGSIb3DQEJ
# BDEWBBRqXYsqmwQDWsJFcySxVepT6vBJ2TANBgkqhkiG9w0BAQEFAASCAgBI52yJ
# a1F4mKtewnaHulPhL/wDfXSHojE7vXUznGvIvRYKuzQiiETd0PXTQrt3Czna1GXc
# MZQCX64FYgan8dV5eaVlESBVsbZqNuHNS1SuBuB+1rOgMmuKs8o6GpWm0DbdE+ck
# h9xeMtl7psyqQHoXdRQed/x56h6LMXXpzgQ/HtIeKHg/3NTBBbZdLSOXFQCHiTFz
# +EaSQXWAi9aX5nAo5nTF1cZGvixwnggT7JiO4TJR3eI1nJqy7ncCHyTADKAIOmhI
# o6g2sqdRaU1fUIiOpVgWAsu60Aqdsx+PHpn4/7r+zZW2CDG0khmF5BoYJBUqy93A
# 4IhhY/DompX61ZRLPQwVmSURIVvtiycq34WqeOXrCQ80sZYiPEmz+pbM3LKcJ1oo
# R6bLBKcEB/ugv9q4zTSs9/1a9an20GZD7xgK/YvNLwb7pIR6KG4axQTkMsuvNOcW
# u5ZZzT2VOqAadBh0eseUHp0tNmTUubTgzPuvo+Vwu96YbXcPVWHWS5Dn/wRbcQHP
# iTE9I71wd5qD3SuT6FobzcEETJAFK6eOvN5mIQyHzQxsMWcHPU2gagzvoSx66WLS
# S8AL0IR01QOgNjlJbh7NH1DZPt9dD+f0o+n92d0sBu+cCfKd5GPlvV7M1H1m5Q20
# eIHLzrXWkxq9YwztxBdXwPRe47MLslkqxg+AvQ==
# SIG # End signature block
