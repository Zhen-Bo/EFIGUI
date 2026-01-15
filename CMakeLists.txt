cmake_minimum_required(VERSION 3.16)
project(EFIGUI VERSION 0.5.0 LANGUAGES CXX)

# =============================================
# Options
# =============================================

option(EFIGUI_BUILD_EXAMPLES "Build EFIGUI examples" OFF)
option(EFIGUI_BUILD_TESTS "Build unit tests" OFF)
option(EFIGUI_ENABLE_DX11 "Enable DirectX 11 blur backend" ON)
option(EFIGUI_ENABLE_DX12 "Enable DirectX 12 blur backend (future)" OFF)
option(EFIGUI_ENABLE_VULKAN "Enable Vulkan blur backend (future)" OFF)
option(EFIGUI_ENABLE_OPENGL "Enable OpenGL blur backend (future)" OFF)

# =============================================
# EFIGUI Library
# =============================================

set(EFIGUI_SOURCES
    src/EFIGUI/Core/EFIGUI.cpp
    src/EFIGUI/Core/Animation.cpp
    src/EFIGUI/Core/Layer.cpp
    src/EFIGUI/Core/Draw.cpp
    src/EFIGUI/Core/Style.cpp
    src/EFIGUI/Theme.cpp
    # Components (split into separate files for maintainability)
    src/EFIGUI/Components/Window.cpp
    src/EFIGUI/Components/Panel.cpp
    src/EFIGUI/Components/Navigation.cpp
    src/EFIGUI/Components/Button.cpp
    src/EFIGUI/Components/Toggle.cpp
    src/EFIGUI/Components/Input.cpp
    src/EFIGUI/Components/NumericInput.cpp
    src/EFIGUI/Components/Slider.cpp
    src/EFIGUI/Components/Combo.cpp
    src/EFIGUI/Components/Progress.cpp
    src/EFIGUI/Components/Card.cpp
    src/EFIGUI/Components/Tooltip.cpp
    src/EFIGUI/Components/Layout.cpp
    src/EFIGUI/Components/ConfigImpl.cpp
)

set(EFIGUI_HEADERS
    src/EFIGUI/EFIGUI.h
    src/EFIGUI/Core/EFIGUI.h
    src/EFIGUI/Core/Animation.h
    src/EFIGUI/Core/Layer.h
    src/EFIGUI/Core/Draw.h
    src/EFIGUI/Core/Style.h
    src/EFIGUI/Styles/StyleTypes.h
    src/EFIGUI/Styles/CheckboxStyle.h
    src/EFIGUI/Themes/CyberpunkTheme.h
    src/EFIGUI/Components.h
    src/EFIGUI/Theme.h
    src/EFIGUI/ThemeConfig.h
    src/EFIGUI/Components/Internal.h
    src/EFIGUI/Backend/IBlurBackend.h
)

# Add backend sources based on platform
if(WIN32 AND EFIGUI_ENABLE_DX11)
    list(APPEND EFIGUI_SOURCES src/EFIGUI/Backend/BlurBackendDX11.cpp)
    list(APPEND EFIGUI_HEADERS src/EFIGUI/Backend/BlurBackendDX11.h)
endif()

# Create library
add_library(EFIGUI STATIC ${EFIGUI_SOURCES} ${EFIGUI_HEADERS})

# C++23 standard
target_compile_features(EFIGUI PUBLIC cxx_std_23)

# MSVC-specific options
if(MSVC)
    target_compile_options(EFIGUI PRIVATE /EHsc)
endif()

# Include directories
target_include_directories(EFIGUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

# =============================================
# Dependencies
# =============================================

# ImGui - user must provide imgui target or include path
# Option 1: Set IMGUI_INCLUDE_DIR to point to imgui headers
# Option 2: Link an imgui target (target_link_libraries(EFIGUI PUBLIC imgui))
# Option 3: Use local imgui folder if exists
if(IMGUI_INCLUDE_DIR)
    target_include_directories(EFIGUI PUBLIC
        $<BUILD_INTERFACE:${IMGUI_INCLUDE_DIR}>
    )
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h")
    target_include_directories(EFIGUI PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
    )
else()
    message(WARNING "EFIGUI: ImGui not found. Set IMGUI_INCLUDE_DIR or place imgui/ folder in project root.")
endif()

# Windows-specific libraries
if(WIN32)
    if(EFIGUI_ENABLE_DX11)
        target_link_libraries(EFIGUI PRIVATE
            d3d11
            d3dcompiler
        )
        target_compile_definitions(EFIGUI PRIVATE EFIGUI_ENABLE_DX11=1)
    endif()
endif()

# =============================================
# Examples
# =============================================

if(EFIGUI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# =============================================
# Installation
# =============================================

include(GNUInstallDirs)

install(TARGETS EFIGUI
    EXPORT EFIGUITargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY src/EFIGUI/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/EFIGUI
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT EFIGUITargets
    FILE EFIGUITargets.cmake
    NAMESPACE EFIGUI::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EFIGUI
)

# =============================================
# Package configuration
# =============================================

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EFIGUIConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/EFIGUIConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EFIGUI
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/EFIGUIConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/EFIGUIConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/EFIGUIConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EFIGUI
)

# =============================================
# Unit Tests
# =============================================

if(EFIGUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
