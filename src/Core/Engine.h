#pragma once

#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"

extern GLFWwindow* g_OpenGLWindow;

namespace Engine
{
    GLFWwindow* Run();

    GLFWwindow* GetOpenGLWindow();
};