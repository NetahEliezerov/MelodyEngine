#include "WorldLevel.h"

#include "../Levels/Components/Letter.hpp"
//
//#include "../Core/Model3D.h"
//
//#include "../Core/LightPoint.h"
//
#include "TriggerBox.h"

void WorldLevel::RenderUpdate(float deltaTime)
{
    for (std::any& item : sceneHierarchy)
    {
        if (std::any_cast<Model3D*>(&item))
        {
            Model3D* model = std::any_cast<Model3D*>(item);
            model->Update(character->movement.position, *character->light, deltaTime);
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
            model->Update(character->movement.position, *character->light, deltaTime);
        }
    }
}