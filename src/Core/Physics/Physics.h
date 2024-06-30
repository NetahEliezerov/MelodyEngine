#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct PGameObject {
	std::string idx;
	glm::vec3 scale;
	glm::vec3 transform;
	float scaleOffset;
};

class PPhysicsState {
public:
	std::vector<PGameObject> objects;
};

class PPhysicsWorld {
public:
	static PPhysicsState state;
};
