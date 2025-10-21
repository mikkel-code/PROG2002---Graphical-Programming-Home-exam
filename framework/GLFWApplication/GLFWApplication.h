#pragma once
#include <string>

class GLFWApplication {
public:
    GLFWApplication(const std::string& name, const std::string& version);
    virtual ~GLFWApplication();

    virtual unsigned Init();             // Default behavior (setup GLFW, GLAD)
    virtual unsigned Run() const = 0;    // Must be defined by subclasses
};