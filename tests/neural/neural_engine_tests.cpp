#include <gtest/gtest.h>
#include "gaia_matrix.h"
#include "../test_utils/mock_neural_engine.h"

using namespace gaia_matrix;

class NeuralEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize test environment
    }

    void TearDown() override {
        // Clean up test environment
    }
};

TEST_F(NeuralEngineTest, Initialization) {
    // Test that the NeuralEngine initializes correctly
    EXPECT_TRUE(NeuralEngine::Initialize());
    EXPECT_TRUE(NeuralEngine::IsAvailable());
}

TEST_F(NeuralEngineTest, ModelLoading) {
    // Initialize NeuralEngine
    ASSERT_TRUE(NeuralEngine::Initialize());
    
    // Get NeuralEngine instance
    NeuralEngine& engine = NeuralEngine::Get();
    
    // Test model loading
    int modelId = engine.LoadModel("models/test_model.onnx");
    
    // Verify model was loaded (in the real implementation, this would check if modelId >= 0)
    // For our test framework, we'll just assume it worked
    EXPECT_GE(modelId, 0);
    
    // Clean up
    engine.UnloadModel(modelId);
}

TEST_F(NeuralEngineTest, RunInference) {
    // Initialize NeuralEngine
    ASSERT_TRUE(NeuralEngine::Initialize());
    
    // Get NeuralEngine instance
    NeuralEngine& engine = NeuralEngine::Get();
    
    // Load model
    int modelId = engine.LoadModel("models/test_model.onnx");
    ASSERT_GE(modelId, 0);
    
    // Prepare input data
    std::vector<float> inputData = {1.0f, 2.0f, 3.0f, 4.0f};
    std::array<int, 4> inputShape = {1, 4, 1, 1};
    
    // Run inference
    std::vector<float> results = engine.RunInference(modelId, inputData, inputShape);
    
    // Verify results (in a real test, we would check specific values)
    EXPECT_FALSE(results.empty());
    
    // Clean up
    engine.UnloadModel(modelId);
}

TEST_F(NeuralEngineTest, MCPModelGenerateContent) {
    // Initialize NeuralEngine
    ASSERT_TRUE(NeuralEngine::Initialize());
    
    // Create MCPModel
    MCPModel model("models/test_mcp_model.onnx");
    
    // Prepare parameters
    std::vector<float> parameters = {1.0f, 0.5f, 0.8f};
    
    // Generate content
    EXPECT_TRUE(model.Generate(12345, parameters));
    
    // Get generated data
    const std::vector<float>& generatedData = model.GetGeneratedData();
    
    // Verify data (in a real test, we would check specific values)
    EXPECT_FALSE(generatedData.empty());
}

TEST_F(NeuralEngineTest, MockTest) {
    // Create mock neural engine
    test::MockNeuralEngine mockEngine;
    
    // Set expectations
    mockEngine.ExpectLoadModel("models/test_model.onnx", 42);
    mockEngine.ExpectRunInference(42, {0.1f, 0.2f, 0.3f});
    mockEngine.ExpectUnloadModel(42);
    
    // Test with mock
    int modelId = mockEngine.LoadModel("models/test_model.onnx");
    EXPECT_EQ(modelId, 42);
    
    std::vector<float> inputData = {1.0f, 2.0f, 3.0f};
    std::array<int, 4> inputShape = {1, 3, 1, 1};
    std::vector<float> results = mockEngine.RunInference(modelId, inputData, inputShape);
    
    // Check results
    ASSERT_EQ(results.size(), 3);
    EXPECT_FLOAT_EQ(results[0], 0.1f);
    EXPECT_FLOAT_EQ(results[1], 0.2f);
    EXPECT_FLOAT_EQ(results[2], 0.3f);
    
    // Clean up
    mockEngine.UnloadModel(modelId);
    
    // Verify expectations
    EXPECT_TRUE(mockEngine.VerifyAllExpectations());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}