add_executable(lab4 main.cpp "Lab4Application.cpp" "Lab4Application.h" "Shaders.h"
        "ProgramState.h"
        "InputHandler.h")

target_link_libraries(lab4 PRIVATE glfw)
target_link_libraries(lab4 PRIVATE glad)
target_link_libraries(lab4 PRIVATE Framework::GLFWApplication)
target_link_libraries(lab4 PRIVATE Framework::GeometricTools)
target_link_libraries(lab4 PRIVATE Framework::Rendering)

add_custom_command(
        TARGET lab4 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube_texture.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube_texture.png)

add_custom_command(
        TARGET lab4 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/floor_texture.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/floor_texture.png)

target_compile_definitions(lab4 PRIVATE
        TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")

target_compile_definitions(lab4 PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(lab4 PRIVATE STB_IMAGE_STATIC)
target_include_directories(lab4 PRIVATE
        ${CMAKE_SOURCE_DIR}/external/stb
)