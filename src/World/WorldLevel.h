#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Core/LightPoint.h"

#include <any>

#include <variant>

#include "../Core/Shader.hpp"

#include "../Types.h"


class Player;


class WorldLevel
{
public:
	virtual void Init(Renderer renderer, std::function<void()> func, Player* playerPointer, float* timeScaleRec) = 0;
	virtual void Update(float deltaTime) = 0;

	// virtual void Fire(Model3D* hitObject) = 0;

	void RenderUpdate(float deltaTime);

	Player* character;

	std::vector<std::any> sceneHierarchy;

};