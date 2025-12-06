#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Shader.h"
#include "RenderCommands.h"

class Cube {
public:
    glm::vec4 color{0.0f};
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{0.05f};
    int boardCoordx;
    int boardCoordz;

    Cube() = default;
    Cube(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scl = glm::vec3(0.05f))//, const int xCoord, const int zCoord)
        : position(pos), rotation(rot), scale(scl) {} // , boardCoordx(xCoord), boardCoordz(zCoord)

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void Draw(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> cubeVA, int greenX, int greenZ) const {
        shader->Bind();
        shader->UploadUniformMat4("model", GetModelMatrix());
        if (greenX == boardCoordx && greenZ == boardCoordz) {
            shader->UploadUniformVec4("col", glm::vec4(0.0f,1.0f,0.0f,1.0f));
        } else {shader->UploadUniformVec4("col", color);}
        cubeVA->Bind();
        RenderCommands::DrawIndex(GL_TRIANGLES, cubeVA);
    }
};

#endif // CUBE_H