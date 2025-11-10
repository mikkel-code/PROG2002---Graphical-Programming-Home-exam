#include <iostream>
#include "GLFWApplication.h"
#include "Assignment.h"

int main(int argc, char* argv[]) {
    Assignment app("Assigment", "1.0");
    if (app.Init() != 0) { std::cout << "Something failed during initilalization.\n"; return -1; }
    return app.Run();
}