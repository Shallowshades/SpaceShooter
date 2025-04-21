#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "SDL3/SDL.h"
#include "Scene.h"


class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();
    virtual void init();
    void update();
    void render();
    void clean();
    void handleEvent(SDL_Event*);
private:
    
};

#endif