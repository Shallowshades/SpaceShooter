#include "Game.h"
#include "SceneMain.h"

Game::Game() {

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
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC
        | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS) == false) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    // 创建窗口
    window = SDL_CreateWindow("Shooter", 800, 600, 0);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    currentScene = new SceneMain();
    currentScene->init();
}

void Game::clean() {
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
        if (event->type == SDL_EVENT_QUIT) {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}

void Game::update() {

}

void Game::render() {
    // 清空
    SDL_RenderClear(renderer);
    // 绘制
    currentScene->render();
    // 更新
    SDL_RenderPresent(renderer);
}
