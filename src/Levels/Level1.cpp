#include "Level1.h"

void Level1::Init(Renderer renderer, Player* playerPointer)
{
	character = playerPointer;
	ObjectSettings cubeSettings = { "assets/meshes/cube.obj", "assets/textures/2.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(15, 0.2, 15), glm::vec3(0, -4, 0), glm::vec3(0,0,0), true, character->shader };
	
	ObjectSettings handSettings = { "assets/meshes/hand.obj", "assets/textures/2.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.13f, -0.75f, 0.66), glm::vec3(7,196,104.5f), true, character->shader };
	
	//     * assets/meshes/Pistol/M_Pistol_PM_BaseColor.jpg
	// ObjectSettings pistolSettings = { "assets/meshes/Pistol/m1911.obj", "assets/textures/2.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(-0.1f, -1.12f, 0.56f), glm::vec3(11.f,90.44f,-0.57f), true, character->shader };
	ObjectSettings pistolSettings = { "assets/meshes/Pistol/2.obj", "assets/meshes/Pistol/M_Pistol_PM_BaseColor.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(18,18,18), glm::vec3(-0.255, -0.70, 0.85), glm::vec3(0,200,-1), true, character->shader };

	LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/2.jpg", glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(1, 1, 1), glm::vec3(5, 5, 5), character->shader };
	
	pistol.Init(pistolSettings);
	cube.Init(cubeSettings);
	light.Init(lightSettings);
	hand.Init(handSettings);
}

void Level1::Update(float deltaTime)
{

	hand.SetPosition(character->movement.position, character->movement.lookingAngle, character->movement.cameraUp, 1.8f, .5f, 0);
	hand.Update(character->movement.position, light);

	pistol.SetPosition(character->movement.position, character->movement.lookingAngle, character->movement.cameraUp, 2.6f, .5f, 0);
	pistol.Update(character->movement.position, light);
	
	cube.Update(character->movement.position, light);
	light.Update(character->movement.position);
}
