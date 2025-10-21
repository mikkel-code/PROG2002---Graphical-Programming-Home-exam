#include "Lab2Application.h"
#include "GeometricTools.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct coordinateState {
    GLuint program;
    int coordinateX = 0;
    int coordinateY = 0;
};

Lab2Application::Lab2Application(const std::string& name, const std::string& version)
    : GLFWApplication(name, version) {
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned Lab2Application::Run() const {
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) return -1;

    // Assign call back to glfw
    glfwSetKeyCallback(window, keyCallback);

    coordinateState state;

    // change the viewport so it becomes a square and not stretched
    glViewport(100, 100, 500, 500);

    GLuint vao; //, ebo; //, vbo;

    glGenVertexArrays(1, &vao);
    //glGenBuffers(1, &vbo);
    //glGenBuffers(1, &ebo);

    auto shape = GeometricTools::UnitGrid2D(8,8);
    auto indices  = GeometricTools::UnitGridTopologyTriangles(8,8);

    VertexBuffer vb(shape.data(), static_cast<GLsizei>(shape.size())); //* sizeof(float)));
    IndexBuffer ibo(indices.data(),static_cast<GLsizei>(indices.size() * sizeof(unsigned int))); //what to put here?);
    // Binds VAO and VBO
    // Bind vertex buffer
    vb.Bind();
    glBindVertexArray(vao);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(float), shape.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glEnableVertexAttribArray(0);
    // bind EBO
    ibo.Bind();
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Compile shaders from Shaders.h
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vss = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vss, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fss = fragmentShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fss, nullptr);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Shaders are no longer needed after linking so they can be deleted
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    state.program = program;
    glfwSetWindowUserPointer(window, &state);

    glUseProgram(program);
    // === Render loop ===
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        // for vertices
        //glDrawArrays(GL_TRIANGLES, 0, shape.size() / 2);
        // Draw using Draw elements for indices. for indices instead of vertices.
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    // clean up after rendering but not currently needed in lab2
    glDeleteProgram(program);
    //glDeleteBuffers(1, &vbo);
    //glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return; // So the coordinates doesnt skip one

    // Get back coordinateState pointer
    auto* state = static_cast<coordinateState*>(glfwGetWindowUserPointer(window));
    if (!state) {std::cout << "Something wrong with UserPointer in key callbacks"; return;}

    if (key == GLFW_KEY_UP) state->coordinateY+=1;
    if (key == GLFW_KEY_DOWN) state->coordinateY -=1;

    if (key == GLFW_KEY_RIGHT) state->coordinateX +=1;
    if (key == GLFW_KEY_LEFT) state->coordinateX -=1;
    // Update uniform

    glUseProgram(state->program);
    glUniform1i(glGetUniformLocation(state->program, "greenY"), state->coordinateY);
    glUniform1i(glGetUniformLocation(state->program, "greenX"), state->coordinateX);

}