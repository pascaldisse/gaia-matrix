#pragma once

#include <string>
#include <memory>
#include <vector>

namespace gaia_matrix {

/**
 * @brief Platform identification
 */
enum class PlatformType {
    Unknown,
    GaiaOS,
    MacOS,
    Windows,
    Linux
};

/**
 * @brief Platform abstraction layer
 * 
 * Provides cross-platform functionality for GAIA MATRIX.
 */
class Platform {
public:
    /**
     * @brief Initialize the platform layer
     * @return True if initialization succeeded
     */
    static bool Initialize();

    /**
     * @brief Shutdown the platform layer
     */
    static void Shutdown();

    /**
     * @brief Get the current platform type
     * @return Platform type
     */
    static PlatformType GetPlatformType();

    /**
     * @brief Check if running on Gaia OS
     * @return True if running on Gaia OS
     */
    static bool IsGaiaOS();

    /**
     * @brief Check if Neural Engine is available
     * @return True if Neural Engine is available
     */
    static bool IsNeuralEngineAvailable();

    /**
     * @brief Get application directory
     * @return Application directory path
     */
    static std::string GetApplicationDirectory();

    /**
     * @brief Get user documents directory
     * @return User documents directory path
     */
    static std::string GetUserDocumentsDirectory();

    /**
     * @brief Get platform name as string
     * @return Platform name
     */
    static std::string GetPlatformName();

    /**
     * @brief Get the singleton instance
     * @return Platform instance
     */
    static Platform& Get();

private:
    // Private constructor and destructor for singleton pattern
    Platform();
    ~Platform();
    
    // Static instance
    static Platform* s_Instance;
    bool m_IsInitialized;
    PlatformType m_PlatformType;
};

/**
 * @brief File system operations with platform abstraction
 */
class FileSystem {
public:
    /**
     * @brief Check if file exists
     * @param path File path
     * @return True if file exists
     */
    static bool FileExists(const std::string& path);

    /**
     * @brief Check if directory exists
     * @param path Directory path
     * @return True if directory exists
     */
    static bool DirectoryExists(const std::string& path);

    /**
     * @brief Create directory
     * @param path Directory path
     * @return True if directory created successfully
     */
    static bool CreateDirectory(const std::string& path);

    /**
     * @brief Get files in directory
     * @param path Directory path
     * @param extension Optional file extension filter
     * @return List of files
     */
    static std::vector<std::string> GetFilesInDirectory(const std::string& path, const std::string& extension = "");
};

} // namespace gaia_matrix