#include "Shader.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
{
    // Create and compile vertex shader
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = vertexSrc.c_str();
    glShaderSource(VertexShader, 1, &vertexSource, nullptr);
    glCompileShader(VertexShader);

    // Check for vertex shader compile errors
    GLint success;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";}

    // Create and compile fragment shader
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = fragmentSrc.c_str();
    glShaderSource(FragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(FragmentShader);

    // Check for fragment shader compile errors
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";}

    // Create program and link shaders
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    // Check for linking errors
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";}

    // Shaders are linked, can now be deleted
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}
Shader::~Shader() {
    glDeleteProgram(ShaderProgram);
}

void Shader::Bind() const {
    glUseProgram(ShaderProgram);
}
void Shader::Unbind() const {
    glUseProgram(0);
}
void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector) {
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());

    // Check for error in UniformLocation
    if (location == -1){
        std::cerr << "WARNING::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform2f(location, vector.x, vector.y);
}

void Shader::UploadUniformInt(const std::string& name, GLint value) {
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());

    // Check for error in UniformLocation
    if (location == -1){
        std::cerr << "WARNING::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1i(location, value);
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1) {
        std::cout << "WARNING::SHADER::UNIFORM_NOT_FOUND" << name << std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::UploadUniformVec4(const std::string& name, const glm::vec4& vec) {
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1) {
        std::cout << "WARNING::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

// For possible new uses IDK
void Shader::CompileShader(GLenum shaderType, const std::string &shaderSrc)
{
    GLuint shader = glCreateShader(shaderType);
    const char* src = shaderSrc.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
}
