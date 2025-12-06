#ifndef SHADERS_H
#define SHADERS_H
#include <string>

// Chessboard
const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec2 aTex;
    layout(location = 2) in vec4 aColor;

    out vec3 fragmentPos;
    out vec2 TexCoord;
    out vec4 fragmentColor;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main() {
        fragmentPos = position + 0.5;
        fragmentColor = aColor;    // pass vertex color
        gl_Position = proj * view * model * vec4(position, 1.0);

        TexCoord = aTex;
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core

    in vec3 fragmentPos;
    in vec2 TexCoord;
    in vec4 fragmentColor;
    out vec4 color;

    // These have to be corresponding with the size of the board
    uniform int divisionsX = 8;
    uniform int divisionsY = 8;

    // For making green
    uniform int greenX = 0;
    uniform int greenY = 0;
    uniform sampler2D u_floorTextureSampler;
    uniform bool useTexture = true;

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

        vec4 texColor = mix(baseColor, texture(u_floorTextureSampler, TexCoord), useTexture ? 1.0 : 0.0);
        color = texColor;
    }
)";


// Cube

const std::string cubeVertexShaderSrc = R"(
    #version 410 core
    layout(location = 0) in vec3 a_Position;

    out vec4 fragmentColor;
    out vec3 vs_position;

    uniform mat4 model = mat4(1.0);
    uniform mat4 view = mat4(1.0);
    uniform mat4 proj = mat4(1.0);
    uniform vec4 col;

    void main() {
        gl_Position = proj * view * model * vec4(a_Position, 1.0);
        fragmentColor = col;
        vs_position = a_Position;
    }
)";

const std::string cubeFragmentShaderSrc = R"(
    #version 410 core

    uniform samplerCube uTexture;
    uniform bool useTexture = true;

    in vec4 fragmentColor;
    in vec3 vs_position;

    out vec4 color;

    void main() {
        color = mix(fragmentColor,
                texture(uTexture, vs_position),
                useTexture ? 1.0 : 0.0);
    }
)";

#endif