# Cross-platform build & run helper for both Visual Studio (MSVC) and MinGW Ninja
param(
 [string]$Config = "Debug",
 [string]$Generator = "",
 [string]$Arch = "x64", # Desired target architecture when using MSVC (x64 or Win32)
 [switch]$Clean,
 [switch]$Run
)

$ErrorActionPreference = "Stop"

# Decide generator if not provided
if (-not $Generator) {
 if ($env:VisualStudioVersion -or $env:VSCMD_VER) {
 if (Get-Command ninja -ErrorAction SilentlyContinue) { $Generator = "Ninja" } else { $Generator = "Ninja" }
 } elseif (Get-Command ninja -ErrorAction SilentlyContinue) {
 $Generator = "Ninja"
 } else {
 Write-Host "Ninja not found. Install Ninja or specify a generator via -Generator." -ForegroundColor Yellow
 $Generator = "Ninja"
 }
}

$buildDir = Join-Path $PWD "build"
if ($Clean -and (Test-Path $buildDir)) {
 Write-Host "Cleaning build directory" -ForegroundColor Cyan
 Remove-Item $buildDir -Recurse -Force
}

# Simple architecture validation for existing cache
$cacheFile = Join-Path $buildDir "CMakeCache.txt"
if ((Test-Path $cacheFile) -and (-not $Clean)) {
 $cacheContent = Get-Content $cacheFile -ErrorAction SilentlyContinue
 # Check for x86 machine type in linker flags
 if ($cacheContent -match "/machine:X86" -or $cacheContent -match "/machine:x86") {
 Write-Warning "Existing build directory is 32-bit. The bundled DataSource.lib is 64-bit. Re-configuring as 64-bit." 
 Remove-Item $buildDir -Recurse -Force
 }
}

# Check if we're in the wrong architecture environment for Ninja + MSVC
if (($Generator -match "Ninja") -and ($Arch -eq "x64") -and ($env:VisualStudioVersion -or $env:VSCMD_VER)) {
 # Check if we're in an x86 environment
 $vcvarsArch = $env:VSCMD_ARG_TGT_ARCH
 if (($vcvarsArch -eq "x86") -or ($env:Platform -eq "x86")) {
 Write-Host ""
 Write-Host "============================================" -ForegroundColor Red
 Write-Host "ERROR: You are in an x86 (32-bit) Developer Command Prompt!" -ForegroundColor Red
 Write-Host "============================================" -ForegroundColor Red
 Write-Host ""
 Write-Host "The DataSource.lib library is 64-bit, but your environment is 32-bit." -ForegroundColor Yellow
 Write-Host ""
 Write-Host "Please open 'x64 Native Tools Command Prompt for VS 2022' and run this script again." -ForegroundColor Cyan
 Write-Host ""
 Write-Host "Or run build_x64.bat which sets up the environment automatically." -ForegroundColor Cyan
 Write-Host ""
 exit 1
 }
}

if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }

$cmakeArgs = @("-S", ".", "-B", "$buildDir")
if ($Generator) { $cmakeArgs += "-G"; $cmakeArgs += "$Generator" }

# For Ninja with MSVC, architecture is controlled by the environment (vcvars), not -A flag
# -A only works with Visual Studio generators
if ($Generator -match "Visual Studio") {
 $cmakeArgs += "-A"; $cmakeArgs += $Arch
}

# Multi-config vs single-config handling
if (($Generator -notmatch "Visual Studio") -and ($Generator -notmatch "Xcode") -and ($Generator -notmatch "Multi-Config")) {
 $cmakeArgs += "-DCMAKE_BUILD_TYPE=$Config"
}

Write-Host "Configuring with generator '$Generator' (Target: $Arch)..." -ForegroundColor Cyan
cmake @cmakeArgs

# After configure, verify we are 64-bit when linking the provided library
$cacheFile = Join-Path $buildDir "CMakeCache.txt"
if (Test-Path $cacheFile) {
 $cacheContent = Get-Content $cacheFile -ErrorAction SilentlyContinue
 $is64 = $false
 
 # Check for pointer size (Unix/MinGW style)
 if ($cacheContent -match "CMAKE_SIZEOF_VOID_P:INTERNAL=8") { $is64 = $true }
 # Check for x64 machine type in linker flags (MSVC style)
 if ($cacheContent -match "/machine:x64" -or $cacheContent -match "/machine:X64") { $is64 = $true }
 
 if (-not $is64) {
 Write-Host ""
 Write-Error "Detected 32-bit build configuration. DataSource.lib is 64-bit."
 Write-Host ""
 Write-Host "If using Ninja + MSVC, you MUST run from 'x64 Native Tools Command Prompt for VS 2022' or use build_x64.bat" -ForegroundColor Yellow
 Write-Host "If using MinGW, ensure you're using 64-bit MinGW toolchain (e.g., msys64/mingw64 or msys64/ucrt64)" -ForegroundColor Yellow
 Write-Host ""
 exit 1
 }
 
 Write-Host "Architecture check: OK (64-bit)" -ForegroundColor Green
}

# Build
Write-Host "Building ($Config)..." -ForegroundColor Cyan
if (($Generator -match "Visual Studio") -or ($Generator -match "Xcode") -or ($Generator -match "Multi-Config")) {
 cmake --build $buildDir --config $Config
} else {
 cmake --build $buildDir -- -k0
}

# Run
if ($Run) {
 Write-Host "Running executable..." -ForegroundColor Cyan
 $exe = Join-Path $buildDir "main.exe"
 if (-not (Test-Path $exe)) { $exe = Join-Path (Join-Path $buildDir $Config) "main.exe" }
 if (-not (Test-Path $exe)) { throw "Executable not found after build: $exe" }
 
 # Change to build directory so DLL and data files are found
 Push-Location (Split-Path $exe -Parent)
 try {
 & (Split-Path $exe -Leaf)
 } finally {
 Pop-Location
 }
}