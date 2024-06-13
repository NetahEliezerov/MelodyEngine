#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../Types.h"
// #include "../Player/Player.h"
class Player;

enum InventoryItemType
{
    InventoryKeyItemType = 0
};

struct InventoryItem
{
    int id;
    std::string label;
    InventoryItemType type;
};

class GameState
{
public:
    int currentMission = -1;
    int currentSubMission = -1;
    std::string currentObjective;

    Player* character = nullptr;

    std::vector<InventoryItem> inventory;

    bool* isShowingInventory;
    bool* hideHud;

    int WIDTH;
    int HEIGHT;

    LetterSettings* currentLetter = nullptr;
};

class Game
{
public:
    static GameState state;
};