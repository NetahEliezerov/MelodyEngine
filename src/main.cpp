#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Core/Engine.h"
#include "Core/Renderer.h"
#include "Core/Input.h"
#include "Core/Model3D.h"
#include "Player/Player.h"
#include "Core/LightPoint.h"


#include "Core/Shader.hpp"

#include "Levels/LevelManager.hpp"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include <ft2build.h>
#include FT_FREETYPE_H


unsigned int VBO, VAO;

struct Character {
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};


static std::map<char, Character> characters;

void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color, Shader& shader) {
    shader.use();
    shader.setVec3("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float xPos = x;
    for (const auto& c : text) {
        Character ch = characters[c];

        float xpos = xPos + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        xPos += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void setupTextRendering() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void loadFont(const std::string& fontPath, int fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}


unsigned int quadVAO, quadVBO;

void setupQuad() {
    float quadVertices[] = {
        // positions   // texture Coords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void renderQuad() {
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

int main(void) {
    GLFWwindow* window = Engine::Run();
    Renderer _renderer;
    const char* glsl_version = "#version 130";


    bool isInInteractionZone = false;

    LevelManager levelManager;
    Player character;

    unsigned int* playerShader = nullptr;

    character.Init(_renderer, false, playerShader, &isInInteractionZone);

    levelManager.GameStart(_renderer, &character);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    float fps = 0;

    setupQuad();

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glfwSwapInterval(1); // Enable vsync

    GLFWwindow* imgui_window = nullptr;
    bool show_demo_window = true;
    bool show_another_window = false;

    Shader textShader("shaders/postprocess/vertex.glsl", "shaders/postprocess/fragment.glsl");

    setupTextRendering();
    loadFont("assets/fonts/typewriter.ttf", 48);

    bool show = true;
    textShader.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1920), 0.0f, static_cast<float>(1080));
    textShader.setMat4("projection", projection);
    textShader.setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Set text color to white
    



    // Create FBO
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create texture attachment
    unsigned int fboTexture;
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    // Create depth buffer for FBO
    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


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
    static const char* items[]{ "TriggerBox", "Model3D", "LightPoint", "Fog", "Post Process"};
    static int Selecteditem = 0;

    float vignetteIntensity = 1.5f;
    float vignetteRadius = 0.3f;
    float vignetteSmooth = -0.75f;

    float bloomIntensity = .8f;
    float gammaIntensity = 1.f;

    float grainIntensity = 0.05f;
    float grainSize = 0.01f;
    static float col1[3] = { 1.f, 1.f, 1.f };
    static float fog[3] = { 1.f, 1.f, 1.f };
    bool isLightOn = true;
    bool isFogOn = true;
    // !glfwWindowShouldClose(imgui_window)
    Shader vignetteShader("shaders/fbo_vertex.glsl", "shaders/fbo_fragment.glsl");
    vignetteShader.setVec2("screenSize", glm::vec2(1920.0f, 1080.0f));

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
                case 4:
                    ImGui::Text("Bloom");
                    ImGui::DragFloat("Bloom Intensity", &bloomIntensity);
                    ImGui::DragFloat("Gamma", &gammaIntensity);

                    ImGui::Text("Vignette");
                    ImGui::DragFloat("Vignette Intensity", &vignetteIntensity);
                    ImGui::DragFloat("Radius", &vignetteRadius);
                    ImGui::DragFloat("Smooth", &vignetteSmooth);

                    ImGui::Text("Grain");
                    ImGui::DragFloat("Grain Intensity", &grainIntensity);

                    ImGui::Text("Chromatic Abberation");
                    ImGui::DragFloat("Chromatism", &grainSize);
                    break;
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

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        character.Update(deltaTime);
        levelManager.GameUpdate(deltaTime);

        glDisable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Clear the default framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render quad with vignette shader
        vignetteShader.use();
        vignetteShader.setInt("screenTexture", 0);
        vignetteShader.setFloat("vignetteIntensity", vignetteIntensity);
        vignetteShader.setFloat("vignetteRadius", vignetteRadius);
        vignetteShader.setFloat("vignetteSmooth", vignetteSmooth);
        vignetteShader.setFloat("exposure", bloomIntensity);
        vignetteShader.setFloat("gamma", gammaIntensity);
        vignetteShader.setFloat("time", glfwGetTime());
        vignetteShader.setFloat("grainIntensity", grainIntensity);
        vignetteShader.setFloat("chromaticAberrationOffset", grainSize);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboTexture);

        renderQuad();

        // Enable blending for text rendering
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        renderText("Objective: " + Game::state.currentObjective, 50.0f, 50.0f, 0.75f, glm::vec3(1.0f, 1.0f, 1.0f), textShader);

        if (isInInteractionZone)
        {
            renderText("Press E", 1920/2 - 100, 1080 / 2, 1.f, glm::vec3(1.0f, 1.0f, 1.0f), textShader);
        }
        // Disable blending after text rendering
        glDisable(GL_BLEND);

        // Re-enable depth testing if needed for other rendering
        glEnable(GL_DEPTH_TEST);

        if (Input::inputState.keys[GLFW_KEY_F])
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (Input::inputState.keys[GLFW_KEY_G])
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glUseProgram(character.shader);
        glfwSwapBuffers(window);
    }
    if (show)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(imgui_window);
    }
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &fboTexture);
    glDeleteProgram(vignetteShader.ID);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteProgram(textShader.ID);
    glDeleteProgram(character.shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
