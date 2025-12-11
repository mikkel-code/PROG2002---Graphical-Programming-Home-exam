add_executable(exam main.cpp "ExamApplication.cpp" "ExamApplication.h" "Shaders.h"
        "ProgramState.h"
        "InputHandler.h"
        "BlockTypes.h"
        Piece.h)

target_link_libraries(exam PRIVATE glfw)
target_link_libraries(exam PRIVATE glad)
target_link_libraries(exam PRIVATE Framework::GLFWApplication)
target_link_libraries(exam PRIVATE Framework::GeometricTools)
target_link_libraries(exam PRIVATE Framework::Rendering)

add_custom_command(
        TARGET exam POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube_texture.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube_texture.png)

add_custom_command(
        TARGET exam POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/floor_texture.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/floor_texture.png)

target_compile_definitions(exam PRIVATE
        TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")

target_compile_definitions(exam PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(exam PRIVATE STB_IMAGE_STATIC)
target_include_directories(exam PRIVATE
        ${CMAKE_SOURCE_DIR}/external/stb
)