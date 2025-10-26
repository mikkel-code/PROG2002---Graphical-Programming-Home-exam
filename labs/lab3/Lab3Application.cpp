#include "Lab3Application.h"
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
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

struct programState {
    std::shared_ptr<Shader> shader;
    int coordinateX = 0;
    int coordinateY = 0;
    float xRadians = 0.0f;
    float yRadians = 0.0f;
};

Lab3Application::Lab3Application(const std::string& name, const std::string& version)
    : GLFWApplication(name, version) {
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned Lab3Application::Run() const {
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) return -1;

    // Assign call back to glfw
    glfwSetKeyCallback(window, keyCallback);

    programState state;

    // change the viewport so it becomes a square and not stretched
    glViewport(100, 100, 500, 500);
    glEnable(GL_DEPTH_TEST);
    // chessboard
    auto chessboard = GeometricTools::UnitGrid3D(8,8);
    auto chessboardIndices  = GeometricTools::UnitGridTopologyTriangles(8,8);

    auto chessboardVB = std::make_shared<VertexBuffer>(chessboard.data(), static_cast<GLsizei>(chessboard.size()));
    auto chessboardIB = std::make_shared<IndexBuffer>(chessboardIndices.data(), static_cast<GLsizei>(chessboardIndices.size()));

    BufferLayout chessboardLayout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" },
    };
    chessboardVB->SetLayout(chessboardLayout);
    auto chessboardVA = std::make_shared<VertexArray>();
    chessboardVA->Bind();
    chessboardVA->AddVertexBuffer(chessboardVB);
    chessboardVA->SetIndexBuffer(chessboardIB);
    chessboardVA->Unbind();

    auto model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.1f, 0.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 proj = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 10.0f);

    auto chessboardShader = std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc);
    chessboardShader->Bind();
    chessboardShader->UploadUniformMat4("model", model);
    chessboardShader->UploadUniformMat4("view", view);
    chessboardShader->UploadUniformMat4("proj", proj);
    chessboardShader->UploadUniformInt("divisionsX", 8);
    chessboardShader->UploadUniformInt("divisionsY", 8);


    // cube
    auto cube = GeometricTools::Cube3D;
    auto cubeIndices = GeometricTools::CubeIndices();

    auto cubeVB = std::make_shared<VertexBuffer>(cube.data(), static_cast<GLsizei>(cube.size()));
    auto cubeIB = std::make_shared<IndexBuffer>(cubeIndices.data(), static_cast<GLsizei>(cubeIndices.size()));

    BufferLayout cubeLayout = {
        { ShaderDataType::Float3, "a_Position" },
    };
    cubeVB->SetLayout(cubeLayout);
    auto cubeVA = std::make_shared<VertexArray>();
    cubeVA->Bind();
    cubeVA->AddVertexBuffer(cubeVB);
    cubeVA->SetIndexBuffer(cubeIB);
    cubeVA->Unbind();

    // cube shader
    auto cubeShader = std::make_shared<Shader>(cubeVertexShaderSrc, cubeFragmentShaderSrc);
    cubeShader->Bind();
    cubeShader->UploadUniformMat4("view", view);
    cubeShader->UploadUniformMat4("proj", proj);
    state.shader = chessboardShader;
    glfwSetWindowUserPointer(window, &state);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.5f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chessboardShader->Bind();
        chessboardVA->Bind();
        glDrawElements(GL_TRIANGLES, chessboardIB->GetCount(), GL_UNSIGNED_INT, nullptr);
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f,0.1f,0.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(0.05f));
        cubeModel = glm::rotate(cubeModel, glm::radians(state.xRadians), glm::vec3(1.0f, 0.0f, 0.0f));
        cubeModel = glm::rotate(cubeModel, glm::radians(state.yRadians), glm::vec3(0.0f, 1.0f, 0.0f));

        cubeShader->Bind();
        cubeShader->UploadUniformMat4("model", cubeModel);
        cubeVA->Bind();
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, cubeIB->GetCount(), GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);

    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //if (action != GLFW_PRESS) return; // So the coordinates doesnt skip one with key presses

    // Get back coordinateState pointer
    auto* state = static_cast<programState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)     state->coordinateY +=1;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)   state->coordinateY -=1;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)  state->coordinateX +=1;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)   state->coordinateX -=1;

    if (key == GLFW_KEY_W) state->xRadians += 10;
    if (key == GLFW_KEY_S) state->xRadians -= 10;
    if (key == GLFW_KEY_D) state->yRadians += 10;
    if (key == GLFW_KEY_A) state->yRadians -= 10;

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {static int mode = 2;
        mode = (mode + 1) % 3;
        GLenum modes[] = {GL_LINE, GL_FILL, GL_POINT};
        glPolygonMode(GL_FRONT_AND_BACK, modes[mode]);
    }
    // Update uniform

    state->shader->Bind();
    state->shader->UploadUniformInt("greenX", state->coordinateX);
    state->shader->UploadUniformInt("greenY", state->coordinateY);
}