#pragma once
#include <glm/glm.hpp>
#include <functional>
#include <memory>
#include <any>

#include "../Core/Model3D.h"
#include "../Player/Player.h"

#include "WorldLevel.h"

class Interactable {
public:
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotation;
    bool* isInInteractionZone;
    bool interactable = true;

    bool firstTimeAfterNot = true;

    float yOffset;

    bool isTriggered;
    bool canBeReTriggered;

    float radiusMultiplier = 1;
    std::function<void()> interactFunction;
    Model3D model;

public:

    void Destroy()
    {
        interactable = false;
        model.visibility = false;
        *isInInteractionZone = false;
    }

    void Init(ObjectSettings objectSettings, std::function<void()> func, Player* characterRec, float multiplierRadiusRec, float yOffsetRec)
    {
        position = objectSettings.recTransform;
        size = objectSettings.recScale;
        rotation = objectSettings.recRotation;
        interactFunction = func;
        radiusMultiplier = multiplierRadiusRec;
        yOffset = yOffsetRec;
        isInInteractionZone = characterRec->isInInteractionZone;

        ObjectSettings cubeSettings = {
            objectSettings.label,
            objectSettings.fileName,
            objectSettings.texturePaths,
            objectSettings.recIsLight,
            objectSettings.recColor,
            objectSettings.recScale * 0.35f,
            objectSettings.recTransform,
            objectSettings.recRotation,
            objectSettings.isDynamicRec,
            characterRec->shader
        };

        model.Init(cubeSettings);

        std::cout << "Interactable: " << this << std::endl;
    }

    void Update(const glm::vec3& playerPosition, LightPoint light, float deltaTime) {
        if (interactable)
        {
            if (IsPlayerInside(playerPosition)) {
                // HERE
                *isInInteractionZone = true;
                if (Input::inputState.keys[GLFW_KEY_E])
                {

                    auto currentTime = std::chrono::steady_clock::now();
                    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - timeSinceShoot).count();

                    const int delayBetweenShots = 1050;

                    if (elapsedTime >= delayBetweenShots) {
                        interactFunction();
                        timeSinceShoot = currentTime;
                    }
                }
                firstTimeAfterNot = true;
            }
            else
            {
                if (firstTimeAfterNot)
                {
                    *isInInteractionZone = false;
                }
                firstTimeAfterNot = false;
            }
        }

        model.transform = position;
        model.scale = size * 0.35f;
        model.rotation = rotation;
        model.Update(playerPosition, light, deltaTime);
    }

    bool IsPlayerInside(const glm::vec3& playerPosition) {
        //float halfWidth = size.x / 2.0f;
        //float halfHeight = size.y / 2.0f;
        //float halfDepth = size.z / 2.0f;
        float halfWidth = size.x / 2.0f * radiusMultiplier;
        float halfHeight = (size.y / 2.0f * radiusMultiplier) * yOffset;
        float halfDepth = size.z / 2.0f * radiusMultiplier;

        return (playerPosition.x >= position.x - halfWidth && playerPosition.x <= position.x + halfWidth &&
            playerPosition.y >= position.y - halfHeight && playerPosition.y <= position.y + halfHeight &&
            playerPosition.z >= position.z - halfDepth && playerPosition.z <= position.z + halfDepth);
    }

    void Reset() {
        isTriggered = false;
    }
private:
    std::chrono::steady_clock::time_point timeSinceShoot;
};