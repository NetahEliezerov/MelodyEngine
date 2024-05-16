#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include "../Game/Game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

#include <thread>
#include <random>

#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Core/Shader.hpp"
#include "../Player/Player.h"
#include "../Core/LightPoint.h"

#include "../AI/CharacterNPC.h"

#include "../World/WorldLevel.h"
#include "../World/TriggerBox.h"
#include "../World/Interactable.hpp"

#include "Components/Letter.hpp"

#include "../Core/AudioManager.hpp"

class BasicLevel : public WorldLevel
{

public:

    virtual void Init(Renderer renderer, std::function<void()> funcRec, Player* playerPointer, float* timeScaleRec) override
    {
        character = playerPointer;
        playerPointer->level = this;
        playerPointer->light = &light;
        func = funcRec;

        ObjectSettings wallSettings = { "Exterior", "assets/meshes/cube.obj", {"assets/textures/Wall/images.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(6, 3, 6), glm::vec3(0, -1, 0), glm::vec3(0,0,0), true, character->shader };
        ObjectSettings handSettings = { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, -3.5, -1), glm::vec3(87, 165,98), true, character->shader };
        LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(1, 0, 2), character->shader };


        wall.Init(wallSettings);
        hand.Init(handSettings);
        light.Init(lightSettings);


        Game::state.currentMission = 0;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Investigate";

        sceneHierarchy.push_back(&hand);
        sceneHierarchy.push_back(&light);
        sceneHierarchy.push_back(&wall);
    };

    virtual void Update(float deltaTime) override
    {
        FlickLight(deltaTime);

        hand.rotation.z += 7.5 * deltaTime;
    };

private:
    Model3D wall;
    Model3D hand;

    LightPoint light;

    std::function<void()> func;

    bool lightOn = true;
    float flickerTimer = 0.0f;
    float flickerInterval = 0.2f;


    void FlickLight(float deltaTime)
    {
        flickerTimer += deltaTime;

        if (flickerTimer >= flickerInterval)
        {
            flickerTimer = 0.0f;
            lightOn = !lightOn;

            if (lightOn)
                light.color = glm::vec4(1, 0.78, 0.6, 1.f);
            else
                light.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.1f, 0.5f);
            flickerInterval = dis(gen);
        }
    };
};
