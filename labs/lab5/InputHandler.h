#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>

#include "GLFW/glfw3.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* state = static_cast<ProgramState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}
    // Move cubes
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        // pick up the cube
        if (!state->pickUpMode) {
            for (int i = 0; i < state->cubes.size(); i++) {
                if (state->cubes[i].boardCoordx == state->coordinateX &&
                    state->cubes[i].boardCoordz == state->coordinateY) {

                    state->pickedCubeColor = state->cubes[i].color;
                    state->cubes[i].color = glm::vec4(1.0f,1.0f,0.05f,1.0f);

                    state->pickedCubeIndex = i;
                    state->pickUpMode = true;
                    std::cout << "Picked cube " << i << "\n";
                    return;
                }
            }
        }
        // Place the cube
        if (state->pickUpMode && state->pickedCubeIndex != -1) {
            constexpr float step = 1.0f / 8.0f;
            if (state->boardstate[state->coordinateX + state->coordinateY * 8] == false) {
                state->boardstate[state->coordinateX + state->coordinateY * 8] = true;

                // sets the old spot open so new cubes can take its place
                state->boardstate[state->cubes[state->pickedCubeIndex].boardCoordx + state->cubes[state->pickedCubeIndex].boardCoordz * 8] = false;

                state->cubes[state->pickedCubeIndex].boardCoordx = state->coordinateX;
                state->cubes[state->pickedCubeIndex].boardCoordz = state->coordinateY;

                state->cubes[state->pickedCubeIndex].position =
                    glm::vec3(-0.5f + step * (state->coordinateX + 0.5f),
                              0.05f,
                              -0.5f + step * (state->coordinateY + 0.5f));
                std::cout << "Placed cube " << state->pickedCubeIndex << "\n";
            } else {
                std::cout << "Can't place cube " << state->pickedCubeIndex << "\n";
            }
            state->cubes[state->pickedCubeIndex].color = state->pickedCubeColor;
            state->pickedCubeIndex = -1;
            state->pickUpMode = false;
        }
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        static int mode = 1;
        mode = (mode + 1) % 2; mode ? RenderCommands::SetSolidMode() : RenderCommands::SetWireframeMode();
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {glfwSetWindowShouldClose(window, true);}

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        static int mode = 1;
        state->useTexture = (mode + 1) % 2; // true / false
    }

    state->shader->Bind();

    state->shader->UploadUniformInt("greenX", state->coordinateX);
    state->shader->UploadUniformInt("greenY", state->coordinateY);
}

const float cooldown = 0.15f;

void processInput(GLFWwindow* window, ProgramState& state, float deltaTime) {
    static float moveTimer = 0.0f;
    moveTimer -= deltaTime;
    if (moveTimer <= 0.0f) {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)      {if (state.coordinateY < 7) {state.coordinateY +=1; moveTimer = cooldown;}}
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)    {if (state.coordinateY > 0) {state.coordinateY -=1; moveTimer = cooldown;}}
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)    {if (state.coordinateX < 7) {state.coordinateX +=1; moveTimer = cooldown;}}
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)   {if (state.coordinateX > 0) {state.coordinateX -=1; moveTimer = cooldown;}}
    }
    // zoom in
    if (glfwGetKey(window, GLFW_KEY_O)) {
        state.fov = glm::max(1.0f, state.fov - 0.5f);
        state.cameraViewProjection = glm::perspective(glm::radians(state.fov), 800.0f / 600.0f, 0.1f, 100.0f);
    }
    // zoom out
    if (glfwGetKey(window, GLFW_KEY_P)) {
        state.fov = glm::min(45.0f, state.fov + 0.5f);
        state.cameraViewProjection = glm::perspective(glm::radians(state.fov), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    // rotate left
    if (glfwGetKey(window, GLFW_KEY_H)) {
        state.cameraAngle+=glm::radians(3.0f);
        state.cameraPosition.x = state.cameraRadius * cos(state.cameraAngle);
        state.cameraPosition.z = state.cameraRadius * sin(state.cameraAngle);
    }
    // rotate right
    if (glfwGetKey(window, GLFW_KEY_L)) {
        state.cameraAngle -=glm::radians(3.0f);
        state.cameraPosition.x = state.cameraRadius * cos(state.cameraAngle);
        state.cameraPosition.z = state.cameraRadius * sin(state.cameraAngle);
    }

    state.shader->Bind();
    state.shader->UploadUniformInt("greenX", state.coordinateX);
    state.shader->UploadUniformInt("greenY", state.coordinateY);
}
#endif //INPUTHANDLER_H