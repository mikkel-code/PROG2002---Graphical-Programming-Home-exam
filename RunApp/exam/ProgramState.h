#ifndef PROGRAMSTATE_H
#define PROGRAMSTATE_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "cube.h"
#include "Shader.h"

struct ProgramState {
    //std::shared_ptr<Shader> shader;

    std::vector<Cube> cubes;
    bool boardstate[5][5][10]= { false };
    std::vector<Cube> activeCube;

    bool useTexture = false;
    float ambientIllumination = 0.5;

    // camera
    float cameraAngle = 0.0f;
    float cameraRadius = 3.0f;
    glm::mat4 cameraViewProjection = glm::mat4(1.0f);

    glm::vec3 cameraPosition = glm::vec3(
        cameraRadius * cos(cameraAngle),
        1.0f,
        cameraRadius * sin(cameraAngle)
    );
    float fov = 45.0f;
};

#endif //PROGRAMSTATE_H
