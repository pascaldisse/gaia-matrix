#pragma once

#include "gaia_matrix/neural_engine.h"
#include <string>
#include <vector>
#include <map>

namespace gaia_matrix {
namespace test {

/**
 * @brief Mock implementation of NeuralEngine for testing
 * 
 * This class provides a mock implementation of the NeuralEngine for testing purposes.
 * It allows setting expectations for method calls and returning predefined results.
 */
class MockNeuralEngine {
public:
    MockNeuralEngine();
    ~MockNeuralEngine();

    // Mock configuration methods
    void ExpectLoadModel(const std::string& modelPath, int returnId);
    void ExpectUnloadModel(int modelId);
    void ExpectRunInference(int modelId, const std::vector<float>& returnData);
    void SetAvailable(bool available);

    // NeuralEngine API mock implementations
    static bool Initialize();
    static bool IsAvailable();
    int LoadModel(const std::string& modelPath);
    void UnloadModel(int modelId);
    std::vector<float> RunInference(
        int modelId, 
        const std::vector<float>& inputData, 
        const std::array<int, 4>& inputShape
    );

    // Verification methods
    bool VerifyAllExpectations() const;
    void Reset();

private:
    bool m_IsAvailable;
    
    // Expectations
    struct ModelExpectation {
        std::string path;
        int returnId;
        bool called;
    };
    
    struct UnloadExpectation {
        int modelId;
        bool called;
    };
    
    struct InferenceExpectation {
        int modelId;
        std::vector<float> returnData;
        bool called;
    };
    
    std::vector<ModelExpectation> m_LoadExpectations;
    std::vector<UnloadExpectation> m_UnloadExpectations;
    std::vector<InferenceExpectation> m_InferenceExpectations;
    
    // Original NeuralEngine instance for restoration
    static NeuralEngine* s_OriginalInstance;
};

} // namespace test
} // namespace gaia_matrix