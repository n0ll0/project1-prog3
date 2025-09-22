# Data Structures Coursework Project

This is a C++ coursework project implementing various data structures with different item types and header configurations. The project demonstrates linked list management, hash table implementation using buckets, and file I/O operations for persistence.

## Project Overview

The project implements a data structure management system that:
- Supports 10 different item types (ITEM1-ITEM10) with various date/time formats
- Uses 5 different header structures (HEADER_A through HEADER_E) for organizing items
- Implements hash table functionality using character-based buckets
- Provides serialization/deserialization to binary files
- Includes comprehensive evaluation testing

### Core Components

- **DataStructure Class**: Main data structure implementation using HEADER_C (bucket-based organization)
- **Item Types**: 10 different item structures with ID, Code, and time/date information
- **Header Types**: 5 different organizational structures for managing items
- **DataSource Library**: External DLL providing item generation and data structure creation
- **Test Framework**: Template-based evaluation system for testing functionality

## Cross-Platform Support

**Recent Updates:** This project has been enhanced with cross-platform compatibility features.

### Windows (Primary Platform)
- **MinGW-w64** with GCC/G++ compilers
- **CMake** (version 3.10.0 or higher)  
- **Ninja** build system
- **DataSource.dll** for full functionality

### Linux/Unix (Development Support)
- **GCC/G++ compilers**
- **CMake** and **Ninja** 
- **Mock DataSource** implementation for basic testing
- Limited functionality (ITEM2 support only)

### Cross-Platform Features Added
- **Platform Compatibility Layer**: `platform_compat.h` handles Windows-specific code
- **Safe String Operations**: `STRCPY_SAFE` macro for secure string copying across platforms
- **Conditional Compilation**: CMake automatically selects appropriate source files
- **Mock DataSource**: Basic implementation for non-Windows development

## Build Requirements

### Windows (Primary Platform)
- **MinGW-w64** with GCC/G++ compilers
- **CMake** (version 3.10.0 or higher)
- **Ninja** build system
- **MSYS2** environment (recommended)

### Prerequisites Installation (Windows)

