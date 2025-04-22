#pragma once

#include <string>
#include <memory>
#include <vector>

namespace gaia_matrix {

/**
 * @brief Core engine initialization and management functions
 */
class Engine {
public:
    /**
     * @brief Initialize the GAIA MATRIX engine
     * @param appName Name of the application
     * @param enableNeuralEngine Whether to enable Neural Engine features
     * @return True if initialization succeeded
     */
    static bool Initialize(const std::string& appName, bool enableNeuralEngine = true);

    /**
     * @brief Shutdown the engine and release resources
     */
    static void Shutdown();

    /**
     * @brief Check if Neural Engine is available on current hardware
     * @return True if Neural Engine is available
     */
    static bool IsNeuralEngineAvailable();

    /**
     * @brief Run the main engine loop
     */
    static void Run();
    
    /**
     * @brief Get the singleton instance
     * @return Engine instance
     */
    static Engine& Get();

private:
    // Private constructor and destructor for singleton pattern
    Engine();
    ~Engine();
    
    // Static instance
    static Engine* s_Instance;
    bool m_IsInitialized;
    bool m_NeuralEngineEnabled;
    std::string m_AppName;
};

} // namespace gaia_matrix