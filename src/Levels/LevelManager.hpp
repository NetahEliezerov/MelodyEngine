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


#include "../World/LevelManagerState.hpp"

#include "../Levels/Level1.hpp"
#include "../Levels/Level2.hpp"

struct LevelManager : LevelManagerState
{
    Level1 _level1;
    Level2 _level2;

    void GameStart(Renderer rendererRec, Player* playerPointer, float* timeScaleRec)
    {
        renderer = rendererRec;
        character = playerPointer;
        timeScale = timeScaleRec;

        _level1.Init(renderer, [this]() { OnDoorEnter(); }, playerPointer, timeScale);
        currentLevel = &_level1; // Set the current level to Level1
    }

    void GameUpdate(float deltaTime)
    {
        currentLevel->Update(deltaTime);
    }


    void OnDoorEnter()
    {
        currentLevel = &_level2;
        _level2.Init(renderer, [this]() {  }, character, timeScale);
    }
};