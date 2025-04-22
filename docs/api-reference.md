# GAIA MATRIX API Reference

## Core Engine API

### Engine

```cpp
namespace gaia_matrix {

class Engine {
public:
    // Initialize the GAIA MATRIX engine
    // appName: Name of the application
    // enableNeuralEngine: Whether to enable Neural Engine features
    // Returns: True if initialization succeeded
    static bool Initialize(const std::string& appName, bool enableNeuralEngine = true);
    
    // Shutdown the engine and release resources
    static void Shutdown();
    
    // Check if Neural Engine is available on current hardware
    // Returns: True if Neural Engine is available
    static bool IsNeuralEngineAvailable();
    
    // Run the main engine loop
    static void Run();
};

} // namespace gaia_matrix
```

### Platform

```cpp
namespace gaia_matrix {

enum class PlatformType {
    Unknown,
    GaiaOS,
    MacOS,
    Windows,
    Linux
};

class Platform {
public:
    // Initialize the platform layer
    // Returns: True if initialization succeeded
    static bool Initialize();
    
    // Shutdown the platform layer
    static void Shutdown();
    
    // Get the current platform type
    // Returns: Platform type
    static PlatformType GetPlatformType();
    
    // Check if running on Gaia OS
    // Returns: True if running on Gaia OS
    static bool IsGaiaOS();
    
    // Check if Neural Engine is available
    // Returns: True if Neural Engine is available
    static bool IsNeuralEngineAvailable();
    
    // Get application directory
    // Returns: Application directory path
    static std::string GetApplicationDirectory();
    
    // Get user documents directory
    // Returns: User documents directory path
    static std::string GetUserDocumentsDirectory();
    
    // Get platform name as string
    // Returns: Platform name
    static std::string GetPlatformName();
    
    // Get the singleton instance
    // Returns: Platform instance
    static Platform& Get();
};

class FileSystem {
public:
    // Check if file exists
    // path: File path
    // Returns: True if file exists
    static bool FileExists(const std::string& path);
    
    // Check if directory exists
    // path: Directory path
    // Returns: True if directory exists
    static bool DirectoryExists(const std::string& path);
    
    // Create directory
    // path: Directory path
    // Returns: True if directory created successfully
    static bool CreateDirectory(const std::string& path);
    
    // Get files in directory
    // path: Directory path
    // extension: Optional file extension filter
    // Returns: List of files
    static std::vector<std::string> GetFilesInDirectory(
        const std::string& path, 
        const std::string& extension = ""
    );
};

} // namespace gaia_matrix
```

## Neural Engine API

### NeuralEngine

```cpp
namespace gaia_matrix {

class NeuralEngine {
public:
    // Initialize the Neural Engine
    // Returns: True if initialization succeeded
    static bool Initialize();
    
    // Check if Neural Engine is available
    // Returns: True if Neural Engine is available
    static bool IsAvailable();
    
    // Load an ONNX model for Neural Engine execution
    // modelPath: Path to the ONNX model file
    // Returns: Model ID or -1 if loading failed
    int LoadModel(const std::string& modelPath);
    
    // Unload a previously loaded model
    // modelId: Model ID to unload
    void UnloadModel(int modelId);
    
    // Run inference on loaded model
    // modelId: Model ID to run inference on
    // inputData: Input data for the model
    // inputShape: Shape of the input data
    // Returns: Output data from the model
    std::vector<float> RunInference(
        int modelId, 
        const std::vector<float>& inputData, 
        const std::array<int, 4>& inputShape
    );
    
    // Enable profiling for inference operations
    void EnableProfiling();
    
    // Disable profiling
    void DisableProfiling();
    
    // Get profiling data from the last inference
    ProfilingData GetProfilingData();
    
    // Check if model is using fallback implementation
    // modelId: Model ID to check
    // Returns: True if using fallback
    bool IsUsingFallback(int modelId);
    
    // Set Neural Engine configuration
    // config: Configuration settings
    void SetConfig(const NeuralEngineConfig& config);
    
    // Enable logging for Neural Engine operations
    // level: Logging level
    void EnableLogging(LogLevel level);
    
    // Get the last error message
    // Returns: Error message
    std::string GetLastError();
    
    // Get the singleton instance
    // Returns: NeuralEngine instance
    static NeuralEngine& Get();
};

// Neural network model for Model-Controlled Procedural Generation (MCP)
class MCPModel {
public:
    // Constructor
    // modelPath: Path to the ONNX model file
    MCPModel(const std::string& modelPath);
    
    // Destructor
    ~MCPModel();
    
    // Generate procedural content based on input parameters
    // seed: Random seed for generation
    // parameters: Generation parameters
    // Returns: True if generation succeeded
    bool Generate(int seed, const std::vector<float>& parameters);
    
    // Get generated data
    // Returns: Generated data
    const std::vector<float>& GetGeneratedData() const;
};

// Configuration for Neural Engine
struct NeuralEngineConfig {
    enum class FallbackType {
        CPU,
        GPU,
        None
    };
    
    enum class FallbackPriority {
        Performance,
        Accuracy
    };
    
    FallbackType fallbackType = FallbackType::CPU;
    FallbackPriority fallbackPriority = FallbackPriority::Performance;
    int maxBatchSize = 4;
    bool cacheModels = true;
    bool optimizeModels = true;
};

// Profiling data for Neural Engine operations
struct ProfilingData {
    float inferenceTimeMs = 0.0f;
    float preProcessingTimeMs = 0.0f;
    float postProcessingTimeMs = 0.0f;
    float totalTimeMs = 0.0f;
    int batchSize = 1;
    std::string deviceUsed;
};

// Logging levels for Neural Engine
enum class LogLevel {
    None,
    Error,
    Warning,
    Info,
    Debug,
    Verbose
};

} // namespace gaia_matrix
```

