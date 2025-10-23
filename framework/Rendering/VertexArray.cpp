#include "VertexArray.h"
#include "ShadersDataTypes.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
}

VertexArray::~VertexArray() {
    glDeleteBuffers(1, &m_vertexArrayID);
}

void VertexArray::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayID);
}

void VertexArray::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    vertexBuffer->Bind();

    GLuint index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexArrayAttrib(m_vertexArrayID,index);
        glVertexAttribPointer(index,
            element.GetElementCount(),
            ShaderDataTypeComponentCount(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(), (const void*)element.Offset);
        index++;
    }
    VertexBuffers.push_back(vertexBuffer);
}
void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
    glBindVertexArray(m_vertexArrayID);
    indexBuffer->Bind();
    IdxBuffer = indexBuffer;
}

