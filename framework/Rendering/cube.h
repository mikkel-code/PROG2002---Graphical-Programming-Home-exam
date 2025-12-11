#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Shader.h"
#include "RenderCommands.h"

class Cube {
private:
    glm::vec4 GetColorPerZ(int Layer) const {
        switch(Layer) {
            case 0: {return glm::vec4(0.7f,0.7f,0.7f,1.0f);}
            case 1: {return glm::vec4(0.3f,0.05f,0.3f,1.0f);}
            case 2: {return glm::vec4(0.05f,0.5f,0.9f,1.0f);}
            case 3: {return glm::vec4(0.75f,0.4f,0.05f,1.0f);}
            case 4: {return glm::vec4(0.1f,0.5f,0.1f,1.0f);}
            case 5: {return glm::vec4(0.5f,0.1f,0.5f,1.0f);}
            case 6: {return glm::vec4(0.5f,0.5f,0.1f,1.0f);}
            case 7: {return glm::vec4(0.1f,0.5f,0.5f,1.0f);}
            case 8: {return glm::vec4(0.5f,0.1f,0.1f,1.0f);}
            case 9: {return glm::vec4(0.1f,0.1f,0.5f,1.0f);}
        }
    }
public:
    glm::vec4 color{0.0f};
    glm::vec3 position{0.0f};
    glm::vec3 targetPosition{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{0.05f};

    int boardCoordx = 0;
    int boardCoordy = 0;
    int boardCoordz = 0;

    bool active = false;

    Cube() = default;
    Cube(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scl = glm::vec3(0.05f))
        : position(pos), rotation(rot), scale(scl){}

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void Draw(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> cubeVA) const {
        shader->Bind();
        shader->UploadUniformMat4("model", GetModelMatrix());
        if (active) {
            shader->UploadUniformVec4("col", glm::vec4(0.0f,1.0f,0.0f,0.2f));
        } else {shader->UploadUniformVec4("col", GetColorPerZ(boardCoordz));}
        cubeVA->Bind();
        RenderCommands::DrawIndex(GL_TRIANGLES, cubeVA);
    }
};


#endif // CUBE_H