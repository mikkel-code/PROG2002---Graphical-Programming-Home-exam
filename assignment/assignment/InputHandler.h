#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>

#include "GLFW/glfw3.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    auto* state = static_cast<ProgramState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)     if (state->coordinateY < 7) state->coordinateY +=1;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)   if (state->coordinateY > 0) state->coordinateY -=1;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)  if (state->coordinateX < 7) state->coordinateX +=1;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)   if (state->coordinateX > 0) state->coordinateX -=1;

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

    if (key == GLFW_KEY_H) {
        state->cameraAngle+=glm::radians(3.0f);
        state->cameraPosition.x = state->cameraRadius * cos(state->cameraAngle);
        state->cameraPosition.z = state->cameraRadius * sin(state->cameraAngle);
    }

    if (key == GLFW_KEY_L) {
        state->cameraAngle -=glm::radians(3.0f);
        state->cameraPosition.x = state->cameraRadius * cos(state->cameraAngle);
        state->cameraPosition.z = state->cameraRadius * sin(state->cameraAngle);
    }
    
    if (key == GLFW_KEY_O) {
        state->fov = glm::max(1.0f, state->fov - 2.0f);
        state->cameraViewProjection = glm::perspective(glm::radians(state->fov), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    if (key == GLFW_KEY_P) {
        state->fov = glm::min(45.0f, state->fov + 2.0f);
        state->cameraViewProjection = glm::perspective(glm::radians(state->fov), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        static int mode = 1;
        mode = (mode + 1) % 2; mode ? RenderCommands::SetSolidMode() : RenderCommands::SetWireframeMode();
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {glfwSetWindowShouldClose(window, true);}

    state->shader->Bind();
    state->shader->UploadUniformInt("greenX", state->coordinateX);
    state->shader->UploadUniformInt("greenY", state->coordinateY);
}
#endif //INPUTHANDLER_H