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

#include "Level1.hpp"
#include "Level2.hpp"
#include "OpenWorld.hpp"

struct LevelManager : LevelManagerState
{
    Level1 _level1;
    Level2 _level2;
    OpenWorld _openWorld;

    virtual void GameStart(Renderer* rendererRec, Player* playerPointer, float* timeScaleRec) override
    {
        SetLevel(&_level1, [this]() { OnDoorEnter(); });
    }

    virtual void GameUpdate(float deltaTime) override
    {
    }


    void OnDoorEnter()
    {
        SetLevel(&_level1, [this]() {});
    }
};