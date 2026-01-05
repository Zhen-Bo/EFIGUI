# =============================================
# EFIGUI Unit Tests
# =============================================

# Test executable
add_executable(efigui_tests
    test_main.cpp
    test_core.cpp
)

# Link EFIGUI library
target_link_libraries(efigui_tests PRIVATE EFIGUI)

# Include directories
# 1. Test directory itself (for doctest.h)
# 2. Project root imgui folder (if exists)
# 3. External IMGUI_INCLUDE_DIR (for CI/CD scenarios)
target_include_directories(efigui_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/imgui
    ${IMGUI_INCLUDE_DIR}
)

# C++17 standard
target_compile_features(efigui_tests PRIVATE cxx_std_17)

# Register test with CTest
add_test(NAME efigui_tests COMMAND efigui_tests)
