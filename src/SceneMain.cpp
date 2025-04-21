#include "SceneMain.h"

#include <iostream>

// Constructor
SceneMain::SceneMain() {
    // Initialization code
    std::cout << "SceneMain initialized." << std::endl;
}

// Destructor
SceneMain::~SceneMain() {
    // Cleanup code
    std::cout << "SceneMain destroyed." << std::endl;
}

// Example method implementation
void SceneMain::init() {
    // Code to initialize the scene
    std::cout << "SceneMain is being initialized." << std::endl;
}

void SceneMain::update() {
    // Code to update the scene
    std::cout << "SceneMain updated with deltaTime: " << std::endl;
}

void SceneMain::render() {
    // Code to render the scene
    std::cout << "SceneMain rendering." << std::endl;
}

void SceneMain::clean()
{
}

void SceneMain::handleEvent(SDL_Event*)
{
}
