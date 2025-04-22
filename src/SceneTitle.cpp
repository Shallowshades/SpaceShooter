#include "SceneTitle.h"

void SceneTitle::init() {
    // Load background music
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", Mix_GetError());
        return;
    }
    // Play background music
    Mix_PlayMusic(bgm, -1);
    timer = 0.f;
}

void SceneTitle::update(float deltaTime) {
    timer += deltaTime;
    if (timer > 1.f) {
        timer = 0.f; // Reset timer
    }
}

void SceneTitle::render() {
    // Render title
    std::string titleText = "Space Shooter";
    game.renderTextCentered(titleText, 0.4f, true);

    // Render text
    if (timer < 0.5f) {
        std::string instructions = "Press J to Start";
        game.renderTextCentered(instructions, 0.8f, false);
    }
}

void SceneTitle::clean() {
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
}

void SceneTitle::handleEvent(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_J) {
            // Change to main scene
            game.changeScene(new SceneMain());
        }
    }
}
