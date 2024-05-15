#include "Model3D.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include "LoadFromVertices.h"

#include <chrono>
#include <thread>

void Model3D::Init(ObjectSettings settings)
{
    label = settings.label;
    transform = settings.recTransform;
    scale = settings.recScale;
    rotation = settings.recRotation;
    shader = settings.recShader;
    isDynamic = settings.isDynamicRec;
    color = settings.recColor;
    isLight = settings.recIsLight;

    std::pair<int, float*> verticesData = loadVerticesFromFileOld(settings.fileName, numIndices, indices, boneInfos, boneMapping, numBones);
    numVertices = verticesData.first;
    vertices = verticesData.second;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    for (const std::string& texturePath : settings.texturePaths)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            unsigned int textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            textureIDs.push_back(textureID);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture: " << texturePath << std::endl;
        }
    }

    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::scale(scaleMatrix, settings.recScale);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(settings.recRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(settings.recRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(settings.recRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::translate(model, transform) * rotationMatrix * scaleMatrix;

    collisionRadius = std::max(scale.x, std::max(scale.y, scale.z)) * 0.5f;

    modelLoc = glGetUniformLocation(shader, "model");
    colorLoc = glGetUniformLocation(shader, "objectColor");

    lightPosLoc = glGetUniformLocation(shader, "lightPos");
    lightColorLoc = glGetUniformLocation(shader, "lightColor");
    viewPosLoc = glGetUniformLocation(shader, "viewPos");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    boneTransformsLoc = glGetUniformLocation(shader, "boneTransforms");
    glUniform1i(glGetUniformLocation(shader, "numBones"), numBones);
}


void Model3D::SetPosition(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, float distance, float rightOffset, float upOffset)
{
    isAttached = true;

    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));


    glm::vec3 cubePos = cameraPos + cameraFront * distance + right * rightOffset + cameraUp * upOffset;


    glm::vec3 up = glm::normalize(glm::cross(right, cameraFront));
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix[0] = glm::vec4(right, 0.0f);
    rotationMatrix[1] = glm::vec4(up, 0.0f);
    rotationMatrix[2] = glm::vec4(-cameraFront, 0.0f);


    model = glm::translate(glm::mat4(1.0f), cubePos) * rotationMatrix;
}

void Model3D::Update(glm::vec3 cameraPos, LightPoint light, float deltaTime)
{
    if (visibility)
    {
        glUseProgram(shader);

        float red = color.r;
        float green = color.g;
        float blue = color.b;
        float alpha = color.a;

        glm::mat4 scaleMatrix = glm::mat4(1.0f);
        scaleMatrix = glm::scale(scaleMatrix, scale);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        if (isAttached)
            model = glm::translate(model, transform) * rotationMatrix * scaleMatrix;
        else
            model = glm::translate(glm::mat4(1.0f), transform) * rotationMatrix * scaleMatrix;

        glUniform3f(colorLoc, red, green, blue);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.transform));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.color));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));


        //if (boneInfos.empty())
        //{
        //    std::vector<glm::mat4> boneTransforms;
        //    boneTransforms.reserve(numBones);
        //    for (const auto& boneInfo : boneInfos)
        //    {
        //        boneTransforms.push_back(model * boneInfo.finalTransformation);
        //    }
        //    glUniformMatrix4fv(boneTransformsLoc, numBones, GL_FALSE, glm::value_ptr(boneTransforms[0]));
        //}

        for (unsigned int i = 0; i < textureIDs.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}