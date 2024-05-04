#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>


struct ObjectSettings
{
    std::string label;
    std::string fileName;
    std::vector<std::string> texturePaths;
    bool recIsLight;
    glm::vec4 recColor;
    glm::vec3 recScale;
    glm::vec3 recTransform;
    glm::vec3 recRotation;
    bool isDynamicRec;
    unsigned int recShader;
};

struct LightSettings
{
    std::string fileName;
    std::string texturePath;
    glm::vec4 recColor;
    glm::vec3 recScale;
    glm::vec3 recTransform;
    unsigned int recShader;
};