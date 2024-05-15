<div align="center">

# MelodyEngine

![alt text](http://url/to/img.png)


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


## Basic Example
This piece of code is starting by rendering the "_level1" level, and points the "currentLevel" pointer to it.
```cpp
struct LevelManager : LevelManagerState
{
    Level1 _level1;
    Level2 _level2;

    void GameStart(Renderer rendererRec, Player* playerPointer, float* timeScaleRec)
    {
        renderer = rendererRec;
        character = playerPointer;
        timeScale = timeScaleRec;

        _level1.Init(renderer, [this]() { OnDoorEnter(); }, playerPointer, timeScale);
        currentLevel = &_level1; // Set the current level to Level1
    }

    void GameUpdate(float deltaTime)
    {
        currentLevel->Update(deltaTime);
    }


    void OnDoorEnter()
    {
        currentLevel = &_level2;
        _level2.Init(renderer, [this]() {  }, character, timeScale);
    }
};
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
