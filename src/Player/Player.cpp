#include "Player.h"
#include "../Core/Engine.h"
#include "../Core/Input.h"


#include <fstream>
#include <sstream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../World/WorldLevel.h"
#include <stb/stb_image.h>

#include "../Core/AudioManager.hpp"

AudioManager audioManager;

struct RayA {
    glm::vec3 origin;
    glm::vec3 direction;
};
unsigned int lutTexture;

RayA calculatePistolTarget(const glm::vec3& cameraPos, const glm::vec3& cameraFront, float distance)
{
    RayA ray;
    ray.origin = cameraPos;
    ray.direction = glm::normalize(cameraFront);
    return ray;
}


bool raySphereIntersection(const RayA& ray, const glm::vec3& sphereCenter, float sphereRadius, float& hitDistance)
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

void Player::Update(float deltaTime, int* isGradingLow)
{
    glUseProgram(shader);
    bool isMoving = false;

    // Set uniform variables
    glUniform1f(glGetUniformLocation(shader, "fogDensity"), fogDensity);
    if (*isGradingLow == 1)
    {
        glUniform1f(glGetUniformLocation(shader, "fogDensity"), fogDensity);
        glUniform1f(glGetUniformLocation(shader, "gradingStrength"), toneStrength);
    }
    else {
        glUniform1f(glGetUniformLocation(shader, "fogDensity"), 0);
        glUniform1f(glGetUniformLocation(shader, "gradingStrength"), 0);
    }
    glUniform3fv(glGetUniformLocation(shader, "gradingColor"), 1, glm::value_ptr(toneColor));
    glUniform3fv(glGetUniformLocation(shader, "fogColor"), 1, glm::value_ptr(fogColor));

    if (glfwGetCurrentContext() == Engine::GetOpenGLWindow())
    {


        if (Input::inputState.keys[GLFW_KEY_LEFT_SHIFT])
            Sprint(true);
        else
            Sprint(false);


        movement.yaw += Input::inputState.mouseDeltaX * movement.sensitivity;
        movement.pitch -= Input::inputState.mouseDeltaY * movement.sensitivity;


        const float pitchLimit = 89.0f;
        movement.pitch = glm::clamp(movement.pitch, -pitchLimit, pitchLimit);


        glm::vec3 direction;
        direction.x = cos(glm::radians(movement.yaw)) * cos(glm::radians(movement.pitch));
        direction.y = sin(glm::radians(movement.pitch));
        direction.z = sin(glm::radians(movement.yaw)) * cos(glm::radians(movement.pitch));


        bool isMoving = false;

        if (Input::inputState.keys[GLFW_KEY_W] || Input::inputState.keys[GLFW_KEY_S] || Input::inputState.keys[GLFW_KEY_A] || Input::inputState.keys[GLFW_KEY_D])
        {
            isMoving = true;
        }

        if (isMoving)
        {
            aasdasd += rotationSpeed * deltaTime;
            movement.roll = rotationMultiplier * sin(0.33*aasdasd);

            glm::mat4 rotationMatrix = glm::mat4(1.0f);
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(movement.roll), movement.lookingAngle);

            movement.cameraUp = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }






        movement.lookingAngle = glm::normalize(direction);


        Input::inputState.mouseDeltaX = 0;
        Input::inputState.mouseDeltaY = 0;



        if (Input::inputState.keys[GLFW_KEY_SPACE] && !isJumping) {
            isJumping = true;
            velocity.y = jumpForce * deltaTime;
            // body->applyWorldForceAtCenterOfMass(GLMToReact({ 0,20,0 }) * 650);
        }


        velocity.y += gravity * deltaTime;


        movement.position += velocity * deltaTime;


        float groundLevel = 0.0f;
        if (isJumping && movement.position.y <= groundLevel) {
            isJumping = false;
            movement.position.y = groundLevel;
            velocity.y = 0.0f;
        }

        if (!isJumping)
        {
            movement.position.y = groundLevel;
        }



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
            forward.y = 0.0f;
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


            if (glm::length(moveDirection) > 0.0f)
                moveDirection = glm::normalize(moveDirection);

            if (Input::inputState.keys[GLFW_KEY_H])
                std::cout << movement.position.x << " " << movement.position.y << " " << movement.position.z << std::endl;

            if (Input::inputState.keys[GLFW_KEY_W] || Input::inputState.keys[GLFW_KEY_S] || Input::inputState.keys[GLFW_KEY_A] || Input::inputState.keys[GLFW_KEY_D])
            {
                moveDirection.y = 0;
            }
            glm::vec3 newPosition = movement.position;

            if (Input::inputState.keys[GLFW_KEY_W] || Input::inputState.keys[GLFW_KEY_S] || Input::inputState.keys[GLFW_KEY_A] || Input::inputState.keys[GLFW_KEY_D])
            {
                glm::vec3 moveOffset = moveDirection * velocity;
                newPosition += moveOffset;
                movement.position = newPosition;
            }
        }
    };


    glm::vec3 cameraPosition = movement.position;
    if (isCrouching) {
        cameraPosition.y = crouchHeight;
    }
    view = glm::lookAt(cameraPosition, cameraPosition + movement.lookingAngle, movement.cameraUp);
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

        if (footstepTimer >= footstepDelay)
        {

            audioManager.PlaySound(0);

            footstepTimer = 0.0f;
        }
    }
    else
    {
        footstepTimer = 0.0f;
    }

    audioManager.Update();

    if (!isJumping)
    {
        if (Input::inputState.keys[GLFW_KEY_LEFT_CONTROL])
            Crouch(true);
        else
            Crouch(false);
    }
    //pistol.SetPosition(movement.position, movement.lookingAngle, movement.cameraUp, 1.3f, .75, 0);
    //pistol.Update(movement.position, light);

    //hand.SetPosition(movement.position, movement.lookingAngle, movement.cameraUp, 1.3f, .75, 0);
    //hand.Update(movement.position, *light, deltaTime);


    //if (Input::inputState.keys[GLFW_MOUSE_BUTTON_LEFT]) {
    //    Fire();
    //}
}

