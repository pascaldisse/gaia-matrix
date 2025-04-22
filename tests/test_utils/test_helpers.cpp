#include "test_helpers.h"
#include <fstream>
#include <random>
#include <iostream>

namespace fs = std::filesystem;

namespace gaia_matrix {
namespace test {

std::string TestHelpers::CreateTempDirectory() {
    // Create a unique temporary directory path
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    
    std::string tempDir = fs::temp_directory_path().string() + "/gaia_matrix_test_" + std::to_string(dis(gen));
    
    try {
        fs::create_directories(tempDir);
        return tempDir;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating temporary directory: " << e.what() << std::endl;
        return "";
    }
}

bool TestHelpers::DeleteTempDirectory(const std::string& path) {
    try {
        fs::remove_all(path);
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error deleting temporary directory: " << e.what() << std::endl;
        return false;
    }
}

std::string TestHelpers::CreateTempFile(
    const std::string& directory,
    const std::string& filename,
    const std::string& content
) {
    std::string filePath = directory + "/" + filename;
    
    try {
        std::ofstream file(filePath);
        if (!file) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return "";
        }
        
        file << content;
        file.close();
        
        return filePath;
    } catch (const std::exception& e) {
        std::cerr << "Error creating temporary file: " << e.what() << std::endl;
        return "";
    }
}

std::string TestHelpers::CreateDummyONNXModel(
    const std::string& directory,
    const std::string& filename
) {
    // Create a very simple dummy ONNX model file
    // In a real implementation, this would create a valid ONNX file
    // For testing purposes, we'll just create a file with some dummy content
    
    // Dummy header to make the file appear like an ONNX model
    const std::string dummyHeader = "ONNX-ML-IR-VERSION:7";
    
    return CreateTempFile(directory, filename, dummyHeader);
}

std::string TestHelpers::CreateDummyAOPLScript(
    const std::string& directory,
    const std::string& filename
) {
    // Create a simple AOPL script for testing
    const std::string aopl = R"(
# Test AOPL Script
N ⊢ E〈TestEntity〉〈T⊕C⊕I〉
T: P 0 0 0 → R 0 0 0 → S 1 1 1
C: F TestFunction → ⊻ OnUpdate
I: ⊢ K → M

# Test Component
N〈TestComponent〉: V ⊢ I → F Move
Move: I.K W → T.P z+ 0.1
)";
    
    return CreateTempFile(directory, filename, aopl);
}

std::string TestHelpers::GetTestResourcesPath() {
    // In a real implementation, this would return the path to test resources
    // For our demo, we'll just return a placeholder
    return fs::current_path().string() + "/tests/resources";
}

} // namespace test
} // namespace gaia_matrix