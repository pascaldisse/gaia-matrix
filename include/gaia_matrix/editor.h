#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace gaia_matrix {

// Forward declarations
class Scene;

/**
 * @brief Editor configuration
 */
struct EditorConfig {
    int width = 1920;
    int height = 1080;
    bool darkTheme = true;
    std::string projectPath = "";
    std::string editorLayoutFile = "layouts/default.layout";
};

/**
 * @brief Editor module for GAIA MATRIX
 * 
 * Provides integrated development environment with AI assistance.
 */
class Editor {
public:
    /**
     * @brief Initialize the editor
     * @param config Editor configuration
     * @return True if initialization succeeded
     */
    static bool Initialize(const EditorConfig& config);

    /**
     * @brief Shutdown the editor and release resources
     */
    static void Shutdown();

    /**
     * @brief Run the editor main loop
     */
    void Run();

    /**
     * @brief Open a project
     * @param projectPath Path to project to open
     * @return True if project opened successfully
     */
    bool OpenProject(const std::string& projectPath);

    /**
     * @brief Create a new project
     * @param projectName Name of new project
     * @param projectPath Path to create project at
     * @return True if project created successfully
     */
    bool CreateProject(const std::string& projectName, const std::string& projectPath);

    /**
     * @brief Get the active scene
     * @return Active scene
     */
    std::shared_ptr<Scene> GetActiveScene() const;

    /**
     * @brief Get the singleton instance
     * @return Editor instance
     */
    static Editor& Get();

private:
    Editor();
    ~Editor();
    bool m_IsInitialized = false;
    EditorConfig m_Config;
    std::shared_ptr<Scene> m_ActiveScene;
};

/**
 * @brief AI Assistant for in-editor development assistance
 */
class AIAssistant {
public:
    /**
     * @brief Initialize the AI Assistant
     * @return True if initialization succeeded
     */
    static bool Initialize();

    /**
     * @brief Shutdown the AI Assistant
     */
    static void Shutdown();

    /**
     * @brief Query the AI Assistant
     * @param query Query text
     * @param callback Callback to receive response
     */
    void Query(const std::string& query, std::function<void(const std::string&)> callback);

    /**
     * @brief Generate AOPL code
     * @param prompt Code generation prompt
     * @param callback Callback to receive generated code
     */
    void GenerateCode(const std::string& prompt, std::function<void(const std::string&)> callback);

    /**
     * @brief Get the singleton instance
     * @return AIAssistant instance
     */
    static AIAssistant& Get();

private:
    AIAssistant();
    ~AIAssistant();
    bool m_IsInitialized = false;
};

} // namespace gaia_matrix