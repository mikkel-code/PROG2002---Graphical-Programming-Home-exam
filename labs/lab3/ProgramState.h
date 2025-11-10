#ifndef PROGRAMSTATE_H
#define PROGRAMSTATE_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Cube.h"
#include "Shader.h"

struct ProgramState {
    std::shared_ptr<Shader> shader;
    // green tile
    int coordinateX = 0;
    int coordinateY = 0;

    // pick up and cubes
    bool pickUpMode = false;
    int pickedCubeIndex = -1;
    glm::vec4 pickedCubeColor;
    std::vector<Cube> cubes;
    bool boardstate[64] = { false };

    // camera
    float cameraAngle = 0.0f;
    float cameraRadius = 3.0f;
    glm::mat4 cameraViewProjection = glm::mat4(1.0f);

    glm::vec3 cameraPosition = glm::vec3(
        cameraRadius * cos(cameraAngle),
        2.0f,
        cameraRadius * sin(cameraAngle)
    );
    float fov = 45.0f;
};

#endif //PROGRAMSTATE_H
