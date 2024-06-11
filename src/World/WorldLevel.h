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
#include <any>
#include <variant>

#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Core/LightPoint.h"
#include "../Core/Shader.hpp"
#include "../Game/Game.h"
#include "../Types.h"


class Player;


class WorldLevel
{
public:
	virtual void LevelStart() = 0;
	virtual void Update(float deltaTime) = 0;


	void Init(Renderer* rendererRec, std::function<void()> funcRec, Player* playerPointer, float* timeScaleRec);
	// virtual void Fire(Model3D* hitObject) = 0;

	void RenderUpdate(float deltaTime);

	Player* character;
	Renderer* renderer;
	float* timeScale;
	std::function<void()> func;
	LightPoint light;
	std::string d;
	

	std::vector<std::any> sceneHierarchy;

};