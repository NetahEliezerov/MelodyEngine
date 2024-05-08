
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "Core/Engine.h"
#include "Core/Renderer.h"
#include "Core/Input.h"
#include "Core/Model3D.h"
#include "Player/Player.h"
#include "Core/LightPoint.h"

#include "Core/Shader.hpp"

#include "Levels/Level1.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

int main(void) {
    GLFWwindow* window = Engine::Run();
    Renderer _renderer;
    const char* glsl_version = "#version 130";
    Level1 _level1;
    Player character;

    character.Init(_renderer, false);

    _level1.Init(_renderer, &character);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    float fps = 0;


    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glfwSwapInterval(1); // Enable vsync

    GLFWwindow* imgui_window = nullptr;
    bool show_demo_window = true;
    bool show_another_window = false;

    bool show = true;

    if (show)
    {
        // Our state
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        imgui_window = glfwCreateWindow(800, 600, "Dear ImGui Window", NULL, NULL);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(imgui_window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        glfwSetKeyCallback(imgui_window, ImGui_ImplGlfw_KeyCallback);
        glfwSetCharCallback(imgui_window, ImGui_ImplGlfw_CharCallback);
        glfwSetMouseButtonCallback(imgui_window, ImGui_ImplGlfw_MouseButtonCallback);
        glfwSetScrollCallback(imgui_window, ImGui_ImplGlfw_ScrollCallback);
    }

    Model3D* modelLocation = nullptr;
    TriggerBox* triggerLocation = nullptr;
    LightPoint* lightLocation = nullptr;
    Player* playerLocation = nullptr;

    static char inputAdd[256] = "";
    static const char* items[]{ "TriggerBox", "Model3D", "LightPoint", "Fog" };
    static int Selecteditem = 0;

    static float col1[3] = { 1.f, 1.f, 1.f };
    static float fog[3] = { 1.f, 1.f, 1.f };
    bool isLightOn = true;
    bool isFogOn = true;

    // !glfwWindowShouldClose(imgui_window)

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps = 1000 / deltaTime / 1000;

        // Process events for both windows
        glfwPollEvents();

        // Switch to the Dear ImGui window
        if (show)
        {
            glfwMakeContextCurrent(imgui_window);
            ImGui::SetCurrentContext(ImGui::GetCurrentContext());
            glClear(GL_COLOR_BUFFER_BIT);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (show_demo_window)
            {
                ImGui::Begin("Another Window", &show_another_window);
                bool check = ImGui::Combo("MyCombo", &Selecteditem, items, IM_ARRAYSIZE(items));
                std::string asdasad(std::to_string(fps));
                ImGui::Text(asdasad.c_str());

                ImGui::InputText("Memory Address", inputAdd, 256);

                switch (Selecteditem)
                {
                case 3:
                    if (ImGui::Button("Update Pointer"))
                    {
                        std::stringstream ss;
                        ss << std::hex << inputAdd;
                        ss >> reinterpret_cast<std::uintptr_t&>(playerLocation);
                    }

                    if (playerLocation != nullptr)
                    {
                        ImGui::DragFloat("Density", &playerLocation->fogDensity);
                        ImGui::ColorPicker4("Color", fog);
                        playerLocation->fogColor.x = fog[0];
                        playerLocation->fogColor.y = fog[1];
                        playerLocation->fogColor.z = fog[2];
                    }

                    break;
                case 2:
                    if (ImGui::Button("Update Pointer"))
                    {
                        std::stringstream ss;
                        ss << std::hex << inputAdd;
                        ss >> reinterpret_cast<std::uintptr_t&>(lightLocation);
                    }
                    if (lightLocation != nullptr)
                    {

                        ImGui::Checkbox("On/Off", &isLightOn);
                        ImGui::Checkbox("Flicker Light", &lightLocation->flickLight);

                        ImGui::Text("Transform");
                        ImGui::DragFloat("X Transform", &lightLocation->transform.x);
                        ImGui::DragFloat("Y Transform", &lightLocation->transform.y);
                        ImGui::DragFloat("Z Transform", &lightLocation->transform.z);
                        ImGui::ColorPicker3("Color", col1);


                        if (isLightOn)
                        {
                            lightLocation->color = glm::vec4(col1[0], col1[1], col1[2], 1.f);
                        }
                        else
                        {
                            lightLocation->color = glm::vec4(0.f, 0.f, 0.f, 0.f);
                        }
                    }
                    break;
                case 1:
                    if (ImGui::Button("Update Pointer"))
                    {
                        std::stringstream ss;
                        ss << std::hex << inputAdd;
                        ss >> reinterpret_cast<std::uintptr_t&>(modelLocation);
                    }

                    if (modelLocation != nullptr)
                    {
                        ImGui::Text("Transform");
                        ImGui::InputFloat("X Transform", &modelLocation->transform.x);
                        ImGui::InputFloat("Y Transform", &modelLocation->transform.y);
                        ImGui::InputFloat("Z Transform", &modelLocation->transform.z);

                        ImGui::Text("Rotation");
                        ImGui::SliderFloat("X Rotation", &modelLocation->rotation.x, 0, 360);
                        ImGui::SliderFloat("Y Rotation", &modelLocation->rotation.y, 0, 360);
                        ImGui::SliderFloat("Z Rotation", &modelLocation->rotation.z, 0, 360);

                        ImGui::Text("Scale");
                        ImGui::InputFloat("X Scale", &modelLocation->scale.x);
                        ImGui::InputFloat("Y Scale", &modelLocation->scale.y);
                        ImGui::InputFloat("Z Scale", &modelLocation->scale.z);
                    }
                    break;
                case 0:
                    if (ImGui::Button("Update Pointer"))
                    {
                        std::stringstream ss;
                        ss << std::hex << inputAdd;
                        ss >> reinterpret_cast<std::uintptr_t&>(triggerLocation);
                    }

                    if (triggerLocation != nullptr)
                    {
                        ImGui::Text("Transform");
                        ImGui::InputFloat("X Transform", &triggerLocation->position.x);
                        ImGui::InputFloat("Y Transform", &triggerLocation->position.y);
                        ImGui::InputFloat("Z Transform", &triggerLocation->position.z);

                        ImGui::Text("Scale");
                        ImGui::InputFloat("X Scale", &triggerLocation->size.x);
                        ImGui::InputFloat("Y Scale", &triggerLocation->size.y);
                        ImGui::InputFloat("Z Scale", &triggerLocation->size.z);
                    }
                    break;
                }

                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(imgui_window);

            glfwMakeContextCurrent(window);

            glfwPollEvents();
        }
        
        // Clear the color and depth buffers for the main window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render your OpenGL content in the main window
        character.Update(deltaTime);
        _level1.Update(deltaTime);

        if (Input::inputState.keys[GLFW_KEY_F])
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (Input::inputState.keys[GLFW_KEY_G])
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDepthFunc(GL_LESS);
        glfwSwapBuffers(window);
    }
    if (show)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(imgui_window);
    }

    glDeleteProgram(character.shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
