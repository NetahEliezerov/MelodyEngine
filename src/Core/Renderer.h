#pragma once

#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

class Renderer
{
public:
    unsigned int CompileShader(unsigned int type, const std::string& source);
    int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
