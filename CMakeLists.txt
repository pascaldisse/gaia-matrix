cmake_minimum_required(VERSION 3.16)
project(GAIA_MATRIX VERSION 0.1.0 LANGUAGES CXX)

# C++17 required for std::filesystem
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files (exclude main.cpp for library)
file(GLOB_RECURSE LIB_SOURCES
    "src/ai/*.cpp"
    "src/aopl/*.cpp"
    "src/core/*.cpp"
    "src/platform/*.cpp"
    "src/renderer/*.cpp"
    "src/editor/*.cpp"
    "src/utils/*.cpp"
    "src/web/*.cpp"
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Platform-specific checks and configurations
if(APPLE)
    # Check if running on Apple Silicon
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        add_definitions(-DAPPLE_SILICON)
        message(STATUS "Building for Apple Silicon")
    else()
        message(STATUS "Building for Intel Mac")
    endif()
    
    # Add Metal framework for rendering
    find_library(METAL_LIBRARY Metal REQUIRED)
    find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
    find_library(APPKIT_LIBRARY AppKit REQUIRED)
    set(PLATFORM_LIBS ${METAL_LIBRARY} ${FOUNDATION_LIBRARY} ${APPKIT_LIBRARY})
    
    # For macOS specific code
    add_definitions(-DMACOS)
elseif(UNIX AND NOT APPLE)
    # Linux
    add_definitions(-DLINUX)
    find_package(Vulkan REQUIRED)
    set(PLATFORM_LIBS Vulkan::Vulkan)
elseif(WIN32)
    # Windows
    add_definitions(-DWINDOWS)
    find_package(Vulkan REQUIRED)
    set(PLATFORM_LIBS Vulkan::Vulkan)
endif()

# Gaia OS check (would be implemented properly in a real scenario)
option(GAIA_OS "Building for Gaia OS" OFF)
if(GAIA_OS)
    add_definitions(-D__GAIA_OS__)
    message(STATUS "Building for Gaia OS")
endif()

# First create a library target
add_library(gaia_matrix_lib STATIC ${LIB_SOURCES})
target_link_libraries(gaia_matrix_lib PUBLIC ${PLATFORM_LIBS})
set_target_properties(gaia_matrix_lib PROPERTIES OUTPUT_NAME "gaia_matrix")

# Main executable
add_executable(gaia_matrix_exe "src/main.cpp")
target_link_libraries(gaia_matrix_exe PRIVATE gaia_matrix_lib)
set_target_properties(gaia_matrix_exe PROPERTIES OUTPUT_NAME "gaia_matrix")

# Install targets
install(TARGETS gaia_matrix_exe DESTINATION bin)
install(TARGETS gaia_matrix_lib DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# Testing
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Documentation
option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, documentation will not be built")
    endif()
endif()

# Examples
option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Print configuration summary
message(STATUS "GAIA MATRIX configuration:")
message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Build docs: ${BUILD_DOCS}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  Gaia OS: ${GAIA_OS}")