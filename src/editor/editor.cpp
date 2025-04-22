#include "gaia_matrix/editor.h"
#include <iostream>

namespace gaia_matrix {

// Editor implementation
static Editor* s_Instance = nullptr;

Editor::Editor() : m_IsInitialized(false) {
}

Editor::~Editor() {
}

bool Editor::Initialize(const EditorConfig& config) {
    if (s_Instance) {
        std::cerr << "Editor already initialized!" << std::endl;
        return false;
    }
    
    s_Instance = new Editor();
    s_Instance->m_Config = config;
    
    std::cout << "Editor initialized successfully!" << std::endl;
    s_Instance->m_IsInitialized = true;
    
    return true;
}

void Editor::Shutdown() {
    if (!s_Instance) {
        return;
    }
    
    s_Instance->m_IsInitialized = false;
    delete s_Instance;
    s_Instance = nullptr;
    
    std::cout << "Editor shut down successfully!" << std::endl;
}

void Editor::Run() {
    if (!s_Instance || !s_Instance->m_IsInitialized) {
        std::cerr << "Editor not initialized!" << std::endl;
        return;
    }
    
    std::cout << "Editor running... Press Ctrl+C to exit." << std::endl;
    
    // Main editor loop (stub for now)
    bool running = true;
    while (running) {
        // Process events
        // Update editor UI
        // Render editor UI
        
        // For the example, just exit after one iteration
        running = false;
    }
}

bool Editor::OpenProject(const std::string& projectPath) {
    if (!s_Instance || !s_Instance->m_IsInitialized) {
        std::cerr << "Editor not initialized!" << std::endl;
        return false;
    }
    
    s_Instance->m_Config.projectPath = projectPath;
    std::cout << "Project opened: " << projectPath << std::endl;
    
    return true;
}

bool Editor::CreateProject(const std::string& projectName, const std::string& projectPath) {
    if (!s_Instance || !s_Instance->m_IsInitialized) {
        std::cerr << "Editor not initialized!" << std::endl;
        return false;
    }
    
    s_Instance->m_Config.projectPath = projectPath;
    std::cout << "Project created: " << projectName << " at " << projectPath << std::endl;
    
    return true;
}

std::shared_ptr<Scene> Editor::GetActiveScene() const {
    // Stub implementation
    return nullptr;
}

Editor& Editor::Get() {
    if (!s_Instance) {
        std::cerr << "Editor not initialized! Call Initialize() first." << std::endl;
        static Editor dummy;
        return dummy;
    }
    
    return *s_Instance;
}

// AIAssistant implementation
static AIAssistant* s_AIInstance = nullptr;

AIAssistant::AIAssistant() : m_IsInitialized(false) {
}

bool AIAssistant::Initialize() {
    if (s_AIInstance) {
        std::cerr << "AI Assistant already initialized!" << std::endl;
        return false;
    }
    
    s_AIInstance = new AIAssistant();
    s_AIInstance->m_IsInitialized = true;
    
    std::cout << "AI Assistant initialized successfully!" << std::endl;
    
    return true;
}

void AIAssistant::Shutdown() {
    if (!s_AIInstance) {
        return;
    }
    
    s_AIInstance->m_IsInitialized = false;
    delete s_AIInstance;
    s_AIInstance = nullptr;
    
    std::cout << "AI Assistant shut down successfully!" << std::endl;
}

void AIAssistant::Query(const std::string& query, std::function<void(const std::string&)> callback) {
    if (!s_AIInstance || !s_AIInstance->m_IsInitialized) {
        std::cerr << "AI Assistant not initialized!" << std::endl;
        return;
    }
    
    // Stub implementation
    std::string response = "AI Assistant response to: " + query;
    callback(response);
}

void AIAssistant::GenerateCode(const std::string& prompt, std::function<void(const std::string&)> callback) {
    if (!s_AIInstance || !s_AIInstance->m_IsInitialized) {
        std::cerr << "AI Assistant not initialized!" << std::endl;
        return;
    }
    
    // Stub implementation
    std::string code = "# Generated AOPL code for: " + prompt + "\n";
    code += "N ⊢ E〈GeneratedEntity〉〈T⊕C〉\n";
    code += "T: P 0 0 0 → R 0 0 0 → S 1 1 1\n";
    code += "C: F GeneratedFunction\n";
    
    callback(code);
}

AIAssistant& AIAssistant::Get() {
    if (!s_AIInstance) {
        std::cerr << "AI Assistant not initialized! Call Initialize() first." << std::endl;
        static AIAssistant dummy;
        return dummy;
    }
    
    return *s_AIInstance;
}

AIAssistant::~AIAssistant() {
}

} // namespace gaia_matrix