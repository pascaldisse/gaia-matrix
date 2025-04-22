#include "mock_neural_engine.h"
#include <iostream>

namespace gaia_matrix {
namespace test {

// Static variable to store original NeuralEngine
NeuralEngine* MockNeuralEngine::s_OriginalInstance = nullptr;

MockNeuralEngine::MockNeuralEngine() : m_IsAvailable(true) {
    // Store the original instance for later restoration
    // In a real implementation, this would involve more complex pointer manipulation
    // This is just a simulated implementation for our demo
    s_OriginalInstance = &NeuralEngine::Get();
    
    // Clear expectations
    Reset();
}

MockNeuralEngine::~MockNeuralEngine() {
    // Verify all expectations were met
    if (!VerifyAllExpectations()) {
        std::cerr << "Warning: Not all MockNeuralEngine expectations were met!" << std::endl;
    }
    
    // Restore original NeuralEngine (in a real implementation)
}

void MockNeuralEngine::ExpectLoadModel(const std::string& modelPath, int returnId) {
    ModelExpectation expectation;
    expectation.path = modelPath;
    expectation.returnId = returnId;
    expectation.called = false;
    
    m_LoadExpectations.push_back(expectation);
}

void MockNeuralEngine::ExpectUnloadModel(int modelId) {
    UnloadExpectation expectation;
    expectation.modelId = modelId;
    expectation.called = false;
    
    m_UnloadExpectations.push_back(expectation);
}

void MockNeuralEngine::ExpectRunInference(int modelId, const std::vector<float>& returnData) {
    InferenceExpectation expectation;
    expectation.modelId = modelId;
    expectation.returnData = returnData;
    expectation.called = false;
    
    m_InferenceExpectations.push_back(expectation);
}

void MockNeuralEngine::SetAvailable(bool available) {
    m_IsAvailable = available;
}

bool MockNeuralEngine::Initialize() {
    // Mock initialization always succeeds
    return true;
}

bool MockNeuralEngine::IsAvailable() {
    return true; // Always available in tests unless specifically set otherwise
}

int MockNeuralEngine::LoadModel(const std::string& modelPath) {
    // Find matching expectation
    for (auto& expectation : m_LoadExpectations) {
        if (expectation.path == modelPath && !expectation.called) {
            expectation.called = true;
            return expectation.returnId;
        }
    }
    
    // No matching expectation found
    std::cerr << "Unexpected call to LoadModel with path: " << modelPath << std::endl;
    return -1;
}

void MockNeuralEngine::UnloadModel(int modelId) {
    // Find matching expectation
    for (auto& expectation : m_UnloadExpectations) {
        if (expectation.modelId == modelId && !expectation.called) {
            expectation.called = true;
            return;
        }
    }
    
    // No matching expectation found
    std::cerr << "Unexpected call to UnloadModel with ID: " << modelId << std::endl;
}

std::vector<float> MockNeuralEngine::RunInference(
    int modelId, 
    const std::vector<float>& inputData, 
    const std::array<int, 4>& inputShape
) {
    // Find matching expectation
    for (auto& expectation : m_InferenceExpectations) {
        if (expectation.modelId == modelId && !expectation.called) {
            expectation.called = true;
            return expectation.returnData;
        }
    }
    
    // No matching expectation found
    std::cerr << "Unexpected call to RunInference with model ID: " << modelId << std::endl;
    return {};
}

bool MockNeuralEngine::VerifyAllExpectations() const {
    bool allMet = true;
    
    // Check all load expectations
    for (const auto& expectation : m_LoadExpectations) {
        if (!expectation.called) {
            std::cerr << "Expected LoadModel call with path '" << expectation.path 
                      << "' was not made" << std::endl;
            allMet = false;
        }
    }
    
    // Check all unload expectations
    for (const auto& expectation : m_UnloadExpectations) {
        if (!expectation.called) {
            std::cerr << "Expected UnloadModel call with ID " << expectation.modelId 
                      << " was not made" << std::endl;
            allMet = false;
        }
    }
    
    // Check all inference expectations
    for (const auto& expectation : m_InferenceExpectations) {
        if (!expectation.called) {
            std::cerr << "Expected RunInference call with model ID " << expectation.modelId 
                      << " was not made" << std::endl;
            allMet = false;
        }
    }
    
    return allMet;
}

void MockNeuralEngine::Reset() {
    m_LoadExpectations.clear();
    m_UnloadExpectations.clear();
    m_InferenceExpectations.clear();
}

} // namespace test
} // namespace gaia_matrix