<div align="center">

# MelodyEngine

![Banner](https://github.com/NetahEliezerov/MelodyEngine/blob/master/docs/assets/banner.png?raw=true)


## 3D first-person game engine

MelodyEngine is a 3D game engine written in C++.
It uses OpenGL and other libraries to build an efficient result.

</div>


## Features

MelodyEngine offers plenty of features, designed to run first-person horror games.

* Integrated HUD: HUD elements like Objectives, Subtitles, etc..
* 3D Renderer: Advanced 3D rendering scheme made for better designing flow
* Level Manager: Easy-to-use level manager built for easy, efficient use
* Realtime Lighting: Magnificent-quality realtime lighting, with shadow casting
* Built-in Components: World components like Interactables, Trigger Boxes, etc..
* Control Over Engine: MelodyEngine provides the right balance between ease of use and complete control over the engine
* Advanced Post-Processing: Advanced post-processing features with many effects supported like LUT, Film Grain, Chromatic Abberation, etc..
* Basic Physics Support: Incomplete physics engine
* Audio Manager: Basic audio support
* Game State: Built-in game state manager, which supports Objectives, Missions, etc..


![Level Editor](https://github.com/NetahEliezerov/MelodyEngine/blob/master/docs/assets/LevelEditorImage.png?raw=true)


## Basic Example
### LevelManager
This piece of code is starting by rendering the "_level1" level, and points the "currentLevel" pointer to it.
```cpp
struct LevelManager : LevelManagerState
{
    BasicLevel _basicLevel;

    void GameStart(Renderer rendererRec, Player* playerPointer, float* timeScaleRec)
    {
        renderer = rendererRec;
        character = playerPointer;
        timeScale = timeScaleRec;

        _basicLevel.Init(renderer, [this]() {}, playerPointer, timeScale);
        currentLevel = &_basicLevel; // Set the current level to Level1
    }

    void GameUpdate(float deltaTime)
    {
        currentLevel->Update(deltaTime);
        currentLevel->RenderUpdate(deltaTime);
    }
};
```
### BasicLevel
```cpp
class BasicLevel : public WorldLevel
{
public:
    
    // Init: CALLED ON GAME START
    virtual void Init(Renderer renderer, std::function<void()> funcRec, Player* playerPointer, float* timeScaleRec) override
    {
        // SET PRIMARY MEMBERS
        character = playerPointer;
        playerPointer->level = this;
        playerPointer->light = &light;
        func = funcRec;

        // DEFINE SCENE MEMBERS
        ObjectSettings wallSettings = { "Exterior", "assets/meshes/cube.obj", {"assets/textures/Wall/images.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(6, 3, 6), glm::vec3(0, -1, 0), glm::vec3(0,0,0), true, character->shader };
        ObjectSettings handSettings = { "Hand", "assets/meshes/hand.obj", {"assets/textures/aga.jpg"}, true, glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, -3.5, -1), glm::vec3(87, 165,98), true, character->shader };
        LightSettings lightSettings = { "assets/meshes/cube.obj", "assets/textures/zizim.jpg", glm::vec4(1, 0.78, 0.6, 1.f), glm::vec3(0.15, 0.15, 0.15), glm::vec3(1, 0, 2), character->shader };

        
        // INITIALIZE SCENE MEMBERS
        wall.Init(wallSettings);
        hand.Init(handSettings);
        light.Init(lightSettings);

        // SET CURRENT GAME STATE
        Game::state.currentMission = 0;
        Game::state.currentSubMission = 0;
        Game::state.currentObjective = "Investigate";

        // ADD OBJECTS/ETC TO HIERARCHY
        sceneHierarchy.push_back(&hand);
        sceneHierarchy.push_back(&light);
        sceneHierarchy.push_back(&wall);
    };

    
    // Update: CALLED ON FRAME UPDATE
    virtual void Update(float deltaTime) override
    {
        // CALL FlickLight
        FlickLight(deltaTime);

        // HAND ROTATION ANIMATION
        hand.rotation.z += 7.5 * deltaTime;
    };

private:
    // DEFINE MEMBERS
    Model3D wall;
    Model3D hand;
    LightPoint light;

    std::function<void()> func;

    bool lightOn = true;
    float flickerTimer = 0.0f;
    float flickerInterval = 0.2f;

    // DEFINE FLICK LIGHT
    void FlickLight(float deltaTime)
    {
        flickerTimer += deltaTime;

        if (flickerTimer >= flickerInterval)
        {
            flickerTimer = 0.0f;
            lightOn = !lightOn;

            if (lightOn)
                light.color = glm::vec4(1, 0.78, 0.6, 1.f);
            else
                light.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.1f, 0.5f);
            flickerInterval = dis(gen);
        }
    };
};
```
Result should look like this:

![Example Result](https://github.com/NetahEliezerov/MelodyEngine/blob/master/docs/assets/ExampleLevelImage.png?raw=truee)



## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
