#pragma once
#include <string>

const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location=0) in vec3 position;
    layout(location=1) in vec4 aColor;

    out vec3 fragmentPos;
    out vec4 fragmentColor;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main() {
        fragmentPos = position + 0.5;
        fragmentColor = aColor;    // pass vertex color
        gl_Position = proj * view * model * vec4(position, 1.0);
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core

    in vec3 fragmentPos;
    in vec4 fragmentColor;
    out vec4 color;

    // These have to be corresponding with the size of the board
    uniform int divisionsX = 8;
    uniform int divisionsY = 8;

    // For making green
    uniform int greenX = 0;
    uniform int greenY = 0;

    void main()
    {
        int ix = int(fragmentPos.x * divisionsX);
        int iy = int(fragmentPos.z * divisionsY);

        vec4 baseColor;
        if (ix == greenX && iy == greenY)
            baseColor  = vec4(0.0, 1.0, 0.0, 1.0); // green
        else if ((ix + iy) % 2 == 0)
            baseColor = vec4(1.0, 1.0, 1.0, 1.0); // white
        else
            baseColor = vec4(0.0, 0.0, 0.0, 1.0); // black
        color = baseColor * fragmentColor;
    }
)";

const std::string cubeVertexShaderSrc = R"(
    #version 410 core
    layout(location = 0) in vec3 a_Position;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main() {
        gl_Position = proj * view * model * vec4(a_Position, 1.0);
    }
)";

const std::string cubeFragmentShaderSrc = R"(
    #version 410 core
    out vec4 fragmentColor;
    void main() {
        fragmentColor = vec4(0.0, 1.0, 0.0, 1.0); // green cube
    }
)";