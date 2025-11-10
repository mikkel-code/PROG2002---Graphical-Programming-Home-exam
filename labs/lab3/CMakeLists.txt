add_executable(lab3 main.cpp "Lab3Application.cpp" "Lab3Application.h" "Shaders.h"
        "ProgramState.h"
        "InputHandler.h")

target_link_libraries(lab3 PRIVATE glfw)
target_link_libraries(lab3 PRIVATE glad)
target_link_libraries(lab3 PRIVATE Framework::GLFWApplication)
target_link_libraries(lab3 PRIVATE Framework::GeometricTools)
target_link_libraries(lab3 PRIVATE Framework::Rendering)