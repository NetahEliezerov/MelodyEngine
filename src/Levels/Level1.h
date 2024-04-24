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
#include <map>

#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Player/Player.h"
#include "../Core/LightPoint.h"

class Level1
{
public:
	void Init(Renderer renderer, Player* playerPointer);
	void Update(float deltaTime);


	Player* character;
private:
	Model3D pistol;
	Model3D hand;
	Model3D cube;
	LightPoint light;
};

