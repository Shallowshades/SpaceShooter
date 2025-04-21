#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Scene.h"


class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();
    virtual void init() override;
    virtual void update() override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event*) override;
private:

};

#endif