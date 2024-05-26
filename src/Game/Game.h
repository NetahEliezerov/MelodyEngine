#pragma once
#include <string>
#include <reactphysics3d/reactphysics3d.h>
#include "../Types.h"

class GameState
{
public:
	int currentMission = -1;
	int currentSubMission = -1;
	std::string currentObjective;

	LetterSettings* currentLetter = nullptr;

	reactphysics3d::PhysicsWorld* world;
	reactphysics3d::PhysicsCommon* common;
};

class Game
{
public:
	static GameState state;
};
