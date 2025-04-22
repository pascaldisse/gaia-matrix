#include <gtest/gtest.h>
#include "gaia_matrix.h"
#include "../test_utils/test_helpers.h"

using namespace gaia_matrix;
using namespace gaia_matrix::test;

class PlatformTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize test environment
        Platform::Initialize();
        m_TempDir = TestHelpers::CreateTempDirectory();
        ASSERT_FALSE(m_TempDir.empty());
    }

    void TearDown() override {
        // Clean up test environment
        Platform::Shutdown();
        if (!m_TempDir.empty()) {
            TestHelpers::DeleteTempDirectory(m_TempDir);
        }
    }

    std::string m_TempDir;
};

TEST_F(PlatformTest, Initialization) {
    // Test that the platform layer initializes correctly
    // (It's already initialized in SetUp, but we'll test it can be initialized again)
    Platform::Shutdown();
    EXPECT_TRUE(Platform::Initialize());
}

TEST_F(PlatformTest, GetPlatformType) {
    // Test getting platform type
    PlatformType type = Platform::GetPlatformType();
    
    // We can't assert a specific value here since it depends on the OS
    // Just make sure the function returns a valid enum value
    EXPECT_TRUE(type == PlatformType::GaiaOS || 
                type == PlatformType::MacOS || 
                type == PlatformType::Windows || 
                type == PlatformType::Linux || 
                type == PlatformType::Unknown);
}

TEST_F(PlatformTest, GetPlatformName) {
    // Test getting platform name
    std::string name = Platform::GetPlatformName();
    
    // The name should not be empty
    EXPECT_FALSE(name.empty());
}

TEST_F(PlatformTest, IsGaiaOS) {
    // Test checking if running on Gaia OS
    bool isGaiaOS = Platform::IsGaiaOS();
    
    // We can't assert a specific value here since it depends on the OS
    // Just make sure the function runs without crashing
    SUCCEED();
}

TEST_F(PlatformTest, GetApplicationDirectory) {
    // Test getting application directory
    std::string appDir = Platform::GetApplicationDirectory();
    
    // The directory should not be empty
    EXPECT_FALSE(appDir.empty());
}

TEST_F(PlatformTest, GetUserDocumentsDirectory) {
    // Test getting user documents directory
    std::string docsDir = Platform::GetUserDocumentsDirectory();
    
    // The directory should not be empty
    EXPECT_FALSE(docsDir.empty());
}

TEST_F(PlatformTest, FileSystemFileExists) {
    // Test file existence check
    std::string testFile = TestHelpers::CreateTempFile(m_TempDir, "test.txt", "test content");
    ASSERT_FALSE(testFile.empty());
    
    // Check existence
    EXPECT_TRUE(FileSystem::FileExists(testFile));
    EXPECT_FALSE(FileSystem::FileExists(m_TempDir + "/nonexistent.txt"));
}

TEST_F(PlatformTest, FileSystemDirectoryExists) {
    // Test directory existence check
    EXPECT_TRUE(FileSystem::DirectoryExists(m_TempDir));
    EXPECT_FALSE(FileSystem::DirectoryExists(m_TempDir + "/nonexistent"));
}

TEST_F(PlatformTest, FileSystemCreateDirectory) {
    // Test directory creation
    std::string newDir = m_TempDir + "/newdir";
    EXPECT_TRUE(FileSystem::CreateDirectory(newDir));
    EXPECT_TRUE(FileSystem::DirectoryExists(newDir));
}

TEST_F(PlatformTest, FileSystemGetFilesInDirectory) {
    // Create test files
    TestHelpers::CreateTempFile(m_TempDir, "test1.txt", "test content");
    TestHelpers::CreateTempFile(m_TempDir, "test2.txt", "test content");
    TestHelpers::CreateTempFile(m_TempDir, "test.dat", "binary content");
    
    // Test getting all files
    std::vector<std::string> allFiles = FileSystem::GetFilesInDirectory(m_TempDir);
    EXPECT_EQ(allFiles.size(), 3);
    
    // Test getting files with specific extension
    std::vector<std::string> txtFiles = FileSystem::GetFilesInDirectory(m_TempDir, ".txt");
    EXPECT_EQ(txtFiles.size(), 2);
    
    // Test getting files with non-existent extension
    std::vector<std::string> xmlFiles = FileSystem::GetFilesInDirectory(m_TempDir, ".xml");
    EXPECT_TRUE(xmlFiles.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}