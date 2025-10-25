#include "VertexArray.h"

#include <iostream>

#include "ShadersDataTypes.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    vertexBuffer->Bind();

    if (!vertexBuffer->GetLayout().GetAttributes().size()) std::cout << "Vertex buffer has no layout";

    GLuint index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
            element.GetElementCount(),
            ShaderDataTypeToOpenGLBaseType(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            (const void*)element.Offset);
        index++;
    }
    VertexBuffers.push_back(vertexBuffer);
}
void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
    glBindVertexArray(m_vertexArrayID);
    indexBuffer->Bind();
    IdxBuffer = indexBuffer;
}

