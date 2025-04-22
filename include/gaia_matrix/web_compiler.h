#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "gaia_matrix/renderer.h"

namespace gaia_matrix {

/**
 * @brief Shader type enumeration
 */
enum class ShaderType {
    Vertex,
    Fragment,
    Compute
};

/**
 * @brief Web compiler output format
 */
enum class WebOutputFormat {
    ESNext,    // Modern JavaScript
    ES5,       // Legacy JavaScript
    WASM       // WebAssembly
};

/**
 * @brief Web compiler configuration
 */
struct WebCompilerConfig {
    WebOutputFormat outputFormat = WebOutputFormat::ESNext;
    bool minify = true;
    bool generateSourceMap = true;
    bool enableWebWorkers = true;
    bool enableWebGPU = false; // Future option
    std::string outputDir = "./web_build";
};

/**
 * @brief AOPL to WebGL/JavaScript compiler
 * 
 * This class compiles GAIA MATRIX AOPL code to WebGL-compatible JavaScript or WASM
 * that can run in modern web browsers.
 */
class WebCompiler {
public:
    /**
     * @brief Initialize the web compiler
     * @param config Compiler configuration
     * @return True if initialization succeeded
     */
    static bool Initialize(const WebCompilerConfig& config = WebCompilerConfig());
    
    /**
     * @brief Shutdown the compiler and release resources
     */
    static void Shutdown();
    
    /**
     * @brief Get the singleton instance
     * @return WebCompiler instance
     */
    static WebCompiler& Get();
    
    /**
     * @brief Compile AOPL source code to WebGL-compatible JavaScript or WASM
     * @param source AOPL source code
     * @param outputPath Path to write the compiled output
     * @return True if compilation succeeded
     */
    bool CompileAOPL(const std::string& source, const std::string& outputPath);
    
    /**
     * @brief Compile a shader to WebGL-compatible GLSL
     * @param source Shader source code
     * @param type Shader type
     * @return Compiled shader code as string
     */
    std::string CompileShader(const std::string& source, ShaderType type);
    
    /**
     * @brief Generate a complete web application with the engine and compiled code
     * @param projectName Name of the project
     * @param outputDir Directory to output the web application
     * @param aoplSources Map of AOPL source files to compile
     * @param includeEditor Whether to include the browser-based editor
     * @return True if generation succeeded
     */
    bool GenerateWebApp(
        const std::string& projectName,
        const std::string& outputDir,
        const std::map<std::string, std::string>& aoplSources,
        bool includeEditor = false
    );
    
private:
    WebCompiler();
    ~WebCompiler();
    
    /**
     * @brief Generate HTML template for the web application
     * @param projectName Name of the project
     * @param includeEditor Whether to include the browser-based editor
     * @return HTML content as string
     */
    std::string GenerateHTMLTemplate(const std::string& projectName, bool includeEditor);
    
    /**
     * @brief Generate WebGL renderer JavaScript
     * @return JavaScript code as string
     */
    std::string GenerateWebGLRenderer();
    
    /**
     * @brief Generate browser-based editor UI
     * @return JavaScript code for the editor UI
     */
    std::string GenerateBrowserEditor();
    
    /**
     * @brief Generate AOPL runtime for web
     * @return JavaScript code for AOPL runtime
     */
    std::string GenerateAOPLRuntime();
    
    /**
     * @brief Transpile AOPL to JavaScript
     * @param source AOPL source code
     * @return JavaScript code
     */
    std::string TranspileAOPLToJS(const std::string& source);
    
    /**
     * @brief Compile AOPL to WebAssembly
     * @param source AOPL source code
     * @return WebAssembly binary data
     */
    std::vector<uint8_t> CompileAOPLToWASM(const std::string& source);
    
    static WebCompiler* s_Instance;
    WebCompilerConfig m_Config;
    bool m_IsInitialized;
};

} // namespace gaia_matrix