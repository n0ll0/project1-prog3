#!/bin/bash

# Build and run script for Linux/Unix systems
# Note: This project requires DataSource.dll which is Windows-specific
# This script is provided for cross-platform development but will have limited functionality

echo "Building Data Structures Project..."
echo "Warning: This project is designed for Windows with DataSource.dll dependency"

# Clean build directory
rm -rf build/*

# Configure with system compilers
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cd build
ninja

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Note: Runtime will fail due to missing DataSource.dll (Windows-specific)"
    echo "To run: ./main"
else
    echo "Build failed! Check error messages above."
fi

cd ..