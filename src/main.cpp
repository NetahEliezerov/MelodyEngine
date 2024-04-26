
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

#include <imgui_impl_glfw.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "World/SkyBox.h"

std::string readShaderSource(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
};

int main(void) {
    GLFWwindow* window = Engine::Run();
    Renderer _renderer;

    Level1 _level1;
    Player character;

    character.Init(_renderer, false);

    _level1.Init(_renderer, &character);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    float fps = 0;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
    bool show_demo_window = true;


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps = 1000 / deltaTime / 1000;

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render your OpenGL content
        character.Update(deltaTime);
        _level1.Update(deltaTime);

        if (Input::inputState.keys[GLFW_KEY_F])
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (Input::inputState.keys[GLFW_KEY_G])
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        glDepthFunc(GL_LESS);


        // Render the ImGui draw data
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteProgram(character.shader);
    glfwTerminate();
    return 0;
}