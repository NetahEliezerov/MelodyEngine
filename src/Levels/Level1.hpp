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

class Level1 : public WorldLevel
{

public:

    virtual void Init(Renderer renderer, std::function<void()> funcRec, Player* playerPointer, float* timeScaleRec) override
    {
        character = playerPointer;
        playerPointer->level = this;
        playerPointer->light = &light;
        func = funcRec;

        ObjectSettings wall1Settings = { "Wall", "assets/meshes/cube.obj", {"assets/textures/Wall/pngtree-wood-grain-texture-wooden-flooring-design-with-wooden-floor-textures-image_13051712.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(6, 3, 6), glm::vec3(0, -1, 0), glm::vec3(0,0,0), true, character->shader };
        ObjectSettings targetCubeSettings = { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, -3.5, -1), glm::vec3(87, 165,98), true, character->shader };
        ObjectSettings targetCube2Settings = { "Target Cube", "assets/meshes/Shotgun/Shotgun.fbx", {"assets/meshes/Shotgun/Shotgun_DefaultMaterial_BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.004, 0.004, 0.004), glm::vec3(4, -1.5, -5.1), glm::vec3(0,327,270), true, character->shader };
        ObjectSettings tableSettings = { "Target Cube", "assets/meshes/Table/MechanicalTable.fbx", {"assets/meshes/Table/BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.007, 0.012, 0.009), glm::vec3(1.5, -4, -5.2), glm::vec3(0,0,0), true, character->shader };

        LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(0, 0, 1), character->shader };


        wall.Init(wall1Settings);
        targetCube.Init(targetCubeSettings);
        targetCube2.Init(targetCube2Settings);
        table.Init(tableSettings);
        light.Init(lightSettings);

        Game::state.currentMission = 0;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Investigate";


        letter.Init({ "Noah", std::string("Hey. Find me in the factory.") }, renderer, character, &light, timeScaleRec, [this]() { NoahLetterOpen(); });

        triggerBox.Init(glm::vec3(-4.f, 0.0f, 4.f), glm::vec3(2.0f, 8.0f, 2.0f), [this]() { OnRoomExit(); }, character, true, true);

        sceneHierarchy.push_back(&wall);
        sceneHierarchy.push_back(&targetCube);
        sceneHierarchy.push_back(&targetCube2);
        sceneHierarchy.push_back(&table);
        sceneHierarchy.push_back(&light);
        sceneHierarchy.push_back(&triggerBox);
        sceneHierarchy.push_back(&letter);
    };

    virtual void Update(float deltaTime) override
    {
        if (light.flickLight)
        {
            FlickLight(deltaTime);
        }

        targetCube.rotation.z += 7.5 * deltaTime;
    };

    void NoahLetterOpen()
    {
        if (Game::state.currentSubMission == 0)
        {
            Game::state.currentSubMission++;
            Game::state.currentObjective = "Go to the factory";
        }
    }

    void OnRoomExit()
    {
        std::cout << "HELKO JOHNKES";
        if (Game::state.currentSubMission == 1)
        {
            std::cout << "GOT OUT OF ROOM" << std::endl;
            Game::state.currentSubMission++;
            Game::state.currentObjective = "Great!";
            func();
        }
    };
private:
    Model3D pistol;
    Model3D hand;
    Model3D cube;
    Model3D wall;
    Model3D targetCube;
    Model3D targetCube2;

    std::function<void()> func;

    Model3D table;
    Letter letter;

    CharacterNPC npc;

    TriggerBox triggerBox;


    bool lightOn = true;
    float flickerTimer = 0.0f;
    float flickerInterval = 0.2f;

    LightPoint light;

    void FlickLight(float deltaTime)
    {
        flickerTimer += deltaTime;

        if (flickerTimer >= flickerInterval)
        {
            flickerTimer = 0.0f;
            lightOn = !lightOn;

            if (lightOn)
                light.color = glm::vec4(0.2f, 0.5f, 0.5f, 0.5f);
            else
                light.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.1f, 0.5f);
            flickerInterval = dis(gen);
        }
    }
    ;
};