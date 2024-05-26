#include <imgui.h>
#include "../World/WorldLevel.h"


static glm::vec3 ReactToGLM(Vector3 rec)
{
    return { rec.x, rec.y, rec.z };
}
static Vector3 GLMToReact(glm::vec3 rec)
{
    return { rec.x, rec.y, rec.z };
}

static Quaternion GLMToQuaternion(glm::vec3 rec)
{
    return Quaternion(rec.x, rec.y, rec.z, 1);
}

void RenderModelTree(const char* label, int idx, Model3D* modelLocation, bool* opened = nullptr, ImGuiTreeNodeFlags flags = 0)
{
    if (ImGui::TreeNodeEx(label, flags))
    {
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
    }
}

void RenderLightTree(const char* label, int idx, LightPoint* modelLocation, bool* opened = nullptr, ImGuiTreeNodeFlags flags = 0)
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
    }

    ImGui::End();
}