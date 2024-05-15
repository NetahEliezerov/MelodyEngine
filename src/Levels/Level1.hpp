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
        func = funcRec;

        ObjectSettings wall1Settings = { "Wall", "assets/meshes/cube.obj", {"assets/textures/Wall/pngtree-wood-grain-texture-wooden-flooring-design-with-wooden-floor-textures-image_13051712.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(6, 3, 6), glm::vec3(0, -1, 0), glm::vec3(0,0,0), true, character->shader };

        ObjectSettings targetCubeSettings = { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, -3.5, -1), glm::vec3(87, 165,98), true, character->shader };
        ObjectSettings targetCube2Settings = { "Target Cube", "assets/meshes/Shotgun/Shotgun.fbx", {"assets/meshes/Shotgun/Shotgun_DefaultMaterial_BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.004, 0.004, 0.004), glm::vec3(4, -1.5, -5.1), glm::vec3(0,327,270), true, character->shader };


        ObjectSettings tableSettings = { "Target Cube", "assets/meshes/Table/MechanicalTable.fbx", {"assets/meshes/Table/BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.007, 0.012, 0.009), glm::vec3(1.5, -4, -5.2), glm::vec3(0,0,0), true, character->shader };


        LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(0, 0, 1), character->shader };


        wall.Init(wall1Settings);

        // npc.Init(renderer, character, true);

        targetCube.Init(targetCubeSettings);
        targetCube2.Init(targetCube2Settings);

        table.Init(tableSettings);

        light.Init(lightSettings);
        playerPointer->light = &light;

        // audioSweetHeart.LoadSound("assets/sounds/sweetheart.wav");

        std::cout << "Wall 1: " << &wall << std::endl;
        std::cout << "Hand: " << &targetCube << std::endl;
        std::cout << "Gun: " << &targetCube2 << std::endl;
        std::cout << "Letter Model: " << &letter.interactable1 << std::endl;
        std::cout << "TABLE 1: " << &table << std::endl;
        std::cout << "Light: " << &light << std::endl;

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

        // sceneModels.push_back(&cube);
        sceneModels.push_back(&wall);
        sceneModels.push_back(&targetCube);
        sceneModels.push_back(&targetCube2);
    };

    virtual void Update(float deltaTime) override
    {
        if (light.flickLight)
        {
            FlickLight(deltaTime);
        }

        targetCube.rotation.z += 7.5 * deltaTime;


        // audioSweetHeart.Update();


        triggerBox.Update(character->movement.position, light, deltaTime);

        letter.Update(deltaTime);

        targetCube.Update(character->movement.position, light, deltaTime);
        wall.Update(character->movement.position, light, deltaTime);
        targetCube2.Update(character->movement.position, light, deltaTime);

        table.Update(character->movement.position, light, deltaTime);
        // npc.Update(deltaTime, light, true);
        //wall2.Update(character->movement.position, light, deltaTime);
        //wall3.Update(character->movement.position, light, deltaTime);
        //wall4.Update(character->movement.position, light, deltaTime);
        //wall5.Update(character->movement.position, light, deltaTime);
        //wall6.Update(character->movement.position, light, deltaTime);
        light.Update(character->movement.position);
    };

    virtual void Fire(Model3D* hitObject) override
    {
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
        // audioSweetHeart.PlaySound(0);
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
    float flickerInterval = 0.2f; // Adjust this value to control the flickering speed

    LightPoint light;

    void FlickLight(float deltaTime)
    {
        flickerTimer += deltaTime;

        if (flickerTimer >= flickerInterval)
        {
            flickerTimer = 0.0f;
            lightOn = !lightOn;

            if (lightOn)
                light.color = glm::vec4(0.2f, 0.5f, 0.5f, 0.5f); // White color when the light is on
            else
                light.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Black color when the light is off

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.1f, 0.5f);
            flickerInterval = dis(gen);
        }
    }
    ;
};
