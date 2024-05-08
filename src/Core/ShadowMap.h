#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

class ShadowMap
{
public:
    ShadowMap();
    ~ShadowMap();

    void Init();
    void BindForWriting();
    void BindForReading(GLenum TextureUnit);
    void SetLightSpaceMatrix(const glm::mat4& matrix);
    glm::mat4 GetLightSpaceMatrix() const;

    unsigned int shadowMap;
private:
    unsigned int FBO;
    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;
    glm::mat4 lightSpaceMatrix;
};