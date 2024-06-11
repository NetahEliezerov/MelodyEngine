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

    virtual void LevelStart() override
    {
        ObjectSettings cubeSettings = { "Pipes", "assets/meshes/Pipes/untitled.obj", {"assets/meshes/Pipes/Untitled_2_DefaultMaterial_BaseColor.png"}, glm::vec3(2, 2, 1.5), glm::vec3(3, -3.5, 4), glm::vec3(0,0,0) };
        ObjectSettings pipe1Settings = { "Pipe", "assets/meshes/Pipes/1/1.obj", {"assets/meshes/Pipes/1/old-rusty-metal-texture-big-22.jpg"}, glm::vec3(1.4, 1.3, 1.3), glm::vec3(-6, -7, -5), glm::vec3(0,90,90) };
        ObjectSettings boxSettings = { "Box", "assets/meshes/Box/box.fbx", {"assets/meshes/Box/box_BaseColor.png"}, glm::vec3(1, 1, 1), glm::vec3(-3, -4, -1), glm::vec3(270,0,56) };
        ObjectSettings wall1Settings = { "Exterior", "assets/meshes/cube.obj", {"assets/textures/Wall/images.jpg"}, glm::vec3(6, 3, 6), glm::vec3(0, -1, 0), glm::vec3(0,0,0) };
        ObjectSettings targetCubeSettings = { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, -3.5, -1), glm::vec3(87, 165,98) };
        LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(1, 0, 2) };


        wall.Init(wall1Settings, this);
        box.Init(boxSettings, this);
        pipes.Init(cubeSettings, this);
        pipe1.Init(pipe1Settings, this);
        targetCube.Init(targetCubeSettings, this);
        light.Init(lightSettings);


        Game::state.currentMission = 1;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Find Noah";
    };

    virtual void Update(float deltaTime) override
    {
        // FlickLight(deltaTime);

        targetCube.rotation.z += 7.5 * deltaTime;
    };

private:
    Model3D pistol;
    Model3D wall;
    Model3D targetCube;
    Model3D pipes;
    Model3D box;
    Model3D pipe1;

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
