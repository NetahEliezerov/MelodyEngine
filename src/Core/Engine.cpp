#include "Engine.h"
#include "../Game/Game.h"

GLFWwindow* g_OpenGLWindow = nullptr;

GLFWwindow* Engine::GetOpenGLWindow() {
    return g_OpenGLWindow;
}
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

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(mode->width, mode->height, "Engine", glfwGetPrimaryMonitor(), NULL);

    Game::state.WIDTH = mode->width;
    Game::state.HEIGHT = mode->height;
    int HEIGHT = mode->height;

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetMouseButtonCallback(window, Input::mouse_button_callback);
    glfwSetCursorPosCallback(window, Input::cursor_position_callback);
    glfwSetKeyCallback(window, Input::key_callback);

    glEnable(GL_DEPTH_TEST);
    g_OpenGLWindow = window;
    // glEnable(GL_CULL_FACE);
    return window;
}