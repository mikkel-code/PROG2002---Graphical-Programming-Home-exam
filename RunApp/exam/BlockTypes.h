#ifndef BLOCKTYPES_H
#define BLOCKTYPES_H
#include "ProgramState.h"

static void SpawnNewCube(ProgramState &state) {
    if (!state.activeCube.empty()) {
        state.activeCube.pop_back();
    }
    Cube c;
    c.position = glm::vec3(-2.0f, -0.5f, 4.5f);
    c.targetPosition = c.position;
    c.scale = glm::vec3(1.0f);
    c.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c.active = true;
    c.boardCoordx = 0; c.boardCoordy = 0; c.boardCoordz = 0;
    state.activeCube.push_back(c);
    Piece piece;
    piece.cubes.push_back(c);
    state.activePiece = piece;
}

static void SpawnNewCubeZ(ProgramState &state) {
    if (!state.activeCube.empty()) {
        state.activeCube.pop_back();
    }
    Cube c;
    c.position = glm::vec3(-2.0f+1.0f, -0.5f, 4.5f);
    c.targetPosition = c.position;
    c.scale = glm::vec3(1.0f);
    c.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c.active = true;
    c.boardCoordx = 1; c.boardCoordy = 0; c.boardCoordz = 0;
    state.activeCube.push_back(c);

    Cube c2;
    c2.position = glm::vec3(-2.0f, -0.5f, 4.5f);
    c2.targetPosition = c2.position;
    c2.scale = glm::vec3(1.0f);
    c2.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c2.active = true;
    c2.boardCoordx = 0; c2.boardCoordy = 0; c2.boardCoordz = 0;
    state.activeCube.push_back(c2);

    Cube c3;
    c3.position = glm::vec3(-2.0f+1.0f, -0.5f+1.0f, 4.5f);
    c3.targetPosition = c3.position;
    c3.scale = glm::vec3(1.0f);
    c3.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c3.active = true;
    c3.boardCoordx = 1; c3.boardCoordy = 1; c3.boardCoordz = 0;
    state.activeCube.push_back(c3);

    Cube c4;
    c4.position = glm::vec3(-2.0f+2.0f, -0.5f+1.0f, 4.5f);
    c4.targetPosition = c4.position;
    c4.scale = glm::vec3(1.0f);
    c4.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c4.active = true;
    c4.boardCoordx = 2; c4.boardCoordy = 1; c4.boardCoordz = 0;
    state.activeCube.push_back(c4);

    Piece piece;
    piece.cubes.push_back(c);
    piece.cubes.push_back(c2);
    piece.cubes.push_back(c3);
    piece.cubes.push_back(c4);
    state.activePiece = piece;;
}

static void SpawnNewCubeL(ProgramState &state) {
    if (!state.activeCube.empty()) {
        state.activeCube.pop_back();
    }

    Cube c;
    c.position = glm::vec3(-2.0f+1.0f, -0.5f+1.0f, 4.5f);
    c.targetPosition = c.position;
    c.scale = glm::vec3(1.0f);
    c.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c.active = true;
    c.boardCoordx = 1; c.boardCoordy = 1; c.boardCoordz = 0;
    state.activeCube.push_back(c);

    Cube c2;
    c2.position = glm::vec3(-2.0f+1.0f, -0.5f, 4.5f);
    c2.targetPosition = c2.position;
    c2.scale = glm::vec3(1.0f);
    c2.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c2.active = true;
    c2.boardCoordx = 1; c2.boardCoordy = 0; c2.boardCoordz = 0;
    state.activeCube.push_back(c2);

    Cube c3;
    c3.position = glm::vec3(-2.0f, -0.5f, 4.5f);
    c3.targetPosition = c3.position;
    c3.scale = glm::vec3(1.0f);
    c3.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c3.active = true;
    c3.boardCoordx = 0; c3.boardCoordy = 0; c3.boardCoordz = 0;
    state.activeCube.push_back(c3);

    Cube c4;
    c4.position = glm::vec3(-2.0f+1.0f, -0.5f+2.0f, 4.5f);
    c4.targetPosition = c4.position;
    c4.scale = glm::vec3(1.0f);
    c4.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c4.active = true;
    c4.boardCoordx = 1; c4.boardCoordy = 2; c4.boardCoordz = 0;
    state.activeCube.push_back(c4);

    Piece piece;
    piece.cubes.push_back(c);
    piece.cubes.push_back(c2);
    piece.cubes.push_back(c3);
    piece.cubes.push_back(c4);
    state.activePiece = piece;;
}

static void SpawnNewCubeT(ProgramState &state) {
    if (!state.activeCube.empty()) {
        state.activeCube.pop_back();
    }

    Cube c;
    c.position = glm::vec3(-2.0f+1.0f, -0.5f+1.0f, 4.5f);
    c.targetPosition = c.position;
    c.scale = glm::vec3(1.0f);
    c.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c.active = true;
    c.boardCoordx = 1; c.boardCoordy = 1; c.boardCoordz = 0;
    state.activeCube.push_back(c);

    Cube c2;
    c2.position = glm::vec3(-2.0f+1.0f, -0.5f, 4.5f);
    c2.targetPosition = c2.position;
    c2.scale = glm::vec3(1.0f);
    c2.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c2.active = true;
    c2.boardCoordx = 1; c2.boardCoordy = 0; c2.boardCoordz = 0;
    state.activeCube.push_back(c2);

    Cube c3;
    c3.position = glm::vec3(-2.0f, -0.5f+2.0f, 4.5f);
    c3.targetPosition = c3.position;
    c3.scale = glm::vec3(1.0f);
    c3.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c3.active = true;
    c3.boardCoordx = 0; c3.boardCoordy = 2; c3.boardCoordz = 0;
    state.activeCube.push_back(c3);

    Cube c4;
    c4.position = glm::vec3(-2.0f+1.0f, -0.5f+2.0f, 4.5f);
    c4.targetPosition = c4.position;
    c4.scale = glm::vec3(1.0f);
    c4.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c4.active = true;
    c4.boardCoordx = 1; c4.boardCoordy = 2; c4.boardCoordz = 0;
    state.activeCube.push_back(c4);

    Cube c5;
    c5.position = glm::vec3(-2.0f+2.0f, -0.5f+2.0f, 4.5f);
    c5.targetPosition = c5.position;
    c5.scale = glm::vec3(1.0f);
    c5.color = glm::vec4(0.1f,0.1f,0.5f,1.0f);
    c5.active = true;
    c5.boardCoordx = 2; c5.boardCoordy = 2; c5.boardCoordz = 0;
    state.activeCube.push_back(c5);

    Piece piece;
    piece.cubes.push_back(c);
    piece.cubes.push_back(c2);
    piece.cubes.push_back(c3);
    piece.cubes.push_back(c4);
    piece.cubes.push_back(c5);
    state.activePiece = piece;
}


#endif //BLOCKTYPES_H