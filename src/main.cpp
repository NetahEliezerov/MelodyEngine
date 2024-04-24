
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
    TextRenderer textRenderer;
    textRenderer.Init("assets/fonts/typewriter.ttf", 24);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps = 1000 / deltaTime / 1000;
        // std::cout << fps << " FPS" << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        character.Update(deltaTime);
        _level1.Update(deltaTime);

        if (Input::inputState.keys[GLFW_KEY_F])
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        if (Input::inputState.keys[GLFW_KEY_G])
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        
        // hand.Update(thePlayer.movement.position, glm::vec3(4.f, 5.f, 4.5f), glm::vec3(1.f, 1.f, 1.f));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    textRenderer.Cleanup();
    glDeleteProgram(character.shader);
    glfwTerminate();
    return 0;
}