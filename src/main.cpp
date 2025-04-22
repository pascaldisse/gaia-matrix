#include "gaia_matrix.h"
#include "gaia_matrix/web_compiler.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>

/**
 * @brief Build web version of GAIA MATRIX project 
 * @param outputDir Directory to output the web application
 * @param includeEditor Whether to include the browser-based editor
 * @param format Output format (ESNext, ES5, WASM)
 * @param minify Whether to minify the output
 * @return True if build succeeded
 */
bool BuildWebVersion(const std::string& outputDir, bool includeEditor, 
                    gaia_matrix::WebOutputFormat format, bool minify) {
    // Initialize web compiler
    gaia_matrix::WebCompilerConfig config;
    config.outputFormat = format;
    config.minify = minify;
    config.outputDir = outputDir;
    
    if (!gaia_matrix::WebCompiler::Initialize(config)) {
        std::cerr << "Failed to initialize web compiler!" << std::endl;
        return false;
    }
    
    // Collect AOPL source files
    std::map<std::string, std::string> aoplSources;
    if (std::filesystem::exists("examples")) {
        for (const auto& entry : std::filesystem::directory_iterator("examples")) {
            if (entry.path().extension() == ".aopl") {
                std::string filename = entry.path().filename().string();
                std::ifstream file(entry.path());
                
                if (file) {
                    std::stringstream buffer;
                    buffer << file.rdbuf();
                    aoplSources[filename] = buffer.str();
                }
            }
        }
    }
    
    // Add a default example if none found
    if (aoplSources.empty()) {
        aoplSources["default.aopl"] = 
            "// Default GAIA MATRIX AOPL Example\n"
            "E SimpleEntity {\n"
            "  T {\n"
            "    position: [0, 0, 0],\n"
            "    rotation: [0, 0, 0],\n"
            "    scale: [1, 1, 1]\n"
            "  }\n"
            "\n"
            "  ⊻ Update(dt) {\n"
            "    // Simple rotation\n"
            "    T.rotation.y → T.rotation.y + 1.0 * dt\n"
            "  }\n"
            "}";
    }
    
    // Generate web application
    if (!gaia_matrix::WebCompiler::Get().GenerateWebApp("GAIA MATRIX Demo", outputDir, aoplSources, includeEditor)) {
        std::cerr << "Failed to generate web application!" << std::endl;
        gaia_matrix::WebCompiler::Shutdown();
        return false;
    }
    
    gaia_matrix::WebCompiler::Shutdown();
    return true;
}

/**
 * @brief Print command line usage
 * @param programName Name of the executable
 */
void PrintUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --no-editor          Disable editor (run in runtime mode)" << std::endl;
    std::cout << "  --no-neural-engine   Disable Neural Engine" << std::endl;
    std::cout << "  --project <path>     Path to project" << std::endl;
    std::cout << "  --app-name <name>    Application name" << std::endl;
    std::cout << "  --web-build <dir>    Build web version to specified directory" << std::endl;
    std::cout << "  --web-editor         Include browser editor in web build" << std::endl;
    std::cout << "  --web-format <fmt>   Web output format: esnext, es5, wasm (default: esnext)" << std::endl;
    std::cout << "  --no-minify          Disable minification of web output" << std::endl;
    std::cout << "  --help               Show this help message" << std::endl;
}

/**
 * @brief GAIA MATRIX main entry point
 */
int main(int argc, char** argv) {
    using namespace gaia_matrix;
    
    std::cout << "GAIA MATRIX Engine " << Version::GetVersionString() << std::endl;
    std::cout << "Game Artificial Intelligence Acceleration: Machine-learning Architecture for Technology, Rendering, Intelligence & cross-platform" << std::endl;
    std::cout << std::endl;
    
    // Parse command line arguments
    bool enableEditor = true;
    bool enableNeuralEngine = true;
    bool webBuild = false;
    bool webEditor = false;
    bool minify = true;
    WebOutputFormat webFormat = WebOutputFormat::ESNext;
    std::string appName = "GAIA MATRIX";
    std::string projectPath = "";
    std::string webOutputDir = "./web_build";
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--no-editor") {
            enableEditor = false;
        } else if (arg == "--no-neural-engine") {
            enableNeuralEngine = false;
        } else if (arg == "--project" && i + 1 < argc) {
            projectPath = argv[++i];
        } else if (arg == "--app-name" && i + 1 < argc) {
            appName = argv[++i];
        } else if (arg == "--web-build" && i + 1 < argc) {
            webBuild = true;
            webOutputDir = argv[++i];
        } else if (arg == "--web-editor") {
            webEditor = true;
        } else if (arg == "--web-format" && i + 1 < argc) {
            std::string format = argv[++i];
            if (format == "esnext") {
                webFormat = WebOutputFormat::ESNext;
            } else if (format == "es5") {
                webFormat = WebOutputFormat::ES5;
            } else if (format == "wasm") {
                webFormat = WebOutputFormat::WASM;
            } else {
                std::cerr << "Unknown web format: " << format << std::endl;
                PrintUsage(argv[0]);
                return 1;
            }
        } else if (arg == "--no-minify") {
            minify = false;
        } else if (arg == "--help") {
            PrintUsage(argv[0]);
            return 0;
        }
    }
    
    // Handle web build mode
    if (webBuild) {
        std::cout << "Building web version to: " << webOutputDir << std::endl;
        if (BuildWebVersion(webOutputDir, webEditor, webFormat, minify)) {
            std::cout << "Web build successful!" << std::endl;
            return 0;
        } else {
            std::cerr << "Web build failed!" << std::endl;
            return 1;
        }
    }
    
    // Initialize engine
    if (!Engine::Initialize(appName, enableNeuralEngine)) {
        std::cerr << "Failed to initialize GAIA MATRIX Engine!" << std::endl;
        return 1;
    }
    
    // Initialize editor if enabled
    if (enableEditor) {
        EditorConfig editorConfig;
        editorConfig.projectPath = projectPath;
        
        if (!Editor::Initialize(editorConfig)) {
            std::cerr << "Failed to initialize editor!" << std::endl;
            Engine::Shutdown();
            return 1;
        }
        
        // Initialize AI Assistant
        if (!AIAssistant::Initialize()) {
            std::cerr << "Warning: Failed to initialize AI Assistant" << std::endl;
        }
        
        // Run editor
        Editor::Get().Run();
    } else {
        // Run engine in runtime mode
        Engine::Run();
    }
    
    // Shutdown in reverse order of initialization
    if (enableEditor) {
        AIAssistant::Shutdown();
        Editor::Shutdown();
    }
    
    Engine::Shutdown();
    
    return 0;
}