1. **Install MSYS2**:
   - Download from [https://www.msys2.org/](https://www.msys2.org/)
   - Install to default location (C:\msys64)

2. **Install Development Tools**:
   ```bash
   # Open MSYS2 UCRT64 terminal
   pacman -S mingw-w64-ucrt-x86_64-gcc
   pacman -S mingw-w64-ucrt-x86_64-cmake
   pacman -S mingw-w64-ucrt-x86_64-ninja
   ```

3. **Add to PATH** (if not automatically added):
   ```
   C:\msys64\ucrt64\bin
   ```

## Building the Project

### Option 1: Using PowerShell Script (Windows)

The project includes a convenient PowerShell build script:

```powershell
# Run from project root directory
.\build_and_run.ps1
```

This script will:
1. Clean the build directory
2. Configure CMake with MinGW compilers
3. Build using Ninja
4. Run the executable
5. Return to project root

### Option 2: Manual Build (Windows)

```bash
# Clean previous build
rm -rf build/*

# Configure with MinGW compilers
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe \
  -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe

# Build
cd build
ninja

# Run
./main.exe

# Return to project root
cd ..
```

### Option 3: Cross-Platform Build (Linux/Unix)

For development and testing on non-Windows platforms:

```bash
# Make build script executable
chmod +x build_and_run.sh

# Build and get status
./build_and_run.sh
```

Or manually:
```bash
# Configure and build
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Debug
cd build
ninja

# Note: Limited functionality due to mock DataSource implementation
./main
```

### Cross-Platform Considerations

**Note**: While cross-platform compatibility has been added, full functionality requires Windows due to:
- Complete DataSource.dll implementation (Windows only)
- Full item type support (ITEM1-ITEM10)
- All header structure types (HEADER_A through HEADER_E)

**Linux/Unix Support**:
- ✅ Builds successfully with mock DataSource
- ✅ Basic ITEM2 functionality
- ✅ Core DataStructure operations
- ❌ Limited to basic testing scenarios
- ❌ No file I/O with full DataSource features

For development purposes, the mock implementation provides:
- Basic item generation for ITEM2
- Random color name assignment
- Core data structure testing
- Build system verification

## Project Structure

```
project1-prog3/
├── src/
│   ├── main.cpp              # Main entry point
│   ├── DataStructure.cpp     # DataStructure class implementation
│   └── MockDataSource.cpp    # Cross-platform mock implementation
├── Headers/
│   ├── DataStructure.h       # DataStructure class declaration
│   ├── DataSource.h          # External library interface
│   ├── Headers.h             # Header type definitions (HEADER_A-E)
│   ├── Items.h               # Item type definitions (ITEM1-10)
│   ├── DateTime.h            # Date/time structure definitions
│   ├── Test.h                # Testing framework template
│   └── platform_compat.h     # Cross-platform compatibility layer
├── libs/
│   ├── DataSource.dll        # Windows DLL for data generation
│   ├── DataSource.lib        # MSVC import library
│   ├── libDataSource.a       # MinGW import library
│   └── DataSource.def        # DLL definition file
├── deps/
│   └── Colors.txt            # Color names database for item IDs
├── build_and_run.ps1         # PowerShell build script
├── build_and_run.sh          # Unix/Linux build script  
├── CMakeLists.txt            # CMake configuration
├── README.md                 # This file
└── CODE_REVIEW.md            # Comprehensive code review report
```

## Usage

### Basic Usage

The program demonstrates data structure operations:

```cpp
#include "DataStructure.h"
#include "DataSource.h"
#include "Test.h"

int main() {
    // Create a data structure with 200 items
    DataStructure* structure = new DataStructure(200);
    
    // Print structure contents
    std::cout << *structure << std::endl;
    
    // Run evaluation tests for ITEM2
    EvaluationTest<ITEM2>(2, "./Datastructure.bin");
    
    return 0;
}
```

### Key Features

1. **Item Generation**: Items are created using the DataSource library with random IDs from Colors.txt
2. **Bucket Organization**: Items are organized in buckets based on the first letter of the second word in their ID
3. **File Persistence**: Structures can be saved to and loaded from binary files
4. **Template Testing**: Comprehensive test suite using template functions

### Data Structure Details

- **HEADER_C Implementation**: Uses an array of pointers (`void** ppItems`) for bucket-based organization
- **26 Buckets**: One for each letter A-Z based on second word initial in item ID
- **Linked Lists**: Each bucket contains a linked list of items
- **Memory Management**: Automatic cleanup of dynamically allocated items and buckets

## Testing

The project includes comprehensive testing through the `EvaluationTest` template function:

1. **Constructor Testing**: Tests different constructor variations
2. **Operator Testing**: Tests `+=`, `-=`, `=`, and `==` operators
3. **Search Operations**: Tests item retrieval by ID
4. **File I/O**: Tests binary file save/load operations
5. **Memory Management**: Tests copy constructor and destructor

## Troubleshooting

### Common Build Issues

1. **Compiler Not Found**:
   - Ensure MinGW-w64 is properly installed
   - Check PATH environment variable
   - Verify compiler paths in CMakeLists.txt

2. **DLL Not Found**:
   - Ensure DataSource.dll is in the build directory
   - Check that CMake copies DLL files properly

3. **Link Errors**:
   - Verify that libDataSource.a is present for MinGW linking
   - Check that all source files are included in CMakeLists.txt

### Platform-Specific Notes

**Windows**: 
- Use MSYS2 UCRT64 terminal for consistent environment
- Ensure antivirus doesn't interfere with DLL loading

**Other Platforms**: 
- This project is not directly compatible with Linux/macOS due to Windows-specific dependencies
- Porting would require significant modifications to the DataSource interface

## Development

### Code Style
- Uses C-style structs for compatibility with external DLL
- Implements RAII principles in DataStructure class
- Includes comprehensive error handling with exceptions

### Memory Management
- Dynamic allocation for items and buckets
- Proper cleanup in destructors
- Deep copying in copy constructor

## Code Review

A comprehensive code review has been conducted and documented in `CODE_REVIEW.md`. Key findings include:

### ✅ Strengths Identified
- **Solid Architecture**: Well-structured data organization with clear separation of concerns
- **Memory Safety**: Proper RAII implementation and cleanup patterns
- **Comprehensive Testing**: Template-based test framework covering multiple scenarios
- **Cross-Platform Support**: Recent enhancements for broader compatibility

### 🔧 Improvements Made
- **Platform Compatibility**: Added cross-platform support with compatibility layer
- **Build System**: Enhanced CMake configuration for multiple environments
- **Security**: Replaced platform-specific functions with safe alternatives
- **Documentation**: Created comprehensive build and usage instructions

### 📋 Recommendations for Future Development
- **Code Style**: Standardize naming conventions across the project
- **Modern C++**: Consider adopting more modern C++ practices (smart pointers, etc.)
- **Error Handling**: Implement custom exception classes for better error categorization
- **Performance**: Optimize string operations where applicable

**Overall Assessment**: The project demonstrates solid understanding of data structures and memory management principles, with recent improvements making it more accessible and maintainable.

## License

This is a coursework project. Please refer to your institution's academic integrity policies regarding use and modification.