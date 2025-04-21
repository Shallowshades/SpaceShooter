#include "Game.h"
#include "SceneMain.h"

// 获取游戏单例
Game& Game::getInstance() {
    static Game instance;
    return instance;
}

// 初始化
Game::Game()
    : isRunning(true),
    window(nullptr),
    renderer(nullptr),
    currentScene(nullptr),
    screenHeight(800),
    screenWidth(600) {
}

Game::~Game() {
    clean();
}

void Game::run() {
    while (isRunning) {
        SDL_Event event;
        handleEvent(&event);

        update();

        render();

    }
}

void Game::init() {
    // SDL 初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    // 创建窗口
    window = SDL_CreateWindow("Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    currentScene = new SceneMain();
    currentScene->init();
}

void Game::clean() {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
        currentScene = nullptr;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene* scene) {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event* event) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}

void Game::update() {
    currentScene->update();
}

void Game::render() {
    // 清空
    SDL_RenderClear(renderer);
    // 绘制
    currentScene->render();
    // 更新
    SDL_RenderPresent(renderer);
}
