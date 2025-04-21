#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL3/SDL.h>
#include "Scene.h"
#include "SceneMain.h"

class Game {
public:
    static Game& getInstance() {
        static Game instance;
        return instance;
    }

    Game();
    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);
    void handleEvent(SDL_Event* event);
    void update();
    void render();
private:
    bool isRunning;
    Scene* currentScene;
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif