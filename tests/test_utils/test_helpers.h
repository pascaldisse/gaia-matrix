#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace gaia_matrix {
namespace test {

/**
 * @brief Test utilities for GAIA MATRIX
 */
class TestHelpers {
public:
    /**
     * @brief Create a temporary directory for tests
     * @return Path to created directory
     */
    static std::string CreateTempDirectory();

    /**
     * @brief Delete a temporary directory and its contents
     * @param path Path to directory to delete
     * @return True if deletion was successful
     */
    static bool DeleteTempDirectory(const std::string& path);

    /**
     * @brief Create a temporary file with given content
     * @param directory Directory to create file in
     * @param filename Name of the file
     * @param content Content to write to file
     * @return Full path to created file
     */
    static std::string CreateTempFile(
        const std::string& directory,
        const std::string& filename,
        const std::string& content
    );

    /**
     * @brief Create a dummy ONNX model file for testing
     * @param directory Directory to create model in
     * @param filename Name of the model file
     * @return Full path to created model file
     */
    static std::string CreateDummyONNXModel(
        const std::string& directory,
        const std::string& filename = "test_model.onnx"
    );

    /**
     * @brief Create a dummy AOPL script file for testing
     * @param directory Directory to create script in
     * @param filename Name of the script file
     * @return Full path to created script file
     */
    static std::string CreateDummyAOPLScript(
        const std::string& directory,
        const std::string& filename = "test_script.aopl"
    );

    /**
     * @brief Get path to test resources directory
     * @return Path to test resources
     */
    static std::string GetTestResourcesPath();
};

} // namespace test
} // namespace gaia_matrix