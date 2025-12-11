#ifndef LAB3APPLICATION_H
#define LAB3APPLICATION_H
#include "GLFWApplication.h"
#include <string>
#include <glad/glad.h>

#include "ProgramState.h"

class ExamApplication : public GLFWApplication {
public:
    GLuint LoadCubeMap(std::string filepath, GLuint slot) const;

    GLuint LoadTexture(const std::string &filepath, GLuint slot) const;

    ExamApplication(const std::string& name, const std::string& version);

    unsigned Run() const override;  // render loop

    static void SpawnRandomCube(ProgramState &state);

    static void PlaceCube(ProgramState &state);

    static void MoveCube(ProgramState &state, float deltaTime);
};
#endif