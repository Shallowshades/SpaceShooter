#ifndef GAME_H
#define GAME_H

#include <string>
#include <map>
#include <fstream>

#include "Scene.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "Object.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"


class Game {
    // 获取游戏单例
public:
    static Game& getInstance();

public:
    ~Game();

    // 游戏主循环
public:
    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);
    void handleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();

    // 渲染文本
public:
    SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
    void renderTextPos(std::string text, int32_t posX, int32_t posY, bool isLeft = true);

    // setters and getters
public:
    void setFinalScore(int score) { finalScore = score; }
    void insertLeaderBoard(int score, std::string name);

    SDL_Renderer* getRenderer() { return renderer; }
    SDL_Window* getWindow() { return window; }
    int32_t getWindowWidth() { return windowWidth; }
    int32_t getWindowHeight() { return windowHeight; }
    int getFinalScore() { return finalScore; }
    std::multimap<int, std::string, std::greater<int>>& getLeaderBoard() { return leaderBoard; }

    // 单例
private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // 背景
private:
    void backgroundUpdate(float deltaTime); // 更新背景
    void backgroundRender();                // 渲染背景

    // 存档
private:
    void saveData();            // 保存数据
    void loadData();            // 加载数据

private:
    bool isRunning;             // 游戏是否运行
    bool isFullScreen;          // 是否全屏
    Scene* currentScene;        // 当前场景
    SDL_Window* window;         // 窗口
    SDL_Renderer* renderer;     // 渲染器
    int32_t windowHeight;       // 窗口高度
    int32_t windowWidth;        // 窗口宽度
    int32_t FPS;                // 帧率
    uint32_t frameTime;         // 帧时间
    float deltaTime;            // 帧间隔时间
    uint32_t finalScore;        // 最终得分

    TTF_Font* titleFont;        // 标题字体
    TTF_Font* textFont;         // 文本字体

    Background nearStars;       // 近景星空
    Background farStars;        // 远景星空

    std::multimap<int, std::string, std::greater<int>> leaderBoard; // 排行榜
};

#endif