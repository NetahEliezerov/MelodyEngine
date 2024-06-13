#pragma once

#ifndef DOOR_KEY_H
#define DOOR_KEY_H
#include <glm/glm.hpp>
#include <functional>
#include <algorithm>
#include <memory>
#include <any>

#include "../../Player/Player.h"
#include "../../World/Interactable.hpp"
#include "../../Game/Game.h"

class DoorKey {
public:

	void Init(ObjectSettings interactSettingsRec, int keyIdRec, WorldLevel* level, float* timeScaleRec)
	{
		character = Game::state.character;
		light = Game::state.character->light;
		timeScale = timeScaleRec;
		std::cout << "DOOR KEY INITIATED!" << std::endl;
		keyId = keyIdRec;
		interactSettings = interactSettingsRec;

		interactable1.Init(interactSettings, level, [this]() { OnLetterOpen(); }, character, 800, 100);
		level->sceneHierarchy.push_back(this);
	}

	void Update(float deltaTime)
	{
		interactable1.Update(character->movement.position, light, deltaTime);

		if (isOpen)
		{
			if (Input::inputState.keys[GLFW_KEY_TAB])
			{
				*timeScale = 1;
				isOpen = false;
				*Game::state.hideHud = false;
				interactable1.Destroy();
				// *character->hideHudButLetter = false;
			}
		}
	}

	void OnLetterOpen()
	{
		std::cout << "Got the Key" << std::endl;
		*timeScale = 0;
		isOpen = true;
		InventoryItem item = { keyId, interactSettings.label, InventoryKeyItemType };

		// Check if the item already exists in the inventory
		auto it = std::find_if(Game::state.inventory.begin(), Game::state.inventory.end(),
			[&](const InventoryItem& i) { return i.id == item.id; });

		// If the item is not found, append it to the inventory
		if (it == Game::state.inventory.end())
		{
			Game::state.inventory.push_back(item);
		}

		*Game::state.isShowingInventory = true;
		*Game::state.hideHud = true;
	}


	bool isOpen = false;
	float* timeScale;
	ObjectSettings interactSettings;
	int keyId;
	Interactable interactable1;
	Player* character = nullptr;
	LightPoint* light = nullptr;
};
#endif