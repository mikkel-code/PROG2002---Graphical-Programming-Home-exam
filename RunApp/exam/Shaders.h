#ifndef SHADERS_H
#define SHADERS_H
#include <string>

const std::string vertexShaderSrc = R"(
    #version 430 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec2 a_TexCoord;

    out vec3 fragmentPos;
    out vec3 localPos;
    out vec2 TexCoord;
    out vec3 vs_normal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main()
    {
        localPos = a_Position;

        vec4 worldPos = model * vec4(a_Position, 1.0);
        fragmentPos = worldPos.xyz;

        vec3 normalModel = vec3(0.0, 1.0, 0.0);
        vs_normal = normalize(mat3(model) * normalModel);

        TexCoord = a_TexCoord;

        gl_Position = proj * view * worldPos;
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core

    in vec3 fragmentPos;
    in vec3 localPos;
    in vec2 TexCoord;
    in vec4 fragmentColor;
    in vec3 vs_normal;

    out vec4 color;

    uniform int divisionsX = 5;
    uniform int divisionsY = 10;
    uniform float boardWidth;
    uniform float boardLength;

    uniform sampler2D u_floorTextureSampler;
    uniform bool useTexture = true;

    uniform vec3 u_lightSourcePosition;
    uniform vec3 u_ambientColor;
    uniform vec3 u_diffuseColor;
    uniform float u_ambientStrength = 1.0;

    uniform vec3 u_cameraPosition;
    uniform float u_specularStrength;

    const float lineThickness = 0.02;

    void main() {
        float normalizedTileX = (localPos.x + boardWidth * 0.5) / boardWidth;
        float normalizedTileY = (localPos.z + boardLength * 0.5) / boardLength;

        float fx = fract(normalizedTileX * float(divisionsX));
        float fy = fract(normalizedTileY * float(divisionsY));

        // line if line is near tile boundray
        bool nearLeftX  = fx < lineThickness;
        bool nearRightX = fx > 1.0 - lineThickness;

        bool nearBottomY = fy < lineThickness;
        bool nearTopY    = fy > 1.0 - lineThickness;

        float gridMask = (nearLeftX || nearRightX || nearBottomY || nearTopY) ? 1.0 : 0.0;

        //float gridMask = clamp(lineX + lineY, 0.0, 1.0);

        vec3 texRGB = useTexture ? texture(u_floorTextureSampler, TexCoord).rgb : vec3(0.1);

        vec3 finalSurfaceColor = mix(texRGB, vec3(0.0,0.5,0.0), gridMask);

        // lighting
        vec3 N = normalize(vs_normal);
        vec3 L = normalize(u_lightSourcePosition - fragmentPos);

        vec3 V = normalize(u_cameraPosition - fragmentPos);
        vec3 H = normalize(L + V);

        float diffCoef = max(dot(N, L), 0.0);
        vec3 diffuse = u_diffuseColor * finalSurfaceColor * diffCoef;

        float specFactor = pow(max(dot(N, H), 0.0), 64.0);
        vec3 specular = vec3(u_specularStrength) * specFactor;

        vec3 ambient = u_ambientColor * finalSurfaceColor * u_ambientStrength;

        color = vec4(ambient + diffuse + specular, 1.0);
    }
)";

const std::string cubeVertexWithNormalsSrc = R"(
    #version 410 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec3 a_Normal;

    out vec4 fragmentColor;
    out vec3 vs_position;     // World position
    out vec3 vs_localPosition; // Local position (for cubemap sampling!)
    out vec3 vs_normal;

    uniform mat4 model = mat4(1.0);
    uniform mat4 view = mat4(1.0);
    uniform mat4 proj = mat4(1.0);
    uniform vec4 col;

    void main() {
        vec4 worldPos = model * vec4(a_Position, 1.0);

        vs_position = worldPos.xyz;
        vs_localPosition = a_Position;
        vs_normal = normalize(mat3(model) * a_Normal);

        fragmentColor = col;
        gl_Position = proj * view * worldPos;
    }
)";

const std::string cubeFragmentWithNormalsSrc = R"(
    #version 410 core

    in vec4 fragmentColor;
    in vec3 vs_position;
    in vec3 vs_localPosition; // to sample cubemap
    in vec3 vs_normal;

    out vec4 color;

    uniform samplerCube uTexture;
    uniform bool useTexture = true;

    uniform vec3 u_lightSourcePosition;
    uniform vec3 u_ambientColor;
    uniform vec3 u_diffuseColor;

    uniform vec3 u_cameraPosition;
    uniform float u_specularStrength;

    void main() {
        vec3 finalColor;

        if (useTexture) {
            vec3 sampleDir = normalize(vs_localPosition);
            finalColor = mix(texture(uTexture, sampleDir).rgb, fragmentColor.rgb, 0.5f);
        } else { finalColor = fragmentColor.rgb; }
        vec3 N = normalize(vs_normal);
        vec3 L = normalize(u_lightSourcePosition - vs_position);
        vec3 V = normalize(u_cameraPosition - vs_position);
        vec3 H = normalize(L + V);

        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = u_diffuseColor * diff;

        float specFactor = pow(max(dot(N, H), 0.0), 64.0);
        vec3 specular = vec3(u_specularStrength) * specFactor;
        vec3 ambient = u_ambientColor;
        finalColor = finalColor * (ambient + diffuse) + specular;

        color = vec4(finalColor, fragmentColor.a);
    }
)";
#endif