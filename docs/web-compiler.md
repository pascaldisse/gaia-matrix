# GAIA MATRIX Web Compiler

The GAIA MATRIX Web Compiler allows you to compile AOPL code and entire projects to run in web browsers using WebGL and modern JavaScript or WebAssembly. 

## Overview

The Web Compiler provides the following capabilities:

- Compile AOPL code to JavaScript (ES5 or ESNext) or WebAssembly
- Generate complete web applications that can run in modern browsers
- Include an optional browser-based editor for developing AOPL code
- Use WebGL for rendering GAIA MATRIX content in browsers
- Support for the AOPL language features and Neural Engine capabilities

## Command Line Usage

You can use the GAIA MATRIX command line tool to build web applications:

```bash
./gaia_matrix --web-build <output_directory> [options]
```

### Options:

- `--web-build <dir>`: Specify the output directory for the web build
- `--web-editor`: Include the browser-based AOPL editor in the build
- `--web-format <format>`: Specify the output format:
  - `esnext`: Modern JavaScript (default)
  - `es5`: Legacy JavaScript with compatibility for older browsers
  - `wasm`: WebAssembly for better performance
- `--no-minify`: Disable minification of JavaScript output

## Web Architecture

The web implementation consists of several components:

1. **WebGL Renderer**: Implements the GAIA MATRIX rendering pipeline using WebGL 2.0
2. **AOPL Runtime**: JavaScript implementation of the AOPL interpreter
3. **Neural Enhancement**: Web implementation of neural features via WebGL shaders and TensorFlow.js
4. **Web Editor**: Browser-based AOPL code editor with Monaco editor integration

### WebGL Renderer

The WebGL renderer provides a complete implementation of the GAIA MATRIX rendering pipeline using WebGL 2.0, including:

- Shader-based rendering with PBR materials
- Neural-enhanced post-processing effects
- Scene graph management
- Texture and mesh handling

### AOPL Runtime

The AOPL runtime for web includes:

- Parser for AOPL syntax
- Transpiler to convert AOPL to JavaScript
- Runtime environment for executing AOPL entities and components
- Support for all AOPL symbols and features

### Browser Editor

The browser-based editor provides:

- Syntax highlighting for AOPL
- Real-time compilation and preview
- File management for AOPL scripts
- Integration with the GAIA MATRIX engine
- Example code templates

## WebAssembly Support

WebAssembly (WASM) compilation provides better performance by:

- Compiling AOPL directly to optimized WASM bytecode
- Reducing parsing and execution overhead
- Enabling near-native performance for computationally intensive tasks
- Supporting integration with JavaScript for DOM manipulation

## Neural Enhancement in WebGL

Neural enhancement features are implemented in the browser through:

- WebGL shader-based neural effects
- Optional TensorFlow.js integration for more complex models
- WebGL compute shaders (where available) for GPU acceleration

## Example Usage

### Basic Web Build

```bash
./gaia_matrix --web-build ./web_output
```

This creates a basic web application in the `./web_output` directory.

### Web Build with Editor

```bash
./gaia_matrix --web-build ./web_output --web-editor
```

This builds a web application with the browser-based AOPL editor included.

### WebAssembly Build

```bash
./gaia_matrix --web-build ./web_output --web-format wasm
```

This builds a web application using WebAssembly for better performance.

## Programmatic API

The Web Compiler can also be used programmatically from C++ code:

```cpp
#include "gaia_matrix/web_compiler.h"

// Initialize compiler
gaia_matrix::WebCompilerConfig config;
config.outputFormat = gaia_matrix::WebOutputFormat::ESNext;
config.minify = true;
gaia_matrix::WebCompiler::Initialize(config);

// Compile AOPL code
gaia_matrix::WebCompiler::Get().CompileAOPL(aoplSource, outputPath);

// Generate complete web application
gaia_matrix::WebCompiler::Get().GenerateWebApp(
    "My Project", 
    "./web_output", 
    aoplSources, 
    includeEditor
);

// Shutdown when done
gaia_matrix::WebCompiler::Shutdown();
```

## Browser Compatibility

The generated web applications are compatible with:

- Chrome 69+
- Firefox 62+
- Safari 12+
- Edge 79+

Older browsers may require the ES5 output format and additional polyfills.