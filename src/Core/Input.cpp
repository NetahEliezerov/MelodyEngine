#include "Input.h"
#include <iostream>

InputState::InputState() : mouseX(0), mouseY(0), prevMouseX(0), prevMouseY(0), mouseDeltaX(0), mouseDeltaY(0) {}
InputState::~InputState() {}

InputState Input::inputState;

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    Input::inputState.mouseDeltaX = xpos - Input::inputState.prevMouseX;
    Input::inputState.mouseDeltaY = ypos - Input::inputState.prevMouseY;
    Input::inputState.prevMouseX = xpos;
    Input::inputState.prevMouseY = ypos;

    if (xpos <= 0 || xpos >= width - 1 || ypos <= 0 || ypos >= height - 1) {
        // Set the cursor position to the center of the window
        glfwSetCursorPos(window, width / 2, height / 2);
        Input::inputState.prevMouseX = width / 2;
        Input::inputState.prevMouseY = height / 2;
    }
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        Input::inputState.keys[key] = true;
    else if (action == GLFW_RELEASE)
        Input::inputState.keys[key] = false;
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        Input::inputState.keys[button] = true;
    else if (action == GLFW_RELEASE)
        Input::inputState.keys[button] = false;
}
