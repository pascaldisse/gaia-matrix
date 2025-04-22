#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>

namespace gaia_matrix {

/**
 * @brief Neural Engine Integration for GAIA MATRIX
 * 
 * Provides direct access to Apple Silicon Neural Engine via Gaia OS.
 * Falls back to CPU implementation when Neural Engine is not available.
 */
class NeuralEngine {
public:
    /**
     * @brief Initialize the Neural Engine
     * @return True if initialization succeeded
     */
    static bool Initialize();

    /**
     * @brief Check if Neural Engine is available
     * @return True if Neural Engine is available
     */
    static bool IsAvailable();
    
    /**
     * @brief Load an ONNX model for Neural Engine execution
     * @param modelPath Path to the ONNX model file
     * @return Model ID or -1 if loading failed
     */
    int LoadModel(const std::string& modelPath);
    
    /**
     * @brief Unload a previously loaded model
     * @param modelId Model ID to unload
     */
    void UnloadModel(int modelId);
    
    /**
     * @brief Run inference on loaded model
     * @param modelId Model ID to run inference on
     * @param inputData Input data for the model
     * @param inputShape Shape of the input data
     * @return Output data from the model
     */
    std::vector<float> RunInference(int modelId, const std::vector<float>& inputData, const std::array<int, 4>& inputShape);
    
    /**
     * @brief Get the singleton instance
     * @return NeuralEngine instance
     */
    static NeuralEngine& Get();

private:
    // Private constructor and destructor for singleton pattern
    NeuralEngine();
    ~NeuralEngine();
    
    /**
     * @brief Shutdown and release resources
     */
    void Shutdown();
    
    static NeuralEngine* s_Instance;
    bool m_IsInitialized;
    bool m_IsNeuralEngineAvailable;
    
    // Model storage
    struct Model;
    std::vector<std::unique_ptr<Model>> m_LoadedModels;
};

/**
 * @brief Neural network model for Model-Controlled Procedural Generation (MCP)
 */
class MCPModel {
public:
    MCPModel(const std::string& modelPath);
    ~MCPModel();
    
    /**
     * @brief Generate procedural content based on input parameters
     * @param seed Random seed for generation
     * @param parameters Generation parameters
     * @return True if generation succeeded
     */
    bool Generate(int seed, const std::vector<float>& parameters);
    
    /**
     * @brief Get generated data
     * @return Generated data
     */
    const std::vector<float>& GetGeneratedData() const;
    
private:
    int m_ModelId = -1;
    std::vector<float> m_GeneratedData;
    bool m_IsGenerated = false;
};

} // namespace gaia_matrix