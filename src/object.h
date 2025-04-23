#ifndef OBJECT_H
#define OBJECT_H

#include "SDL.h"

enum class ItemType {
    Life,
    Boost,
    Shield,
    Time
};

struct Player {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    int32_t width = 0;
    int32_t height = 0;
    int32_t speed = 300;
    int32_t currentHealth = 3;
    int32_t maxHealth = 3;
    uint32_t coolDown = 500;
    uint32_t lastShootTime = 0;
    uint32_t boostRateOfFire = 0;
    uint32_t boostDamage = 0;
    uint32_t boostSpeed = 0;
    uint32_t boostProjectileNumber = 0;
};

struct Shield {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    int32_t width = 0;
    int32_t height = 0;
    uint32_t shieldTime = 0;
    uint32_t shieldStartTime = 0;
};

struct Enemy {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    int32_t width = 0;
    int32_t height = 0;
    int32_t speed = 150;
    int32_t currentHealth = 2;
    uint32_t coolDown = 2000;
    uint32_t lastShootTime = 0;
};

struct ProjectilePlayer {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    int32_t width = 0;
    int32_t height = 0;
    int32_t speed = 600;
    int32_t damage = 1;
};

struct ProjectileEnemy {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    SDL_FPoint direction = { 0.f, 0.f };
    int32_t width = 0;
    int32_t height = 0;
    int32_t speed = 400;
    int32_t damage = 1;
};

struct Explosion {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    int32_t width = 0;
    int32_t height = 0;
    int32_t currentFrame = 0;
    int32_t totalFrame = 0;
    uint32_t startTime = 0;
    uint32_t FPS = 10;
};

struct Item {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    SDL_FPoint direction = { 0.f, 0.f };
    int32_t width = 0;
    int32_t height = 0;
    int32_t speed = 200;
    int32_t bounceCount = 3;
    ItemType type = ItemType::Life;
};

struct Background {
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0.f, 0.f };
    float offset = 0.f;
    int32_t width = 0;
    int32_t height = 0;
    int32_t speed = 30;
};

#endif