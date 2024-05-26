#pragma once

#ifndef FOO_BAR_BsAZ_H_
#define FOO_BAR_BsAZ_H_
#include <glm/glm.hpp>
#include <functional>
#include <memory>
#include <any>

#include "../../Player/Player.h"

#include "../../World/Interactable.hpp"

#include "../../Game/Game.h"

class Letter {
public:

	void Init(LetterSettings letterSettingsRec, Renderer renderer, Player* playerPointer, LightPoint *lightRec, float* timeScaleRec, std::function<void()> func)
	{
		light = lightRec;
		character = playerPointer;
		timeScale = timeScaleRec;

		letterSettings = letterSettingsRec;

		letterOpenHandle = func;

		interactSettings = { "interact", "assets/meshes/Letter/Letter.FBX", {"assets/meshes/Letter/Letter.png"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.01, 0.01, 0.01), glm::vec3(2, -1.45, -5), glm::vec3(90, 0, -27), false, character->shader, false };
		
		interactable1.Init(interactSettings, [this]() { OnLetterOpen(); }, playerPointer, 325.0f, 200);


	}

	void Update(float deltaTime)
	{
		interactable1.Update(character->movement.position, *light, deltaTime);

		if (isOpen)
		{
			if (Input::inputState.keys[GLFW_KEY_ESCAPE])
			{
				*timeScale = 1;
				isOpen = false;
				*character->hideHudButLetter = false;
				Game::state.currentLetter = nullptr;
				letterOpenHandle();
			}
		}
	}

	void OnLetterOpen()
	{
		std::cout << "OPENED LETTER: " << letterSettings.title << std::endl;
		*timeScale = 0;
		isOpen = true;
		*character->hideHudButLetter = true;
		Game::state.currentLetter = &letterSettings;
	}


	bool isOpen = false;
	float* timeScale;
	std::function<void()> letterOpenHandle;
	LetterSettings letterSettings;
	ObjectSettings interactSettings;
	Interactable interactable1;
	Player* character = nullptr;
	LightPoint* light = nullptr;
};
#endif