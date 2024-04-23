#pragma once
#include <glm/glm.hpp>

#include "../Core/Renderer.h"
#include "PlayerMovement.h"

class Player
{
public:
	void Update(float deltaTime);
	void Init(Renderer _renderer, bool recIsGodMode);

	PlayerMovement movement;

	unsigned int shader;

	glm::mat4 view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 2.0f),  // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f),  // Look at the origin
		glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
	);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

private:
	bool isGodMode;
};

