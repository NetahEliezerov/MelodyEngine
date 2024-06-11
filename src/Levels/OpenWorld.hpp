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

class OpenWorld : public WorldLevel
{

public:

    virtual void LevelStart() override
    {
        character->level = this;
        character->light = &light;
        wall.Init({ "Wall", "assets/textures/Bricks/plane.obj", {"assets/textures/Bricks/castle_wall_slates_diff_4k.jpg"}, glm::vec3(3, 1, 10), glm::vec3(-6, -4, 0), glm::vec3(0,0,0) }, this);
        wall2.Init({ "Wall 2", "assets/textures/Bricks/plane.obj", {"assets/textures/Bricks/castle_wall_slates_diff_4k.jpg"}, glm::vec3(3, 1, 10), glm::vec3(0, -4, 0), glm::vec3(0,180,0) }, this);
        grass1.Init({ "Grass", "assets/textures/Grass/lowpoly_cartoon_grass.obj", {"assets/textures/Grass/20240406_084105.jpg"}, glm::vec3(0.25, 0.25, 0.25), glm::vec3(0, -4, 0), glm::vec3(0,0,0) }, this);
        tree1.Init({ "Tree 1", "assets/textures/Trees/oak_01.fbx", {"assets/textures/Trees/tree-bark-texture-detailed-close-up-of-a-textured-pattern-rough-surface-green-moss-lichen-and-natural-wood_9985369.jpg"}, glm::vec3(0.25, 0.25, 0.25), glm::vec3(0, -4, 0), glm::vec3(0,0,0)}, this);
        // cube.Init({ "Switch", "assets/meshes/cube.obj", {"assets/textures/Wall/pngtree-wood-grain-texture-wooden-flooring-design-with-wooden-floor-textures-image_13051712.jpg"}, glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 0), glm::vec3(0,0,0) }, this);
        
        light.Init({ "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(0, 3, 0) });
        
        Game::state.currentMission = 0;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Investigate";

        std::cout << character->movement.position.y << ", LEVEL START!q WITH " << light.transform.x << " " << light.transform.y << " " << light.transform.z << " " << std::endl;

    };

    virtual void Update(float deltaTime) override
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
    Interactable cube;
    Model3D wall;
    Model3D wall2;
    Model3D grass1;
    Model3D tree1;

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