#ifndef SHADERS_H
#define SHADERS_H
#include <string>

const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location = 0) in vec2 position;
    layout(location = 1) in vec4 a_Color;

    out vec2 fragmentPos;
    out vec4 fragmentColor;

    void main()
    {
        fragmentPos = position + vec2(0.5); // Line the fragment shader up for easier use
        gl_Position = vec4(position, 0.0, 1.0);
        fragmentColor = a_Color; // passing forward the color
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core

    in vec2 fragmentPos;
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
        int iy = int(fragmentPos.y * divisionsY);

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
#endif