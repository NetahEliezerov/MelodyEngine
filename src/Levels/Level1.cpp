#include "Level1.h"
#include <thread>
#include <random>
#include "../Game/Game.hpp"


#include "../Core/AudioManager.hpp"

// AudioManager audioSweetHeart;

void Level1::OnTriggerEntered()
{
    // audioSweetHeart.PlaySound(0);
    std::cout << "GOT OUT OF ROOM" << std::endl;
    Game::state.currentSubMission++;
}

void Level1::Init(Renderer renderer, Player* playerPointer)
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
    // ObjectSettings targetCube2Settings = { "Target Cube", "assets/meshes/pistol.obj", {"assets/textures/sp226-color-2.jpeg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.4, 0.4, 0.4), glm::vec3(2.5, 0, 0), glm::vec3(0,0,0), true, character->shader };
    ObjectSettings targetCube2Settings = { "Target Cube", "assets/meshes/Shotgun/Shotgun.fbx", {"assets/meshes/Shotgun/Shotgun_DefaultMaterial_BaseColor.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.007, 0.007, 0.007), glm::vec3(2.5, 0, 0), glm::vec3(0,0,0), true, character->shader };


    LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(0.15, 0.15, 0.15, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(0, 5, 2), character->shader };

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
    Game::state.currentMission = 1;

    triggerBox.Init(glm::vec3(-6.f, 0.0f, 7.f), glm::vec3(2.0f, 8.0f, 2.0f), [this]() { OnTriggerEntered(); }, character, true, false);
}

void Level1::Update(float deltaTime)
{
    if (light.flickLight)
    {
        FlickLight(deltaTime);
    }


    // audioSweetHeart.Update();


    // Update the trigger box
    triggerBox.Update(character->movement.position, light);

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
}

void Level1::Fire(Model3D* hitObject)
{
    if (hitObject != nullptr)
    {
        if (hitObject->label == "Target Cube")
        {
            hitObject->visibility = false;
        }
        std::cout << hitObject->label << std::endl;
    }
}

void Level1::FlickLight(float deltaTime)
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
