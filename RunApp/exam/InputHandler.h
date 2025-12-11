#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>

#include "GLFW/glfw3.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* state = static_cast<ProgramState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}
    // Move cubes

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        static int solidMode = 1;
        solidMode = (solidMode + 1) % 2; solidMode ? RenderCommands::SetSolidMode() : RenderCommands::SetWireframeMode();
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {glfwSetWindowShouldClose(window, true);}

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        state->useTexture = (state->useTexture + 1) % 2; // true / false
    }
    //state->shader->Bind();
}

const float cooldown = 0.2f;

void processInput(GLFWwindow* window, ProgramState& state, float deltaTime) {
    static float moveTimer = 0.0f;
    moveTimer -= deltaTime;

    if (moveTimer <= 0.0f) {
        bool canMove = true;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            bool canMove = true;
            for (auto& cube : state.activeCube) {
                if (cube.boardCoordy >= 4 || state.boardstate[cube.boardCoordx][cube.boardCoordy + 1][cube.boardCoordz]) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& cube : state.activeCube) {
                    cube.boardCoordy += 1;
                    cube.position.y += 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            bool canMove = true;
            for (auto& cube : state.activeCube) {
                if (cube.boardCoordy <= 0 || state.boardstate[cube.boardCoordx][cube.boardCoordy - 1][cube.boardCoordz]) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& cube : state.activeCube) {
                    cube.boardCoordy -= 1;
                    cube.position.y -= 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            bool canMove = true;
            for (auto& cube : state.activeCube) {
                if (cube.boardCoordx <= 0 || state.boardstate[cube.boardCoordx - 1][cube.boardCoordy][cube.boardCoordz]) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& cube : state.activeCube) {
                    cube.boardCoordx -= 1;
                    cube.position.x -= 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            bool canMove = true;
            for (auto& cube : state.activeCube) {
                if (cube.boardCoordx >= 4 || state.boardstate[cube.boardCoordx + 1][cube.boardCoordy][cube.boardCoordz]) {
                    canMove = false;
                }
            }
            if (canMove) {
                for (auto& cube : state.activeCube) {
                    cube.boardCoordx += 1;
                    cube.position.x += 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            bool canMove = true;
            for (auto& cube : state.activeCube) {
                if (cube.boardCoordz == 9 || state.boardstate[cube.boardCoordx][cube.boardCoordy][cube.boardCoordz + 1]) {
                    canMove = false;
                    break;
                }
            }
            if (!canMove) {
                ExamApplication::PlaceCube(state);
                moveTimer = cooldown;
            } else {
                for (auto& cube : state.activeCube) {
                    cube.boardCoordz += 1;
                    cube.position.z -= 1.0f;
                    moveTimer = cooldown;
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            int j = 11;
            for (auto& cube : state.activeCube) {
                int drop = 0;
                for (int i = cube.boardCoordz; i < 10; i++) {
                    if (i == 9 ||
                        state.boardstate[cube.boardCoordx][cube.boardCoordy][i + 1]) {
                        drop = i - cube.boardCoordz;
                        break;
                    }
                }
                if (drop < j) {
                    j = drop;
                }
            }
            for (auto& cube : state.activeCube) {
                cube.boardCoordz += j;
                cube.position.z -= static_cast<float>(j);
                moveTimer = cooldown;
            }
            ExamApplication::PlaceCube(state);
        }
    }

    // zoom in
    if (glfwGetKey(window, GLFW_KEY_O)) {
        state.fov = glm::max(1.0f, state.fov - 0.5f);
        state.cameraViewProjection = glm::perspective(glm::radians(state.fov), 1024.0f / 768.0f, 0.1f, 100.0f);
    }
    // zoom out
    if (glfwGetKey(window, GLFW_KEY_P)) {
        state.fov = glm::min(240.0f, state.fov + 0.5f);
        state.cameraViewProjection = glm::perspective(glm::radians(state.fov), 1024.0f / 768.0f, 0.1f, 100.0f);
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
    //state.shader->Bind();
}
#endif //INPUTHANDLER_H