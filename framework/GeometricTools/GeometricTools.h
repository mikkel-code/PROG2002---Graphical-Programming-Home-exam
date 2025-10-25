#pragma once
#include <array>
#include <vector>

namespace GeometricTools {
    // A 2D unit triangle centered at the origin
    constexpr std::array<float, 3 * 2> UnitTriangle2D = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    // A 2D unit triangle centered at the origin
    constexpr std::array<float, 6 * 2> UnitSquare2DTriangles = {
        -0.5f, -0.5f, // bottom-left
         0.5f, -0.5f, // bottom-right
         0.5f,  0.5f, // top-right

         0.5f,  0.5f, // top-right
        -0.5f,  0.5f, // top-left
        -0.5f, -0.5f  // bottom-left
    };

    // A 2D Unit square grid centered at the origin
    // Is a vector so it will run at compile time. Same as constexpr
    static std::vector<float> UnitGrid2D(int divisionsX, int divisionsY) {
        std::vector<float> vertices;

        float stepX = 1.0f / divisionsX; // how far each should move.
        float stepY = 1.0f / divisionsY;

        for (int y = 0; y <= divisionsY; ++y) {
            float fy = -0.5f + y * stepY;  // centered at origin
            for (int x = 0; x <= divisionsX; ++x) {
                float fx = -0.5f + x * stepX;  // centered at origin
                vertices.push_back(fx);
                vertices.push_back(fy);
                vertices.push_back(1.0f); // r
                vertices.push_back(1.0f); // g
                vertices.push_back(1.0f); // b
                vertices.push_back(1.0f); // a

            }
        }
        return vertices;
    }

    // The point of find each square of my UnitGrid2D and store its value into the EBO. The EBO will then decide
    // which are the useful ones and not the ones overlapping.

    static std::vector<unsigned int> UnitGridTopologyTriangles(int divisionsX, int divisionsY) {
        std::vector<unsigned int> indices;

        for (int y = 0; y < divisionsY; ++y) {
            for (int x = 0; x < divisionsX; ++x) {

                // each square has 4 vertices
                int i0 = y * (divisionsX + 1) + x;
                int i1 = i0 + 1;
                int i2 = i0 + (divisionsX + 1);
                int i3 = i2 + 1;

                indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
                indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
            }
        }
    return indices;
    }
}