add_executable(lab2 main.cpp "Lab2Application.cpp" "Lab2Application.h" "Shaders.h")

target_link_libraries(lab2 PRIVATE glfw)
target_link_libraries(lab2 PRIVATE glad)
target_link_libraries(lab2 PRIVATE Framework::GLFWApplication)
target_link_libraries(lab2 PRIVATE Framework::GeometricTools)