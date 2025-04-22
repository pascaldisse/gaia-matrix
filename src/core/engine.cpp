#include "gaia_matrix/core.h"
#include "gaia_matrix/neural_engine.h"
#include "gaia_matrix/renderer.h"
#include "gaia_matrix/platform.h"
#include <iostream>

namespace gaia_matrix {

Engine* Engine::s_Instance = nullptr;

Engine::Engine() : 
    m_IsInitialized(false),
    m_NeuralEngineEnabled(false) {
}

Engine::~Engine() {
    if (m_IsInitialized) {
        // Ensure proper shutdown if the instance is destroyed
        Renderer::Shutdown();
        Platform::Shutdown();
    }
}

bool Engine::Initialize(const std::string& appName, bool enableNeuralEngine) {
    if (s_Instance) {
        std::cerr << "Engine already initialized!" << std::endl;
        return false;
    }

    s_Instance = new Engine();
    s_Instance->m_AppName = appName;
    s_Instance->m_NeuralEngineEnabled = enableNeuralEngine;

    // Initialize platform layer
    if (!Platform::Initialize()) {
        std::cerr << "Failed to initialize platform layer!" << std::endl;
        delete s_Instance;
        s_Instance = nullptr;
        return false;
    }

    // Check if Neural Engine is available
    bool neuralEngineAvailable = Platform::IsNeuralEngineAvailable();
    
    // Initialize Neural Engine if enabled and available
    if (enableNeuralEngine) {
        if (neuralEngineAvailable) {
            if (!NeuralEngine::Initialize()) {
                std::cerr << "Warning: Failed to initialize Neural Engine, falling back to CPU implementation" << std::endl;
            }
        } else {
            std::cerr << "Warning: Neural Engine not available on this platform, falling back to CPU implementation" << std::endl;
        }
    }

    // Initialize renderer
    RendererConfig rendererConfig;
    rendererConfig.windowTitle = appName;
    rendererConfig.enableNeuralEnhancement = enableNeuralEngine && neuralEngineAvailable;
    
    if (!Renderer::Initialize(rendererConfig)) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        Platform::Shutdown();
        delete s_Instance;
        s_Instance = nullptr;
        return false;
    }

    s_Instance->m_IsInitialized = true;
    std::cout << "GAIA MATRIX Engine initialized successfully!" << std::endl;
    std::cout << "Platform: " << Platform::GetPlatformName() << std::endl;
    std::cout << "Neural Engine: " << (neuralEngineAvailable ? "Available" : "Not available") << std::endl;
    
    return true;
}

void Engine::Shutdown() {
    if (!s_Instance) {
        return;
    }

    // Shutdown in reverse order of initialization
    Renderer::Shutdown();
    Platform::Shutdown();

    s_Instance->m_IsInitialized = false;
    delete s_Instance;
    s_Instance = nullptr;
    
    std::cout << "GAIA MATRIX Engine shut down successfully!" << std::endl;
}

bool Engine::IsNeuralEngineAvailable() {
    return Platform::IsNeuralEngineAvailable();
}

void Engine::Run() {
    if (!s_Instance || !s_Instance->m_IsInitialized) {
        std::cerr << "Engine not initialized!" << std::endl;
        return;
    }

    auto& renderer = Renderer::Get();
    
    std::cout << "Engine running in runtime mode... Processing 5 frames." << std::endl;
    
    // Main engine loop - for demo, just process 5 frames
    for (int i = 0; i < 5; i++) {
        // Process platform events
        // ...

        // Begin frame
        renderer.BeginFrame();

        // Render frame
        // ...

        // End frame
        renderer.EndFrame();
    }
    
    std::cout << "Engine runtime completed." << std::endl;
}

Engine& Engine::Get() {
    if (!s_Instance) {
        std::cerr << "Engine not initialized! Call Initialize() first." << std::endl;
        // Create a temporary instance for safety
        static Engine dummy;
        return dummy;
    }
    
    return *s_Instance;
}

} // namespace gaia_matrix