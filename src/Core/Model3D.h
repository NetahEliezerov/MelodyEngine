#pragma once
#include <string>
#include <glm/glm.hpp>
#include "LightPoint.h"
#include "../Types.h"

class Model3D
{
public:
	void Init(ObjectSettings settings);
	void Update(glm::vec3 cameraPos, LightPoint light);
	void SetPosition(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, float distance, float rightOffset, float upOffset);

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
	bool isDynamic = false;
	float* vertices;
	int numVertices;
	bool isAttached = false;
	bool isLight;

	unsigned int textureID;

public:
	std::string label;
	glm::vec3 transform;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec4 color;
	bool visibility = true;
	float radius = 1.f;
};

