#ifndef GEOMETRICTOOLS_H
#define GEOMETRICTOOLS_H
#include <array>
#include <vector>

namespace GeometricTools {
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

    std::vector<float> UnitGridGeometry2DWTCoords(unsigned int X, unsigned int Y) {
        std::vector<float> vertices;
        vertices.reserve((X + 1) * (Y + 1) * 5);

        float width = float(X);
        float length = float(Y);

        for (unsigned int j = 0; j <= Y; ++j) {
            float v = float(j) / Y;
            float z = v * length - length / 2.0f;

            for (unsigned int i = 0; i <= X; ++i) {
                float u = float(i) / X;
                float x = u * width - width / 2.0f;

                vertices.push_back(x);
                vertices.push_back(0.0f);
                vertices.push_back(z);
                vertices.push_back(u);
                vertices.push_back(v);
            }
        }
        return vertices;
    }

    static std::vector<float> UnitGrid3D(int divisionsX, int divisionsY) {
        std::vector<float> vertices;

        float stepX = 1.0f / divisionsX; // how far each should move.
        float stepY = 1.0f / divisionsY;

        for (int y = 0; y <= divisionsY; ++y) {
            float fy = -0.5f + y * stepY;  // centered at origin
            for (int x = 0; x <= divisionsX; ++x) {
                float fx = -0.5f + x * stepX;  // centered at origin
                vertices.push_back(fx);  // x
                vertices.push_back(0.0f);  // z
                vertices.push_back(fy);  // y
                vertices.push_back(1.0f); // r
                vertices.push_back(1.0f); // g
                vertices.push_back(1.0f); // b
                vertices.push_back(1.0f); // a

            }
        }
        return vertices;
    }

    constexpr std::array<float, 3 * 24 * 2> UnitCube3D24WNormals = {

        -0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,
         0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,
         0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,
        -0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,

         0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,
        -0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,
        -0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,
         0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,

        -0.5f, -0.5f, -0.5f,  -1.f, 0.f, 0.f,
        -0.5f, -0.5f,  0.5f,  -1.f, 0.f, 0.f,
        -0.5f,  0.5f,  0.5f,  -1.f, 0.f, 0.f,
        -0.5f,  0.5f, -0.5f,  -1.f, 0.f, 0.f,

         0.5f, -0.5f,  0.5f,   1.f, 0.f, 0.f,
         0.5f, -0.5f, -0.5f,   1.f, 0.f, 0.f,
         0.5f,  0.5f, -0.5f,   1.f, 0.f, 0.f,
         0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,

        -0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,
         0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,
         0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,
        -0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,

        -0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,
         0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,
         0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,
        -0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,
     };

    constexpr std::array<unsigned int, 6 * 3 * 2> UnitCube3DTopologyTriangles24 = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
        };
}

#endif