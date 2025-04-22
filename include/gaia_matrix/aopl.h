#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace gaia_matrix {
namespace aopl {

// Forward declarations
class Node;
class Entity;
class Component;
class Transform;
class Function;

/**
 * @brief AOPL Symbol definitions
 */
namespace Symbol {
    constexpr char NODE = 'N';               // Node/Network declaration
    constexpr char ENTITY = 'E';             // Entity
    constexpr char COMPONENT = 'C';          // Component
    constexpr char TRANSFORM = 'T';          // Transform
    constexpr char FUNCTION = 'F';           // Function
    constexpr const char* EVENT = "⊻";       // Event handler
    constexpr const char* CONDITIONAL = "⊿"; // Conditional
    constexpr const char* ASSIGNMENT = "⊸";  // Assignment
    constexpr const char* DATAFLOW = "→";    // Data flow
    constexpr const char* NEURAL_NET = "NN"; // Neural network
    constexpr const char* REINFORCE = "RL";  // Reinforcement learning
    constexpr const char* MODEL_PROC = "MCP";// Model-controlled procedural generation
    constexpr const char* GENETIC = "GA";    // Genetic algorithm
}

/**
 * @brief Parser for the AI-Optimized Programming Language (AOPL)
 */
class Parser {
public:
    Parser();
    ~Parser();

    /**
     * @brief Parse AOPL code from string
     * @param code AOPL code to parse
     * @return True if parsing was successful
     */
    bool Parse(const std::string& code);

    /**
     * @brief Get parsed entities
     * @return Vector of parsed entities
     */
    const std::vector<std::shared_ptr<Entity>>& GetEntities() const;

    /**
     * @brief Compile AOPL code to executable format
     * @return True if compilation was successful
     */
    bool Compile();

private:
    std::vector<std::shared_ptr<Entity>> m_Entities;
    std::unordered_map<std::string, std::shared_ptr<Node>> m_NodeRegistry;
    bool m_IsParsed = false;
};

/**
 * @brief Base class for all AOPL nodes
 */
class Node {
public:
    Node(const std::string& name);
    virtual ~Node() = default;

    /**
     * @brief Get the name of the node
     * @return Node name
     */
    const std::string& GetName() const;

private:
    std::string m_Name;
};

/**
 * @brief Entity node in AOPL
 */
class Entity : public Node {
public:
    Entity(const std::string& name);
    ~Entity() override = default;

    /**
     * @brief Add a component to the entity
     * @param component Component to add
     */
    void AddComponent(std::shared_ptr<Component> component);

    /**
     * @brief Get all components
     * @return Vector of components
     */
    const std::vector<std::shared_ptr<Component>>& GetComponents() const;

    /**
     * @brief Get transform component
     * @return Transform component
     */
    std::shared_ptr<Transform> GetTransform() const;

private:
    std::vector<std::shared_ptr<Component>> m_Components;
    std::shared_ptr<Transform> m_Transform;
};

} // namespace aopl
} // namespace gaia_matrix