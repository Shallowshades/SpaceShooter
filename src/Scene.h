#ifndef SECNE_H
#define SECNE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

class Game; // 前向声明

class Scene {
public:
    Scene();
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event*) = 0;

protected:
    Game& game;
};

#endif