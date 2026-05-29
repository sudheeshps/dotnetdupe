# WSL Build Setup for DotNetDupe

This document outlines the steps taken to set up a local WSL (Ubuntu) environment for building and testing the DotNetDupe library.

## 1. WSL Distribution Installation
If no WSL distribution is installed, install Ubuntu:
```powershell
wsl --install -d Ubuntu
```

## 2. Dependency Installation
Install the necessary build tools and CMake inside the WSL instance:
```bash
# Update package lists
sudo apt-get update

# Install build-essential (gcc, g++, make) and cmake
sudo apt-get install -y build-essential cmake
```

## 3. Library Build Process
Navigate to the project directory within WSL (typically mounted under `/mnt/`) and execute the following:
```bash
# Create a dedicated build directory for WSL
mkdir -p build-wsl && cd build-wsl

# Configure the project with CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build the library and test suite
cmake --build .
```

## 4. Running Tests
Execute the full test suite using CTest:
```bash
ctest --output-on-failure
```

## 5. Running the Demo Application
Run the console demonstration to verify basic functionality:
```bash
./DotNetDupeDemo
```

## Key Cross-Platform Adjustments Made
- Replaced Win32-specific secure CRT functions (`sprintf_s`, `sscanf_s`) with standard C++ versions (`snprintf`, `sscanf`).
- Refactored `System::IO::Path` to use `std::filesystem` primitives instead of direct Win32 API calls for path manipulation.
- Implemented Linux-specific `System::Environment` providers using `/proc/self/status`, `/proc/self/cmdline`, and `environ`.
- Updated unit tests to use platform-agnostic paths and conditional expectations.
