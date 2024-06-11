#include "WorldLevel.h"

#include "../Levels/Components/Letter.hpp"
#include "TriggerBox.h"
#include "../Player/Player.h"

void WorldLevel::Init(Renderer* rendererRec, std::function<void()> funcRec, Player* playerPointer, float* timeScaleRec)
{
    func = funcRec;
    renderer = rendererRec;
    character = playerPointer;
    timeScale = timeScaleRec;
    std::cout << "HELLO YA BEN ZONA " << playerPointer->fogDensity << std::endl;

    LevelStart();
    std::cout << "HEY, LEVEL START!q WITH " << playerPointer->light->transform.x << " " << playerPointer->light->transform.y << " " << playerPointer->light->transform.z << std::endl;

}

void WorldLevel::RenderUpdate(float deltaTime)
{
    for (std::any& item : sceneHierarchy)
    {
        if (std::any_cast<Model3D*>(&item))
        {
            Model3D* model = std::any_cast<Model3D*>(item);
            model->Update(character->movement.position, character->light, deltaTime);
        }
        if (std::any_cast<LightPoint*>(&item))
        {
            LightPoint* model = std::any_cast<LightPoint*>(item);
            model->Update(character->movement.position);
        }
        if (std::any_cast<Letter*>(&item))
        {
            Letter* model = std::any_cast<Letter*>(item);
            model->Update(deltaTime);
        }
        if (std::any_cast<TriggerBox*>(&item))
        {
            TriggerBox* model = std::any_cast<TriggerBox*>(item);
            model->Update(character->movement.position, &light, deltaTime);
        }
    }
}