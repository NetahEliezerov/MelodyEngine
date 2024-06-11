#pragma once

#include <GLFW/glfw3.h>

class InputState
{
public:
	bool keys[GLFW_KEY_LAST];

	double mouseX;
	double mouseY;
	double prevMouseX;
	double prevMouseY;
	float mouseDeltaX;
	float mouseDeltaY;

	InputState();
	~InputState();
};

class Input
{
public:
	static InputState inputState;
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};