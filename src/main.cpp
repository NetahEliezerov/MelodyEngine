
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


std::string readShaderSource(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(void) {
    GLFWwindow* window = Engine::Run();
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    Renderer _renderer;
    Player thePlayer;
    thePlayer.Init(_renderer, false);



    glEnable(GL_DEPTH_TEST);

    Model3D pistol;
    pistol.Init("assets/meshes/Pistol/1.obj", "assets/textures/2.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0, 0, 0), true, thePlayer.shader);


    Model3D cube;
    cube.Init("assets/meshes/cube.obj", "assets/textures/2.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(15, 0.2, 15), glm::vec3(0, -4, 0), true, thePlayer.shader);

    // Model3D hand;
    // hand.Init("assets/meshes/boobies/11084_WomanRunning_v3.obj", true, glm::vec4(.3f, .4f, .6f, 1.f), glm::vec3(8, 8, 8), false, thePlayer.shader);

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

        thePlayer.Update(deltaTime);

        pistol.SetPosition(thePlayer.movement.position, thePlayer.movement.lookingAngle, thePlayer.movement.cameraUp, 1.8f, .5f);

        cube.Update(thePlayer.movement.position, glm::vec3(4.f, 5.f, 4.5f), glm::vec3(1.f, 1.f, 1.f));
        pistol.Update(thePlayer.movement.position, glm::vec3(4.f, 5.f, 4.5f), glm::vec3(1.f, 1.f, 1.f));

        if (Input::inputState.keys[GLFW_KEY_F])
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        if (Input::inputState.keys[GLFW_KEY_G])
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        
        // hand.Update(thePlayer.movement.position, glm::vec3(4.f, 5.f, 4.5f), glm::vec3(1.f, 1.f, 1.f));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    textRenderer.Cleanup();
    glDeleteProgram(thePlayer.shader);
    glfwTerminate();
    return 0;
}