## Rendering API

### Renderer

```cpp
namespace gaia_matrix {

enum class RenderAPI {
    None = 0,
    Metal,
    Vulkan,
    OpenGL,
    WebGL
};

struct RendererConfig {
    int width = 1280;
    int height = 720;
    bool vsync = true;
    bool fullscreen = false;
    bool enableNeuralEnhancement = true;
    RenderAPI api = RenderAPI::Metal;
    std::string windowTitle = "GAIA MATRIX";
};

class Renderer {
public:
    // Initialize the renderer
    // config: Renderer configuration
    // Returns: True if initialization succeeded
    static bool Initialize(const RendererConfig& config);
    
    // Shutdown the renderer and release resources
    static void Shutdown();
    
    // Begin a new frame
    void BeginFrame();
    
    // End the current frame and present to screen
    void EndFrame();
    
    // Check if neural enhancement is enabled
    // Returns: True if neural enhancement is enabled
    bool IsNeuralEnhancementEnabled() const;
    
    // Enable or disable neural enhancement
    // enable: Whether to enable neural enhancement
    void SetNeuralEnhancement(bool enable);
    
    // Get the singleton instance
    // Returns: Renderer instance
    static Renderer& Get();
};

class Scene {
public:
    // Constructor
    // name: Scene name
    Scene(const std::string& name);
    
    // Destructor
    ~Scene();
    
    // Render the scene
    void Render();
};

} // namespace gaia_matrix
```

## Editor API

### Editor

```cpp
namespace gaia_matrix {

struct EditorConfig {
    int width = 1920;
    int height = 1080;
    bool darkTheme = true;
    std::string projectPath = "";
    std::string editorLayoutFile = "layouts/default.layout";
};

class Editor {
public:
    // Initialize the editor
    // config: Editor configuration
    // Returns: True if initialization succeeded
    static bool Initialize(const EditorConfig& config);
    
    // Shutdown the editor and release resources
    static void Shutdown();
    
    // Run the editor main loop
    void Run();
    
    // Open a project
    // projectPath: Path to project to open
    // Returns: True if project opened successfully
    bool OpenProject(const std::string& projectPath);
    
    // Create a new project
    // projectName: Name of new project
    // projectPath: Path to create project at
    // Returns: True if project created successfully
    bool CreateProject(const std::string& projectName, const std::string& projectPath);
    
    // Get the active scene
    // Returns: Active scene
    std::shared_ptr<Scene> GetActiveScene() const;
    
    // Get the singleton instance
    // Returns: Editor instance
    static Editor& Get();
};

class AIAssistant {
public:
    // Initialize the AI Assistant
    // Returns: True if initialization succeeded
    static bool Initialize();
    
    // Shutdown the AI Assistant
    static void Shutdown();
    
    // Query the AI Assistant
    // query: Query text
    // callback: Callback to receive response
    void Query(
        const std::string& query, 
        std::function<void(const std::string&)> callback
    );
    
    // Generate AOPL code
    // prompt: Code generation prompt
    // callback: Callback to receive generated code
    void GenerateCode(
        const std::string& prompt, 
        std::function<void(const std::string&)> callback
    );
    
    // Analyze project code
    // callback: Callback to receive analysis results
    void AnalyzeProject(
        std::function<void(const ProjectAnalysis&)> callback
    );
    
    // Explain selected code
    // code: Code to explain
    // callback: Callback to receive explanation
    void ExplainCode(
        const std::string& code, 
        std::function<void(const std::string&)> callback
    );
    
    // Optimize selected code
    // code: Code to optimize
    // target: Optimization target (e.g., "performance", "neural_engine")
    // callback: Callback to receive optimized code
    void OptimizeCode(
        const std::string& code,
        const std::string& target,
        std::function<void(const std::string&)> callback
    );
    
    // Enable learning from project
    // enable: Whether to enable learning
    // includePaths: Paths to include in learning
    void EnableProjectLearning(
        bool enable,
        const std::vector<std::string>& includePaths = {}
    );
    
    // Get the singleton instance
    // Returns: AIAssistant instance
    static AIAssistant& Get();
};

struct ProjectAnalysis {
    std::vector<std::string> suggestions;
    std::vector<std::string> optimizations;
    std::vector<std::string> architectureRecommendations;
    std::vector<std::string> bestPractices;
};

} // namespace gaia_matrix
```

