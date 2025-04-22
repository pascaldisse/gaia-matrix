# Tests for GAIA MATRIX Engine

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test utilities
add_library(test_utils STATIC
    test_utils/mock_neural_engine.h
    test_utils/mock_neural_engine.cpp
    test_utils/test_helpers.h
    test_utils/test_helpers.cpp
)
target_link_libraries(test_utils PRIVATE gaia_matrix_lib)

# Core tests
add_executable(core_tests
    core/engine_tests.cpp
)
target_link_libraries(core_tests PRIVATE 
    gaia_matrix_lib 
    test_utils
    ${GTEST_LIBRARIES}
    pthread
)

# AOPL tests
add_executable(aopl_tests
    aopl/parser_tests.cpp
)
target_link_libraries(aopl_tests PRIVATE 
    gaia_matrix_lib 
    test_utils
    ${GTEST_LIBRARIES}
    pthread
)

# Neural Engine tests
add_executable(neural_tests
    neural/neural_engine_tests.cpp
)
target_link_libraries(neural_tests PRIVATE 
    gaia_matrix_lib 
    test_utils
    ${GTEST_LIBRARIES}
    pthread
)

# Platform tests
add_executable(platform_tests
    platform/platform_tests.cpp
)
target_link_libraries(platform_tests PRIVATE 
    gaia_matrix_lib 
    test_utils
    ${GTEST_LIBRARIES}
    pthread
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(core_tests)
gtest_discover_tests(aopl_tests)
gtest_discover_tests(neural_tests)
gtest_discover_tests(platform_tests)

# Create a custom target to run all tests
add_custom_target(tests
    COMMAND core_tests
    COMMAND aopl_tests
    COMMAND neural_tests
    COMMAND platform_tests
    COMMENT "Running all GAIA MATRIX tests"
)