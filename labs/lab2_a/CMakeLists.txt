add_executable(lab2_a main.cpp "Lab2Application.cpp" "Lab2Application.h" "Shaders.h")

target_link_libraries(lab2_a PRIVATE glfw)
target_link_libraries(lab2_a PRIVATE glad)
target_link_libraries(lab2_a PRIVATE Framework::GLFWApplication)
target_link_libraries(lab2_a PRIVATE Framework::GeometricTools)
target_link_libraries(lab2_a PRIVATE Framework::Rendering)