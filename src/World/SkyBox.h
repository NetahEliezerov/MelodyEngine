#pragma once

#include <vector>
#include <string>
#include "../Core/Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class SkyBox
{
public:
    void Init(std::vector<std::string> faces, Renderer _renderer);
    void Update(glm::mat4 view, glm::mat4 projection);

    unsigned int textureIdGlobal;
    unsigned int skyboxVAOGlobal;

private:
    Renderer renderer;
    unsigned int shader;
};