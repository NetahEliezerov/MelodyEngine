#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../Core/Engine.h"
#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Player/Player.h"
#include "../Core/LightPoint.h"
#include "../Core/Shader.hpp"
#include "../Levels/Level1.hpp"

struct LevelManager
{
    Level1 _level1;
    Player* character;
    WorldLevel* currentLevel;

    void GameStart(Renderer renderer, Player* playerPointer, float* timeScale)
    {
        character = playerPointer;
        _level1.Init(renderer, playerPointer, timeScale);
        currentLevel = &_level1; // Set the current level to Level1
    }

    void GameUpdate(float deltaTime)
    {
        _level1.Update(deltaTime);
    }

    void RenderShadows(Shader& shadowShader)
    {
        if (currentLevel != nullptr)
        {
            for (const auto& model : currentLevel->sceneModels)
            {
                // Render the model using the shadow shader
                glUseProgram(shadowShader.ID);

                // Set up model matrix
                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), model->transform);
                modelMatrix = glm::scale(modelMatrix, model->scale);
                modelMatrix = glm::rotate(modelMatrix, glm::radians(model->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(model->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(model->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

                // Pass the model matrix to the shadow shader
                glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

                // Bind VAO and draw the model
                glBindVertexArray(model->VAO);
                glDrawElements(GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }
};