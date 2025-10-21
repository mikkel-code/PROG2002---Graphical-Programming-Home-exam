#include "GLFWApplication.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


GLFWApplication::GLFWApplication(const std::string& name, const std::string& version) {
    std::cout << "Starting" << name << " v" << version << std::endl;
}

GLFWApplication::~GLFWApplication() {
    //###################### Termination code (SECTION 6) ##############
    std::cout << "Terminated\n";
    glfwTerminate();
}


unsigned GLFWApplication::Init() {
    //###################### GLFW initialization code (SECTION 2) ##############
    if (!glfwInit()) {
        std::cerr << "Failed to initialize";
        return -1;
    }

    // Creating window hints for OpenGL version update 4.6 (works for my windows)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating the window and giving it context.
    GLFWwindow* window = glfwCreateWindow(800, 600, "MY window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    //###################### OpenGL initialization code (SECTION 3) ##############

   // SECTION 3: OpenGL initialization
    // Sets window in current context
    glfwMakeContextCurrent(window);
    // Load OpenGL with Glad

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not initialize GLAD" << std::endl;
    }

    return 0;
    std::cout << "OpenGL initialized\n";
}

