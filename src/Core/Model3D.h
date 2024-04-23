#pragma once
#include <utility>
#include <string>
#include <glm/glm.hpp>

class Model3D
{
public:
	void Init(std::string fileName, std::string texturePath, bool recIsLight, glm::vec4 recColor, glm::vec3 recScale, glm::vec3 recTransform, bool isDynamicRec, unsigned int recShader);
	void Update(glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor);
	void SetPosition(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, float distance, float rightOffset);

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
	bool isDynamic;
	float* vertices;
	int numVertices;
	bool isLight;

	unsigned int textureID;

public:
	glm::vec3 transform;
	glm::vec4 color;
};

