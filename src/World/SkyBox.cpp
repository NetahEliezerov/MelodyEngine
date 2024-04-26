#include "SkyBox.h"
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

#include <fstream>
#include <sstream>

static std::string readShaderSource(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


void SkyBox::Init(std::vector<std::string> faces, Renderer _renderer)
{
    renderer = _renderer;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    textureIdGlobal = textureID;
    float skyboxVertices[] = {
        // Positions          
        -10.0f,  10.0f, -10.0f,
        -10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

        -10.0f,  10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f,  10.0f
    };

    // Generate and bind VAO and VBO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Store the skybox VAO
    skyboxVAOGlobal = skyboxVAO;

    std::string vertexShader = readShaderSource("shaders/SkyBox/vertex.glsl");
    std::string fragmentShader = readShaderSource("shaders/SkyBox/frag.glsl");
    shader = renderer.CreateShader(vertexShader, fragmentShader);
}

void SkyBox::Update(glm::mat4 view, glm::mat4 projection)
{
    glDepthFunc(GL_LEQUAL);
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(skyboxVAOGlobal);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdGlobal);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    //glDepthMask(GL_FALSE);
    // glUseProgram(shader);
}
