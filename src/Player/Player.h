#pragma once
#include "PlayerMovement.h"

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
#include "../Core/LightPoint.h"
#include "../Types.h"
#include "../World/WorldLevel.h"
class Player
{
public:
	void Update(float deltaTime);
	void Init(Renderer _renderer, bool recIsGodMode, unsigned int* shaderPointer, bool* isInInteractionZone);

	PlayerMovement movement;

	WorldLevel* level;

	unsigned int shader;

	float footstepTimer = 0.0f;
	float footstepDelay = 0.5f;

	glm::mat4 view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 2.0f),  // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f),  // Look at the origin
		glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
	);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	bool isJumping = false;
	LightPoint light;
	float jumpForce = 5.0f;
	float gravity = -13.8f;
	bool isSprinting = false;

	float fogDensity = 0.06f;
	glm::vec3 fogColor;
	

	bool isCrouching = false;
	float crouchSpeed = 2.0f;
	float crouchHeight = -1.f;
	float standingHeight = 1.0f;

	glm::vec3 velocity = glm::vec3(0.0f);
	bool* isInInteractionZone;

private:
	void Sprint(bool value);
	void Crouch(bool value);
	bool isGodMode;
	Model3D hand;
	Model3D pistol;
	void FireAnimation(Model3D* whatto, float toHow);
	void Fire();
	std::chrono::steady_clock::time_point timeSinceShoot;
};

