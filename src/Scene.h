#ifndef SECNE_H
#define SECNE_H

#include <SDL.h>


class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event*) = 0;

private:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
};

#endif