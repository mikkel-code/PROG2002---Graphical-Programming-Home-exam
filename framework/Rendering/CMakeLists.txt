# Create the library "Rendering" with its sources
add_library(Rendering
        VertexBuffer.cpp
        IndexBuffer.cpp
        ShadersDataTypes.h
        BufferLayout.h
        VertexArray.h
        VertexArray.cpp
        Shader.cpp
        RenderCommands.h
        Camera.h
        PerspectiveCamera.h
        OrthographicCamera.h
        PerspectiveCamera.cpp
        OrthographicCamera.cpp
)

# Optional: create an alias for convenience
add_library(Framework::Rendering ALIAS Rendering)

# Include directories for the library
target_include_directories(Rendering
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries this library depends on
target_link_libraries(Rendering
        PUBLIC glad glfw glm
)