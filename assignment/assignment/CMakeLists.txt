add_executable(assignment main.cpp "Assignment.cpp" "Assignment.h" "Shaders.h"
        "ProgramState.h"
        "InputHandler.h")

target_link_libraries(assignment PRIVATE glfw)
target_link_libraries(assignment PRIVATE glad)
target_link_libraries(assignment PRIVATE Framework::GLFWApplication)
target_link_libraries(assignment PRIVATE Framework::GeometricTools)
target_link_libraries(assignment PRIVATE Framework::Rendering)