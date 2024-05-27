#define GLM_ENABLE_EXPERIMENTAL
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

#include <stb/stb_image.h>

#include "Core/Shader.hpp"

#include "Levels/LevelManager.hpp"
#include "Game/Game.h"
#include "Debugging/Hierarchy.hpp"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include <ft2build.h>
#include FT_FREETYPE_H


unsigned int colorGradingLUTTexture;
unsigned int imageTexture;

unsigned int VBO, VAO;

struct Character {
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};


static std::map<char, Character> characters;

void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color, Shader& shader, bool isAligned) {
    shader.use();
    shader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float textWidth = 0.0f;
    for (const auto& c : text) {
        Character ch = characters[c];
        textWidth += (ch.advance >> 6) * scale;
    }

    float xPos = x;
    if (isAligned) {
        xPos = (Game::state.WIDTH - textWidth) / 2.0f;
    }

    for (const auto& c : text) {
        Character ch = characters[c];
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        float xpos = xPos + ch.bearing.x * scale;

        float vertices[6][4] = {
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos, ypos, 0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
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

    Player character;
    LevelManager levelManager;

    unsigned int* playerShader = nullptr;

    bool hideHudButLetter = false;

    float timeScale = 1.f;

    character.Init(_renderer, false, playerShader, &isInInteractionZone, &hideHudButLetter);
    levelManager.GameStart(_renderer, &character, &timeScale);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    float fps = 0;

    setupQuad();

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glfwSwapInterval(1); // Enable vsync

    GLFWwindow* imgui_window = nullptr;
    bool show_demo_window = true;
    bool show_another_window = true;

    Shader textShader("shaders/postprocess/vertex.glsl", "shaders/postprocess/fragment.glsl");

    Shader shadowShader("shaders/postprocess/shadow.glsl", "shaders/postprocess/shadowfrag.glsl");

    setupTextRendering();
    loadFont("assets/fonts/typewriter.ttf", 48);

    bool show = true;
    textShader.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::state.WIDTH), 0.0f, static_cast<float>(Game::state.HEIGHT));
    textShader.setMat4("projection", projection);
    textShader.setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));
    




    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);


    unsigned int fboTexture;
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Game::state.WIDTH, Game::state.HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);


    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Game::state.WIDTH, Game::state.HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    if (show)
    {

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        imgui_window = glfwCreateWindow(800, 600, "Level Editor", NULL, NULL);


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;


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
    Interactable* interactableLocation = nullptr;
    Player* playerLocation = nullptr;
    
    static char inputAdd[256] = "";
    static const char* items[]{ "TriggerBox", "Model3D", "LightPoint", "Fog", "Post Process", "Interactable" };
    static int Selecteditem = 0;


    float vignetteIntensity = 1.2f;
    float vignetteRadius = 0.3f;
    float vignetteSmooth = -0.75f;


    float overAllVignetteIntensity = vignetteIntensity;
    float overAllVignetteRadius = vignetteRadius;
    float overAllVignetteSmooth = vignetteSmooth;

    float colorGradingIntensity = 0.07f;

    float bloomIntensity = 0.75f;
    float gammaIntensity = 0.5f;

    float grainIntensity = 0.13f;

    float grainSize = 0.01f;

    float specularStrength = 0.2;
    float ambientStrength = 0.6;

    static float fog[3] = { character.fogColor.x, character.fogColor.y, character.fogColor.z };
    static float tone[3] = { character.toneColor.x, character.toneColor.y, character.toneColor.z };
    bool isLightOn = true;
    bool isFogOn = true;
    int lutWidth, lutHeight, lutChannels;
    unsigned char* lutData = stbi_load("assets/textures/LUTS/LUT_TealOrangeContrastTable.jpg", &lutWidth, &lutHeight, &lutChannels, 0);
    if (lutData) {
        glGenTextures(1, &colorGradingLUTTexture);
        glBindTexture(GL_TEXTURE_2D, colorGradingLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lutWidth, lutHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, lutData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(lutData);
    }
    else {
        std::cout << "Failed to load LUT texture" << std::endl;
    }

    Shader vignetteShader("shaders/fbo_vertex.glsl", "shaders/fbo_fragment.glsl");
    vignetteShader.setVec2("screenSize", glm::vec2(Game::state.WIDTH, Game::state.HEIGHT));

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = Game::state.WIDTH, SHADOW_HEIGHT = Game::state.HEIGHT;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    float near_plane = 1.0f, far_plane = 10.0f;

    glm::vec3 something1 = glm::vec3(0);
    glm::vec3 something2 = glm::vec3(0);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = (currentFrame - lastFrame) * timeScale;
        lastFrame = currentFrame;
        fps = 1000 / deltaTime / 1000;
        int fpsInt = fps;
        std::string asdasad(std::to_string(fpsInt));


        glfwPollEvents();


        if (show)
        {
            glfwMakeContextCurrent(imgui_window);
            ImGui::SetCurrentContext(ImGui::GetCurrentContext());
            glClear(GL_COLOR_BUFFER_BIT);


            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            if (show_demo_window)
            {
                ImGui::Begin("Global Settings", &show_another_window);

                ImGui::Text("Lighting");

                ImGui::DragFloat("Far clip", &far_plane);

                ImGui::DragFloat("Near clip", &near_plane);


                ImGui::InputFloat("Ambient Strength", &ambientStrength);
                ImGui::InputFloat("Specular Strength", &specularStrength);

                ImGui::Text("Post Processing");

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

                ImGui::Text("LUT");
                ImGui::DragFloat("LUT Intensity", &colorGradingIntensity);

                ImGui::Text("Tone");
                ImGui::DragFloat("Tone Intensity", &character.toneStrength);
                ImGui::ColorPicker3("Tone Color", tone);

                ImGui::End();

                ImGui::Begin("Fog", &show_another_window);

                ImGui::DragFloat("Density", &character.fogDensity);
                ImGui::ColorPicker4("Color", fog);
                character.fogColor.x = fog[0];
                character.fogColor.y = fog[1];
                character.fogColor.z = fog[2];

                character.toneColor.x = tone[0];
                character.toneColor.y = tone[1];
                character.toneColor.z = tone[2];

                ImGui::End();
            }

            ShowTreeExample(&character);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(imgui_window);

            glfwMakeContextCurrent(window);

            glfwPollEvents();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(character.light->transform, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowShader.use();
        glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        levelManager.RenderShadows(shadowShader);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        // glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        character.Update(deltaTime);
        levelManager.GameUpdate(deltaTime);
        glUseProgram(character.shader);
        glUniformMatrix4fv(glGetUniformLocation(character.shader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform1f(glGetUniformLocation(character.shader, "ambientStrength"), ambientStrength);
        glUniform1f(glGetUniformLocation(character.shader, "specularStrength"), specularStrength);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glUniform1i(glGetUniformLocation(character.shader, "shadowMap"), 2);
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        vignetteShader.use();
        vignetteShader.setInt("screenTexture", 0);
        vignetteShader.setInt("colorGradingLUT", 1);
        vignetteShader.setFloat("vignetteIntensity", overAllVignetteIntensity);
        vignetteShader.setFloat("colorGradingIntensity", colorGradingIntensity);
        vignetteShader.setFloat("vignetteRadius", overAllVignetteRadius);
        vignetteShader.setFloat("vignetteSmooth", overAllVignetteSmooth);
        vignetteShader.setFloat("exposure", bloomIntensity);
        vignetteShader.setFloat("gamma", gammaIntensity);
        vignetteShader.setFloat("time", glfwGetTime());
        vignetteShader.setFloat("grainIntensity", grainIntensity);
        vignetteShader.setFloat("chromaticAberrationOffset", grainSize);




        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colorGradingLUTTexture);

        renderQuad();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (!hideHudButLetter)
        {
            if (isInInteractionZone)
            {
                renderText("Press E", Game::state.WIDTH / 2 - 100, Game::state.HEIGHT / 2, 1.f, glm::vec3(1.0f, 1.0f, 1.0f), textShader, false);
            }

            renderText("Objective: " + Game::state.currentObjective, 50.0f, 50.0f, 0.75f, glm::vec3(1.0f, 0.3f, 0.3f), textShader, false);
            renderText(asdasad + " FPS", 50.0f, Game::state.HEIGHT - 50, 0.75f, glm::vec3(1.0f, 1.0f, 1.0f), textShader, false);
        }

        if (Game::state.currentLetter != nullptr)
        {
            overAllVignetteIntensity = 1500;
            overAllVignetteRadius = 1.3;
            overAllVignetteSmooth = -0.75;
            renderText("[ESCAPE]", 0                                 , Game::state.HEIGHT / 2 + 300,  1.f, glm::vec3(1.0f, 0.3f, 0.3f), textShader, true);
            renderText("From: " + Game::state.currentLetter->title, 0, Game::state.HEIGHT / 2,        1.f, glm::vec3(1.0f, 1.0f, 1.0f), textShader, true);
            renderText(Game::state.currentLetter->content, 0         , Game::state.HEIGHT / 2 - 150, 0.6f, glm::vec3(1.0f, 1.0f, 1.0f), textShader, true);
        }
        else
        {
            overAllVignetteIntensity = vignetteIntensity;
            overAllVignetteRadius = vignetteRadius;
            overAllVignetteSmooth = vignetteSmooth;
        }


        glDisable(GL_BLEND);


        glEnable(GL_DEPTH_TEST);

        if (Input::inputState.keys[GLFW_KEY_F])
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (Input::inputState.keys[GLFW_KEY_G])
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
