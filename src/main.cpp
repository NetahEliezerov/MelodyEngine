
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
#include "Core/TextRenderer.hpp"
#include "Core/LightPoint.h"

#include "Levels/Level1.h"

#include "World/SkyBox.h"

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

    Model3D* location = nullptr;
    static char inputAdd[256] = "";
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
                std::string asdasad(std::to_string(fps));
                ImGui::Text(asdasad.c_str());

                ImGui::InputText("Memory Address", inputAdd, 256);
                if (ImGui::Button("Update Pointer"))
                {
                    std::stringstream ss;
                    ss << std::hex << inputAdd;
                    ss >> reinterpret_cast<std::uintptr_t&>(location);
                }

                if (location != nullptr)
                {
                    ImGui::Text("Transform");
                    ImGui::SliderFloat("X Transform", &location->transform.x, -10, 10);
                    ImGui::SliderFloat("Y Transform", &location->transform.y, -10, 10);
                    ImGui::SliderFloat("Z Transform", &location->transform.z, -10, 10);

                    ImGui::Text("Rotation");
                    ImGui::SliderFloat("X Rotation", &location->rotation.x, 0, 360);
                    ImGui::SliderFloat("Y Rotation", &location->rotation.y, 0, 360);
                    ImGui::SliderFloat("Z Rotation", &location->rotation.z, 0, 360);

                    ImGui::Text("Scale");
                    ImGui::SliderFloat("X Scale", &location->scale.x, 0, 1);
                    ImGui::SliderFloat("Y Scale", &location->scale.y, 0, 1);
                    ImGui::SliderFloat("Z Scale", &location->scale.z, 0, 1);
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
