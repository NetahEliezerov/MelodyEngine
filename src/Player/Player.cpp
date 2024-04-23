#include "Player.h"
#include "../Core/Engine.h"
#include "../Core/Input.h"

#include "fstream"
#include "sstream"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static std::string readShaderSource(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Player::Update(float deltaTime)
{
    // Apply pitch and yaw rotation
    movement.yaw += Input::inputState.mouseDeltaX * movement.sensitivity;
    movement.pitch -= Input::inputState.mouseDeltaY * movement.sensitivity;

    // Limit the pitch angle to prevent flipping
    const float pitchLimit = 89.0f; // Adjust this value as needed
    movement.pitch = glm::clamp(movement.pitch, -pitchLimit, pitchLimit);

    // Calculate the direction vector based on the pitch and yaw angles
    glm::vec3 direction;
    direction.x = cos(glm::radians(movement.yaw)) * cos(glm::radians(movement.pitch));
    direction.y = sin(glm::radians(movement.pitch));
    direction.z = sin(glm::radians(movement.yaw)) * cos(glm::radians(movement.pitch));
    movement.lookingAngle = glm::normalize(direction);

    // Reset the mouse movement
    Input::inputState.mouseDeltaX = 0;
    Input::inputState.mouseDeltaY = 0;

    // Update camera position based on keyboard input
    float velocity = movement.cameraSpeed * deltaTime;
    if (isGodMode)
    {
        if (Input::inputState.keys[GLFW_KEY_W])
            movement.position += movement.lookingAngle * velocity;
        if (Input::inputState.keys[GLFW_KEY_S])
            movement.position -= movement.lookingAngle * velocity;
        if (Input::inputState.keys[GLFW_KEY_A])
            movement.position -= glm::normalize(glm::cross(movement.lookingAngle, movement.cameraUp)) * velocity;
        if (Input::inputState.keys[GLFW_KEY_D])
            movement.position += glm::normalize(glm::cross(movement.lookingAngle, movement.cameraUp)) * velocity;
    }
    else
    {
        glm::vec3 forward = movement.lookingAngle;
        forward.y = 0.0f; // Ignore vertical component of the looking direction
        forward = glm::normalize(forward);

        glm::vec3 right = glm::normalize(glm::cross(forward, movement.cameraUp));

        glm::vec3 moveDirection = glm::vec3(0.0f);

        if (Input::inputState.keys[GLFW_KEY_W])
            moveDirection += forward;
        if (Input::inputState.keys[GLFW_KEY_S])
            moveDirection -= forward;
        if (Input::inputState.keys[GLFW_KEY_A])
            moveDirection -= right;
        if (Input::inputState.keys[GLFW_KEY_D])
            moveDirection += right;

        // Normalize the moveDirection vector to avoid faster diagonal movement
        if (glm::length(moveDirection) > 0.0f)
            moveDirection = glm::normalize(moveDirection);

        // Update only the X and Z components of the position
        movement.position.x += moveDirection.x * velocity;
        movement.position.z += moveDirection.z * velocity;
    }


    // Update the view matrix
    view = glm::lookAt(movement.position, movement.position + movement.lookingAngle, movement.cameraUp);
    glUniformMatrix4fv(movement.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(movement.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Player::Init(Renderer _renderer, bool recIsGodMode)
{
    std::string vertexShader = readShaderSource("shaders/vertex_texture.glsl");
    std::string fragmentShader = readShaderSource("shaders/fragment_texture.glsl");

    isGodMode = recIsGodMode;

    shader = _renderer.CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    movement.viewLoc = glGetUniformLocation(shader, "view");
    movement.projectionLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(movement.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(movement.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
