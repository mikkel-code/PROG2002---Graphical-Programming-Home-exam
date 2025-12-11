#ifndef SHADER_H
#define SHADER_H

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

    void UploadUniformFloat(const std::string &name, float value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& vector);
    void UploadUniformInt(const std::string& name, GLint value);

    void UploadUniformBool(const std::string &name, bool value);

    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    void UploadUniformVec4(const std::string& name, const glm::vec4& vec);

private:
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint ShaderProgram;

    void CompileShader(GLenum shaderType, const std::string &shaderSrc);
};

#endif
