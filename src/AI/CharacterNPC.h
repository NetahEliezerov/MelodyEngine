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

#include "../World/WorldLevel.h"

class CharacterNPC
{
public:
	void Update(float deltaTime, LightPoint light);
	void Init(Renderer renderer, Player* playerPointer);

	Player* character;
	Model3D targetCube3;

	bool followPlayer = true;
private:
	// ...
	float followDistance = 4.0f;  // Adjust the follow distance as needed
	float moveSpeed = 3.0f;       // Adjust the movement speed as needed
	float rotateSpeed = 5.0f;
};

