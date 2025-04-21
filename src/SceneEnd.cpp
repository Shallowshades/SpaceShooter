#include "SceneEnd.h"
#include "SceneMain.h"
#include "Game.h"
#include <string>

void SceneEnd::init()
{
    // 载入背景音乐
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (!bgm) {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);

    if (!SDL_IsTextInputActive()) {
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive()) {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to start text input: %s", SDL_GetError());
    }
}

void SceneEnd::update(float deltaTime)
{
    blinkTimer -= deltaTime;
    if (blinkTimer <= 0) {
        blinkTimer += 1.0f;
    }
}

void SceneEnd::render()
{
    if (isTyping) {
        renderPhase1();
    }
    else {
        renderPhase2();
    }
}

void SceneEnd::clean()
{
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneEnd::handleEvent(SDL_Event* event)
{
    if (isTyping) {
        if (event->type == SDL_TEXTINPUT) {
            name += event->text.text;
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                isTyping = false;
                SDL_StopTextInput();
                if (name == "") {
                    name = "Unkowned";
                }
                game.insertLeaderBoard(game.getFinalScore(), name);
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                removeLastUTF8Char(name);
            }
        }
    }
    else {
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_KP_ENTER) {
                auto sceneMain = new SceneMain();
                game.changeScene(sceneMain);
            }
        }
    }
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "Your Score is : " + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instrutionText = "Please Enter Your Name: ";
    game.renderTextCentered(scoreText, 0.1f, false);
    game.renderTextCentered(gameOver, 0.4f, true);
    game.renderTextCentered(instrutionText, 0.6f, false);

    if (name != "") {
        SDL_Point p = game.renderTextCentered(name, 0.8f, false);
        if (blinkTimer < 0.5f) {
            game.renderTextPos("_", p.x, p.y);
        }
    }
    else {
        if (blinkTimer < 0.5f) {
            game.renderTextCentered("_", 0.8f, false);
        }
    }
}

void SceneEnd::renderPhase2()
{
    game.renderTextCentered("Score Board", 0.05f, true);
    int32_t posY = static_cast<int32_t>(std::ceil(0.2f * game.getWindowHeight()));
    uint32_t i = 1;
    for (auto item : game.getLeaderBoard()) {
        std::string name(std::to_string(i) + ". " + item.second);
        std::string score = std::to_string(item.first);
        game.renderTextPos(name, 100, posY);
        game.renderTextPos(score, 100, posY, false);
        posY += 45;
        i++;
    }
    if (blinkTimer < 0.5f) {
        game.renderTextCentered("Please Key Enter to Entry game", 0.85f, false);
    }
}

void SceneEnd::removeLastUTF8Char(std::string& str)
{
    if (str.empty()) return;

    auto lastchar = str.back();
    if ((lastchar & 0b10000000) == 0b10000000) { // 中文字符的后续字节
        str.pop_back();
        while ((str.back() & 0b11000000) != 0b11000000) { // 判断是否是中文字符的第一个字节
            str.pop_back();
        }
    }
    str.pop_back();
}