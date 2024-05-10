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


class Level1 : public WorldLevel
{

public:

	virtual void Init(Renderer renderer, Player* playerPointer) override;
	virtual void Update(float deltaTime) override;

	virtual void Fire(Model3D* hitObject) override;

	void OnRoomExit();
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

	glm::mat4 lightSpaceMatrix;

	ShadowMap shadowMap;

	bool lightOn = true;
	float flickerTimer = 0.0f;
	float flickerInterval = 0.2f; // Adjust this value to control the flickering speed

	LightPoint light;

	void FlickLight(float deltaTime);
};

