#include "CharacterNPC.h"

void CharacterNPC::Init(Renderer renderer, Player* playerPointer, bool followPlayerRec)
{
    character = playerPointer;
    followPlayer = followPlayerRec;
    ObjectSettings targetCube3Settings = { "Target Cube", "assets/meshes/Oskar_Gear_LoRes_Mesh.fbx", {
        "assets/textures/Character/Oskar_Gear_LoRes_Mesh_Eye_MAT_BaseColor.jpg",
        "assets/textures/Character/Oskar_Gear_LoRes_Mesh_Skin_MAT_BaseColor.jpg",
        "assets/textures/Character/Oskar_Gear_LoRes_Mesh_Shirt_MAT_BaseColor.jpg",
        "assets/textures/Character/Oskar_Gear_LoRes_Mesh_Jeans_MAT_BaseColor.jpg",
        //"assets/textures/Character/Oskar_Gear_LoRes_Mesh_shoes_MAT_BaseColor.jpg",
    }, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.03, 0.03, 0.03), glm::vec3(5, 0, 0), glm::vec3(0,0,0), true, character->shader };
    targetCube3.Init(targetCube3Settings);
    std::cout << &character << std::endl;
}

void CharacterNPC::Update(float deltaTime, LightPoint light, bool followPlayerRec)
{
    //targetCube3.Update(character->movement.position, light, leve);
    followPlayer = followPlayerRec;
    if (followPlayer)
    {
        glm::vec3 direction = character->movement.position - targetCube3.transform;
        float distance = glm::length(direction);

        if (distance > followDistance)
        {
            direction = glm::normalize(direction);
            glm::vec3 movement = direction * moveSpeed * deltaTime;
            targetCube3.transform += movement;
        }

        // Calculate the desired rotation to look towards the player
        glm::vec3 targetDirection = glm::normalize(character->movement.position - targetCube3.transform);
        targetDirection.y = 0.0f; // Ignore vertical component for rotation

        // Calculate the rotation angle around the Y-axis
        float angle = atan2(targetDirection.x, targetDirection.z);

        // Update the CharacterNPC's rotation
        targetCube3.rotation.y = glm::degrees(angle);

        // Adjust the CharacterNPC's vertical position to match the ground level
        float groundLevel = -1; // Adjust this value based on your ground level
        targetCube3.transform.y = groundLevel;
    }
}