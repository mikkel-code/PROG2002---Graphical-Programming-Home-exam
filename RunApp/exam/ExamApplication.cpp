#include "ExamApplication.h"
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

#include "BlockTypes.h"
#include "Shaders.h"
#include "ProgramState.h"
#include "InputHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/fwd.hpp>

constexpr double TARGET_FRAME_TIME = 1.0 / 60; // fps
constexpr double AUTOMOVE_TIMER = 2.0; // seconds
constexpr double CUBE_MOVESPEED = 5.0;

const std::string TEXTURE_DIR = "resources/textures/";


ExamApplication::ExamApplication(const std::string& name, const std::string& version)
    : GLFWApplication(name, version) {
}

unsigned ExamApplication::Run() const {
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) return -1;

    // assign key call back to glfw
    glfwSetKeyCallback(window, keyCallback);

    ProgramState state;

    glfwSetWindowUserPointer(window, &state);

    RenderCommands::SetClearColor(glm::vec3(state.ambientIllumination*0.3));
    // change the viewport so it becomes a square and not stretched
    glViewport(0, 0, 1024, 768);
    glEnable(GL_DEPTH_TEST);
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //=====CAMERA========================================================================================//
    PerspectiveCamera::Frustrum frustrum;
    frustrum.angle = 50.0f;
    frustrum.width = 1024.0f;
    frustrum.height = 768.0f;
    frustrum.near = 0.1f;
    frustrum.far = 10.0f;

    state.cameraPosition = glm::vec3(0.0f, 1.5f, 11.0f);
    PerspectiveCamera camera(frustrum, state.cameraPosition,
        glm::vec3(0.0f, 1.5f, 0.0f), // look at center of room
        glm::vec3(0.0f, 1.0f, 0.0f) // up
    );

    auto view = camera.GetViewMatrix();
    auto proj = glm::perspective(glm::radians(state.fov), (1024.0f / 768.0f), 0.1f, 100.0f);
    //=====CAMERA END====================================================================================//

    //===== SCENE ========================================================================================//
    std::vector<float> scene;
    std::vector<unsigned int> indices;

    auto floor = GeometricTools::UnitGridGeometry2DWTCoords(5,10);
    auto floorIndices = GeometricTools::UnitGridTopologyTriangles(5,10);

    auto floorVB = std::make_shared<VertexBuffer>(floor.data(), static_cast<GLsizei>(floor.size()));
    auto floorIB = std::make_shared<IndexBuffer>(floorIndices.data(), static_cast<GLsizei>(floorIndices.size()));

    float width = 5.0f;
    float length = 10.0f;
    float height = 3.0f;

    glm::mat4 floorModel = glm::mat4(1.0f);
    floorModel = glm::translate(floorModel, glm::vec3(0.0f, -1.0f, 0.0f));

    glm::mat4 ceilingModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height+1, 0.0f));
    ceilingModel = glm::rotate(ceilingModel, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 leftWallModel = glm::translate(glm::mat4(1.0f), glm::vec3(-width/2.0f, height/2.0f, 0.0f));
    leftWallModel = glm::rotate(leftWallModel, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 rightWallModel = glm::translate(glm::mat4(1.0f), glm::vec3(width/2.0f, height/2.0f, 0.0f));
    rightWallModel = glm::rotate(rightWallModel, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 backWallModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height/2.0f + 2.5, -length/2.0f));
    backWallModel = glm::rotate(backWallModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    TextureManager* tm = TextureManager::GetInstance();
    tm->LoadTexture2DRGBA("floor",std::string(TEXTURE_DIR) + "floor_texture.png", 0);

    BufferLayout floorLayout = {
        { ShaderDataType::Float3, "a_Position" }, // x y z
        { ShaderDataType::Float2, "a_TexCoord" }  // u v
    };

    floorVB->SetLayout(floorLayout);
    auto floorVA = std::make_shared<VertexArray>();
    floorVA->Bind();
    floorVA->AddVertexBuffer(floorVB);
    floorVA->SetIndexBuffer(floorIB);
    floorVA->Unbind();

    auto model = glm::mat4(1.0f);

    auto floorShader = std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc);
    floorShader->Bind();
    floorShader->UploadUniformMat4("model", model);
    floorShader->UploadUniformMat4("view", view);
    floorShader->UploadUniformInt("divisionsX", 5);
    floorShader->UploadUniformInt("divisionsY", 10);
    floorShader->UploadUniformBool("useTexture", state.useTexture);
    floorShader->UploadUniformFloat3("u_lightSourcePosition", state.cameraPosition);
    floorShader->UploadUniformFloat3("u_ambientColor", glm::vec3(state.ambientIllumination));
    floorShader->UploadUniformFloat3("u_diffuseColor", glm::vec3(0.5f));
    floorShader->UploadUniformFloat("boardWidth", 5.0f);
    floorShader->UploadUniformFloat("boardLength", 10.0f);
    floorShader->UploadUniformFloat("u_specularStrength", 0.5f);
    floorShader->UploadUniformMat4("proj", proj);
    floorShader->UploadUniformFloat3("u_cameraPosition", state.cameraPosition);

    // Back wall
    auto backWall = GeometricTools::UnitGridGeometry2DWTCoords(5, 5);
    auto backWallIndices = GeometricTools::UnitGridTopologyTriangles(5, 5);

    auto backWallVB = std::make_shared<VertexBuffer>(backWall.data(), static_cast<GLsizei>(backWall.size()));
    auto backWallIB = std::make_shared<IndexBuffer>(backWallIndices.data(), static_cast<GLsizei>(backWallIndices.size()));

    backWallVB->SetLayout(floorLayout);

    auto backWallVA = std::make_shared<VertexArray>();
    backWallVA->Bind();
    backWallVA->AddVertexBuffer(backWallVB);
    backWallVA->SetIndexBuffer(backWallIB);
    backWallVA->Unbind();
    //=====SCENE END=====================================================================================//

    //=====CUBE==========================================================================================//
    auto cube3d = GeometricTools::UnitCube3D24WNormals;
    auto cubeIndices = GeometricTools::UnitCube3DTopologyTriangles24;

    auto cubeVB = std::make_shared<VertexBuffer>(cube3d.data(), static_cast<GLsizei>(cube3d.size()));
    auto cubeIB = std::make_shared<IndexBuffer>(cubeIndices.data(), static_cast<GLsizei>(cubeIndices.size()));

    tm->LoadCubeMapRGBA("CubeCubeMap", std::string(TEXTURES_DIR) + "cube_texture.png", 1);

    BufferLayout cubeLayout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Normal"}
    };
    cubeVB->SetLayout(cubeLayout);

    auto cubeVA = std::make_shared<VertexArray>();
    cubeVA->Bind();
    cubeVA->AddVertexBuffer(cubeVB);
    cubeVA->SetIndexBuffer(cubeIB);
    cubeVA->Unbind();

    auto cubeShader = std::make_shared<Shader>(cubeVertexWithNormalsSrc, cubeFragmentWithNormalsSrc);
    cubeShader->Bind();
    cubeShader->UploadUniformMat4("model", model);
    cubeShader->UploadUniformMat4("view", view);
    cubeShader->UploadUniformMat4("proj", proj);
    cubeShader->UploadUniformInt("uTexture", 1);
    cubeShader->UploadUniformBool("useTexture", state.useTexture);
    cubeShader->UploadUniformFloat3("u_ambientColor", glm::vec3(state.ambientIllumination));
    cubeShader->UploadUniformFloat3("u_diffuseColor", glm::vec3(0.5f));
    cubeShader->UploadUniformFloat("u_specularStrength", 0.5f);
    cubeShader->UploadUniformFloat3("u_cameraPosition", state.cameraPosition);
    //=====CUBE END=======================================================================================//

    SpawnNewCube(state);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Run loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        RenderCommands::Clear();

        processInput(window, state, deltaTime);
        MoveCube(state, deltaTime);

        floorShader->Bind();
        floorShader->UploadUniformBool("useTexture", state.useTexture);
        floorShader->UploadUniformFloat3("u_lightSourcePosition", state.activePiece.cubes[0].position);

        floorVA->Bind();

        floorShader->UploadUniformMat4("model", floorModel);
        RenderCommands::DrawIndex(GL_TRIANGLES, floorVA);

        floorShader->UploadUniformMat4("model", ceilingModel);
        RenderCommands::DrawIndex(GL_TRIANGLES, floorVA);

        floorShader->UploadUniformMat4("model", leftWallModel);
        RenderCommands::DrawIndex(GL_TRIANGLES, floorVA);

        floorShader->UploadUniformMat4("model", rightWallModel);
        RenderCommands::DrawIndex(GL_TRIANGLES,floorVA);

        floorShader->UploadUniformMat4("model", backWallModel);
        RenderCommands::DrawIndex(GL_TRIANGLES, backWallVA);

        cubeShader->Bind();
        cubeShader->UploadUniformBool("useTexture", state.useTexture);
        cubeShader->UploadUniformFloat3("u_lightSourcePosition", state.activePiece.cubes[0].position);

        for (const auto& cube : state.cubes) {
            cube.Draw(cubeShader, cubeVA);
        }

        glDepthMask(GL_FALSE);
        for (const auto& cube : state.activePiece.cubes) {
            cube.Draw(cubeShader, cubeVA);
        }
        glDepthMask(GL_TRUE);

        // smooth movements
        for (auto& cube : state.activePiece.cubes) {
            cube.position = glm::mix(cube.position, cube.targetPosition, CUBE_MOVESPEED * deltaTime);
        }

        glfwSwapBuffers(window);

        //busy wait loop
        while ((glfwGetTime() - currentFrame) < TARGET_FRAME_TIME) {} // 60 fps
    }
    return 0;
}


