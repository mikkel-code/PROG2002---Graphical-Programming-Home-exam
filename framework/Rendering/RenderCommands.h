#ifndef RENDERCOMMANDS_H
#define RENDERCOMMANDS_H
#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "VertexArray.h"

namespace RenderCommands
{
    inline void Clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
        glClear(mode);
    }

    inline void SetPolygonMode(GLenum face, GLenum mode)
    {
        glPolygonMode(face, mode);
    }

    inline void DrawIndex(GLenum primitive, const std::shared_ptr<VertexArray>& vao)
    {
        glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
    inline void SetClearColor(const glm::vec3& rgb) {
        glClearColor(rgb.r,rgb.g,rgb.b, 1.0f);
    }
    inline void SetWireframeMode() {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
    inline void SetSolidMode() {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
}

#endif //RENDERCOMMANDS_H
