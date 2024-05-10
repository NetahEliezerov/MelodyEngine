#pragma once
#include <string>

class GameState
{
public:
	int currentMission = -1;
	int currentSubMission = -1;
	std::string currentObjective;
};

class Game
{
public:
	static GameState state;
};
