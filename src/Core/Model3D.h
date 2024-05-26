#pragma once
#include <string>
#include <glm/glm.hpp>
#include "LightPoint.h"
#include "../Types.h"
#include "LoadFromVertices.h"

class Model3D
{
public:
	void Init(ObjectSettings settings);
	void Update(glm::vec3 cameraPos, LightPoint light, float deltaTime);
	void SetPosition(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, float distance, float rightOffset, float upOffset);
	void Draw(unsigned int shader);

private:
	unsigned int modelLoc;
	unsigned int colorLoc;
	unsigned int lightPosLoc;
	unsigned int lightColorLoc;
	unsigned int boneTransformsLoc;
	unsigned int viewPosLoc;

	std::vector<unsigned int> textureIDs;


	std::vector<BoneInfo> boneInfos;
	std::map<std::string, unsigned int> boneMapping;
	int numBones;
	unsigned int textureID;

public:
	std::string label;
	glm::vec3 transform;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec4 color;
	bool visibility = true;
	float radius = 1.f;
	float collisionRadius;
	float colliderOffsetX = -1.f;

	unsigned int VBO, VAO;
	glm::mat4 model = glm::mat4(1.0f);
	unsigned int shader;
	bool isDynamic = false;
	float* vertices;
	int numVertices;
	bool isAttached = false;
	bool isLight;

	int numIndices;
	unsigned int* indices;
	unsigned int EBO;
};