void Player::Crouch(bool value) {
    if (value) {
        isCrouching = true;
        movement.cameraSpeed = crouchSpeed;
        movement.position.y = crouchHeight;
    }
    else {
        isCrouching = false;
        movement.cameraSpeed = movement.walkSpeed;
        movement.position.y = standingHeight;
    }
}

bool Player::CheckCollision(Model3D* model)
{
    float distance = glm::distance(model->transform, movement.position);
    float sumRadii = model->collisionRadius + collisionRadius;
    return distance <= sumRadii;
}

void Player::Init(Renderer _renderer, bool recIsGodMode, unsigned int* shaderPointer, bool* isInInteractionRec, bool* hideHudButLetterRec)
{
    audioManager.LoadSound("assets/sounds/footstep.wav");
    hideHudButLetter = hideHudButLetterRec;
    isInInteractionZone = isInInteractionRec;

    std::string vertexShader = readShaderSource("shaders/vertex_texture.glsl");
    std::string fragmentShader = readShaderSource("shaders/fragment_texture.glsl");

    isGodMode = recIsGodMode;


    shader = _renderer.CreateShader(vertexShader, fragmentShader);
    shaderPointer = &shader;
    glUseProgram(shader);

    movement.viewLoc = glGetUniformLocation(shader, "view");
    movement.projectionLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(movement.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(movement.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    ObjectSettings targetCube2Settings = { "Target Cube", "assets/meshes/Shotgun/Shotgun.fbx", {"assets/meshes/Shotgun/Shotgun_DefaultMaterial_BaseColor.png"}, glm::vec3(0.007, 0.007, 0.007), glm::vec3(2.5, 0, 0), glm::vec3(0,0,0) };
    std::cout << "JOHNNY " << &targetCube2Settings << std::endl;
    //ObjectSettings pistolSettings = { "Pistol", "assets/meshes/pistol.obj", "assets/textures/sp226-color-2.jpeg", true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(-0.2f, -0.2f, 0), glm::vec3(0,270,0), true, shader };
    //ObjectSettings handSettings = { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(-0.1f, -0.3, 1.2f), glm::vec3(270,180,0), true, shader };
    //pistol.Init(pistolSettings);
    // hand.Init(targetCube2Settings, level);
    timeSinceShoot = std::chrono::steady_clock::now(); // Initialize lastShotTime

    fogColor = { 0, 0, 0 };
    toneColor = { 1, 0.694, 0.401 };
    toneStrength = 1.0;
    std::cout << "PLAYER: " << this << std::endl;
}
void Player::Fire()
{
    //auto currentTime = std::chrono::steady_clock::now();
    //auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - timeSinceShoot).count();

    //const int delayBetweenShots = 200;

    //if (elapsedTime >= delayBetweenShots) {
    //    FireAnimation(&pistol, 4);

    //    glm::vec3 cameraPos = movement.position;
    //    glm::vec3 cameraFront = movement.lookingAngle;
    //    float shootDistance = 100.0f;

    //    Ray shootRay = calculatePistolTarget(cameraPos, cameraFront, shootDistance);

    //    float closestHitDistance = std::numeric_limits<float>::max();
    //    Model3D* hitObject = nullptr;

    //    for (auto& model : level->sceneModels) {
    //        float hitDistance;
    //        if (raySphereIntersection(shootRay, model->transform, model->radius, hitDistance)) {
    //            if (hitDistance < closestHitDistance) {
    //                closestHitDistance = hitDistance;
    //                hitObject = model;
    //            }
    //        }
    //    }
    //    level->Fire(hitObject);

    //    timeSinceShoot = currentTime;
    //}
}

void Player::Sprint(bool value)
{
    if (value)
    {
        isSprinting = true;
        movement.cameraSpeed = movement.sprintSpeed;
        rotationSpeed = 30;
        rotationMultiplier = 1.25f;
    }
    else
    {
        isSprinting = false;
        movement.cameraSpeed = movement.walkSpeed;
        rotationSpeed = 20;
        rotationMultiplier = 0.5f;
    }
}


void Player::FireAnimation(Model3D* whatto, float toHow)
{
    float targetRotation = toHow;
    float startRotation = whatto->rotation.x;
    std::thread([this, targetRotation, whatto]() {
        float startRotation = whatto->rotation.x;
        float duration = 0.1f;

        auto startTime = std::chrono::high_resolution_clock::now();

        while (true) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

            if (elapsedTime >= duration)
                break;

            float t = elapsedTime / duration;

            float currentRotation = startRotation + t * (targetRotation - startRotation);

            whatto->rotation.x = currentRotation;
        }

        whatto->rotation.x = targetRotation;

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

        whatto->rotation.x = 0.0f;

        }).detach();
}