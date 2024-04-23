#include "Engine.h"

GLFWwindow* Engine::Run()
{
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cout << "glfw init err" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

#ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(1920, 1080, "Engine", NULL, NULL);

    if (!window) {
        std::cout << " can't create window!!!" << std::endl;
        glfwTerminate();
    }

    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);

    std::cout << "OpenGL shader version: " << major << "." << minor << std::endl;

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSetCursorPosCallback(window, Input::cursor_position_callback);
    glfwSetKeyCallback(window, Input::key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}