#include "Lab4Application.h"
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
#include "TextureManager.h"

#include "Shaders.h"
#include "ProgramState.h"
#include "InputHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/fwd.hpp>

const std::string TEXTURE_DIR = "resources/textures/";

Lab4Application::Lab4Application(const std::string& name, const std::string& version)
    : GLFWApplication(name, version) {
}

unsigned Lab4Application::Run() const {
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) return -1;

    // Assign call back to glfw
    glfwSetKeyCallback(window, keyCallback);

    ProgramState state;

    glfwSetWindowUserPointer(window, &state);

    RenderCommands::SetClearColor(glm::vec3(0.5f, 0.5f, 0.5f));
    // change the viewport so it becomes a square and not stretched
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    // Enable blending
    glEnable(GL_BLEND);
    // Set the blending function: s*alpha + d(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    PerspectiveCamera::Frustrum frustrum;
    frustrum.angle = 50.0f;
    frustrum.width = 800.0f;   // aspect ratio 1:1
    frustrum.height = 600.0f;
    frustrum.near = 0.1f;
    frustrum.far = 10.0f;

    PerspectiveCamera camera(
        frustrum,
        glm::vec3(state.cameraPosition),   // position
        glm::vec3(0.0f, 0.0f, 0.0f),  // lookAt
        glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );

    auto view = camera.GetViewMatrix();
    auto proj = camera.GetProjectionMatrix();
    // chessboard
    auto chessboard = GeometricTools::UnitGridGeometry2DWTCoordsColors(8,8);
    auto chessboardIndices  = GeometricTools::UnitGridTopologyTriangles(8,8);

    auto chessboardVB = std::make_shared<VertexBuffer>(chessboard.data(), static_cast<GLsizei>(chessboard.size()));
    auto chessboardIB = std::make_shared<IndexBuffer>(chessboardIndices.data(), static_cast<GLsizei>(chessboardIndices.size()));

    TextureManager* tm = TextureManager::GetInstance();
    tm->LoadTexture2DRGBA("Chessboard",std::string(TEXTURES_DIR) + "floor_texture.png", 0);

    BufferLayout chessboardLayout = {
        { ShaderDataType::Float3, "a_Position" }, // x y z
        { ShaderDataType::Float2, "a_TexCoord" }, // u v
        { ShaderDataType::Float4, "a_Color"}, // r g b

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
    chessboardShader->UploadUniformInt("u_floorTextureSampler", 0);
    chessboardShader->UploadUniformBool("useTexture", state.useTexture);

    state.shader = chessboardShader;


    // cube
    auto cube3d = GeometricTools::Cube3D;
    auto cubeIndices = GeometricTools::CubeIndices();

    auto cubeVB = std::make_shared<VertexBuffer>(cube3d.data(), static_cast<GLsizei>(cube3d.size()));
    auto cubeIB = std::make_shared<IndexBuffer>(cubeIndices.data(), static_cast<GLsizei>(cubeIndices.size()));

    tm->LoadCubeMapRGBA("CubeCubeMap", std::string(TEXTURES_DIR) + "cube_texture.png", 1);

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
    cubeShader->UploadUniformMat4("model", model);
    cubeShader->UploadUniformMat4("view", view);
    cubeShader->UploadUniformMat4("proj", proj);
    cubeShader->UploadUniformInt("uTexture", 1);
    cubeShader->UploadUniformBool("useTexture", state.useTexture);

    constexpr float step = 1.0f/8.0f;

    int boardstateCount = 0;
    for (int x = 0; x < 8; x++) { // front row
        for (int z = 0; z < 2; z++) {
            Cube c;
            c.position = glm::vec3(-0.5f + step * (x + 0.5f), 0.05f, -0.5f + step * (z + 0.5f));
            c.scale = glm::vec3(0.03f);
            c.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
            c.boardCoordx = x; c.boardCoordz = z;
            state.cubes.push_back(c);
            state.boardstate[boardstateCount++] = true;
        }
    }

    boardstateCount = 63;
    for (int x = 0; x < 8; x++) { // back row
        for (int z = 6; z < 8; z++) {
            Cube c;
            c.position = glm::vec3(-0.5f + step * (x + 0.5f), 0.05f, -0.5f + step * (z + 0.5f));
            c.scale = glm::vec3(0.03f);
            c.color = glm::vec4(0.5f,0.1f,0.1f,1.0f);
            c.boardCoordx = x; c.boardCoordz = z;
            state.cubes.push_back(c);
            state.boardstate[boardstateCount--] = true;
        }
    }

    // For delta time
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Run loop
    while (!glfwWindowShouldClose(window)) {

        //
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        processInput(window, state, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(state.fov), (800.0f / 600.0f), 0.1f, 100.0f);

        camera.SetPosition(state.cameraPosition);
        auto view = camera.GetViewMatrix();

        chessboardShader->Bind();
        chessboardShader->UploadUniformMat4("model", model);
        chessboardShader->UploadUniformMat4("view", view);
        chessboardShader->UploadUniformMat4("proj", projection);

        chessboardShader->Bind();
        chessboardVA->Bind();
        RenderCommands::DrawIndex(GL_TRIANGLES, chessboardVA);

        cubeShader->Bind();
        cubeShader->UploadUniformMat4("model", model);
        cubeShader->UploadUniformMat4("view", view);
        cubeShader->UploadUniformMat4("proj", projection);

        for (const auto& cube : state.cubes) {
            cube.Draw(cubeShader, cubeVA, state.coordinateX, state.coordinateY);
        }
        glfwSwapBuffers(window);
    }
    return 0;
}