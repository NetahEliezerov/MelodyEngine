#include "Level1.h"
#include <thread>

void Level1::Init(Renderer renderer, Player* playerPointer)
{
	character = playerPointer;
    playerPointer->level = this;
	ObjectSettings cubeSettings = { "Ground", "assets/meshes/cube.obj", "assets/textures/2.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(10, 0.2, 10), glm::vec3(0, -4, 0), glm::vec3(0,0,0), false, character->shader };
    
    // ObjectSettings skyBoxSettings = { "Skybox", "assets/meshes/plane.obj", "assets/textures/zizim.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(270,0,90), false, character->shader };
    ObjectSettings wall1Settings = { "Wall", "assets/meshes/plane.obj", "assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(7, 2, 4), glm::vec3(8, 0, 0), glm::vec3(90,0,90), true, character->shader };
    ObjectSettings wall2Settings = { "Wall", "assets/meshes/plane.obj", "assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(7, 2, 4), glm::vec3(-8, 0, 0), glm::vec3(90,0,270), true, character->shader};
    ObjectSettings wall3Settings = { "Wall", "assets/meshes/plane.obj", "assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(7, 2, 4), glm::vec3(0, 0, 8), glm::vec3(90,0,180), true, character->shader };
    ObjectSettings wall4Settings = { "Wall", "assets/meshes/plane.obj", "assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(7, 2, 4), glm::vec3(0, 0, -4), glm::vec3(90,0,0), true, character->shader };

    ObjectSettings targetCubeSettings = { "Something", "assets/meshes/hand.obj", "assets/textures/aga.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0, 0, 0), glm::vec3(0,0,0), true, character->shader };
    ObjectSettings targetCube2Settings = { "Target Cube", "assets/meshes/pistol.obj", "assets/textures/sp226-color-2.jpeg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.4, 0.4, 0.4), glm::vec3(2.5, 0, 0), glm::vec3(0,0,0), true, character->shader };
    ObjectSettings targetCube3Settings = { "Target Cube", "assets/meshes/cube.obj", "assets/textures/2.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.4, 0.4, 0.4), glm::vec3(5, 0, 0), glm::vec3(0,0,0), true, character->shader };

	LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(1, 1, 1), glm::vec3(0, 5, 2), character->shader };
	
    wall.Init(wall1Settings);
    wall2.Init(wall2Settings);
    wall3.Init(wall3Settings);
    wall4.Init(wall4Settings);

	cube.Init(cubeSettings);
    targetCube.Init(targetCubeSettings);
    targetCube2.Init(targetCube2Settings);
    targetCube3.Init(targetCube3Settings);
	light.Init(lightSettings);
    playerPointer->light = light;
    // skybox.Init(skyBoxSettings);
    sceneModels.push_back(&cube);
    sceneModels.push_back(&targetCube);
    sceneModels.push_back(&targetCube2);
    sceneModels.push_back(&targetCube3);
    sceneModels.push_back(&skybox);
}


void Level1::Update(float deltaTime)
{
    targetCube.Update(character->movement.position, light);
    wall.Update(character->movement.position, light);
    targetCube2.Update(character->movement.position, light);
    targetCube3.Update(character->movement.position, light);
    wall2.Update(character->movement.position, light);
    wall3.Update(character->movement.position, light);
    wall4.Update(character->movement.position, light);
    // skybox.Update(character->movement.position, light);
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