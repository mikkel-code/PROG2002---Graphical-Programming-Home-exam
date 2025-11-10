#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#include "GLFWApplication.h"
#include <string>

class Assignment : public GLFWApplication {
public:
    Assignment(const std::string& name, const std::string& version);

    unsigned Run() const override;  // render loop + shape
};
#endif