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

class Level2 : public WorldLevel
{

public:

    virtual void Init(Renderer renderer, std::function<void()> funcRec, Player* playerPointer, float* timeScaleRec) override
    {
        character = playerPointer;
        playerPointer->level = this;
        func = funcRec;

        ObjectSettings cubeSettings = { "Pipes", "assets/meshes/Pipes/untitled.obj", {"assets/meshes/Pipes/Untitled_2_DefaultMaterial_BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(2, 2, 1.5), glm::vec3(3, -3.5, 4), glm::vec3(0,0,0), false, character->shader };


        ObjectSettings pipe1Settings = { "Pipe", "assets/meshes/Pipes/1/1.obj", {"assets/meshes/Pipes/1/old-rusty-metal-texture-big-22.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(1.4, 1.3, 1.3), glm::vec3(-6, -7, -5), glm::vec3(0,90,90), false, character->shader };


        ObjectSettings boxSettings = { "Box", "assets/meshes/Box/box.fbx", {"assets/meshes/Box/box_BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(1, 1, 1), glm::vec3(-3, -4, -1), glm::vec3(270,0,56), false, character->shader };

        ObjectSettings wall1Settings = { "Exterior", "assets/meshes/cube.obj", {"assets/textures/Wall/images.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(6, 3, 6), glm::vec3(0, -1, 0), glm::vec3(0,0,0), true, character->shader };

        ObjectSettings targetCubeSettings = { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, -3.5, -1), glm::vec3(87, 165,98), true, character->shader };

        LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(1, 0, 2), character->shader };


        wall.Init(wall1Settings);
        box.Init(boxSettings);

        pipes.Init(cubeSettings);
        pipe1.Init(pipe1Settings);

        // npc.Init(renderer, character, true);

        targetCube.Init(targetCubeSettings);


        light.Init(lightSettings);
        playerPointer->light = &light;

        // audioSweetHeart.LoadSound("assets/sounds/sweetheart.wav");

        Game::state.currentMission = 0;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Find Noah";

        std::cout << "PIPE 1: " << &pipe1 << std::endl;
        std::cout << "BOX: " << &box << std::endl;
        std::cout << "LIGHT: " << &light << std::endl;


        // sceneModels.push_back(&cube);

        sceneHierarchy.push_back(&box);
        sceneHierarchy.push_back(&pipes);
        sceneHierarchy.push_back(&pipe1);
        sceneHierarchy.push_back(&targetCube);
        sceneHierarchy.push_back(&light);
        sceneHierarchy.push_back(&wall);

        sceneModels.push_back(&wall);
        sceneModels.push_back(&targetCube);
        sceneModels.push_back(&pipes);
        sceneModels.push_back(&box);
    };

    virtual void Update(float deltaTime) override
    {
        FlickLight(deltaTime);

        targetCube.rotation.z += 7.5 * deltaTime;

        pipes.Update(character->movement.position, light, deltaTime);
        pipe1.Update(character->movement.position, light, deltaTime);

        box.Update(character->movement.position, light, deltaTime);
        targetCube.Update(character->movement.position, light, deltaTime);
        wall.Update(character->movement.position, light, deltaTime);



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
    Model3D wall;
    Model3D targetCube;

    Model3D pipes;
    Model3D box;

    Model3D pipe1;


    std::function<void()> func;


    CharacterNPC npc;


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
