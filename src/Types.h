#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "Core/Shader.hpp"


struct ObjectSettings
{
    std::string label;
    std::string fileName;
    std::vector<std::string> texturePaths;
    glm::vec3 recScale;
    glm::vec3 recTransform;
    glm::vec3 recRotation;
};

struct LetterSettings
{
    std::string title;
    std::string content;
};

struct Text2DSettings
{
    const std::string& text;
    glm::vec2 transform;
    float scale;
    glm::vec3 color;
    Shader& shader;

    unsigned int VBO;
    unsigned int VAO;
};

struct ShaderType
{
    Shader* shader;
    unsigned int VBO;
    unsigned int VAO;
};

struct LightSettings
{
    std::string fileName;
    std::string texturePath;
    glm::vec4 recColor;
    glm::vec3 recScale;
    glm::vec3 recTransform;
    bool isSpotlight;
};