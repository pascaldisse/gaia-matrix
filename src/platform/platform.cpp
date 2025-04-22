#include "gaia_matrix/platform.h"
#include <iostream>
#include <filesystem>
#include <fstream>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

namespace fs = std::filesystem;

namespace gaia_matrix {

Platform* Platform::s_Instance = nullptr;

Platform::Platform() : 
    m_IsInitialized(false),
    m_PlatformType(PlatformType::Unknown) {
}

Platform::~Platform() {
}

bool Platform::Initialize() {
    if (s_Instance) {
        std::cerr << "Platform already initialized!" << std::endl;
        return false;
    }
    
    s_Instance = new Platform();
    
    // Detect platform
#if defined(__APPLE__)
    // Check if running on Gaia OS
    const bool isGaiaOS = false; // In a real implementation, this would check for Gaia OS
    
    if (isGaiaOS) {
        s_Instance->m_PlatformType = PlatformType::GaiaOS;
    } else {
        s_Instance->m_PlatformType = PlatformType::MacOS;
    }
#elif defined(_WIN32)
    s_Instance->m_PlatformType = PlatformType::Windows;
#elif defined(__linux__)
    s_Instance->m_PlatformType = PlatformType::Linux;
#else
    s_Instance->m_PlatformType = PlatformType::Unknown;
#endif
    
    s_Instance->m_IsInitialized = true;
    std::cout << "Platform initialized: " << GetPlatformName() << std::endl;
    
    return true;
}

void Platform::Shutdown() {
    if (!s_Instance) {
        return;
    }
    
    s_Instance->m_IsInitialized = false;
    delete s_Instance;
    s_Instance = nullptr;
}

PlatformType Platform::GetPlatformType() {
    if (!s_Instance) {
        std::cerr << "Platform not initialized!" << std::endl;
        return PlatformType::Unknown;
    }
    
    return s_Instance->m_PlatformType;
}

bool Platform::IsGaiaOS() {
    if (!s_Instance) {
        return false;
    }
    
    return s_Instance->m_PlatformType == PlatformType::GaiaOS;
}

bool Platform::IsNeuralEngineAvailable() {
    if (!s_Instance) {
        return false;
    }
    
    // Check for Apple Silicon (M1/M2/M3)
#if defined(__APPLE__)
    #if defined(__arm64__)
        // On arm64 Apple Silicon, Neural Engine is available
        // In a real implementation, this would check for specific chip models and capabilities
        return true;
    #else
        // On Intel Macs, no Neural Engine
        return false;
    #endif
#elif defined(__GAIA_OS__)
    // On Gaia OS, Neural Engine should be available
    return true;
#else
    // No Neural Engine on other platforms
    return false;
#endif
}

std::string Platform::GetApplicationDirectory() {
    // Get the current working directory
    return fs::current_path().string();
}

std::string Platform::GetUserDocumentsDirectory() {
    // This is a simplified implementation
    // In a real application, this would use platform-specific APIs
    
#if defined(_WIN32)
    return std::string(getenv("USERPROFILE")) + "\\Documents";
#else
    return std::string(getenv("HOME")) + "/Documents";
#endif
}

std::string Platform::GetPlatformName() {
    if (!s_Instance) {
        return "Unknown";
    }
    
    switch (s_Instance->m_PlatformType) {
        case PlatformType::GaiaOS:
            return "Gaia OS";
        case PlatformType::MacOS:
            return "macOS";
        case PlatformType::Windows:
            return "Windows";
        case PlatformType::Linux:
            return "Linux";
        default:
            return "Unknown";
    }
}

Platform& Platform::Get() {
    if (!s_Instance) {
        std::cerr << "Platform not initialized! Call Initialize() first." << std::endl;
        static Platform dummy;
        return dummy;
    }
    
    return *s_Instance;
}

// FileSystem implementation
bool FileSystem::FileExists(const std::string& path) {
    return fs::exists(path) && fs::is_regular_file(path);
}

bool FileSystem::DirectoryExists(const std::string& path) {
    return fs::exists(path) && fs::is_directory(path);
}

bool FileSystem::CreateDirectory(const std::string& path) {
    try {
        return fs::create_directories(path);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> FileSystem::GetFilesInDirectory(const std::string& path, const std::string& extension) {
    std::vector<std::string> files;
    
    if (!DirectoryExists(path)) {
        std::cerr << "Directory does not exist: " << path << std::endl;
        return files;
    }
    
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            if (extension.empty() || entry.path().extension() == extension) {
                files.push_back(entry.path().string());
            }
        }
    }
    
    return files;
}

} // namespace gaia_matrix