#include "gaia_matrix/web_compiler.h"
#include "gaia_matrix/aopl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace gaia_matrix {

WebCompiler* WebCompiler::s_Instance = nullptr;

WebCompiler::WebCompiler() : m_IsInitialized(false) {
}

WebCompiler::~WebCompiler() {
}

bool WebCompiler::Initialize(const WebCompilerConfig& config) {
    if (s_Instance) {
        std::cerr << "WebCompiler already initialized!" << std::endl;
        return false;
    }
    
    s_Instance = new WebCompiler();
    s_Instance->m_Config = config;
    s_Instance->m_IsInitialized = true;
    
    std::cout << "WebCompiler initialized with output format: ";
    switch (config.outputFormat) {
        case WebOutputFormat::ESNext:
            std::cout << "ESNext (Modern JavaScript)";
            break;
        case WebOutputFormat::ES5:
            std::cout << "ES5 (Legacy JavaScript)";
            break;
        case WebOutputFormat::WASM:
            std::cout << "WebAssembly";
            break;
        default:
            std::cout << "Unknown";
            break;
    }
    std::cout << std::endl;
    
    return true;
}

void WebCompiler::Shutdown() {
    if (!s_Instance) {
        return;
    }
    
    delete s_Instance;
    s_Instance = nullptr;
    
    std::cout << "WebCompiler shut down successfully!" << std::endl;
}

WebCompiler& WebCompiler::Get() {
    if (!s_Instance) {
        std::cerr << "WebCompiler not initialized! Call Initialize() first." << std::endl;
        static WebCompiler dummy;
        return dummy;
    }
    
    return *s_Instance;
}

