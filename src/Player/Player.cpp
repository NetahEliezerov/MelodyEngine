#include "Player.h"
#include "../Core/Engine.h"
#include "../Core/Input.h"


#include <fstream>
#include <sstream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/AudioManager.hpp"

AudioManager audioManager;

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

Ray calculatePistolTarget(const glm::vec3& cameraPos, const glm::vec3& cameraFront, float distance)
{
    Ray ray;
    ray.origin = cameraPos;
    ray.direction = glm::normalize(cameraFront);
    return ray;
}


bool raySphereIntersection(const Ray& ray, const glm::vec3& sphereCenter, float sphereRadius, float& hitDistance)
{
    glm::vec3 oc = ray.origin - sphereCenter;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }

    float temp = (-b - sqrt(discriminant)) / (2.0f * a);
    if (temp < 0) {
        temp = (-b + sqrt(discriminant)) / (2.0f * a);
        if (temp < 0) {
            return false;
        }
    }

    hitDistance = temp;
    return true;
}

static std::string readShaderSource(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Player::Update(float deltaTime)
{
    bool isMoving = false;


    glUseProgram(shader);
    if (glfwGetCurrentContext() == Engine::GetOpenGLWindow())
    {
        if (Input::inputState.keys[GLFW_KEY_LEFT_SHIFT])
            Sprint(true);
        else
            Sprint(false);

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


        // pistolle jumping
        if (Input::inputState.keys[GLFW_KEY_SPACE] && !isJumping) {
            isJumping = true;
            velocity.y = jumpForce;
        }

        // Apply gravity
        velocity.y += gravity * deltaTime;

        // Update player position based on velocity
        movement.position += velocity * deltaTime;

        // Check if the player has landed on the ground
        float groundLevel = 0.0f; // Adjust this value according to your ground level
        if (isJumping && movement.position.y <= groundLevel) {
            isJumping = false;
            movement.position.y = groundLevel;
            velocity.y = 0.0f;
        }

        if (!isJumping)
        {
            movement.position.y = groundLevel;
        }

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
    }
    // Update the view matrix
    view = glm::lookAt(movement.position, movement.position + movement.lookingAngle, movement.cameraUp);
    glUniformMatrix4fv(movement.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(movement.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    if (!isJumping)
    {
        if (Input::inputState.keys[GLFW_KEY_W] || Input::inputState.keys[GLFW_KEY_S] ||
            Input::inputState.keys[GLFW_KEY_A] || Input::inputState.keys[GLFW_KEY_D])
        {
            isMoving = true;
        }
    }

    footstepTimer += deltaTime;

    if (isMoving)
    {
        // Check if enough time has passed since the last footstep sound
        if (footstepTimer >= footstepDelay)
        {
            // Play the footstep sound
            audioManager.PlaySound(0); // Assuming footstep sound is loaded at index 0

            // Reset the footstep timer
            footstepTimer = 0.0f;
        }
    }
    else
    {
        // Reset the footstep timer when not moving
        footstepTimer = 0.0f;
    }

    audioManager.Update();

    //pistol.SetPosition(movement.position, movement.lookingAngle, movement.cameraUp, 1.3f, .75, 0);
    //pistol.Update(movement.position, light);

    //hand.SetPosition(movement.position, movement.lookingAngle, movement.cameraUp, 1.3f, .75, 0);
    //hand.Update(movement.position, light);


    //if (Input::inputState.keys[GLFW_MOUSE_BUTTON_LEFT]) {
    //    Fire();
    //}
}

void Player::Init(Renderer _renderer, bool recIsGodMode)
{
    audioManager.LoadSound("assets/sounds/footstep.wav");

    movement.position = glm::vec3(0.0f, 1.0f, 0.0f);
    std::string vertexShader = readShaderSource("shaders/vertex_texture.glsl");
    std::string fragmentShader = readShaderSource("shaders/fragment_texture.glsl");

    isGodMode = recIsGodMode;

    shader = _renderer.CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    movement.viewLoc = glGetUniformLocation(shader, "view");
    movement.projectionLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(movement.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(movement.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //ObjectSettings pistolSettings = { "Pistol", "assets/meshes/pistol.obj", "assets/textures/sp226-color-2.jpeg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(-0.2f, -0.2f, 0), glm::vec3(0,270,0), true, shader };
    //ObjectSettings handSettings = { "Hand", "assets/meshes/hand.obj", "assets/textures/aga.jpg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(-0.1f, -0.3, 1.2f), glm::vec3(270,180,0), true, shader };
    //pistol.Init(pistolSettings);
    //hand.Init(handSettings);
    timeSinceShoot = std::chrono::steady_clock::now(); // Initialize lastShotTime
}


void Player::Fire()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - timeSinceShoot).count();

    // Set the desired delay between shots in milliseconds
    const int delayBetweenShots = 200; // 500 milliseconds (0.5 seconds)

    if (elapsedTime >= delayBetweenShots) {
        FireAnimation(&pistol, 4);

        glm::vec3 cameraPos = movement.position;
        glm::vec3 cameraFront = movement.lookingAngle;
        float shootDistance = 100.0f; // Adjust the shoot distance as needed

        Ray shootRay = calculatePistolTarget(cameraPos, cameraFront, shootDistance);

        float closestHitDistance = std::numeric_limits<float>::max();
        Model3D* hitObject = nullptr;

        for (auto& model : level->sceneModels) {
            float hitDistance;
            if (raySphereIntersection(shootRay, model->transform, model->radius, hitDistance)) {
                if (hitDistance < closestHitDistance) {
                    closestHitDistance = hitDistance;
                    hitObject = model;
                }
            }
        }
        level->Fire(hitObject);

        timeSinceShoot = currentTime; // Update the last shot time
    }
}

void Player::Sprint(bool value)
{
    if (value)
    {
        isSprinting = true;
        movement.cameraSpeed = movement.sprintSpeed;
    }
    else
    {
        isSprinting = false;
        movement.cameraSpeed = movement.walkSpeed;
    }
}


void Player::FireAnimation(Model3D* whatto, float toHow)
{
    float targetRotation = toHow;
    // Start a separate thread to pistolle the smooth rotation
    float startRotation = whatto->rotation.x;
    std::thread([this, targetRotation, whatto]() {
        float startRotation = whatto->rotation.x;
        float duration = 0.1f; // Duration of the rotation transition in seconds

        auto startTime = std::chrono::high_resolution_clock::now();

        while (true) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

            if (elapsedTime >= duration)
                break;

            // Calculate the interpolation factor based on elapsed time
            float t = elapsedTime / duration;

            // Interpolate the rotation value using linear interpolation
            float currentRotation = startRotation + t * (targetRotation - startRotation);

            // Update the pistol model's rotation
            whatto->rotation.x = currentRotation;
        }

        // Ensure the final rotation value is set to the target rotation
        whatto->rotation.x = targetRotation;

        // Wait for a certain duration before starting the rotation back to default
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        startTime = std::chrono::high_resolution_clock::now();

        while (true) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

            if (elapsedTime >= duration)
                break;

            float t = elapsedTime / duration;
            float currentRotation = targetRotation - t * targetRotation;
            whatto->rotation.x = currentRotation;
        }

        // Ensure the final rotation value is set to the default rotation
        whatto->rotation.x = 0.0f;

        }).detach();
}
