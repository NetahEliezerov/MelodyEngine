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
#include <vector>
#include <chrono>
#include <map>

#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Player/Player.h"
#include "../Core/LightPoint.h"
class Player;

class WorldLevel
{
public:
	virtual void Init(Renderer renderer, Player* playerPointer) = 0;
	virtual void Update(float deltaTime) = 0;

	virtual void Fire(Model3D* hitObject) = 0;

	Player* character;

	std::vector<Model3D*> sceneModels;
};

