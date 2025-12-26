cmake_minimum_required(VERSION 3.16)
project(EFIGUI VERSION 0.2.2 LANGUAGES CXX)

# =============================================
# Options
# =============================================

option(EFIGUI_BUILD_EXAMPLES "Build EFIGUI examples" OFF)
option(EFIGUI_ENABLE_DX11 "Enable DirectX 11 blur backend" ON)
option(EFIGUI_ENABLE_DX12 "Enable DirectX 12 blur backend (future)" OFF)
option(EFIGUI_ENABLE_VULKAN "Enable Vulkan blur backend (future)" OFF)
option(EFIGUI_ENABLE_OPENGL "Enable OpenGL blur backend (future)" OFF)

# =============================================
# EFIGUI Library
# =============================================

set(EFIGUI_SOURCES
    src/EFIGUI/EFIGUI.cpp
    src/EFIGUI/Theme.cpp
    src/EFIGUI/Animation.cpp
    src/EFIGUI/Layer.cpp
    src/EFIGUI/Draw.cpp
    # Components (split into separate files for maintainability)
    src/EFIGUI/Components/Window.cpp
    src/EFIGUI/Components/Panel.cpp
    src/EFIGUI/Components/Navigation.cpp
    src/EFIGUI/Components/Button.cpp
    src/EFIGUI/Components/Toggle.cpp
    src/EFIGUI/Components/Input.cpp
    src/EFIGUI/Components/Slider.cpp
    src/EFIGUI/Components/Combo.cpp
    src/EFIGUI/Components/Progress.cpp
    src/EFIGUI/Components/Card.cpp
    src/EFIGUI/Components/Tooltip.cpp
    src/EFIGUI/Components/Layout.cpp
)

set(EFIGUI_HEADERS
    src/EFIGUI/EFIGUI.h
    src/EFIGUI/Theme.h
    src/EFIGUI/Animation.h
    src/EFIGUI/Layer.h
    src/EFIGUI/Draw.h
    src/EFIGUI/Components.h
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

# C++17 standard
target_compile_features(EFIGUI PUBLIC cxx_std_17)

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
# Example: target_link_libraries(EFIGUI PUBLIC imgui)

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
