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
#include "Components/DoorKey.hpp"

#include "../Core/AudioManager.hpp"

class Level1 : public WorldLevel
{

public:

    virtual void LevelStart() override
    {
        character->level = this;
        character->light = &light;
        
        
        wall.Init(        { "Wall", "assets/meshes/cube.obj", {"assets/textures/Wall/pngtree-wood-grain-texture-wooden-flooring-design-with-wooden-floor-textures-image_13051712.jpg"}, glm::vec3(6, 3, 6), glm::vec3(0, -1, 0), glm::vec3(0,0,0) }, this);
        targetCube.Init(  { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, -3.5, -1), glm::vec3(87, 165,98) }, this);
        targetCube2.Init( { "Target Cube 1", "assets/meshes/Shotgun/Shotgun.fbx", {"assets/meshes/Shotgun/Shotgun_DefaultMaterial_BaseColor.png"}, glm::vec3(0.004, 0.004, 0.004), glm::vec3(3, -1.5, -5.1), glm::vec3(0,327,270) }, this);
        box.Init({ "Box", "assets/meshes/Box/box.fbx", {"assets/meshes/Box/box_BaseColor.png"}, glm::vec3(1, 1, 1), glm::vec3(-3, -4, -1), glm::vec3(270,0,56) }, this);
        table.Init(       { "Target Cube 2", "assets/meshes/Table/MechanicalTable.fbx", {"assets/meshes/Table/BaseColor.png"}, glm::vec3(0.007, 0.012, 0.009), glm::vec3(1.5, -4, -5.2), glm::vec3(0,0,0) }, this);
        light.Init(       { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(0, 0, 0), true });

        Game::state.currentMission = 0;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Investigate";

        letter.Init({ "Noah", std::string("Hey. Find me in the factory.") }, this, timeScale, [this]() { NoahLetterOpen(); });
        ObjectSettings doorSettings = { "Door Key", "assets/meshes/Key/Key2.obj", {"assets/meshes/Key/Albedo.jpg"}, glm::vec3(0.003, 0.003, 0.003), glm::vec3(4, -1.53, -5), glm::vec3(90, 0, -27) };
        doorKey.Init(doorSettings, 0, this, timeScale);

        triggerBox.Init(this, glm::vec3(-6.f, 0.0f, 4.f), glm::vec3(0.4f, 8.0f, 2.0f), [this]() { OnRoomExit(); }, character, true, true);

    };

    virtual void Update(float deltaTime) override
    {
        if (light.flickLight)
        {
            FlickLight(deltaTime);
        }
        light.spotLightPos = character->movement.position - character->movement.lookingAngle - glm::vec3(0,1,0);
        light.transform = character->movement.position - character->movement.lookingAngle - glm::vec3(0,1,0);
        light.spotLightDir = character->movement.lookingAngle;

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

    Model3D table;
    Model3D box;
    Letter letter;
    DoorKey doorKey;

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