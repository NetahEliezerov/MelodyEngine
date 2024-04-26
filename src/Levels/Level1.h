#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>

#include "../Core/Renderer.h"
#include "../Core/Input.h"
#include "../Core/Model3D.h"
#include "../Player/Player.h"
#include "../Core/LightPoint.h"

#include "../World/WorldLevel.h"

class Level1 : public WorldLevel
{

public:
	virtual void Init(Renderer renderer, Player* playerPointer) override;
	virtual void Update(float deltaTime) override;

	virtual void Fire(Model3D* hitObject) override;

private:
	Model3D pistol;
	Model3D hand;
	Model3D cube;
	Model3D wall;
	Model3D wall2;
	Model3D wall3;
	Model3D wall4;
	Model3D targetCube;
	Model3D targetCube2;
	Model3D targetCube3;
	Model3D skybox;

	LightPoint light;
};

