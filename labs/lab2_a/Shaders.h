#pragma once
#include <string>

const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location = 0) in vec2 position;

    out vec2 fragmentPos;

    void main()
    {
        fragmentPos = position + vec2(0.5); // Line the fragment shader up for easier use
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core

    in vec2 fragmentPos;
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
        if (ix == greenX && iy == greenY)
            color = vec4(0.0, 1.0, 0.0, 1.0); // green
        else if ((ix + iy) % 2 == 0)
            color = vec4(1.0, 1.0, 1.0, 1.0); // white
        else
            color = vec4(0.0, 0.0, 0.0, 1.0); // black
    }
)";

// Step 1, decide green tile
// Step 2, declare green tile in fragment shader
// Step 3, Modify the current shader with "if"
// Step 4, pass the coordinates for the green tile.