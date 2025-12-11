#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <glm/glm.hpp>

#include "cube.h"

class Piece {
public:
    std::vector<Cube> cubes;
    void Rotate(float angleRadians, const glm::vec3& axis) {

        glm::vec3 pivot(0.0f);
        for (const auto& cube : cubes) {pivot += cube.targetPosition;}
        pivot = pivot / static_cast<float>(cubes.size());

        for (auto& cube : cubes) {
            glm::vec3 translated = cube.targetPosition - pivot;
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angleRadians, axis);
            glm::vec4 rotated =  rot * glm::vec4(translated, 1.0f);

            glm::vec3 newPos = glm::vec3(rotated) + pivot;
            cube.targetPosition = newPos;
            cube.position = newPos;
            cube.rotation += axis * angleRadians;
        }
    }
};

#endif //PIECE_H