bool WebCompiler::CompileAOPL(const std::string& source, const std::string& outputPath) {
    if (!m_IsInitialized) {
        std::cerr << "WebCompiler not initialized!" << std::endl;
        return false;
    }
    
    try {
        std::string compiledCode;
        
        // Convert AOPL to JavaScript or WASM based on config
        switch (m_Config.outputFormat) {
            case WebOutputFormat::WASM: {
                std::cout << "Compiling AOPL to WebAssembly..." << std::endl;
                auto wasmBinary = CompileAOPLToWASM(source);
                
                // Write WASM binary to file
                std::ofstream outFile(outputPath, std::ios::binary);
                if (!outFile) {
                    std::cerr << "Failed to open output file: " << outputPath << std::endl;
                    return false;
                }
                
                outFile.write(reinterpret_cast<const char*>(wasmBinary.data()), wasmBinary.size());
                outFile.close();
                
                // Generate companion JavaScript loader
                std::string jsLoaderPath = outputPath + ".js";
                std::ofstream jsLoader(jsLoaderPath);
                if (!jsLoader) {
                    std::cerr << "Failed to create WASM loader file: " << jsLoaderPath << std::endl;
                    return false;
                }
                
                jsLoader << "// GAIA MATRIX WASM Loader\n";
                jsLoader << "const gaiaMatrix = {\n";
                jsLoader << "  async init() {\n";
                jsLoader << "    const response = await fetch('" << std::filesystem::path(outputPath).filename().string() << "');\n";
                jsLoader << "    const wasmBinary = await response.arrayBuffer();\n";
                jsLoader << "    const wasmModule = await WebAssembly.instantiate(wasmBinary, {\n";
                jsLoader << "      env: {\n";
                jsLoader << "        // WASM environment imports\n";
                jsLoader << "      }\n";
                jsLoader << "    });\n";
                jsLoader << "    this.instance = wasmModule.instance;\n";
                jsLoader << "    console.log('GAIA MATRIX WASM initialized');\n";
                jsLoader << "    return this.instance;\n";
                jsLoader << "  }\n";
                jsLoader << "};\n";
                jsLoader.close();
                
                std::cout << "Successfully compiled to WASM: " << outputPath << std::endl;
                std::cout << "WASM loader created: " << jsLoaderPath << std::endl;
                break;
            }
            case WebOutputFormat::ES5:
            case WebOutputFormat::ESNext:
            default: {
                std::cout << "Transpiling AOPL to " 
                    << (m_Config.outputFormat == WebOutputFormat::ES5 ? "ES5" : "ESNext") 
                    << " JavaScript..." << std::endl;
                
                compiledCode = TranspileAOPLToJS(source);
                
                // Add polyfills for ES5 if needed
                if (m_Config.outputFormat == WebOutputFormat::ES5) {
                    compiledCode = "// ES5 Polyfills\n" + compiledCode;
                }
                
                // Write to file
                std::ofstream outFile(outputPath);
                if (!outFile) {
                    std::cerr << "Failed to open output file: " << outputPath << std::endl;
                    return false;
                }
                
                outFile << compiledCode;
                outFile.close();
                
                std::cout << "Successfully compiled to JavaScript: " << outputPath << std::endl;
                break;
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error compiling AOPL: " << e.what() << std::endl;
        return false;
    }
}

std::string WebCompiler::CompileShader(const std::string& source, ShaderType type) {
    std::cout << "Compiling " << (type == ShaderType::Vertex ? "vertex" : 
                               type == ShaderType::Fragment ? "fragment" : 
                               "compute") << " shader..." << std::endl;
    
    // Convert shader to WebGL GLSL (strip incompatible features, add precision qualifiers)
    std::stringstream result;
    
    // Add WebGL GLSL version and precision
    result << "#version 300 es\n";
    result << "precision highp float;\n";
    result << "precision highp int;\n\n";
    
    // Add the shader code (we would need to parse and modify as needed)
    result << "// Auto-converted to WebGL GLSL by GAIA MATRIX\n";
    result << source;
    
    return result.str();
}

bool WebCompiler::GenerateWebApp(
    const std::string& projectName,
    const std::string& outputDir,
    const std::map<std::string, std::string>& aoplSources,
    bool includeEditor
) {
    if (!m_IsInitialized) {
        std::cerr << "WebCompiler not initialized!" << std::endl;
        return false;
    }
    
    try {
        // Create output directory if it doesn't exist
        std::filesystem::create_directories(outputDir);
        
        // Generate HTML file
        std::string htmlPath = outputDir + "/index.html";
        std::ofstream htmlFile(htmlPath);
        if (!htmlFile) {
            std::cerr << "Failed to create HTML file: " << htmlPath << std::endl;
            return false;
        }
        
        htmlFile << GenerateHTMLTemplate(projectName, includeEditor);
        htmlFile.close();
        
        // Generate WebGL renderer
        std::string renderPath = outputDir + "/gaia-webgl-renderer.js";
        std::ofstream renderFile(renderPath);
        if (!renderFile) {
            std::cerr << "Failed to create renderer file: " << renderPath << std::endl;
            return false;
        }
        
        renderFile << GenerateWebGLRenderer();
        renderFile.close();
        
        // Generate AOPL runtime
        std::string runtimePath = outputDir + "/gaia-aopl-runtime.js";
        std::ofstream runtimeFile(runtimePath);
        if (!runtimeFile) {
            std::cerr << "Failed to create runtime file: " << runtimePath << std::endl;
            return false;
        }
        
        runtimeFile << GenerateAOPLRuntime();
        runtimeFile.close();
        
        // Generate browser editor if requested
        if (includeEditor) {
            std::string editorPath = outputDir + "/gaia-editor.js";
            std::ofstream editorFile(editorPath);
            if (!editorFile) {
                std::cerr << "Failed to create editor file: " << editorPath << std::endl;
                return false;
            }
            
            editorFile << GenerateBrowserEditor();
            editorFile.close();
            
            // Create editor CSS
            std::string cssPath = outputDir + "/gaia-editor.css";
            std::ofstream cssFile(cssPath);
            if (!cssFile) {
                std::cerr << "Failed to create CSS file: " << cssPath << std::endl;
                return false;
            }
            
            cssFile << "/* GAIA MATRIX Editor CSS */\n";
            cssFile << "body { margin: 0; padding: 0; font-family: sans-serif; }\n";
            cssFile << ".gaia-editor-container { display: flex; height: 100vh; }\n";
            cssFile << ".gaia-editor-sidebar { width: 250px; background: #222; color: #fff; padding: 10px; }\n";
            cssFile << ".gaia-editor-main { flex: 1; display: flex; flex-direction: column; }\n";
            cssFile << ".gaia-editor-toolbar { background: #333; color: #fff; padding: 10px; }\n";
            cssFile << ".gaia-editor-code { flex: 1; position: relative; }\n";
            cssFile << ".gaia-editor-preview { height: 300px; border-top: 1px solid #ccc; }\n";
            cssFile << ".gaia-editor-output { height: 200px; border-top: 1px solid #ccc; }\n";
            cssFile << ".tab-container { display: flex; flex-direction: column; height: 100%; }\n";
            cssFile << ".tab-buttons { display: flex; background: #444; }\n";
            cssFile << ".tab-button { padding: 8px 16px; background: none; color: #ccc; border: none; cursor: pointer; outline: none; }\n";
            cssFile << ".tab-button.active { background: #555; color: #fff; }\n";
            cssFile << ".tab-content { flex: 1; position: relative; overflow: hidden; }\n";
            cssFile << ".tab-panel { display: none; height: 100%; overflow: auto; }\n";
            cssFile << ".tab-panel.active { display: flex; flex-direction: column; }\n";
            cssFile << ".chat-messages { flex: 1; overflow-y: auto; padding: 10px; background: #2d2d2d; }\n";
            cssFile << ".chat-message { margin-bottom: 12px; }\n";
            cssFile << ".user-message { text-align: right; }\n";
            cssFile << ".ai-message { text-align: left; }\n";
            cssFile << ".message-content { display: inline-block; padding: 8px 12px; border-radius: 8px; max-width: 80%; }\n";
            cssFile << ".user-message .message-content { background: #3a6ea5; color: white; }\n";
            cssFile << ".ai-message .message-content { background: #444; color: white; }\n";
            cssFile << ".loading .message-content { background: #333; font-style: italic; }\n";
            cssFile << ".chat-input-container { display: flex; padding: 8px; background: #333; }\n";
            cssFile << ".chat-input { flex: 1; padding: 8px; border: 1px solid #555; background: #444; color: #fff; }\n";
            cssFile << ".chat-send-button { margin-left: 8px; padding: 8px 16px; background: #4c4c4c; color: #fff; border: none; cursor: pointer; }\n";
            cssFile << ".console-output { height: 100%; padding: 10px; background: #2d2d2d; color: #ddd; font-family: monospace; overflow-y: auto; white-space: pre-wrap; }\n";
            cssFile << ".console-log { color: #ddd; }\n";
            cssFile << ".console-error { color: #ff5555; }\n";
            cssFile << ".console-warn { color: #ffcc00; }\n";
            cssFile << ".console-info { color: #55aaff; }\n";
            cssFile.close();
        }
        
        // Compile each AOPL source file
        std::filesystem::create_directories(outputDir + "/compiled");
        for (const auto& [filename, source] : aoplSources) {
            std::string outputPath = outputDir + "/compiled/" + filename + 
                (m_Config.outputFormat == WebOutputFormat::WASM ? ".wasm" : ".js");
            
            if (!CompileAOPL(source, outputPath)) {
                std::cerr << "Failed to compile AOPL source: " << filename << std::endl;
                return false;
            }
        }
        
        std::cout << "Successfully generated web application in: " << outputDir << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error generating web application: " << e.what() << std::endl;
        return false;
    }
}

std::string WebCompiler::GenerateHTMLTemplate(const std::string& projectName, bool includeEditor) {
    std::stringstream html;
    
    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"en\">\n";
    html << "<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "    <title>" << projectName << " - GAIA MATRIX</title>\n";
    
    if (includeEditor) {
        html << "    <link rel=\"stylesheet\" href=\"gaia-editor.css\">\n";
    }
    
    html << "</head>\n";
    html << "<body>\n";
    
    if (includeEditor) {
        html << "    <div class=\"gaia-editor-container\">\n";
        html << "        <div class=\"gaia-editor-sidebar\">\n";
        html << "            <h3>GAIA MATRIX</h3>\n";
        html << "            <div class=\"file-explorer\">\n";
        html << "                <h4>Files</h4>\n";
        html << "                <ul id=\"file-list\"></ul>\n";
        html << "            </div>\n";
        html << "        </div>\n";
        html << "        <div class=\"gaia-editor-main\">\n";
        html << "            <div class=\"gaia-editor-toolbar\">\n";
        html << "                <button id=\"run-button\">Run</button>\n";
        html << "                <button id=\"save-button\">Save</button>\n";
        html << "                <select id=\"example-selector\">\n";
        html << "                    <option value=\"\">Select Example...</option>\n";
        html << "                    <option value=\"player\">Player Controller</option>\n";
        html << "                    <option value=\"npc\">NPC Behavior</option>\n";
        html << "                </select>\n";
        html << "            </div>\n";
        html << "            <div class=\"gaia-editor-code\" id=\"code-editor\"></div>\n";
        html << "            <div class=\"gaia-editor-preview\">\n";
        html << "                <canvas id=\"preview-canvas\"></canvas>\n";
        html << "            </div>\n";
        html << "            <div class=\"gaia-editor-output\">\n";
        html << "                <div class=\"tab-container\">\n";
        html << "                    <div class=\"tab-buttons\">\n";
        html << "                        <button class=\"tab-button active\" data-tab=\"chat\">Chat</button>\n";
        html << "                        <button class=\"tab-button\" data-tab=\"console\">Console</button>\n";
        html << "                    </div>\n";
        html << "                    <div class=\"tab-content\">\n";
        html << "                        <div class=\"tab-panel active\" id=\"chat-panel\">\n";
        html << "                            <div class=\"chat-messages\"></div>\n";
        html << "                            <div class=\"chat-input-container\">\n";
        html << "                                <input type=\"text\" class=\"chat-input\" placeholder=\"Type your message...\">\n";
        html << "                                <button class=\"chat-send-button\">Send</button>\n";
        html << "                            </div>\n";
        html << "                        </div>\n";
        html << "                        <div class=\"tab-panel\" id=\"console-panel\">\n";
        html << "                            <div class=\"console-output\" id=\"console-log\"></div>\n";
        html << "                        </div>\n";
        html << "                    </div>\n";
        html << "                </div>\n";
        html << "            </div>\n";
        html << "        </div>\n";
        html << "    </div>\n";
    } else {
        html << "    <canvas id=\"gaia-canvas\" style=\"width: 100%; height: 100vh; display: block;\"></canvas>\n";
    }
    
    // Core scripts
    html << "    <script src=\"gaia-webgl-renderer.js\"></script>\n";
    html << "    <script src=\"gaia-aopl-runtime.js\"></script>\n";
    
    if (includeEditor) {
        html << "    <script src=\"gaia-editor.js\"></script>\n";
        html << "    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/monaco-editor/0.36.1/min/vs/loader.js\"></script>\n";
        html << "    <script>\n";
        html << "        require.config({ paths: { 'vs': 'https://cdnjs.cloudflare.com/ajax/libs/monaco-editor/0.36.1/min/vs' }});\n";
        html << "        require(['vs/editor/editor.main'], function() {\n";
        html << "            window.gaiaEditor.initMonacoEditor();\n";
        html << "        });\n";
        html << "    </script>\n";
    }
    
    // Application initialization
    html << "    <script>\n";
    html << "        window.addEventListener('DOMContentLoaded', () => {\n";
    html << "            try {\n";
    html << "                // Initialize GAIA MATRIX Engine\n";
    html << "                const config = {\n";
    html << "                    appName: '" << projectName << "',\n";
    html << "                    width: window.innerWidth,\n";
    html << "                    height: window.innerHeight,\n";
    html << "                    canvas: document.getElementById('" << (includeEditor ? "preview-canvas" : "gaia-canvas") << "')\n";
    html << "                };\n";
    html << "                window.gaiaMatrix.initialize(config);\n";
    if (includeEditor) {
        html << "                window.gaiaEditor.initialize();\n";
    }
    html << "            } catch (error) {\n";
    html << "                console.error('GAIA MATRIX initialization error:', error);\n";
    html << "            }\n";
    html << "        });\n";
    html << "    </script>\n";
    
    html << "</body>\n";
    html << "</html>\n";
    
    return html.str();
}

std::string WebCompiler::GenerateWebGLRenderer() {
    std::stringstream js;
    
    js << "// GAIA MATRIX WebGL Renderer\n";
    js << "window.gaiaMatrix = window.gaiaMatrix || {};\n\n";
    
    js << "class WebGLRenderer {\n";
    js << "    constructor(canvas, config = {}) {\n";
    js << "        this.canvas = canvas;\n";
    js << "        this.config = Object.assign({\n";
    js << "            width: 800,\n";
    js << "            height: 600,\n";
    js << "            clearColor: [0.1, 0.1, 0.1, 1.0],\n";
    js << "            enableNeuralEnhancement: true\n";
    js << "        }, config);\n\n";
    
    js << "        this.gl = null;\n";
    js << "        this.programs = {};\n";
    js << "        this.textures = {};\n";
    js << "        this.buffers = {};\n";
    js << "        this.frameCount = 0;\n";
    js << "        this.lastTime = 0;\n";
    js << "        this.initialized = false;\n";
    js << "        this.neuralEnhancementEnabled = this.config.enableNeuralEnhancement;\n";
    js << "    }\n\n";
    
    js << "    async initialize() {\n";
    js << "        try {\n";
    js << "            // Initialize WebGL context\n";
    js << "            this.gl = this.canvas.getContext('webgl2');\n";
    js << "            if (!this.gl) {\n";
    js << "                throw new Error('WebGL 2.0 not supported in your browser');\n";
    js << "            }\n\n";
    
    js << "            // Configure canvas and viewport\n";
    js << "            this.canvas.width = this.config.width;\n";
    js << "            this.canvas.height = this.config.height;\n";
    js << "            this.gl.viewport(0, 0, this.config.width, this.config.height);\n\n";
    
    js << "            // Set clear color\n";
    js << "            this.gl.clearColor(...this.config.clearColor);\n";
    js << "            this.gl.enable(this.gl.DEPTH_TEST);\n";
    js << "            this.gl.enable(this.gl.CULL_FACE);\n\n";
    
    js << "            // Initialize shaders and programs\n";
    js << "            await this.initShaders();\n\n";
    
    js << "            console.log('WebGL Renderer initialized successfully');\n";
    js << "            this.initialized = true;\n";
    js << "            return true;\n";
    js << "        } catch (error) {\n";
    js << "            console.error('WebGL Renderer initialization failed:', error);\n";
    js << "            return false;\n";
    js << "        }\n";
    js << "    }\n\n";
    
    js << "    async initShaders() {\n";
    js << "        // Create default shader program\n";
    js << "        const vertexShaderSource = `#version 300 es\n";
    js << "            precision highp float;\n";
    js << "            \n";
    js << "            layout(location = 0) in vec3 a_position;\n";
    js << "            layout(location = 1) in vec3 a_normal;\n";
    js << "            layout(location = 2) in vec2 a_texCoord;\n";
    js << "            \n";
    js << "            uniform mat4 u_model;\n";
    js << "            uniform mat4 u_view;\n";
    js << "            uniform mat4 u_projection;\n";
    js << "            \n";
    js << "            out vec3 v_normal;\n";
    js << "            out vec2 v_texCoord;\n";
    js << "            out vec3 v_position;\n";
    js << "            \n";
    js << "            void main() {\n";
    js << "                gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);\n";
    js << "                v_normal = mat3(transpose(inverse(u_model))) * a_normal;\n";
    js << "                v_texCoord = a_texCoord;\n";
    js << "                v_position = (u_model * vec4(a_position, 1.0)).xyz;\n";
    js << "            }`;\n\n";
    
    js << "        const fragmentShaderSource = `#version 300 es\n";
    js << "            precision highp float;\n";
    js << "            \n";
    js << "            in vec3 v_normal;\n";
    js << "            in vec2 v_texCoord;\n";
    js << "            in vec3 v_position;\n";
    js << "            \n";
    js << "            uniform sampler2D u_texture;\n";
    js << "            uniform vec3 u_lightPos;\n";
    js << "            uniform vec3 u_viewPos;\n";
    js << "            uniform vec3 u_lightColor;\n";
    js << "            uniform bool u_hasTexture;\n";
    js << "            uniform vec4 u_color;\n";
    js << "            \n";
    js << "            out vec4 fragColor;\n";
    js << "            \n";
    js << "            void main() {\n";
    js << "                vec3 norm = normalize(v_normal);\n";
    js << "                vec3 lightDir = normalize(u_lightPos - v_position);\n";
    js << "                \n";
    js << "                // Ambient\n";
    js << "                float ambientStrength = 0.1;\n";
    js << "                vec3 ambient = ambientStrength * u_lightColor;\n";
    js << "                \n";
    js << "                // Diffuse\n";
    js << "                float diff = max(dot(norm, lightDir), 0.0);\n";
    js << "                vec3 diffuse = diff * u_lightColor;\n";
    js << "                \n";
    js << "                // Specular\n";
    js << "                float specularStrength = 0.5;\n";
    js << "                vec3 viewDir = normalize(u_viewPos - v_position);\n";
    js << "                vec3 reflectDir = reflect(-lightDir, norm);\n";
    js << "                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);\n";
    js << "                vec3 specular = specularStrength * spec * u_lightColor;\n";
    js << "                \n";
    js << "                vec4 texColor = u_hasTexture ? texture(u_texture, v_texCoord) : u_color;\n";
    js << "                vec3 result = (ambient + diffuse + specular) * texColor.rgb;\n";
    js << "                fragColor = vec4(result, texColor.a);\n";
    js << "            }`;\n\n";
    
    js << "        // Create shader program\n";
    js << "        this.programs.default = this.createProgram(vertexShaderSource, fragmentShaderSource);\n";
    js << "    }\n\n";
    
    js << "    createShader(type, source) {\n";
    js << "        const shader = this.gl.createShader(type);\n";
    js << "        this.gl.shaderSource(shader, source);\n";
    js << "        this.gl.compileShader(shader);\n\n";
    
    js << "        if (!this.gl.getShaderParameter(shader, this.gl.COMPILE_STATUS)) {\n";
    js << "            const error = this.gl.getShaderInfoLog(shader);\n";
    js << "            this.gl.deleteShader(shader);\n";
    js << "            throw new Error(`Failed to compile shader: ${error}`);\n";
    js << "        }\n\n";
    
    js << "        return shader;\n";
    js << "    }\n\n";
    
    js << "    createProgram(vertexSource, fragmentSource) {\n";
    js << "        const vertexShader = this.createShader(this.gl.VERTEX_SHADER, vertexSource);\n";
    js << "        const fragmentShader = this.createShader(this.gl.FRAGMENT_SHADER, fragmentSource);\n\n";
    
    js << "        const program = this.gl.createProgram();\n";
    js << "        this.gl.attachShader(program, vertexShader);\n";
    js << "        this.gl.attachShader(program, fragmentShader);\n";
    js << "        this.gl.linkProgram(program);\n\n";
    
    js << "        if (!this.gl.getProgramParameter(program, this.gl.LINK_STATUS)) {\n";
    js << "            const error = this.gl.getProgramInfoLog(program);\n";
    js << "            this.gl.deleteProgram(program);\n";
    js << "            throw new Error(`Failed to link program: ${error}`);\n";
    js << "        }\n\n";
    
    js << "        // Clean up shaders after linking\n";
    js << "        this.gl.detachShader(program, vertexShader);\n";
    js << "        this.gl.detachShader(program, fragmentShader);\n";
    js << "        this.gl.deleteShader(vertexShader);\n";
    js << "        this.gl.deleteShader(fragmentShader);\n\n";
    
    js << "        return program;\n";
    js << "    }\n\n";
    
    js << "    beginFrame() {\n";
    js << "        if (!this.initialized) return;\n\n";
    
    js << "        this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);\n";
    js << "        this.frameCount++;\n";
    js << "    }\n\n";
    
    js << "    endFrame() {\n";
    js << "        if (!this.initialized) return;\n\n";
    
    js << "        // Apply neural enhancement if enabled\n";
    js << "        if (this.neuralEnhancementEnabled) {\n";
    js << "            this.applyNeuralEnhancement();\n";
    js << "        }\n\n";
    
    js << "        // Calculate FPS\n";
    js << "        const now = performance.now();\n";
    js << "        const deltaTime = now - this.lastTime;\n";
    js << "        this.lastTime = now;\n";
    js << "        const fps = 1000 / deltaTime;\n\n";
    
    js << "        // Update GPU performance metrics\n";
    js << "    }\n\n";
    
    js << "    applyNeuralEnhancement() {\n";
    js << "        // Placeholder for neural enhancement effects\n";
    js << "        // In a real implementation, this would apply post-processing\n";
    js << "        // effects enhanced by neural networks\n";
    js << "    }\n\n";
    
    js << "    setNeuralEnhancement(enabled) {\n";
    js << "        this.neuralEnhancementEnabled = enabled;\n";
    js << "        console.log(`Neural enhancement ${enabled ? 'enabled' : 'disabled'}`);\n";
    js << "    }\n\n";
    
    js << "    resize(width, height) {\n";
    js << "        if (!this.initialized) return;\n\n";
    
    js << "        this.canvas.width = width;\n";
    js << "        this.canvas.height = height;\n";
    js << "        this.gl.viewport(0, 0, width, height);\n";
    js << "        this.config.width = width;\n";
    js << "        this.config.height = height;\n";
    js << "    }\n\n";
    
    js << "    createTexture(imageSource) {\n";
    js << "        // Implementation for texture creation\n";
    js << "    }\n\n";
    
    js << "    createMesh(vertices, indices) {\n";
    js << "        // Implementation for mesh creation\n";
    js << "    }\n";
    js << "}\n\n";
    
    js << "// GAIA MATRIX WebGL Engine initialization\n";
    js << "window.gaiaMatrix.initialize = async function(config) {\n";
    js << "    const canvas = config.canvas || document.getElementById('gaia-canvas');\n";
    js << "    if (!canvas) {\n";
    js << "        throw new Error('Canvas element not found');\n";
    js << "    }\n\n";
    
    js << "    // Create renderer\n";
    js << "    this.renderer = new WebGLRenderer(canvas, {\n";
    js << "        width: config.width || 800,\n";
    js << "        height: config.height || 600,\n";
    js << "        clearColor: config.clearColor || [0.1, 0.1, 0.1, 1.0],\n";
    js << "        enableNeuralEnhancement: config.enableNeuralEnhancement !== false\n";
    js << "    });\n\n";
    
    js << "    // Initialize renderer\n";
    js << "    await this.renderer.initialize();\n\n";
    
    js << "    // Setup main loop\n";
    js << "    this.running = true;\n";
    js << "    this.lastTime = performance.now();\n";
    js << "    this.mainLoop();\n\n";
    
    js << "    console.log(`GAIA MATRIX initialized for project: ${config.appName || 'Untitled'}`);\n";
    js << "    window.addEventListener('resize', () => {\n";
    js << "        if (this.renderer) {\n";
    js << "            this.renderer.resize(window.innerWidth, window.innerHeight);\n";
    js << "        }\n";
    js << "    });\n\n";
    
    js << "    return this;\n";
    js << "};\n\n";
    
    js << "window.gaiaMatrix.mainLoop = function() {\n";
    js << "    if (!this.running) return;\n\n";
    
    js << "    // Calculate delta time\n";
    js << "    const now = performance.now();\n";
    js << "    const deltaTime = (now - this.lastTime) / 1000.0; // Convert to seconds\n";
    js << "    this.lastTime = now;\n\n";
    
    js << "    // Begin frame\n";
    js << "    this.renderer.beginFrame();\n\n";
    
    js << "    // Update game logic here\n";
    js << "    if (this.update) {\n";
    js << "        this.update(deltaTime);\n";
    js << "    }\n\n";
    
    js << "    // Render scene here\n";
    js << "    if (this.render) {\n";
    js << "        this.render();\n";
    js << "    }\n\n";
    
    js << "    // End frame\n";
    js << "    this.renderer.endFrame();\n\n";
    
    js << "    // Queue next frame\n";
    js << "    requestAnimationFrame(() => this.mainLoop());\n";
    js << "};\n\n";
    
    js << "window.gaiaMatrix.shutdown = function() {\n";
    js << "    this.running = false;\n";
    js << "    console.log('GAIA MATRIX shut down');\n";
    js << "};\n";
    
    return js.str();
}

std::string WebCompiler::GenerateBrowserEditor() {
    std::stringstream js;
    
    js << "// GAIA MATRIX Browser Editor\n";
    js << "window.gaiaEditor = window.gaiaEditor || {};\n\n";
    
    js << "window.gaiaEditor.initialize = function() {\n";
    js << "    this.files = {};\n";
    js << "    this.currentFile = null;\n";
    js << "    this.autoCompile = true;\n";
    js << "    \n";
    js << "    // Set up UI event listeners\n";
    js << "    const runButton = document.getElementById('run-button');\n";
    js << "    const saveButton = document.getElementById('save-button');\n";
    js << "    const exampleSelector = document.getElementById('example-selector');\n";
    js << "    \n";
    js << "    if (runButton) {\n";
    js << "        runButton.addEventListener('click', () => this.runCode());\n";
    js << "    }\n";
    js << "    \n";
    js << "    if (saveButton) {\n";
    js << "        saveButton.addEventListener('click', () => this.saveCurrentFile());\n";
    js << "    }\n";
    js << "    \n";
    js << "    if (exampleSelector) {\n";
    js << "        exampleSelector.addEventListener('change', (e) => {\n";
    js << "            if (e.target.value) {\n";
    js << "                this.loadExample(e.target.value);\n";
    js << "            }\n";
    js << "        });\n";
    js << "    }\n";
    js << "    \n";
    js << "    // Set up tab switcher\n";
    js << "    const tabButtons = document.querySelectorAll('.tab-button');\n";
    js << "    tabButtons.forEach(button => {\n";
    js << "        button.addEventListener('click', () => {\n";
    js << "            // Remove active class from all buttons and panels\n";
    js << "            document.querySelectorAll('.tab-button').forEach(btn => btn.classList.remove('active'));\n";
    js << "            document.querySelectorAll('.tab-panel').forEach(panel => panel.classList.remove('active'));\n";
    js << "            \n";
    js << "            // Add active class to current button and its corresponding panel\n";
    js << "            button.classList.add('active');\n";
    js << "            const tabName = button.getAttribute('data-tab');\n";
    js << "            document.getElementById(`${tabName}-panel`).classList.add('active');\n";
    js << "        });\n";
    js << "    });\n";
    js << "    \n";
    js << "    // Initialize chat functionality\n";
    js << "    this.initChat();\n";
    js << "    \n";
    js << "    // Initialize console output capture\n";
    js << "    this.initConsole();\n";
    js << "    \n";
    js << "    // Load examples and populate file list\n";
    js << "    this.loadExamples();\n";
    js << "    \n";
    js << "    console.log('GAIA MATRIX Editor initialized');\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.initMonacoEditor = function() {\n";
    js << "    // Register AOPL language\n";
    js << "    monaco.languages.register({ id: 'aopl' });\n";
    js << "    \n";
    js << "    // Define AOPL syntax highlighting\n";
    js << "    monaco.languages.setMonarchTokensProvider('aopl', {\n";
    js << "        tokenizer: {\n";
    js << "            root: [\n";
    js << "                [/N|E|C|T|F|⊻|⊿|⊸|→/, 'keyword'],\n";
    js << "                [/NN|RL|〈MCP〉|GA/, 'type'],\n";
    js << "                [/[a-zA-Z_$][\\w$]*/, 'identifier'],\n";
    js << "                [/[\\d]+/, 'number'],\n";
    js << "                [/\"[^\"]*\"/, 'string'],\n";
    js << "                [/\\/\\/.*$/, 'comment'],\n";
    js << "            ]\n";
    js << "        }\n";
    js << "    });\n";
    js << "    \n";
    js << "    // Create editor instance\n";
    js << "    this.editor = monaco.editor.create(document.getElementById('code-editor'), {\n";
    js << "        value: '// GAIA MATRIX AOPL Code\\nE MyEntity {\\n  T {\\n    position: [0, 0, 0],\\n    rotation: [0, 0, 0],\\n    scale: [1, 1, 1]\\n  }\\n}',\n";
    js << "        language: 'aopl',\n";
    js << "        theme: 'vs-dark',\n";
    js << "        minimap: { enabled: true },\n";
    js << "        automaticLayout: true\n";
    js << "    });\n";
    js << "    \n";
    js << "    // Set up change listener for auto-compile\n";
    js << "    this.editor.onDidChangeModelContent(() => {\n";
    js << "        if (this.autoCompile && this.currentFile) {\n";
    js << "            this.debounceCompile();\n";
    js << "        }\n";
    js << "    });\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.debounceCompile = function() {\n";
    js << "    if (this.compileTimeout) {\n";
    js << "        clearTimeout(this.compileTimeout);\n";
    js << "    }\n";
    js << "    \n";
    js << "    this.compileTimeout = setTimeout(() => {\n";
    js << "        this.compileCurrentFile();\n";
    js << "    }, 1000); // Compile after 1 second of inactivity\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.loadExamples = function() {\n";
    js << "    // Sample examples\n";
    js << "    const examples = {\n";
    js << "        'player.aopl': `// GAIA MATRIX Player Controller\\nE Player {\\n  T {\\n    position: [0, 1, 0],\\n    rotation: [0, 0, 0],\\n    scale: [1, 1, 1]\\n  }\\n  \\n  C Camera {\\n    fov: 60,\\n    near: 0.1,\\n    far: 1000\\n  }\\n  \\n  C Movement {\\n    speed: 5,\\n    jumpForce: 10\\n  }\\n  \\n  ⊻ Update(dt) {\\n    // Handle movement\\n    ⊿ Input.IsKeyDown('W') {\\n      T.position.z → T.position.z - Movement.speed * dt\\n    }\\n    ⊿ Input.IsKeyDown('S') {\\n      T.position.z → T.position.z + Movement.speed * dt\\n    }\\n  }\\n}`,\n";
    js << "        'npc.aopl': `// GAIA MATRIX NPC with neural behavior\\nE NPC {\\n  T {\\n    position: [10, 0, 10],\\n    rotation: [0, 0, 0],\\n    scale: [1, 1, 1]\\n  }\\n  \\n  C Health {\\n    current: 100,\\n    max: 100\\n  }\\n  \\n  C AI {\\n    state: 'idle',\\n    targetEntity: null,\\n    detectionRadius: 10\\n  }\\n  \\n  NN BehaviorModel {\\n    inputs: ['playerDistance', 'health', 'timeOfDay'],\\n    outputs: ['action', 'moveDirection']\\n  }\\n  \\n  ⊻ Update(dt) {\\n    // Update behavior using neural network\\n    let inputs = [\\n      Entity.GetDistance('Player'),\\n      Health.current / Health.max,\\n      World.GetTimeOfDay()\\n    ]\\n    \\n    let [action, moveDir] = BehaviorModel.Predict(inputs)\\n    AI.state → action\\n    T.position → T.position + moveDir * dt\\n  }\\n}`\n";
    js << "    };\n";
    js << "    \n";
    js << "    // Add examples to editor\n";
    js << "    for (const [filename, content] of Object.entries(examples)) {\n";
    js << "        this.files[filename] = content;\n";
    js << "    }\n";
    js << "    \n";
    js << "    // Update file list in UI\n";
    js << "    this.updateFileList();\n";
    js << "    \n";
    js << "    // Load the first example by default\n";
    js << "    if (Object.keys(examples).length > 0) {\n";
    js << "        this.openFile(Object.keys(examples)[0]);\n";
    js << "    }\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.updateFileList = function() {\n";
    js << "    const fileList = document.getElementById('file-list');\n";
    js << "    if (!fileList) return;\n";
    js << "    \n";
    js << "    fileList.innerHTML = '';\n";
    js << "    \n";
    js << "    for (const filename of Object.keys(this.files)) {\n";
    js << "        const li = document.createElement('li');\n";
    js << "        li.textContent = filename;\n";
    js << "        li.addEventListener('click', () => this.openFile(filename));\n";
    js << "        if (filename === this.currentFile) {\n";
    js << "            li.className = 'active';\n";
    js << "        }\n";
    js << "        fileList.appendChild(li);\n";
    js << "    }\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.openFile = function(filename) {\n";
    js << "    if (!this.files[filename]) return;\n";
    js << "    \n";
    js << "    this.currentFile = filename;\n";
    js << "    if (this.editor) {\n";
    js << "        this.editor.setValue(this.files[filename]);\n";
    js << "    }\n";
    js << "    \n";
    js << "    this.updateFileList();\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.saveCurrentFile = function() {\n";
    js << "    if (!this.currentFile || !this.editor) return;\n";
    js << "    \n";
    js << "    this.files[this.currentFile] = this.editor.getValue();\n";
    js << "    console.log(`Saved file: ${this.currentFile}`);\n";
    js << "    \n";
    js << "    // Compile the file after saving\n";
    js << "    this.compileCurrentFile();\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.compileCurrentFile = function() {\n";
    js << "    if (!this.currentFile || !this.editor) return;\n";
    js << "    \n";
    js << "    try {\n";
    js << "        console.log(`Compiling ${this.currentFile}...`);\n";
    js << "        const source = this.editor.getValue();\n";
    js << "        \n";
    js << "        // Use the AOPL runtime to compile the code\n";
    js << "        const compiled = window.gaiaMatrix.aopl.compileAOPL(source);\n";
    js << "        console.log('Compilation successful');\n";
    js << "        \n";
    js << "        return compiled;\n";
    js << "    } catch (error) {\n";
    js << "        console.error('Compilation error:', error);\n";
    js << "        return null;\n";
    js << "    }\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.runCode = function() {\n";
    js << "    const compiled = this.compileCurrentFile();\n";
    js << "    if (!compiled) return;\n";
    js << "    \n";
    js << "    try {\n";
    js << "        // Reset scene\n";
    js << "        window.gaiaMatrix.reset();\n";
    js << "        \n";
    js << "        // Execute the compiled code\n";
    js << "        window.gaiaMatrix.aopl.executeAOPL(compiled);\n";
    js << "        \n";
    js << "        console.log('Code execution started');\n";
    js << "    } catch (error) {\n";
    js << "        console.error('Execution error:', error);\n";
    js << "    }\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.loadExample = function(exampleName) {\n";
    js << "    const filename = `${exampleName}.aopl`;\n";
    js << "    if (this.files[filename]) {\n";
    js << "        this.openFile(filename);\n";
    js << "    } else {\n";
    js << "        console.error(`Example not found: ${exampleName}`);\n";
    js << "    }\n";
    js << "};\n\n";
    
    js << "window.gaiaEditor.createNewFile = function(filename) {\n";
    js << "    if (!filename.endsWith('.aopl')) {\n";
    js << "        filename += '.aopl';\n";
    js << "    }\n";
    js << "    \n";
    js << "    if (this.files[filename]) {\n";
    js << "        if (!confirm(`File ${filename} already exists. Overwrite?`)) {\n";
    js << "            return;\n";
    js << "        }\n";
    js << "    }\n";
    js << "    \n";
    js << "    this.files[filename] = `// GAIA MATRIX AOPL Code\\n// Created: ${new Date().toISOString()}\\n\\n`;\n";
    js << "    this.updateFileList();\n";
    js << "    this.openFile(filename);\n";
    js << "};\n";
    js << "\n";
    js << "window.gaiaEditor.initChat = function() {\n";
    js << "    const chatInput = document.querySelector('.chat-input');\n";
    js << "    const sendButton = document.querySelector('.chat-send-button');\n";
    js << "    const chatMessages = document.querySelector('.chat-messages');\n";
    js << "    \n";
    js << "    if (!chatInput || !sendButton || !chatMessages) return;\n";
    js << "    \n";
    js << "    // Initialize chat history if not already exists\n";
    js << "    this.chatHistory = this.chatHistory || [];\n";
    js << "    \n";
    js << "    const addMessage = (text, isUser = false) => {\n";
    js << "        const messageElem = document.createElement('div');\n";
    js << "        messageElem.className = `chat-message ${isUser ? 'user-message' : 'ai-message'}`;\n";
    js << "        messageElem.innerHTML = `<div class=\"message-content\">${text}</div>`;\n";
    js << "        chatMessages.appendChild(messageElem);\n";
    js << "        chatMessages.scrollTop = chatMessages.scrollHeight;\n";
    js << "    };\n";
    js << "    \n";
    js << "    // Add loading indicator\n";
    js << "    const showLoading = () => {\n";
    js << "        const loadingElem = document.createElement('div');\n";
    js << "        loadingElem.className = 'chat-message ai-message loading';\n";
    js << "        loadingElem.innerHTML = '<div class=\"message-content\">Thinking...</div>';\n";
    js << "        loadingElem.id = 'loading-message';\n";
    js << "        chatMessages.appendChild(loadingElem);\n";
    js << "        chatMessages.scrollTop = chatMessages.scrollHeight;\n";
    js << "    };\n";
    js << "    \n";
    js << "    const hideLoading = () => {\n";
    js << "        const loadingElem = document.getElementById('loading-message');\n";
    js << "        if (loadingElem) {\n";
    js << "            loadingElem.remove();\n";
    js << "        }\n";
    js << "    };\n";
    js << "    \n";
    js << "    // Send button click handler\n";
    js << "    sendButton.addEventListener('click', () => {\n";
    js << "        const message = chatInput.value.trim();\n";
    js << "        if (!message) return;\n";
    js << "        \n";
    js << "        // Add user message\n";
    js << "        addMessage(message, true);\n";
    js << "        \n";
    js << "        // Clear input\n";
    js << "        chatInput.value = '';\n";
    js << "        \n";
    js << "        // Show loading indicator\n";
    js << "        showLoading();\n";
    js << "        \n";
    js << "        // Process message and get AI response\n";
    js << "        this.processAIQuery(message).then(response => {\n";
    js << "            hideLoading();\n";
    js << "            addMessage(response);\n";
    js << "        }).catch(error => {\n";
    js << "            hideLoading();\n";
    js << "            addMessage('Error: Unable to connect to AI service. Please try again later.');\n";
    js << "            console.error('Chat error:', error);\n";
    js << "        });\n";
    js << "    });\n";
    js << "    \n";
    js << "    // Enter key in input field\n";
    js << "    chatInput.addEventListener('keypress', (e) => {\n";
    js << "        if (e.key === 'Enter') {\n";
    js << "            sendButton.click();\n";
    js << "        }\n";
    js << "    });\n";
    js << "    \n";
    js << "    // Add welcome message\n";
    js << "    addMessage('Welcome to GAIA MATRIX Assistant. How can I help you with your game development today?');\n";
    js << "};\n";
    js << "\n";
    js << "window.gaiaEditor.initConsole = function() {\n";
    js << "    const consoleOutput = document.querySelector('.console-output');\n";
    js << "    if (!consoleOutput) return;\n";
    js << "    \n";
    js << "    // Store original console methods\n";
    js << "    const originalLog = console.log;\n";
    js << "    const originalError = console.error;\n";
    js << "    const originalWarn = console.warn;\n";
    js << "    const originalInfo = console.info;\n";
    js << "    \n";
    js << "    // Helper to append to console output\n";
    js << "    const appendToConsole = (text, type = 'log') => {\n";
    js << "        const span = document.createElement('span');\n";
    js << "        span.className = `console-${type}`;\n";
    js << "        span.textContent = text + '\\n';\n";
    js << "        consoleOutput.appendChild(span);\n";
    js << "        consoleOutput.scrollTop = consoleOutput.scrollHeight;\n";
    js << "    };\n";
    js << "    \n";
    js << "    // Override console methods\n";
    js << "    console.log = function() {\n";
    js << "        originalLog.apply(console, arguments);\n";
    js << "        appendToConsole(Array.from(arguments).join(' '), 'log');\n";
    js << "    };\n";
    js << "    \n";
    js << "    console.error = function() {\n";
    js << "        originalError.apply(console, arguments);\n";
    js << "        appendToConsole(Array.from(arguments).join(' '), 'error');\n";
    js << "    };\n";
    js << "    \n";
    js << "    console.warn = function() {\n";
    js << "        originalWarn.apply(console, arguments);\n";
    js << "        appendToConsole(Array.from(arguments).join(' '), 'warn');\n";
    js << "    };\n";
    js << "    \n";
    js << "    console.info = function() {\n";
    js << "        originalInfo.apply(console, arguments);\n";
    js << "        appendToConsole(Array.from(arguments).join(' '), 'info');\n";
    js << "    };\n";
    js << "    \n";
    js << "    // Add initial message\n";
    js << "    appendToConsole('GAIA MATRIX Console Initialized');\n";
    js << "};\n";
    js << "\n";
    js << "// Process AI query using the Gaia API\n";
    js << "window.gaiaEditor.processAIQuery = function(query) {\n";
    js << "    return new Promise((resolve, reject) => {\n";
    js << "        // Get messages history from state if available\n";
    js << "        const history = this.chatHistory || [];\n";
    js << "        \n";
    js << "        // Add current query to history\n";
    js << "        history.push({role: 'user', content: query});\n";
    js << "        \n";
    js << "        // API endpoint - using default assistant persona\n";
    js << "        const endpoint = 'http://localhost:5000/api/personas/default-assistant/chat';\n";
    js << "        \n";
    js << "        fetch(endpoint, {\n";
    js << "            method: 'POST',\n";
    js << "            headers: {\n";
    js << "                'Content-Type': 'application/json',\n";
    js << "                'Authorization': 'Bearer ' + (this.apiKey || 'dev-key') // In production, use proper API key management\n";
    js << "            },\n";
    js << "            body: JSON.stringify({\n";
    js << "                message: query,\n";
    js << "                history: history.length > 2 ? history.slice(0, -1) : [] // Send previous history, but not current query\n";
    js << "            })\n";
    js << "        })\n";
    js << "        .then(response => {\n";
    js << "            if (!response.ok) {\n";
    js << "                throw new Error('API request failed: ' + response.statusText);\n";
    js << "            }\n";
    js << "            return response.json();\n";
    js << "        })\n";
    js << "        .then(data => {\n";
    js << "            // Add AI response to history\n";
    js << "            history.push({role: 'assistant', content: data.response});\n";
    js << "            \n";
    js << "            // Store updated history (limit to last 10 messages to prevent excessive context)\n";
    js << "            this.chatHistory = history.slice(-10);\n";
    js << "            \n";
    js << "            // Return the AI response\n";
    js << "            resolve(data.response);\n";
    js << "        })\n";
    js << "        .catch(error => {\n";
    js << "            console.error('Error calling Gaia API:', error);\n";
    js << "            resolve('Sorry, I encountered an error connecting to the API. Please try again later.');\n";
    js << "        });\n";
    js << "    });\n";
    js << "};\n";
    
    return js.str();
}

std::string WebCompiler::GenerateAOPLRuntime() {
    std::stringstream js;
    
    js << "// GAIA MATRIX AOPL Runtime for Web\n";
    js << "window.gaiaMatrix = window.gaiaMatrix || {};\n";
    js << "window.gaiaMatrix.aopl = window.gaiaMatrix.aopl || {};\n\n";
    
    js << "// AOPL Symbols\n";
    js << "window.gaiaMatrix.aopl.symbols = {\n";
    js << "    N: 'N',         // Network/Node declaration\n";
    js << "    E: 'E',         // Entity\n";
    js << "    C: 'C',         // Component\n";
    js << "    T: 'T',         // Transform\n";
    js << "    F: 'F',         // Function\n";
    js << "    EVENT: '⊻',     // Event handler\n";
    js << "    CONDITIONAL: '⊿', // Conditional\n";
    js << "    ASSIGN: '⊸',    // Assignment\n";
    js << "    FLOW: '→',      // Data flow\n";
    js << "    NN: 'NN',       // Neural network\n";
    js << "    RL: 'RL',       // Reinforcement learning\n";
    js << "    MCP: '〈MCP〉',  // Model-controlled procedural generation\n";
    js << "    GA: 'GA'        // Genetic algorithm\n";
    js << "};\n\n";
    
    js << "// AOPL Parser\n";
    js << "window.gaiaMatrix.aopl.parse = function(source) {\n";
    js << "    console.log('Parsing AOPL source...');\n";
    js << "    \n";
    js << "    // Basic tokenization and parsing would go here\n";
    js << "    const ast = { type: 'Program', body: [] };\n";
    js << "    \n";
    js << "    // Simple regex-based parsing (very basic, actual implementation would be more complex)\n";
    js << "    const lines = source.split('\\n');\n";
    js << "    let currentEntity = null;\n";
    js << "    let currentBlock = null;\n";
    js << "    let blockStack = [];\n";
    js << "    \n";
    js << "    for (let i = 0; i < lines.length; i++) {\n";
    js << "        const line = lines[i].trim();\n";
    js << "        \n";
    js << "        // Skip empty lines and comments\n";
    js << "        if (line === '' || line.startsWith('//')) continue;\n";
    js << "        \n";
    js << "        // Entity declaration\n";
    js << "        const entityMatch = line.match(/^E\\s+(\\w+)\\s*\\{/);\n";
    js << "        if (entityMatch) {\n";
    js << "            currentEntity = {\n";
    js << "                type: 'Entity',\n";
    js << "                name: entityMatch[1],\n";
    js << "                components: [],\n";
    js << "                functions: []\n";
    js << "            };\n";
    js << "            ast.body.push(currentEntity);\n";
    js << "            blockStack.push({ type: 'Entity', node: currentEntity });\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Component declaration\n";
    js << "        const componentMatch = line.match(/^\\s*C\\s+(\\w+)\\s*\\{/);\n";
    js << "        if (componentMatch && currentEntity) {\n";
    js << "            currentBlock = {\n";
    js << "                type: 'Component',\n";
    js << "                name: componentMatch[1],\n";
    js << "                properties: []\n";
    js << "            };\n";
    js << "            currentEntity.components.push(currentBlock);\n";
    js << "            blockStack.push({ type: 'Component', node: currentBlock });\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Transform component (special case)\n";
    js << "        if (line.match(/^\\s*T\\s*\\{/) && currentEntity) {\n";
    js << "            currentBlock = {\n";
    js << "                type: 'Component',\n";
    js << "                name: 'Transform',\n";
    js << "                properties: []\n";
    js << "            };\n";
    js << "            currentEntity.components.push(currentBlock);\n";
    js << "            blockStack.push({ type: 'Component', node: currentBlock });\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Event handler\n";
    js << "        const eventMatch = line.match(/^\\s*⊻\\s+(\\w+)\\(([^)]*?)\\)\\s*\\{/);\n";
    js << "        if (eventMatch && currentEntity) {\n";
    js << "            currentBlock = {\n";
    js << "                type: 'EventHandler',\n";
    js << "                name: eventMatch[1],\n";
    js << "                parameters: eventMatch[2].split(',').map(p => p.trim()).filter(p => p),\n";
    js << "                body: []\n";
    js << "            };\n";
    js << "            currentEntity.functions.push(currentBlock);\n";
    js << "            blockStack.push({ type: 'EventHandler', node: currentBlock });\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Neural network declaration\n";
    js << "        const nnMatch = line.match(/^\\s*NN\\s+(\\w+)\\s*\\{/);\n";
    js << "        if (nnMatch && currentEntity) {\n";
    js << "            currentBlock = {\n";
    js << "                type: 'NeuralNetwork',\n";
    js << "                name: nnMatch[1],\n";
    js << "                properties: []\n";
    js << "            };\n";
    js << "            currentEntity.components.push(currentBlock); // Treat as a component for now\n";
    js << "            blockStack.push({ type: 'NeuralNetwork', node: currentBlock });\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Property assignment within block\n";
    js << "        const propertyMatch = line.match(/^\\s*(\\w+)\\s*:\\s*(.+),?$/);\n";
    js << "        if (propertyMatch && currentBlock && \n";
    js << "            (currentBlock.type === 'Component' || currentBlock.type === 'NeuralNetwork')) {\n";
    js << "            currentBlock.properties.push({\n";
    js << "                name: propertyMatch[1],\n";
    js << "                value: propertyMatch[2].trim()\n";
    js << "            });\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Conditional statement\n";
    js << "        const conditionalMatch = line.match(/^\\s*⊿\\s+(.+)\\s*\\{/);\n";
    js << "        if (conditionalMatch && currentBlock && currentBlock.type === 'EventHandler') {\n";
    js << "            const conditional = {\n";
    js << "                type: 'Conditional',\n";
    js << "                condition: conditionalMatch[1],\n";
    js << "                body: []\n";
    js << "            };\n";
    js << "            currentBlock.body.push(conditional);\n";
    js << "            blockStack.push({ type: 'Conditional', node: conditional });\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Data flow statement\n";
    js << "        const flowMatch = line.match(/^\\s*(.+?)\\s*→\\s*(.+)$/);\n";
    js << "        if (flowMatch) {\n";
    js << "            const currentTop = blockStack[blockStack.length - 1];\n";
    js << "            if (currentTop && (currentTop.type === 'EventHandler' || currentTop.type === 'Conditional')) {\n";
    js << "                currentTop.node.body.push({\n";
    js << "                    type: 'DataFlow',\n";
    js << "                    source: flowMatch[1].trim(),\n";
    js << "                    target: flowMatch[2].trim()\n";
    js << "                });\n";
    js << "            }\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Simple statement within event handler or conditional\n";
    js << "        if (line.match(/^\\s*[^{}]+$/) && blockStack.length > 0) {\n";
    js << "            const currentTop = blockStack[blockStack.length - 1];\n";
    js << "            if (currentTop && (currentTop.type === 'EventHandler' || currentTop.type === 'Conditional')) {\n";
    js << "                currentTop.node.body.push({\n";
    js << "                    type: 'Statement',\n";
    js << "                    code: line.trim()\n";
    js << "                });\n";
    js << "            }\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Close block\n";
    js << "        if (line.match(/^\\s*\\}\\s*$/)) {\n";
    js << "            if (blockStack.length > 0) {\n";
    js << "                blockStack.pop();\n";
    js << "                \n";
    js << "                // Update current block reference\n";
    js << "                if (blockStack.length > 0) {\n";
    js << "                    const currentTop = blockStack[blockStack.length - 1];\n";
    js << "                    currentBlock = currentTop.node;\n";
    js << "                    if (currentTop.type === 'Entity') {\n";
    js << "                        currentEntity = currentTop.node;\n";
    js << "                    }\n";
    js << "                } else {\n";
    js << "                    currentBlock = null;\n";
    js << "                    currentEntity = null;\n";
    js << "                }\n";
    js << "            }\n";
    js << "            continue;\n";
    js << "        }\n";
    js << "    }\n";
    js << "    \n";
    js << "    console.log('AOPL parsing complete');\n";
    js << "    return ast;\n";
    js << "};\n\n";
    
    js << "// AOPL to JavaScript compiler\n";
    js << "window.gaiaMatrix.aopl.compileAOPL = function(source) {\n";
    js << "    try {\n";
    js << "        // Parse AOPL to AST\n";
    js << "        const ast = this.parse(source);\n";
    js << "        \n";
    js << "        // Compile AST to JavaScript\n";
    js << "        return this.generateJavaScript(ast);\n";
    js << "    } catch (error) {\n";
    js << "        console.error('AOPL compilation error:', error);\n";
    js << "        throw error;\n";
    js << "    }\n";
    js << "};\n\n";
    
    js << "// Generate JavaScript from AOPL AST\n";
    js << "window.gaiaMatrix.aopl.generateJavaScript = function(ast) {\n";
    js << "    // Output JavaScript code\n";
    js << "    let js = '// Generated by GAIA MATRIX AOPL Compiler\\n';\n";
    js << "    js += '(function() {\\n';\n";
    js << "    js += '  const entities = {};\\n\\n';\n";
    js << "    \n";
    js << "    // Process entities\n";
    js << "    for (const entity of ast.body) {\n";
    js << "        if (entity.type !== 'Entity') continue;\n";
    js << "        \n";
    js << "        js += `  // Entity: ${entity.name}\\n`;\n";
    js << "        js += `  entities.${entity.name} = {\\n`;\n";
    js << "        \n";
    js << "        // Components\n";
    js << "        for (const component of entity.components) {\n";
    js << "            js += `    ${component.name}: {\\n`;\n";
    js << "            \n";
    js << "            // Component properties\n";
    js << "            for (const prop of component.properties) {\n";
    js << "                js += `      ${prop.name}: ${prop.value},\\n`;\n";
    js << "            }\n";
    js << "            \n";
    js << "            js += '    },\\n';\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Event handlers and functions\n";
    js << "        for (const fn of entity.functions) {\n";
    js << "            js += `    ${fn.name}: function(${fn.parameters.join(', ')}) {\\n`;\n";
    js << "            \n";
    js << "            // Function body\n";
    js << "            for (const stmt of fn.body) {\n";
    js << "                if (stmt.type === 'Conditional') {\n";
    js << "                    js += `      if (${this.translateCondition(stmt.condition)}) {\\n`;\n";
    js << "                    \n";
    js << "                    for (const innerStmt of stmt.body) {\n";
    js << "                        if (innerStmt.type === 'DataFlow') {\n";
    js << "                            js += `        ${this.translateDataFlow(innerStmt.target, innerStmt.source)};\\n`;\n";
    js << "                        } else if (innerStmt.type === 'Statement') {\n";
    js << "                            js += `        ${this.translateStatement(innerStmt.code)};\\n`;\n";
    js << "                        }\n";
    js << "                    }\n";
    js << "                    \n";
    js << "                    js += '      }\\n';\n";
    js << "                } else if (stmt.type === 'DataFlow') {\n";
    js << "                    js += `      ${this.translateDataFlow(stmt.target, stmt.source)};\\n`;\n";
    js << "                } else if (stmt.type === 'Statement') {\n";
    js << "                    js += `      ${this.translateStatement(stmt.code)};\\n`;\n";
    js << "                }\n";
    js << "            }\n";
    js << "            \n";
    js << "            js += '    },\\n';\n";
    js << "        }\n";
    js << "        \n";
    js << "        js += '  };\\n\\n';\n";
    js << "    }\n";
    js << "    \n";
    js << "    // Register entities with engine\n";
    js << "    js += '  // Register entities with GAIA MATRIX engine\\n';\n";
    js << "    js += '  for (const [name, entity] of Object.entries(entities)) {\\n';\n";
    js << "    js += '    window.gaiaMatrix.registerEntity(name, entity);\\n';\n";
    js << "    js += '  }\\n';\n";
    js << "    js += '})();\\n';\n";
    js << "    \n";
    js << "    return js;\n";
    js << "};\n\n";
    
    js << "// Translate AOPL condition to JavaScript\n";
    js << "window.gaiaMatrix.aopl.translateCondition = function(condition) {\n";
    js << "    // Simple translation for demo purposes\n";
    js << "    return condition;\n";
    js << "};\n\n";
    
    js << "// Translate AOPL data flow to JavaScript assignment\n";
    js << "window.gaiaMatrix.aopl.translateDataFlow = function(target, source) {\n";
    js << "    return `${target} = ${source}`;\n";
    js << "};\n\n";
    
    js << "// Translate AOPL statement to JavaScript\n";
    js << "window.gaiaMatrix.aopl.translateStatement = function(statement) {\n";
    js << "    // Simple translation for demo purposes\n";
    js << "    return statement;\n";
    js << "};\n\n";
    
    js << "// Execute compiled AOPL code\n";
    js << "window.gaiaMatrix.aopl.executeAOPL = function(compiledCode) {\n";
    js << "    try {\n";
    js << "        // Set up entity registration\n";
    js << "        window.gaiaMatrix.registerEntity = function(name, entity) {\n";
    js << "            console.log(`Registering entity: ${name}`);\n";
    js << "            \n";
    js << "            // Store entity in scene\n";
    js << "            if (!window.gaiaMatrix.scene) {\n";
    js << "                window.gaiaMatrix.scene = {};\n";
    js << "            }\n";
    js << "            window.gaiaMatrix.scene[name] = entity;\n";
    js << "            \n";
    js << "            // Set up update function for entity if it has one\n";
    js << "            if (entity.Update) {\n";
    js << "                if (!window.gaiaMatrix.updateFunctions) {\n";
    js << "                    window.gaiaMatrix.updateFunctions = [];\n";
    js << "                }\n";
    js << "                window.gaiaMatrix.updateFunctions.push((dt) => entity.Update(dt));\n";
    js << "            }\n";
    js << "            \n";
    js << "            return entity;\n";
    js << "        };\n";
    js << "        \n";
    js << "        // Set up update function for engine if not already set\n";
    js << "        if (!window.gaiaMatrix.update) {\n";
    js << "            window.gaiaMatrix.update = function(dt) {\n";
    js << "                if (window.gaiaMatrix.updateFunctions) {\n";
    js << "                    for (const updateFn of window.gaiaMatrix.updateFunctions) {\n";
    js << "                        updateFn(dt);\n";
    js << "                    }\n";
    js << "                }\n";
    js << "            };\n";
    js << "        }\n";
    js << "        \n";
    js << "        // Execute the compiled code\n";
    js << "        const scriptElement = document.createElement('script');\n";
    js << "        scriptElement.textContent = compiledCode;\n";
    js << "        document.head.appendChild(scriptElement);\n";
    js << "        document.head.removeChild(scriptElement);\n";
    js << "        \n";
    js << "        console.log('AOPL code executed successfully');\n";
    js << "        return true;\n";
    js << "    } catch (error) {\n";
    js << "        console.error('AOPL execution error:', error);\n";
    js << "        return false;\n";
    js << "    }\n";
    js << "};\n\n";
    
    js << "// Reset scene\n";
    js << "window.gaiaMatrix.reset = function() {\n";
    js << "    console.log('Resetting GAIA MATRIX scene');\n";
    js << "    window.gaiaMatrix.scene = {};\n";
    js << "    window.gaiaMatrix.updateFunctions = [];\n";
    js << "};\n";
    
    return js.str();
}

std::string WebCompiler::TranspileAOPLToJS(const std::string& source) {
    // This is just a placeholder - real implementation would parse the AOPL code
    // and generate proper JavaScript
    return "// Transpiled AOPL Code\n(function() {\n  console.log('AOPL code transpiled to JavaScript');\n})();";
}

std::vector<uint8_t> WebCompiler::CompileAOPLToWASM(const std::string& source) {
    // This is just a placeholder - real implementation would compile to WASM
    // For now, return a minimal valid WASM module
    std::vector<uint8_t> wasmModule = {
        0x00, 0x61, 0x73, 0x6D, // WASM magic number
        0x01, 0x00, 0x00, 0x00  // WASM version 1
    };
    return wasmModule;
}

} // namespace gaia_matrix