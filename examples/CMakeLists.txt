if(EFIGUI_BUILD_EXAMPLES)
    # Basic example - requires user to provide ImGui and graphics API
    add_executable(efigui_basic_example basic_example.cpp)
    target_link_libraries(efigui_basic_example PRIVATE EFIGUI)

    # Note: Users need to link their own ImGui and graphics API
    # target_link_libraries(efigui_basic_example PRIVATE imgui)
endif()
