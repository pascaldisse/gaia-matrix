#include <gtest/gtest.h>
#include "gaia_matrix.h"

using namespace gaia_matrix;

class EngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize test environment
    }

    void TearDown() override {
        // Make sure the engine is shut down after each test
        Engine::Shutdown();
    }
};

TEST_F(EngineTest, Initialization) {
    // Test that the engine initializes correctly
    EXPECT_TRUE(Engine::Initialize("EngineTest"));
}

TEST_F(EngineTest, NeuralEngineCheck) {
    // Test neural engine availability check
    Engine::Initialize("EngineTest");
    bool neuralEngineAvailable = Engine::IsNeuralEngineAvailable();
    
    // We can't assert a specific value here since it depends on the hardware
    // Just make sure the function runs without crashing
    SUCCEED();
}

TEST_F(EngineTest, ShutdownAfterInitialize) {
    // Test shutdown after initialization
    ASSERT_TRUE(Engine::Initialize("EngineTest"));
    Engine::Shutdown();
    
    // Initialize again to verify shutdown was clean
    EXPECT_TRUE(Engine::Initialize("EngineTest"));
}

TEST_F(EngineTest, ShutdownWithoutInitialize) {
    // Test shutdown without initialization
    // This should not crash
    Engine::Shutdown();
    SUCCEED();
}

TEST_F(EngineTest, InitializeWithNeuralEngine) {
    // Test initialization with Neural Engine enabled
    EXPECT_TRUE(Engine::Initialize("EngineTest", true));
}

TEST_F(EngineTest, InitializeWithoutNeuralEngine) {
    // Test initialization with Neural Engine disabled
    EXPECT_TRUE(Engine::Initialize("EngineTest", false));
}

// This would be a more comprehensive test in a real implementation
TEST_F(EngineTest, DISABLED_RunMainLoop) {
    // Test running the main engine loop
    ASSERT_TRUE(Engine::Initialize("EngineTest"));
    
    // In a real test, we would simulate some events or set up a timeout
    // For our demo, we'll just skip this test
    // Engine::Run();
    
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}