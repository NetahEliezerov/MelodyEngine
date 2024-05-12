#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../Core/Engine.h"
#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Player/Player.h"
#include "../Core/LightPoint.h"


#include "../Core/Shader.hpp"

#include "../Levels/Level1.hpp"

struct LevelManager
{
	Level1 _level1;
	Player* character;

	void GameStart(Renderer renderer, Player* playerPointer, float* timeScale)
	{
		character = playerPointer;
		_level1.Init(renderer, playerPointer, timeScale);

	}

	void GameUpdate(float deltaTime)
	{
		_level1.Update(deltaTime);
	}
};