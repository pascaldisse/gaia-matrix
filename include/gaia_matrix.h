#pragma once

// Core engine headers
#include "gaia_matrix/core.h"
#include "gaia_matrix/aopl.h"
#include "gaia_matrix/neural_engine.h"
#include "gaia_matrix/renderer.h"
#include "gaia_matrix/editor.h"
#include "gaia_matrix/platform.h"
#include "gaia_matrix/web_compiler.h"

/**
 * @brief GAIA MATRIX Engine
 * 
 * Game Artificial Intelligence Acceleration: Machine-learning Architecture
 * for Technology, Rendering, Intelligence & cross-platform
 * 
 * GAIA MATRIX is a next-generation game engine designed specifically for 
 * Gaia OS and macOS platforms, leveraging the power of Apple Silicon's 
 * Neural Engine through Gaia OS.
 */
namespace gaia_matrix {

/**
 * @brief GAIA MATRIX version information
 */
struct Version {
    static constexpr int Major = 0;
    static constexpr int Minor = 1;
    static constexpr int Patch = 0;
    static constexpr const char* Name = "Alpha";
    
    /**
     * @brief Get version string
     * @return Version string in format "Major.Minor.Patch-Name"
     */
    static const char* GetVersionString() {
        return "0.1.0-Alpha";
    }
};

} // namespace gaia_matrix