## AOPL API

### Parser

```cpp
namespace gaia_matrix {
namespace aopl {

namespace Symbol {
    constexpr char NODE = 'N';
    constexpr char ENTITY = 'E';
    constexpr char COMPONENT = 'C';
    constexpr char TRANSFORM = 'T';
    constexpr char FUNCTION = 'F';
    constexpr char EVENT = '⊻';
    constexpr char CONDITIONAL = '⊿';
    constexpr char ASSIGNMENT = '⊸';
    constexpr char DATAFLOW = '→';
    constexpr char NEURAL_NET = "NN";
    constexpr char REINFORCE = "RL";
    constexpr char MODEL_PROC = "MCP";
    constexpr char GENETIC = "GA";
}

class Parser {
public:
    // Constructor
    Parser();
    
    // Destructor
    ~Parser();
    
    // Parse AOPL code from string
    // code: AOPL code to parse
    // Returns: True if parsing was successful
    bool Parse(const std::string& code);
    
    // Get parsed entities
    // Returns: Vector of parsed entities
    const std::vector<std::shared_ptr<Entity>>& GetEntities() const;
    
    // Compile AOPL code to executable format
    // Returns: True if compilation was successful
    bool Compile();
};

class Node {
public:
    // Constructor
    // name: Node name
    Node(const std::string& name);
    
    // Destructor
    virtual ~Node() = default;
    
    // Get the name of the node
    // Returns: Node name
    const std::string& GetName() const;
};

class Entity : public Node {
public:
    // Constructor
    // name: Entity name
    Entity(const std::string& name);
    
    // Destructor
    ~Entity() override = default;
    
    // Add a component to the entity
    // component: Component to add
    void AddComponent(std::shared_ptr<Component> component);
    
    // Get all components
    // Returns: Vector of components
    const std::vector<std::shared_ptr<Component>>& GetComponents() const;
    
    // Get transform component
    // Returns: Transform component
    std::shared_ptr<Transform> GetTransform() const;
};

} // namespace aopl
} // namespace gaia_matrix
```

## Web Compiler API

### WebCompiler

```cpp
namespace gaia_matrix {

enum class ShaderType {
    Vertex,
    Fragment,
    Compute
};

enum class WebOutputFormat {
    ESNext,    // Modern JavaScript
    ES5,       // Legacy JavaScript
    WASM       // WebAssembly
};

struct WebCompilerConfig {
    WebOutputFormat outputFormat = WebOutputFormat::ESNext;
    bool minify = true;
    bool generateSourceMap = true;
    bool enableWebWorkers = true;
    bool enableWebGPU = false; // Future option
    std::string outputDir = "./web_build";
};

class WebCompiler {
public:
    // Initialize the web compiler
    // config: Compiler configuration
    // Returns: True if initialization succeeded
    static bool Initialize(const WebCompilerConfig& config = WebCompilerConfig());
    
    // Shutdown the compiler and release resources
    static void Shutdown();
    
    // Get the singleton instance
    // Returns: WebCompiler instance
    static WebCompiler& Get();
    
    // Compile AOPL source code to WebGL-compatible JavaScript or WASM
    // source: AOPL source code
    // outputPath: Path to write the compiled output
    // Returns: True if compilation succeeded
    bool CompileAOPL(const std::string& source, const std::string& outputPath);
    
    // Compile a shader to WebGL-compatible GLSL
    // source: Shader source code
    // type: Shader type
    // Returns: Compiled shader code as string
    std::string CompileShader(const std::string& source, ShaderType type);
    
    // Generate a complete web application with the engine and compiled code
    // projectName: Name of the project
    // outputDir: Directory to output the web application
    // aoplSources: Map of AOPL source files to compile
    // includeEditor: Whether to include the browser-based editor
    // Returns: True if generation succeeded
    bool GenerateWebApp(
        const std::string& projectName,
        const std::string& outputDir,
        const std::map<std::string, std::string>& aoplSources,
        bool includeEditor = false
    );
};

} // namespace gaia_matrix
```

## See Also

- [AOPL Reference](aopl-reference.md)
- [Neural Engine Integration](neural-engine.md)
- [AI Assistance](ai-assistance.md)
- [Cross-Platform Development](cross-platform.md)
- [Web Compiler](web-compiler.md)