#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

class Object {
public:
    Object() = default;
    virtual ~Object() = default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event*) = 0;
};

#endif