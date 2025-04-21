#ifndef SCENETITLE_H
#define SCENETITLE_H

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "Game.h"
#include "Scene.h"
#include "SceneMain.h"

class SceneTitle : public Scene {
public:
    virtual void init() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event*) override;
private:
    Mix_Music* bgm;     // 背景音乐
    float timer;        // 计时器
};

#endif // SCENETITLE_H
