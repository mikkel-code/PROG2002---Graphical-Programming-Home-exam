#ifndef PROGRAMSTATE_H
#define PROGRAMSTATE_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "cube.h"
#include "Shader.h"

struct ProgramState {
    std::vector<Cube> cubes;
    bool boardstate[5][5][10]= { false };
    std::vector<Cube> activeCube;

    bool useTexture = false;
    float ambientIllumination = 0.5;

    glm::vec3 cameraPosition = glm::vec3(1.0f);
    float fov = 45.0f;
};

#endif //PROGRAMSTATE_H
