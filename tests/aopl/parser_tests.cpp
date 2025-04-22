#include <gtest/gtest.h>
#include "gaia_matrix.h"

using namespace gaia_matrix;
using namespace gaia_matrix::aopl;

class AOPLParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser = std::make_unique<Parser>();
    }

    void TearDown() override {
        parser.reset();
    }

    std::unique_ptr<Parser> parser;
};

TEST_F(AOPLParserTest, EmptyCode) {
    // Test parsing empty code
    EXPECT_TRUE(parser->Parse(""));
    EXPECT_TRUE(parser->GetEntities().empty());
}

TEST_F(AOPLParserTest, BasicEntityDefinition) {
    // Test parsing a basic entity definition
    const std::string code = R"(
        # Basic entity
        N ⊢ E〈TestEntity〉
    )";
    
    EXPECT_TRUE(parser->Parse(code));
    
    // Verify entity was parsed
    const auto& entities = parser->GetEntities();
    ASSERT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0]->GetName(), "TestEntity");
}

TEST_F(AOPLParserTest, EntityWithComponents) {
    // Test parsing an entity with components
    const std::string code = R"(
        # Entity with components
        N ⊢ E〈TestEntity〉〈T⊕C⊕I〉
        T: P 0 1 0 → R 0 0 0 → S 1 1 1
        C: F Move Jump → ⊻ OnUpdate OnCollision
        I: ⊢ K → M → G
    )";
    
    EXPECT_TRUE(parser->Parse(code));
    
    // Verify entity and components were parsed
    const auto& entities = parser->GetEntities();
    ASSERT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0]->GetName(), "TestEntity");
    
    // In a real implementation, we would check for specific components
    // For now, we're just checking the basic parsing succeeded
}

TEST_F(AOPLParserTest, MultipleEntities) {
    // Test parsing multiple entities
    const std::string code = R"(
        # First entity
        N ⊢ E〈Entity1〉
        
        # Second entity
        N ⊢ E〈Entity2〉
    )";
    
    EXPECT_TRUE(parser->Parse(code));
    
    // Verify entities were parsed
    const auto& entities = parser->GetEntities();
    ASSERT_EQ(entities.size(), 2);
    EXPECT_EQ(entities[0]->GetName(), "Entity1");
    EXPECT_EQ(entities[1]->GetName(), "Entity2");
}

TEST_F(AOPLParserTest, NeuralNetworkDefinition) {
    // Test parsing a neural network definition
    const std::string code = R"(
        # Neural network
        NN〈Animator〉: E Player → O Animation
        ⊸ Model "models/animator.onnx"
        ⊸ Input V T.R I.K
        ⊸ Output "walk" "run" "jump" "idle"
    )";
    
    EXPECT_TRUE(parser->Parse(code));
    
    // In a real implementation, we would check that the neural network was parsed correctly
    // For now, we're just checking the basic parsing succeeded
}

TEST_F(AOPLParserTest, CompleteExample) {
    // Test parsing a complete example from README
    const std::string code = R"(
        # Player Entity Definition
        N ⊢ E〈PlayerEntity〉〈T⊕C⊕I〉
        T: P 0 1 0 → R 0 0 0 → S 1 1 1
        C: F Move Jump → ⊻ OnUpdate OnCollision
        I: ⊢ K → M → G

        # Player Controller Component
        N〈PlayerController〉: V ⊢ I → F Move → A Jump → C Collision
        Move: I.K W → T.P z+ 0.1
        Move: I.K S → T.P z- 0.1
        Move: I.K A → T.P x- 0.1
        Move: I.K D → T.P x+ 0.1
        Jump: I.K Space → V.y 5 → ⊿ grounded
        Collision: ⊿ ground → ⊸ grounded true → V.y 0

        # Neural Network for Player Animation
        NN〈PlayerAnimator〉: E PlayerEntity → O Animation
        ⊸ Model "models/player_animator.onnx"
        ⊸ Input V T.R I.K
        ⊸ Output "walk" "run" "jump" "idle"
    )";
    
    EXPECT_TRUE(parser->Parse(code));
    
    // Verify entity was parsed
    const auto& entities = parser->GetEntities();
    ASSERT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0]->GetName(), "PlayerEntity");
}

TEST_F(AOPLParserTest, Compilation) {
    // Test compilation
    const std::string code = R"(
        # Basic entity
        N ⊢ E〈TestEntity〉
    )";
    
    EXPECT_TRUE(parser->Parse(code));
    EXPECT_TRUE(parser->Compile());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}