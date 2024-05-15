#include <imgui.h>
#include "../World/WorldLevel.h"


void RenderModelTree(const char* label, int idx, Model3D* modelLocation, bool* opened = nullptr, ImGuiTreeNodeFlags flags = 0)
{
    if (ImGui::TreeNodeEx(label, flags))
    {
        ImGui::Text("Transform");
        ImGui::DragFloat(("X Transform " + idx), &modelLocation->transform.x);
        ImGui::DragFloat(("Y Transform " + idx), &modelLocation->transform.y);
        ImGui::DragFloat(("Z Transform " + idx), &modelLocation->transform.z);

        ImGui::Text("Rotation");
        ImGui::SliderFloat(("X Rotation " + idx), &modelLocation->rotation.x, 0, 360);
        ImGui::SliderFloat(("Y Rotation " + idx), &modelLocation->rotation.y, 0, 360);
        ImGui::SliderFloat(("Z Rotation " + idx), &modelLocation->rotation.z, 0, 360);

        ImGui::Text("Scale");
        ImGui::InputFloat(("X Scale " + idx), &modelLocation->scale.x);
        ImGui::InputFloat(("Y Scale " + idx), &modelLocation->scale.y);
        ImGui::InputFloat(("Z Scale " + idx), &modelLocation->scale.z);
        ImGui::TreePop();
    }
}

void RenderModelTree(const char* label, int idx, LightPoint* modelLocation, bool* opened = nullptr, ImGuiTreeNodeFlags flags = 0)
{
    if (ImGui::TreeNodeEx(label, flags))
    {

        ImGui::Text("Transform");
        ImGui::DragFloat(("X Transform" + idx), &modelLocation->transform.x);
        ImGui::DragFloat(("Y Transform" + idx), &modelLocation->transform.y);
        ImGui::DragFloat(("Z Transform" + idx), &modelLocation->transform.z);

        ImGui::TreePop();
    }
}

void ShowTreeExample(Player* player)
{
    ImGui::Begin("Tree Example");
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
                    std::string name("Light " + std::to_string(i));
                    RenderModelTree(name.c_str(), i, model);
                }
                catch (const std::bad_any_cast& e)
                {
                    std::cout << e.what() << '\n';
                }
            }
        }
    }

    ImGui::End();
}