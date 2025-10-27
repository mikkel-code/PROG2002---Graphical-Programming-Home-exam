#include "Lab2Application.h"
#include "GeometricTools.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "ShadersDataTypes.h"
#include "VertexArray.h"

#include "Shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>

struct coordinateState {
    std::shared_ptr<Shader> shader;
    int coordinateX = 0;
    int coordinateY = 0;
};

Lab2Application::Lab2Application(const std::string& name, const std::string& version)
    : GLFWApplication(name, version) {
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned Lab2Application::Run() const {
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) return -1;

    // Assign call back to glfw
    glfwSetKeyCallback(window, keyCallback);

    coordinateState state;

    // change the viewport so it becomes a square and not stretched
    glViewport(100, 100, 500, 500);

    auto shape = GeometricTools::UnitGrid2D(8,8);
    auto indices  = GeometricTools::UnitGridTopologyTriangles(8,8);

    auto vb = std::make_shared<VertexBuffer>(shape.data(), static_cast<GLsizei>(shape.size())); //  * sizeof(float)?
    auto ib = std::make_shared<IndexBuffer>(indices.data(), static_cast<GLsizei>(indices.size()));

    BufferLayout layout = {
        { ShaderDataType::Float2, "a_Position" },
        { ShaderDataType::Float4, "a_Color" }
    };
    vb->SetLayout(layout);


    auto va = std::make_shared<VertexArray>();
    va->Bind();
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);

    auto chessboardShader = std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc);
    chessboardShader->Bind();
    chessboardShader->UploadUniformInt("divisionsX", 8);
    chessboardShader->UploadUniformInt("divisionsY", 8);

    state.shader = chessboardShader;

    glfwSetWindowUserPointer(window, &state);

    // === Render loop ===
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.3f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //va.Bind();
        glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
        //va.Unbind();
        glfwSwapBuffers(window);
    }
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return; // So the coordinates doesnt skip one with key presses

    // Get back coordinateState pointer
    auto* state = static_cast<coordinateState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}

    if (key == GLFW_KEY_UP)     if (state->coordinateY < 7) state->coordinateY +=1;
    if (key == GLFW_KEY_DOWN)   if (state->coordinateY > 0) state->coordinateY -=1;
    if (key == GLFW_KEY_RIGHT)  if (state->coordinateX < 7) state->coordinateX +=1;
    if (key == GLFW_KEY_LEFT)   if (state->coordinateX > 0) state->coordinateX -=1;

    // Update uniform

    state->shader->Bind();
    state->shader->UploadUniformInt("greenX", state->coordinateX);
    state->shader->UploadUniformInt("greenY", state->coordinateY);
}