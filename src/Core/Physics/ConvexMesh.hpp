#pragma once
#include "../../Game/Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class ConvexMesh {
public:
    ConvexMesh(const float* verticesRec, int numVertices) {
    };
    // Other methods for collision detection
private:
    std::vector<glm::vec3> vertices;
    // Other data structures for efficient collision detection
};