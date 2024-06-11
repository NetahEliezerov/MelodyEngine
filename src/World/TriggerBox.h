#pragma once
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_
#include <glm/glm.hpp>
#include <functional>
#include <memory>
#include <any>

#include "../Core/Model3D.h"
#include "../Player/Player.h"

class TriggerBox {
public:
    glm::vec3 position;
    glm::vec3 size;
    std::function<void()> triggerFunction;
    bool isTriggered;
    bool canBeReTriggered;
    bool showBox;
    Model3D model;

public:
    void Init(WorldLevel* level, const glm::vec3& pos, const glm::vec3& sizeRec, std::function<void()> func, Player* character, bool canBeReTriggeredRec, bool showBoxRec);

    void Update(const glm::vec3& playerPosition, LightPoint* light, float deltaTime);

    bool IsPlayerInside(const glm::vec3& playerPosition);

    void Reset();
};
#endif