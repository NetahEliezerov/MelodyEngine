#include "LightPoint.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include "LoadFromVertices.h"

void LightPoint::Init(LightSettings settings)
{
    transform = settings.recTransform;
    shader = settings.recShader;
    color = settings.recColor;

    std::pair<int, float*> things = loadVerticesFromFile(settings.fileName, numIndices, indices);

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
    unsigned char* data = stbi_load(settings.texturePath.c_str(), &width, &height, &nrChannels, 0);
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
        std::cout << "Failed to load texture: " << settings.texturePath << std::endl;
    }

    model = glm::translate(model, transform);

    modelLoc = glGetUniformLocation(shader, "model");
    colorLoc = glGetUniformLocation(shader, "objectColor");

    lightPosLoc = glGetUniformLocation(shader, "lightPos");
    lightColorLoc = glGetUniformLocation(shader, "lightColor");
    viewPosLoc = glGetUniformLocation(shader, "viewPos");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void LightPoint::Update(glm::vec3 cameraPos)
{
    // model = glm::translate(model, transform);
    glLinkProgram(shader);

    float red = color.r;
    float green = color.g;
    float blue = color.b;
    float alpha = color.a;
    glm::vec3 lightPos = transform;
    glm::vec3 lightColor = color;
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