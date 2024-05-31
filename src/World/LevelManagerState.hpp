#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Player/Player.h"
#include "../Core/LightPoint.h"

#include <any>

#include <variant>

#include "../Core/Shader.hpp"

#include "../Types.h"

class Player;

class LevelManagerState
{
public:
    WorldLevel* currentLevel;
    Renderer renderer;
    Player* character;

    float* timeScale;

    void InitDefault(Renderer rendererRec, Player* playerPointer, float* timeScaleRec)
    {
        renderer = rendererRec;
        character = playerPointer;
        timeScale = timeScaleRec;
        GameStart(rendererRec, playerPointer, timeScaleRec);
    }

    void UpdateDefault(float deltaTime)
    {
        currentLevel->Update(deltaTime);
        currentLevel->RenderUpdate(deltaTime);
        GameUpdate(deltaTime);
    }

    virtual void GameUpdate(float deltaTime) = 0;

    virtual void GameStart(Renderer rendererRec, Player* playerPointer, float* timeScaleRec) = 0;

    void RenderShadows(Shader& shadowShader)
    {
        if (currentLevel != nullptr)
        {

            for (std::any& item : currentLevel->sceneHierarchy)
            {
                if (std::any_cast<Model3D*>(&item))
                {
                    Model3D* model = std::any_cast<Model3D*>(item);

                    glUseProgram(shadowShader.ID);

                    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), model->transform);
                    modelMatrix = glm::scale(modelMatrix, model->scale);
                    modelMatrix = glm::rotate(modelMatrix, glm::radians(model->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                    modelMatrix = glm::rotate(modelMatrix, glm::radians(model->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                    modelMatrix = glm::rotate(modelMatrix, glm::radians(model->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

                    glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

                    glBindVertexArray(model->VAO);
                    glDrawElements(GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0);
                    glBindVertexArray(0);
                }
            }
        }
    }
};
