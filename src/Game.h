#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "SDL.h"


class Game {
public:
    static Game& getInstance();
public:
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
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
private:
    bool isRunning;             // 游戏是否运行
    Scene* currentScene;        // 当前场景
    SDL_Window* window;         // 窗口
    SDL_Renderer* renderer;     // 渲染器
    int32_t screenHeight;       // 窗口高度
    int32_t screenWidth;        // 窗口宽度
};

#endif