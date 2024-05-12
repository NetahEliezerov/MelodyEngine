#pragma once
#include <string>
#include "../Types.h"

class GameState
{
public:
	int currentMission = -1;
	int currentSubMission = -1;
	std::string currentObjective;

	LetterSettings* currentLetter = nullptr;
};

class Game
{
public:
	static GameState state;
};
