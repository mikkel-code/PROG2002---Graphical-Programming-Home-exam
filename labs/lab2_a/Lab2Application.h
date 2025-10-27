#ifndef LAB2APPLICATION_H
#define LAB2APPLICATION_H
#include "GLFWApplication.h"
#include <string>

class Lab2Application : public GLFWApplication {
public:
    Lab2Application(const std::string& name, const std::string& version);

    unsigned Run() const override;  // render loop + shape
};

#endif