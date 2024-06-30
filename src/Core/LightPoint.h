#pragma once
#include <string>
#include <glm/glm.hpp>
#include "../Types.h"

class LightPoint
{
public:
	void Init(LightSettings settings);
	void Update(glm::vec3 cameraPos);

	bool flickLight = false;
	glm::vec3 spotLightPos = glm::vec3(0.0f, 0, 0.0f); // Adjust the position as needed
	glm::vec3 spotLightDir = glm::vec3(1.0f, 0.0f, -1.0f); // Spot light pointing straight down
	float spotLightCutoff = 5; // Inner cutoff angle
	float spotLightOuterCutoff = 14; // Outer cutoff angle

	bool isSpotlight = false;

private:
	unsigned int modelLoc;
	unsigned int colorLoc;
	unsigned int lightPosLoc;
	unsigned int lightColorLoc;
	unsigned int viewPosLoc;


	int numIndices;
	unsigned int* indices;
	unsigned int EBO;

	unsigned int VBO, VAO;
	glm::mat4 model = glm::mat4(1.0f);
	unsigned int shader;
	float* vertices;
	int numVertices;
	bool isLight;

	bool isAttached = false;

	unsigned int textureID;

public:
	glm::vec3 transform;
	glm::vec4 color;
};