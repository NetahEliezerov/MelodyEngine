#include "Model3D.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include "../Utils/OBJ_Loader.h"

std::pair<int, float*> loadVerticesFromFile(std::string fileName, int& numIndices, unsigned int*& indices, glm::vec3 asdaMultiplier)
{
    objl::Loader Loader;

    bool loadout = Loader.LoadFile(fileName);

    objl::Mesh curMesh = Loader.LoadedMeshes[0];
    int numVertices = curMesh.Vertices.size();
    float* vertices = new float[numVertices * 8];
    int index = 0;
    for (int j = 0; j < numVertices; j++) {
        vertices[index++] = curMesh.Vertices[j].Position.X / 3 * asdaMultiplier.x;
        vertices[index++] = curMesh.Vertices[j].Position.Y / 3 * asdaMultiplier.y;
        vertices[index++] = curMesh.Vertices[j].Position.Z / 3 * asdaMultiplier.z;

        vertices[index++] = curMesh.Vertices[j].Normal.X / 3 * asdaMultiplier.x;
        vertices[index++] = curMesh.Vertices[j].Normal.Y / 3 * asdaMultiplier.y;
        vertices[index++] = curMesh.Vertices[j].Normal.Z / 3 * asdaMultiplier.z;

        vertices[index++] = curMesh.Vertices[j].TextureCoordinate.X / 3 * asdaMultiplier.x;
        vertices[index++] = curMesh.Vertices[j].TextureCoordinate.Y / 3 * asdaMultiplier.y;
    }

    numIndices = curMesh.Indices.size();
    indices = new unsigned int[numIndices];
    for (int i = 0; i < numIndices; i++) {
        indices[i] = curMesh.Indices[i];
    }
    return std::make_pair(numVertices, vertices);
}

void Model3D::Init(std::string fileName, std::string texturePath, bool recIsLight, glm::vec4 recColor, glm::vec3 recScale, glm::vec3 recTransform, bool isDynamicRec, unsigned int recShader)
{
    transform = recTransform;
    shader = recShader;
    isDynamic = isDynamicRec;
    color = recColor;
    isLight = recIsLight;

    std::pair<int, float*> things = loadVerticesFromFile(fileName, numIndices, indices,recScale);

    numVertices = things.first;
    vertices = things.second;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << texturePath << std::endl;
    }

    model = glm::translate(model, transform);

    modelLoc = glGetUniformLocation(shader, "model");
    colorLoc = glGetUniformLocation(shader, "objectColor");

    lightPosLoc = glGetUniformLocation(shader, "lightPos");
    lightColorLoc = glGetUniformLocation(shader, "lightColor");
    viewPosLoc = glGetUniformLocation(shader, "viewPos");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Model3D::SetPosition(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, float distance, float rightOffset)
{
    // Calculate the camera's right vector
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));

    // Calculate the position of the cube relative to the camera
    glm::vec3 cubePos = cameraPos + cameraFront * distance + right * rightOffset;

    // Calculate the rotation matrix based on the camera's orientation
    glm::vec3 up = glm::normalize(glm::cross(right, cameraFront));
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix[0] = glm::vec4(right, 0.0f);
    rotationMatrix[1] = glm::vec4(up, 0.0f);
    rotationMatrix[2] = glm::vec4(-cameraFront, 0.0f);

    // Set the model matrix based on the cube's position and rotation
    model = glm::translate(glm::mat4(1.0f), cubePos) * rotationMatrix;
}

void Model3D::Update(glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor)
{
    // model = glm::translate(model, transform);
    glLinkProgram(shader);

    float red = color.r;
    float green = color.g;
    float blue = color.b;
    float alpha = color.a;

    glUniform3f(colorLoc, red, green, blue);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}