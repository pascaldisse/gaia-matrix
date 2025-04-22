#!/bin/bash

# GAIA MATRIX build script

# Parse command line arguments
USE_GAIA_OS=OFF
BUILD_TESTS=OFF
BUILD_DOCS=OFF
BUILD_TYPE="Debug"

for arg in "$@"
do
    case $arg in
        --gaia-os)
        USE_GAIA_OS=ON
        shift
        ;;
        --tests)
        BUILD_TESTS=ON
        shift
        ;;
        --docs)
        BUILD_DOCS=ON
        shift
        ;;
        --release)
        BUILD_TYPE="Release"
        shift
        ;;
        --help)
        echo "GAIA MATRIX build script"
        echo "Usage: $0 [options]"
        echo "Options:"
        echo "  --gaia-os      Build with Gaia OS support"
        echo "  --tests        Build tests"
        echo "  --docs         Build documentation"
        echo "  --release      Build in release mode (default is Debug)"
        echo "  --help         Show this help message"
        exit 0
        ;;
    esac
done

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
echo "Configuring GAIA MATRIX..."
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DGAIA_OS=$USE_GAIA_OS \
    -DBUILD_TESTS=$BUILD_TESTS \
    -DBUILD_DOCS=$BUILD_DOCS

# Build
echo "Building GAIA MATRIX..."
cmake --build . -- -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

# Run tests if built
if [ "$BUILD_TESTS" = "ON" ]; then
    echo "Running tests..."
    ctest --output-on-failure
fi

echo "Build complete!"
echo "You can find the GAIA MATRIX executable at: $(pwd)/bin/gaia_matrix"