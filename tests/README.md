# GAIA MATRIX Tests

This directory contains tests for the GAIA MATRIX engine. The tests are organized by component and can be run individually or as a suite.

## Test Organization

- `core/` - Tests for core engine functionality
- `aopl/` - Tests for AOPL parser and compiler
- `neural/` - Tests for Neural Engine integration
- `platform/` - Tests for platform abstraction layer
- `renderer/` - Tests for rendering system
- `integration/` - Cross-component integration tests

## Running Tests

To run all tests:

```bash
cd build
cmake --build . --target test
```

To run a specific test suite:

```bash
cd build
ctest -R <test_name>
```

For example, to run all AOPL tests:

```bash
ctest -R aopl
```

## Writing Tests

Each test file should follow this template:

```cpp
#include <gtest/gtest.h>
#include "gaia_matrix.h"

TEST(ComponentName, TestName) {
    // Test code
    EXPECT_TRUE(condition);
}
```

## Mocking Neural Engine

For tests that need to mock the Neural Engine, use the `MockNeuralEngine` class:

```cpp
#include "test_utils/mock_neural_engine.h"

TEST(NeuralTests, ModelLoading) {
    MockNeuralEngine engine;
    
    // Set up expectations
    engine.ExpectLoadModel("test_model.onnx", 1);
    
    // Test code using the mock
    // ...
}
```