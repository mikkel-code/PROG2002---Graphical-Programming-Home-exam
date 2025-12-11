#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>

#include "GLFW/glfw3.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* state = static_cast<ProgramState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}
    // Move cubes

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        static int solidMode = 1;
        solidMode = (solidMode + 1) % 2; solidMode ? RenderCommands::SetSolidMode() : RenderCommands::SetWireframeMode();
    }
    //if (key == GLFW_KEY_Q && action == GLFW_PRESS) {glfwSetWindowShouldClose(window, true);}

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        state->useTexture = (state->useTexture + 1) % 2; // true / false
    }



    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        state->activePiece.Rotate(glm::radians(90.0f), glm::vec3(1,0,0));
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        state->activePiece.Rotate(glm::radians(90.0f), glm::vec3(0,1,0));
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        state->activePiece.Rotate(glm::radians(90.0f), glm::vec3(0,0,1));
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        state->activePiece.Rotate(glm::radians(90.0f), glm::vec3(-1,0,0));
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        state->activePiece.Rotate(glm::radians(90.0f), glm::vec3(0,-1,0));
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        state->activePiece.Rotate(glm::radians(90.0f), glm::vec3(0,0,1));
    }
}

const float cooldown = 0.2f;

void processInput(GLFWwindow* window, ProgramState& state, float deltaTime) {
    static float moveTimer = 0.0f;
    moveTimer -= deltaTime;

    if (moveTimer <= 0.0f) {
        bool canMove = true;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            bool canMove = true;
            for (const auto& cube : state.activePiece.cubes) {
                if (cube.boardCoordy >= 4 || state.boardstate[cube.boardCoordx][cube.boardCoordy + 1][cube.boardCoordz]) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& cube : state.activePiece.cubes) {
                    cube.boardCoordy += 1;
                    cube.targetPosition.y += 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            bool canMove = true;
            for (const auto& cube : state.activePiece.cubes) {
                if (cube.boardCoordy <= 0 || state.boardstate[cube.boardCoordx][cube.boardCoordy - 1][cube.boardCoordz]) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& cube : state.activePiece.cubes) {
                    cube.boardCoordy -= 1;
                    cube.targetPosition.y -= 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            bool canMove = true;
            for (const auto& cube : state.activePiece.cubes) {
                if (cube.boardCoordx <= 0 || state.boardstate[cube.boardCoordx - 1][cube.boardCoordy][cube.boardCoordz]) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& cube : state.activePiece.cubes) {
                    cube.boardCoordx -= 1;
                    cube.targetPosition.x -= 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            bool canMove = true;
            for (const auto& cube : state.activePiece.cubes) {
                if (cube.boardCoordx >= 4 || state.boardstate[cube.boardCoordx + 1][cube.boardCoordy][cube.boardCoordz]) {
                    canMove = false;
                }
            }
            if (canMove) {
                for (auto& cube : state.activePiece.cubes) {
                    cube.boardCoordx += 1;
                    cube.targetPosition.x += 1.0f;
                }
                moveTimer = cooldown;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            bool canMove = true;
            for (const auto& cube : state.activePiece.cubes) {
                if (cube.boardCoordz == 9 || state.boardstate[cube.boardCoordx][cube.boardCoordy][cube.boardCoordz + 1]) {
                    canMove = false;
                    break;
                }
            }
            if (!canMove) {
                ExamApplication::PlaceCube(state);
                moveTimer = cooldown;
            } else {
                for (auto& cube : state.activePiece.cubes) {
                    cube.boardCoordz += 1;
                    cube.targetPosition.z -= 1.0f;
                    moveTimer = cooldown;
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            int j = 11;
            for (const auto& cube : state.activePiece.cubes) {
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
            for (auto& cube : state.activePiece.cubes) {
                cube.boardCoordz += j;
                cube.targetPosition.z -= static_cast<float>(j);
                moveTimer = cooldown;
            }
            ExamApplication::PlaceCube(state);
        }
    }
}
#endif //INPUTHANDLER_H