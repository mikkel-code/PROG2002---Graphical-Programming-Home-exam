#ifndef LAB3APPLICATION_H
#define LAB3APPLICATION_H
#include "GLFWApplication.h"
#include <string>
#include <glad/glad.h>

class Lab4Application : public GLFWApplication {
public:
    GLuint LoadCubeMap(std::string filepath, GLuint slot) const;

    GLuint LoadTexture(const std::string &filepath, GLuint slot) const;

    Lab4Application(const std::string& name, const std::string& version);

    unsigned Run() const override;  // render loop + shape
};
#endif