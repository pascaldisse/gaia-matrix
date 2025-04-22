# AOPL Language Reference

## Overview

AOPL (AI-Optimized Programming Language) is a specialized programming language designed for GAIA MATRIX with extreme token efficiency. It features a unique symbolic syntax that optimizes for AI-based development and runtime intelligence.

## Core Concepts

AOPL represents game logic as a network of interconnected nodes with data flowing between them. The language is designed to be both human-readable and optimized for AI processing, with a minimal token footprint.

## Syntax Elements

### Core Symbols

| Symbol | Name | Description | Example |
|--------|------|-------------|---------|
| `N` | Node/Network | Declares a node or network | `N ⊢ E〈Player〉` |
| `E` | Entity | Defines a game entity | `E〈Player〉〈T⊕C⊕I〉` |
| `C` | Component | Defines a component | `C: F Move Jump` |
| `T` | Transform | Transform component | `T: P 0 0 0 → R 0 0 0 → S 1 1 1` |
| `F` | Function | Defines a function | `F Move() → T.P += V * dt` |
| `⊢` | Declaration | Introduces a declaration | `N ⊢ E〈Player〉` |
| `⊻` | Event Handler | Defines event responses | `⊻ OnCollision(E other) → Sound.Play("bump")` |
| `⊿` | Conditional | Represents a condition | `⊿ grounded → Jump()` |
| `⊸` | Assignment | Assigns values | `⊸ speed 10.0` |
| `→` | Data Flow | Shows data flow direction | `I.K W → T.P z+ 0.1` |
| `⊕` | Composition | Combines elements | `T⊕C⊕I` |

### AI-Specific Symbols

| Symbol | Name | Description | Example |
|--------|------|-------------|---------|
| `NN` | Neural Network | Neural network node | `NN〈Animator〉: I pose → O animation` |
| `RL` | Reinforcement Learning | RL agent | `RL〈EnemyAI〉: I state → O action` |
| `〈MCP〉` | Model-Controlled Procedural | AI-driven procedural generation | `〈MCP〉 TerrainGen: I params → O terrain` |
| `GA` | Genetic Algorithm | Genetic algorithm | `GA〈Evolution〉: I fitness → O population` |

## Basic Structure

AOPL code is organized into interconnected nodes that define entities, components, and behaviors:

```
# Entity definition
N ⊢ E〈Entity〉〈T⊕C⊕I〉
T: P x y z → R rx ry rz → S sx sy sz
C: F fn1 fn2 → ⊻ event1 event2
I: ⊢ input_component

# Component definition
N〈Component〉: V ⊢ I → F → C
Function1: input → action
Function2: condition → action

# Neural network integration
NN〈Network〉: I → O
⊸ Model "path/to/model.onnx"
⊸ Input param1 param2
⊸ Output result1 result2
```

## Examples

### Player Controller

```
N ⊢ E〈Player〉〈T⊕C⊕I〉
T: P 0 0 0 → R 0 0 0 → S 1 1 1
C: F Move Jump → ⊻ OnUpdate OnCollision
I: ⊢ K → M → G

N〈PlayerController〉: V ⊢ I → F Move → A Jump → C Collision
Move: I.K W → T.P z+ 0.1
Move: I.K S → T.P z- 0.1
Move: I.K A → T.P x- 0.1
Move: I.K D → T.P x+ 0.1
Jump: I.K Space → V.y 5 → ⊿ grounded
Collision: ⊿ ground → ⊸ grounded true → V.y 0
```

### Neural Network for Animation

```
NN〈Animator〉: E Player → O Animation
⊸ Model "models/animator.onnx"
⊸ Input Player.V Player.T.R Player.I.K
⊸ Output "walk" "run" "jump" "idle"
```

### Model-Controlled Procedural Generation

```
〈MCP〉 TerrainGenerator:
  ⊸ Model "models/terrain.onnx"
  ⊸ Parameters seed biome elevation moisture
  ⊸ Generate "heightmap" "texturemap" "objectmap"
  ⊸ Resolution 512 512
```

## Data Types

AOPL has minimal built-in data types to maintain its compact nature:

| Type | Description | Example |
|------|-------------|---------|
| `N` | Number (float or int context-dependent) | `5`, `3.14` |
| `V` | Vector (2D, 3D, or 4D context-dependent) | `V 1 2 3`, `V.x`, `V.yz` |
| `S` | String | `"hello"` |
| `B` | Boolean | `true`, `false` |
| `E` | Entity reference | `E Player`, `E.Component` |
| `⊢` | Type declaration | `V ⊢ velocity` |

## Advanced Features

### Neural Engine Integration

AOPL provides direct hooks into the Neural Engine through specialized constructs:

```
NN〈Inference〉:
  ⊸ Model "model.onnx"
  ⊸ Device "NeuralEngine"
  ⊸ Fallback "CPU"
  ⊸ BatchSize 4
  ⊸ Input features
  ⊸ Output predictions
```

### Event System

```
⊻ OnCollision(E other):
  ⊿ other.type "enemy" → TakeDamage(10)
  ⊿ other.type "powerup" → Collect()
```

## Best Practices

1. **Token Efficiency**: Use symbols over keywords whenever possible
2. **Context Awareness**: Leverage context to minimize repeated entity/component references
3. **Flow-Oriented**: Think in terms of data flow rather than imperative instructions
4. **Neural First**: Design with neural network integration in mind
5. **Spatial Layout**: Use spatial arrangement to clarify relationships

## Compilation

AOPL is compiled to a runtime-optimized format that can be executed directly by the GAIA MATRIX engine or exported to target platforms.

## AOPL Editor Support

The GAIA MATRIX Editor provides specialized support for AOPL:

- Syntax highlighting with Unicode symbol support
- Auto-completion with context awareness
- Real-time validation
- AI-assisted code generation
- Visual node editing

## See Also

- [Neural Engine Integration](neural-engine.md)
- [AOPL Editor Guide](editor.md)
- [AI Assistance](ai-assistance.md)