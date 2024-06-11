#include "TriggerBox.h"
#include "WorldLevel.h"

void TriggerBox::Init(WorldLevel* level, const glm::vec3& pos, const glm::vec3& sizeRec, std::function<void()> func, Player* character, bool canBeReTriggeredRec, bool showBoxRec)
{
    position = pos;
    size = sizeRec;
    triggerFunction = func;
    isTriggered = false;
    canBeReTriggered = canBeReTriggeredRec;
    showBox = showBoxRec;

    ObjectSettings cubeSettings = { "Ground", "assets/meshes/cube.obj", { "assets/textures/pngtree-ragged-edge-texture-wall-beige-torn-cardboard-with-unique-texture-image_13779231.jpg" }, sizeRec * 0.35f, pos, glm::vec3(0,0,0) };

    model.Init(cubeSettings, level);
    level->sceneHierarchy.push_back(this);
    std::cout << "TriggerBox: " << this << std::endl;
}

void TriggerBox::Update(const glm::vec3& playerPosition, LightPoint* light, float deltaTime) {
    if (!isTriggered && IsPlayerInside(playerPosition)) {
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
    model.visibility = showBox;
}

bool TriggerBox::IsPlayerInside(const glm::vec3& playerPosition) {
    float halfWidth = size.x / 2.0f;
    float halfHeight = size.y / 2.0f;
    float halfDepth = size.z / 2.0f;

    return (playerPosition.x >= position.x - halfWidth && playerPosition.x <= position.x + halfWidth &&
        playerPosition.y >= position.y - halfHeight && playerPosition.y <= position.y + halfHeight &&
        playerPosition.z >= position.z - halfDepth && playerPosition.z <= position.z + halfDepth);
}

void TriggerBox::Reset() {
    isTriggered = false;
}