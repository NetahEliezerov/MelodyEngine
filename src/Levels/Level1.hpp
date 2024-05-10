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

#include "../Core/ShadowMap.h"

#include "../AI/CharacterNPC.h"

#include "../World/WorldLevel.h"
#include "../World/TriggerBox.h"
#include "../World/Interactable.hpp"


#include "../Core/AudioManager.hpp"

class Level1 : public WorldLevel
{

public:

	virtual void Init(Renderer renderer, Player* playerPointer) override
    {
        character = playerPointer;
        playerPointer->level = this;
        ObjectSettings cubeSettings = { "Ground", "assets/meshes/cube.obj", {"assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(10, 0.2, 10), glm::vec3(0, -4, 0), glm::vec3(0,0,0), false, character->shader };

        ObjectSettings wall1Settings = { "Wall", "assets/meshes/plane.obj", {"assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(7, 2, 4), glm::vec3(8, 0, 1), glm::vec3(90,0,90), true, character->shader };
        ObjectSettings wall2Settings = { "Wall", "assets/meshes/plane.obj", {"assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(6, 2, 4), glm::vec3(-6, 0, 0), glm::vec3(90,0,270), true, character->shader };
        ObjectSettings wall3Settings = { "Wall", "assets/meshes/plane.obj", {"assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(7, 2, 4), glm::vec3(1, 0, 8), glm::vec3(90,0,180), true, character->shader };
        ObjectSettings wall4Settings = { "Wall", "assets/meshes/plane.obj", {"assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(8, 2, 4), glm::vec3(0, 0, -6), glm::vec3(90,0,0), true, character->shader };

        ObjectSettings wall5Settings = { "Wall", "assets/meshes/plane.obj", {"assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(8, 1, 7), glm::vec3(2, 4, 1.1), glm::vec3(180,0,0), true, character->shader };

        ObjectSettings targetCubeSettings = { "Something", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0, 0, 0), glm::vec3(0,0,0), true, character->shader };
        ObjectSettings targetCube2Settings = { "Target Cube", "assets/meshes/Shotgun/Shotgun.fbx", {"assets/meshes/Shotgun/Shotgun_DefaultMaterial_BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.007, 0.007, 0.007), glm::vec3(2.5, 0, 0), glm::vec3(0,0,0), true, character->shader };


        LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(0.25, 0.25, 0.25, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(0, 5, 2), character->shader };

        ObjectSettings interactSettings = { "interact", "assets/meshes/cube.obj", {"assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(2, 2, 2), glm::vec3(2, 0, 0), glm::vec3(0,0,0), false, character->shader };


        wall.Init(wall1Settings);
        wall2.Init(wall2Settings);
        wall3.Init(wall3Settings);
        wall4.Init(wall4Settings);
        wall5.Init(wall5Settings);

        // npc.Init(renderer, character, true);

        cube.Init(cubeSettings);
        targetCube.Init(targetCubeSettings);
        targetCube2.Init(targetCube2Settings);
        light.Init(lightSettings);
        playerPointer->light = light;
        sceneModels.push_back(&cube);
        sceneModels.push_back(&targetCube);
        sceneModels.push_back(&targetCube2);

        // audioSweetHeart.LoadSound("assets/sounds/sweetheart.wav");

        std::cout << "Wall 1: " << &wall << std::endl;
        std::cout << "Wall 2: " << &wall2 << std::endl;
        std::cout << "Wall 3: " << &wall3 << std::endl;
        std::cout << "Wall 4: " << &wall4 << std::endl;
        std::cout << "Wall 5: " << &wall5 << std::endl;
        std::cout << "Light: " << &light << std::endl;
        std::cout << "Flick: " << &light.flickLight << std::endl;

        Game::state.currentMission = 0;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Find the way out";
        interactable1.Init(interactSettings, [this]() { OnBoxInteract(); }, character, 2.0f);
        triggerBox.Init(glm::vec3(-6.f, 0.0f, 7.f), glm::vec3(2.0f, 8.0f, 2.0f), [this]() { OnRoomExit(); }, character, false, false);
    };

	virtual void Update(float deltaTime) override
    {
        if (light.flickLight)
        {
            FlickLight(deltaTime);
        }


        // audioSweetHeart.Update();


        triggerBox.Update(character->movement.position, light);
        interactable1.Update(character->movement.position, light);

        targetCube.Update(character->movement.position, light);
        wall.Update(character->movement.position, light);
        targetCube2.Update(character->movement.position, light);
        // npc.Update(deltaTime, light, true);
        wall2.Update(character->movement.position, light);
        wall3.Update(character->movement.position, light);
        wall4.Update(character->movement.position, light);
        wall5.Update(character->movement.position, light);
        cube.Update(character->movement.position, light);
        light.Update(character->movement.position);

    };

    virtual void Fire(Model3D* hitObject) override
    {
    };

    void OnBoxInteract()
    {
        std::cout << "Interact with box!" << std::endl;
        interactable1.Destroy();
    }

	void OnRoomExit()
	{
		// audioSweetHeart.PlaySound(0);
		if (Game::state.currentSubMission == 0)
		{
			std::cout << "GOT OUT OF ROOM" << std::endl;
			Game::state.currentSubMission++;
			Game::state.currentObjective = "Great!";
		}
    };
private:
	Model3D pistol;
	Model3D hand;
	Model3D cube;
	Model3D wall;
	Model3D wall2;
	Model3D wall3;
	Model3D wall4;
	Model3D wall5;
	Model3D targetCube;
	Model3D targetCube2;


	CharacterNPC npc;

	TriggerBox triggerBox;

    Interactable interactable1;

	glm::mat4 lightSpaceMatrix;

	ShadowMap shadowMap;

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