void ExamApplication::SpawnRandomCube(ProgramState &state) {
    int r = rand() % 4;

    switch (r) {
        case 0:
            SpawnNewCube(state);
        break;
        case 1:
            SpawnNewCubeZ(state);
        break;
        case 2:
            SpawnNewCubeL(state);
        break;
        case 3:
            SpawnNewCubeT(state);
        break;
    }
}

void ExamApplication::PlaceCube(ProgramState &state) {
    for (auto &cube : state.activePiece.cubes) {
        cube.position = cube.targetPosition; // snap to cube

        cube.active = false;
        state.boardstate[cube.boardCoordx][cube.boardCoordy][cube.boardCoordz] = true;
        state.cubes.push_back(cube);
    }
    state.activePiece.cubes.clear();
    ExamApplication::SpawnRandomCube(state);
}

void ExamApplication::MoveCube(ProgramState &state, float deltaTime) {
    static float moveDownTimer = AUTOMOVE_TIMER;
    moveDownTimer -= deltaTime;
    if (moveDownTimer <= 0.0f) {
        bool canMove = true;
        for (auto& cube : state.activePiece.cubes) {
            if (cube.boardCoordz == 9 || state.boardstate[cube.boardCoordx][cube.boardCoordy][cube.boardCoordz + 1]) {
                canMove = false;
                break;
                }
        }
        if (!canMove) {
            ExamApplication::PlaceCube(state);
        } else {
            for (auto& cube : state.activePiece.cubes) {
                cube.boardCoordz += 1;
                cube.targetPosition.z -= 1.0f;
            }
        }
        moveDownTimer = AUTOMOVE_TIMER;
    }
}