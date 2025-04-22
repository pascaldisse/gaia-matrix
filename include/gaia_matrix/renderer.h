#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>

namespace gaia_matrix {

/**
 * @brief Render API type
 */
enum class RenderAPI {
    None = 0,
    Metal,
    Vulkan,
    OpenGL,
    WebGL
};

/**
 * @brief Base renderer configuration
 */
struct RendererConfig {
    int width = 1280;
    int height = 720;
    bool vsync = true;
    bool fullscreen = false;
    bool enableNeuralEnhancement = true;
    RenderAPI api = RenderAPI::Metal;
    std::string windowTitle = "GAIA MATRIX";
};

/**
 * @brief Neural-Enhanced Rendering System
 * 
 * Provides AI-enhanced graphics pipeline with Neural Engine acceleration.
 */
class Renderer {
public:
    /**
     * @brief Initialize the renderer
     * @param config Renderer configuration
     * @return True if initialization succeeded
     */
    static bool Initialize(const RendererConfig& config);

    /**
     * @brief Shutdown the renderer and release resources
     */
    static void Shutdown();

    /**
     * @brief Begin a new frame
     */
    void BeginFrame();

    /**
     * @brief End the current frame and present to screen
     */
    void EndFrame();

    /**
     * @brief Check if neural enhancement is enabled
     * @return True if neural enhancement is enabled
     */
    bool IsNeuralEnhancementEnabled() const;

    /**
     * @brief Enable or disable neural enhancement
     * @param enable Whether to enable neural enhancement
     */
    void SetNeuralEnhancement(bool enable);

    /**
     * @brief Get the singleton instance
     * @return Renderer instance
     */
    static Renderer& Get();

private:
    Renderer();
    ~Renderer();

    /**
     * @brief Create render context based on selected API
     * @param api Render API to use
     * @return True if context creation succeeded
     */
    bool CreateContext(RenderAPI api);

    bool m_IsInitialized;
    bool m_NeuralEnhancementEnabled;
    RenderAPI m_API;
    RendererConfig m_Config;
};

/**
 * @brief Scene representation for rendering
 */
class Scene {
public:
    Scene(const std::string& name);
    ~Scene();

    /**
     * @brief Render the scene
     */
    void Render();

private:
    std::string m_Name;
    // Scene data would go here
};

} // namespace gaia_matrix