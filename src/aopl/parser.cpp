#include "gaia_matrix/aopl.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

namespace gaia_matrix {
namespace aopl {

// Implementation of Node class
Node::Node(const std::string& name) : m_Name(name) {}

const std::string& Node::GetName() const {
    return m_Name;
}

// Implementation of Entity class
Entity::Entity(const std::string& name) : Node(name) {}

void Entity::AddComponent(std::shared_ptr<Component> component) {
    m_Components.push_back(component);
}

const std::vector<std::shared_ptr<Component>>& Entity::GetComponents() const {
    return m_Components;
}

std::shared_ptr<Transform> Entity::GetTransform() const {
    return m_Transform;
}

// Implementation of Parser class
Parser::Parser() {}

Parser::~Parser() {}

bool Parser::Parse(const std::string& code) {
    // Clear previous parsing results
    m_Entities.clear();
    m_NodeRegistry.clear();
    m_IsParsed = false;
    
    std::istringstream stream(code);
    std::string line;
    
    std::shared_ptr<Entity> currentEntity = nullptr;
    
    while (std::getline(stream, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Parse line
        // Entity definition: N ⊢ E〈T⊕C⊕I〉
        if (line[0] == Symbol::NODE && line.find('E') != std::string::npos) {
            // Extract entity name (if any)
            std::string entityName = "Entity";
            size_t anglePos = line.find('<');
            if (anglePos != std::string::npos) {
                entityName = line.substr(line.find('E') + 1, anglePos - line.find('E') - 1);
                if (entityName.empty()) {
                    entityName = "Entity";
                }
            }
            
            currentEntity = std::make_shared<Entity>(entityName);
            m_Entities.push_back(currentEntity);
            m_NodeRegistry[entityName] = currentEntity;
            
            // TODO: Parse components in the entity definition
            
            continue;
        }
        
        // Transform component: T: P 0 0 0 → R 0 0 0 → S 1 1 1
        if (line[0] == Symbol::TRANSFORM && currentEntity) {
            // TODO: Parse transform component
            continue;
        }
        
        // Component definition: C: F → ⊻ OnUpdate OnCollision
        if (line[0] == Symbol::COMPONENT && currentEntity) {
            // TODO: Parse component definition
            continue;
        }
        
        // TODO: Parse other AOPL constructs
    }
    
    m_IsParsed = true;
    return true;
}

const std::vector<std::shared_ptr<Entity>>& Parser::GetEntities() const {
    return m_Entities;
}

bool Parser::Compile() {
    if (!m_IsParsed) {
        std::cerr << "Cannot compile: code has not been parsed yet" << std::endl;
        return false;
    }
    
    // TODO: Implement compilation
    
    return true;
}

} // namespace aopl
} // namespace gaia_matrix