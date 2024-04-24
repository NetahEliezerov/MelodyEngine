#pragma once
#include <string>
#include <glm/glm.hpp>


struct ObjectSettings
{
    std::string fileName;
    std::string texturePath;
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