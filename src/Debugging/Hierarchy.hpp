#include <imgui.h>
#include "../World/WorldLevel.h"

static bool isAddingNewModel = false;
static char* newModelLabel;
static char* newModelTexture;
static char* newModelPath;

void RenderModelTree(const char* label, int idx, Model3D* modelLocation, bool* opened = nullptr, ImGuiTreeNodeFlags flags = 0)
{
    if (ImGui::TreeNodeEx(label, flags))
    {
        modelLocation->isSelected = true;
        int index = 1;
        ImGui::Text("Offset");
        ImGui::DragFloat(("O X##" + std::to_string(idx)).c_str(), &modelLocation->colliderOffsetX);

        ImGui::Text("Transform");
        ImGui::DragFloat(("T X##" + std::to_string(idx)).c_str(), &modelLocation->transform.x);
        ImGui::DragFloat(("T Y##" + std::to_string(idx)).c_str(), &modelLocation->transform.y);
        ImGui::DragFloat(("T Z##" + std::to_string(idx)).c_str(), &modelLocation->transform.z);

        ImGui::Text("Rotation");
        ImGui::SliderFloat(("R X##" + std::to_string(idx)).c_str(), &modelLocation->rotation.x, 0, 360);
        ImGui::SliderFloat(("R Y##" + std::to_string(idx)).c_str(), &modelLocation->rotation.y, 0, 360);
        ImGui::SliderFloat(("R Z##" + std::to_string(idx)).c_str(), &modelLocation->rotation.z, 0, 360);

        ImGui::Text("Scale");
        ImGui::InputFloat(("S X##" + std::to_string(idx)).c_str(), &modelLocation->scale.x);
        ImGui::InputFloat(("S Y##" + std::to_string(idx)).c_str(), &modelLocation->scale.y);
        ImGui::InputFloat(("S Z##" + std::to_string(idx)).c_str(), &modelLocation->scale.z);
        ImGui::TreePop();
    } else
        modelLocation->isSelected = false;
}
static const char* items[]{ "Specular","Spotlight" };
static int Selecteditem = 1;
static bool firstTimeSelecting = true;
void RenderLightTree(const char* label, int idx, LightPoint* modelLocation, bool* opened = nullptr, ImGuiTreeNodeFlags flags = 0)
{
    if (ImGui::TreeNodeEx(label, flags))
    {
        bool check = ImGui::Combo("Light Type", &Selecteditem, items, IM_ARRAYSIZE(items));
        
        if (firstTimeSelecting)
        {
            modelLocation->isSpotlight ? Selecteditem = 1 : Selecteditem = 0;
            firstTimeSelecting = false;
        }

        if (Selecteditem == 0)
        {
            modelLocation->isSpotlight = false;
        } else
        {
            modelLocation->isSpotlight = true;
        }

        ImGui::Text("Transform");
        ImGui::DragFloat(("X ##" + std::to_string(idx)).c_str(), &modelLocation->transform.x);
        ImGui::DragFloat(("Y ##" + std::to_string(idx)).c_str(), &modelLocation->transform.y);
        ImGui::DragFloat(("Z ##" + std::to_string(idx)).c_str(), &modelLocation->transform.z);

        if (modelLocation->isSpotlight)
        {
            ImGui::Text("Light Transform");
            ImGui::DragFloat(("Spotlight X ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightPos.x);
            ImGui::DragFloat(("Spotlight Y ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightPos.y);
            ImGui::DragFloat(("Spotlight Z ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightPos.z);
            ImGui::Text("Light Direction");
            ImGui::DragFloat(("Direction X ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightDir.x);
            ImGui::DragFloat(("Direction Y ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightDir.y);
            ImGui::DragFloat(("Direction Z ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightDir.z);
            ImGui::Text("Light Cutoff");
            ImGui::DragFloat(("Inner Cutoff ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightCutoff);
            ImGui::DragFloat(("Outer Cutoff Y ##" + std::to_string(idx)).c_str(), &modelLocation->spotLightOuterCutoff);
        }

        ImGui::TreePop();
    }
}

void ShowTreeExample(Player* player)
{
    ImGui::Begin("Hierarchy");
    int i = 0;
    for (std::any& item : player->level->sceneHierarchy)
    {
        i++;

        std::string asdasd(item.type().name());

        if (asdasd.find("class Model3D") != std::string::npos)
        {
            if (item.has_value())
            {
                try
                {
                    Model3D* model = std::any_cast<Model3D*>(item);
                    std::string name(std::to_string(i) + ": " + model->label);
                    RenderModelTree(name.c_str(), i, model);
                }
                catch (const std::bad_any_cast& e)
                {
                    std::cout << e.what() << '\n';
                }
            }
        } else if (asdasd.find("class LightPoint") != std::string::npos)
        {
            if (item.has_value())
            {
                try
                {
                    LightPoint* model = std::any_cast<LightPoint*>(item);
                    std::string name(std::to_string(i) + ": Light");
                    RenderLightTree(name.c_str(), i, model);
                }
                catch (const std::bad_any_cast& e)
                {
                    std::cout << e.what() << '\n';
                }
            }
        }

        if (player->level->sceneHierarchy.size() == i)
        {
            if (isAddingNewModel)
            {
                ImGui::InputText("Model Label", newModelLabel, sizeof(newModelLabel));
                ImGui::InputText("Model Path", newModelPath, sizeof(newModelPath));
                ImGui::InputText("Model Texture", newModelTexture, sizeof(newModelTexture));
                if (ImGui::Button("Add"))
                {
                    isAddingNewModel = !isAddingNewModel;
                    
                    ObjectSettings handSettings = { newModelLabel, newModelPath, {newModelTexture}, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0,0) };
                    Model3D hands;
                    //hands.Init(handSettings);
                    //player->level->sceneHierarchy.push_back(hands);
                }
            }
        }
    }

    ImGui::End();
}