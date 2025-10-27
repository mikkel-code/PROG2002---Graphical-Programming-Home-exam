#ifndef LAB3APPLICATION_H
#define LAB3APPLICATION_H
#include "GLFWApplication.h"
#include <string>

class Lab3Application : public GLFWApplication {
public:
    Lab3Application(const std::string& name, const std::string& version);

    unsigned Run() const override;  // render loop + shape
};
#endif