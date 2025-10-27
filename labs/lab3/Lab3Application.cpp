#include "Lab3Application.h"
#include "GeometricTools.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "ShadersDataTypes.h"
#include "VertexArray.h"
#include "RenderCommands.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "cube.h"

#include "Shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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
    glViewport(0, 0, 500, 500);
    glEnable(GL_DEPTH_TEST);

    PerspectiveCamera::Frustrum frustrum;
    frustrum.angle = 50.0f;
    frustrum.width = 1.0f;   // aspect ratio 1:1
    frustrum.height = 1.0f;
    frustrum.near = 0.1f;
    frustrum.far = 10.0f;

    PerspectiveCamera camera(
        frustrum,
        glm::vec3(3.0f, 2.0, 0.5f),   // position
        glm::vec3(0.0f, 0.0f, 0.0f),  // lookAt
        glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );

    glfwSetWindowUserPointer(window, &state);

    RenderCommands::SetClearColor(glm::vec3(0.5f, 0.05f, 0.05f));

    auto view = camera.GetViewMatrix();
    auto proj = camera.GetProjectionMatrix();
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

    auto chessboardShader = std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc);
    chessboardShader->Bind();
    chessboardShader->UploadUniformMat4("model", model);
    chessboardShader->UploadUniformMat4("view", view);
    chessboardShader->UploadUniformMat4("proj", proj);
    chessboardShader->UploadUniformInt("divisionsX", 8);
    chessboardShader->UploadUniformInt("divisionsY", 8);

    state.shader = chessboardShader;


    // cube
    auto cube3d = GeometricTools::Cube3D;
    auto cubeIndices = GeometricTools::CubeIndices();

    auto cubeVB = std::make_shared<VertexBuffer>(cube3d.data(), static_cast<GLsizei>(cube3d.size()));
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

    auto cubeShader = std::make_shared<Shader>(cubeVertexShaderSrc, cubeFragmentShaderSrc);
    cubeShader->Bind();
    cubeShader->UploadUniformMat4("view", view);
    cubeShader->UploadUniformMat4("proj", proj);

    std::vector<Cube> cubes;

    for (int x = 0; x < 8; x++) { // front row
        for (int z = 0; z < 2; z++) {
            Cube c;
            float step = 1.0f / 8.0f;
            c.position = glm::vec3(-0.5f + step * (x + 0.5f), 0.05f, -0.5f + step * (z + 0.5f));
            c.scale = glm::vec3(0.05f);
            c.color = glm::vec4(0.05f,0.05f,0.3f,1.0f);
            cubes.push_back(c);
        }
    }

    for (int x = 0; x < 8; x++) { // back row
        for (int z = 6; z < 8; z++) {
            Cube c;
            float step = 1.0f / 8.0f;
            c.position = glm::vec3(-0.5f + step * (x + 0.5f), 0.05f, -0.5f + step * (z + 0.5f));
            c.scale = glm::vec3(0.05f);
            c.color = glm::vec4(0.3f,0.05f,0.05f,1.0f);
            cubes.push_back(c);
        }
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        chessboardShader->Bind();
        chessboardVA->Bind();
        RenderCommands::DrawIndex(GL_TRIANGLES, chessboardVA);

        for (const auto& cube : cubes) {
            cube.Draw(cubeShader, cubeVA);
        }
        glfwSwapBuffers(window);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    auto* state = static_cast<programState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)     if (state->coordinateY < 7) state->coordinateY +=1;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)   if (state->coordinateY > 0) state->coordinateY -=1;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)  if (state->coordinateX < 7) state->coordinateX +=1;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)   if (state->coordinateX > 0) state->coordinateX -=1;

    if (key == GLFW_KEY_W) state->xRadians += 10;
    if (key == GLFW_KEY_S) state->xRadians -= 10;
    if (key == GLFW_KEY_D) state->yRadians += 10;
    if (key == GLFW_KEY_A) state->yRadians -= 10;

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        static int mode = 1;
        mode = (mode + 1) % 2;
        mode ? RenderCommands::SetSolidMode() : RenderCommands::SetWireframeMode();
    }
    state->shader->Bind();
    state->shader->UploadUniformInt("greenX", state->coordinateX);
    state->shader->UploadUniformInt("greenY", state->coordinateY);
}
