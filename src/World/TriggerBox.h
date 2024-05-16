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
    void Init(const glm::vec3& pos, const glm::vec3& sizeRec, std::function<void()> func, Player* character, bool canBeReTriggeredRec, bool showBoxRec)
    {
        position = pos;
        size = sizeRec;
        triggerFunction = func;
        isTriggered = false;
        canBeReTriggered = canBeReTriggeredRec;
        showBox = showBoxRec;

        ObjectSettings cubeSettings = { "Ground", "assets/meshes/cube.obj", { "assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg" }, true, glm::vec4(1.f, 1.f, 1.f, 1.f), sizeRec * 0.35f, pos, glm::vec3(0,0,0), false, character->shader };

        model.Init(cubeSettings);

        std::cout << "TriggerBox: " << this << std::endl;
    }

    void Update(const glm::vec3& playerPosition, LightPoint light, float deltaTime) {
        if (!isTriggered && IsPlayerInside(playerPosition)) {
            // HERE
            triggerFunction();
            isTriggered = true;
        }

        if (canBeReTriggered)
        {
            if (!IsPlayerInside(playerPosition))
            {
                isTriggered = false;
            }
        }

        model.transform = position;
        model.scale = size * 0.4f;
        model.Update(playerPosition, light, deltaTime);
        model.visibility = showBox;
    }

    bool IsPlayerInside(const glm::vec3& playerPosition) {
        float halfWidth = size.x / 2.0f;
        float halfHeight = size.y / 2.0f;
        float halfDepth = size.z / 2.0f;

        return (playerPosition.x >= position.x - halfWidth && playerPosition.x <= position.x + halfWidth &&
            playerPosition.y >= position.y - halfHeight && playerPosition.y <= position.y + halfHeight &&
            playerPosition.z >= position.z - halfDepth && playerPosition.z <= position.z + halfDepth);
    }

    void Reset() {
        isTriggered = false;
    }
};
#endif