#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

class Shader
{
public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);
    void UploadUniformInt(const std::string& name, GLint value);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint ShaderProgram;

    void CompileShader(GLenum shaderType, const std::string &shaderSrc);
};
