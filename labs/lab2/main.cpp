#include <iostream>
#include "GLFWApplication.h"
#include "Lab2Application.h"

int main(int argc, char* argv[]) {
    Lab2Application app("Lab2", "1.0");
    if (app.Init() != 0) { std::cout << "Something failed during initilalization.\n"; return -1; }
    return app.Run();
}