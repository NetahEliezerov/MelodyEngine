#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PlayerMovement
{
public:
	glm::vec3 position = glm::vec3(7.5f, 20.0f, 7.5f);
	glm::vec3 lookingAngle = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float sensitivity = 0.4f;
	float cameraSpeed = 2.5f;
	float walkSpeed = 2.5f;
	float sprintSpeed = 4.5f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	unsigned int projectionLoc;
	unsigned int viewLoc;
};

