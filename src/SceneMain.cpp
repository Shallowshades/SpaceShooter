#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneEnd.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>

const float PI = acos(-1.0f);

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    spawEnemy();
    updateEnemies(deltaTime);
    updatePlayer(deltaTime);
    updatePlayerShield(deltaTime);
    updateExplosions(deltaTime);
    updateItems(deltaTime);
    if (isDead) {
        changeSceneDelayed(deltaTime, 3); // 3秒后切换到标题场景
    }
}

void SceneMain::render()
{
    // 渲染玩家子弹
    renderPlayerProjectiles();
    //渲染敌机子弹
    renderEnemyProjectiles();
    // 渲染玩家
    renderPlayer();
    // 渲染护盾
    renderPlayerShield();
    // 渲染敌人
    renderEnemies();
    // 渲染物品
    renderItems();
    // 渲染爆炸效果
    renderExplosions();
    // 渲染UI
    renderUI();
}

void SceneMain::handleEvent(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            auto sceneTitle = new SceneTitle();
            game.changeScene(sceneTitle);
        }
    }
}

void SceneMain::init()
{
    // 读取并播放背景音乐
    bgm = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);

    // 读取ui Health
    uiHealth = IMG_LoadTexture(game.getRenderer(), "assets/image/Health UI Black.png");

    // 载入字体
    scoreFont = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);

    // 读取音效资源
    sounds["player_shoot"] = Mix_LoadWAV("assets/sound/laser_shoot4.wav");
    sounds["enemy_shoot"] = Mix_LoadWAV("assets/sound/xs_laser.wav");
    sounds["player_explode"] = Mix_LoadWAV("assets/sound/explosion1.wav");
    sounds["enemy_explode"] = Mix_LoadWAV("assets/sound/explosion3.wav");
    sounds["hit"] = Mix_LoadWAV("assets/sound/eff11.wav");
    sounds["get_item"] = Mix_LoadWAV("assets/sound/eff5.wav");

    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", SDL_GetError());
    }
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 5;
    player.height /= 5;
    player.position.x = 1.f * (game.getWindowWidth() / 2 - player.width / 2);
    player.position.y = 1.f * (game.getWindowHeight() - player.height);

    playerShield.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/shield.png");
    if (playerShield.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player shield texture: %s", SDL_GetError());
    }
    SDL_QueryTexture(playerShield.texture, nullptr, nullptr, &playerShield.width, &playerShield.height);
    playerShield.width /= 5;
    playerShield.height /= 5;
    playerShield.position.x = player.position.x + player.width / 2 - playerShield.width / 2;
    playerShield.position.y = player.position.y - playerShield.height;

    // 初始化模版
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
    SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;

    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, nullptr, nullptr, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 2;
    projectileEnemyTemplate.height /= 2;

    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    SDL_QueryTexture(explosionTemplate.texture, nullptr, nullptr, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.height *= 2;
    explosionTemplate.width = explosionTemplate.height;

    itemLifeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    SDL_QueryTexture(itemLifeTemplate.texture, nullptr, nullptr, &itemLifeTemplate.width, &itemLifeTemplate.height);
    itemLifeTemplate.width /= 4;
    itemLifeTemplate.height /= 4;

    itemBoostTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/support.png");
    SDL_QueryTexture(itemBoostTemplate.texture, nullptr, nullptr, &itemBoostTemplate.width, &itemBoostTemplate.height);
    itemBoostTemplate.width /= 2;
    itemBoostTemplate.height /= 2;
    itemBoostTemplate.type = ItemType::Boost;

    itemShieldTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_shield.png");
    SDL_QueryTexture(itemShieldTemplate.texture, nullptr, nullptr, &itemShieldTemplate.width, &itemShieldTemplate.height);
    itemShieldTemplate.width /= 4;
    itemShieldTemplate.height /= 4;
    itemShieldTemplate.type = ItemType::Shield;
}

void SceneMain::clean()
{
    // 清理容器
    for (auto sound : sounds) {
        if (sound.second != nullptr) {
            Mix_FreeChunk(sound.second);
        }
    }
    sounds.clear();

    for (auto& projectile : projectilesPlayer) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();

    for (auto& projectile : projectilesEnemy) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesEnemy.clear();

    for (auto& explosion : explosions) {
        if (explosion != nullptr) {
            delete explosion;
        }
    }
    explosions.clear();

    for (auto& item : items) {
        if (item != nullptr) {
            delete item;
        }
    }
    items.clear();

    // 清理ui
    if (uiHealth != nullptr) {
        SDL_DestroyTexture(uiHealth);
    }

    // 清理字体
    if (scoreFont != nullptr) {
        TTF_CloseFont(scoreFont);
    }

    // 清理模版
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }
    if (explosionTemplate.texture != nullptr) {
        SDL_DestroyTexture(explosionTemplate.texture);
    }
    if (itemLifeTemplate.texture != nullptr) {
        SDL_DestroyTexture(itemLifeTemplate.texture);
    }

    // 清理音乐资源
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    if (isDead) {
        return;
    }
    auto keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_W]) {
        player.position.y -= deltaTime * player.speed + player.boostSpeed;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player.position.y += deltaTime * player.speed + player.boostSpeed;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player.position.x -= deltaTime * player.speed + player.boostSpeed;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player.position.x += deltaTime * player.speed + player.boostSpeed;
    }

    // 限制飞机的移动范围
    if (player.position.x < 0) {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width) {
        player.position.x = 1.f * (game.getWindowWidth() - player.width);
    }
    if (player.position.y < 0) {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height) {
        player.position.y = 1.f * (game.getWindowHeight() - player.height);
    }

    // 子弹发射
    if (keyboardState[SDL_SCANCODE_J]) {
        auto currentTime = SDL_GetTicks();
        // 控制射速
        if (currentTime - player.lastShootTime > player.coolDown - player.boostRateOfFire) {
            shootPlayer();
            player.lastShootTime = currentTime;
        }
    }

}

void SceneMain::shootPlayer()
{
    // 在这里实现发射子弹的逻辑
    // 控制子弹数量和位置
    for (uint32_t i = 0; i < player.boostProjectileNumber + 1; ++i) {
        auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
        projectile->position.x = player.position.x + player.width * (i + 1) / (player.boostProjectileNumber + 2) - projectile->width / 2;
        projectile->position.y = player.position.y;
        projectilesPlayer.push_back(projectile);
    }
    Mix_PlayChannel(0, sounds["player_shoot"], 0);
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    int margin = 32; // 子弹超出屏幕外边界的距离
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        // 检查子弹是否超出屏幕
        if (projectile->position.y + margin < 0) {
            delete projectile;
            it = projectilesPlayer.erase(it);
        }
        else {
            bool hit = false;
            for (auto enemy : enemies) {
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };
                SDL_Rect projectileRect = {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height
                };
                if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
                    // 控制伤害
                    enemy->currentHealth -= projectile->damage + player.boostDamage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    hit = true;
                    Mix_PlayChannel(-1, sounds["hit"], 0);
                    break;
                }
            }
            if (!hit) {
                ++it;
            }
        }
    }
}

void SceneMain::renderPlayerProjectiles()
{
    for (auto projectile : projectilesPlayer) {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &projectileRect);
    }
}

void SceneMain::renderEnemyProjectiles()
{
    for (auto projectile : projectilesEnemy) {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        // SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &projectileRect);
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / PI - 90;
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, nullptr, &projectileRect, angle, nullptr, SDL_FLIP_NONE);
    }
}

void SceneMain::spawEnemy()
{
    if (dis(gen) > 1 / 60.0f) {
        return;
    }
    Enemy* enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -1.f * enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::changeSceneDelayed(float deltaTime, float delay)
{
    timerEnd += deltaTime;
    if (timerEnd > delay) {
        auto sceneEnd = new SceneEnd();
        game.changeScene(sceneEnd);
    }
}

void SceneMain::updateEnemies(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto enemy = *it;
        enemy->position.y += enemy->speed * deltaTime;
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            it = enemies.erase(it);
        }
        else {
            if (currentTime - enemy->lastShootTime > enemy->coolDown && isDead == false) {
                shootEnemy(enemy);
                enemy->lastShootTime = currentTime;
            }
            if (enemy->currentHealth <= 0) {
                enemyExplode(enemy);
                it = enemies.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    auto margin = 32;
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();) {
        auto projectile = *it;
        projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;
        if (projectile->position.y > game.getWindowHeight() + margin ||
            projectile->position.y < -margin ||
            projectile->position.x < -margin ||
            projectile->position.x > game.getWindowWidth() + margin) {
            delete projectile;
            it = projectilesEnemy.erase(it);
        }
        else {
            SDL_Rect projectileRect = {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height
            };
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            if (SDL_HasIntersection(&projectileRect, &playerRect) && !isDead) {
                auto currentTime = SDL_GetTicks();
                // 护盾生效
                if (currentTime - playerShield.shieldStartTime < playerShield.shieldTime) {
                    playerShield.shieldTime = 0;
                }
                else {
                    player.currentHealth -= projectile->damage;
                }
                delete projectile;
                it = projectilesEnemy.erase(it);
                Mix_PlayChannel(-1, sounds["hit"], 0);
            }
            else {
                ++it;
            }
        }
    }
}

void SceneMain::updatePlayer(float)
{
    if (isDead) {
        return;
    }
    if (player.currentHealth <= 0) {
        // TODO: Game over
        auto currentTime = SDL_GetTicks();
        isDead = true;
        auto explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
        explosion->startTime = currentTime;
        explosions.push_back(explosion);
        Mix_PlayChannel(-1, sounds["player_explode"], 0);
        game.setFinalScore(score);
        return;
    }
    for (auto enemy : enemies) {
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            auto currentTime = SDL_GetTicks();
            // 护盾生效
            if (currentTime - playerShield.shieldStartTime < playerShield.shieldTime) {
                playerShield.shieldTime = 0;
            }
            else {
                player.currentHealth -= 1;
            }
            enemy->currentHealth = 0;
        }
    }
}

void SceneMain::updatePlayerShield(float deltaTime) {
    playerShield.position.x = player.position.x + player.width / 2 - playerShield.width / 2;
    playerShield.position.y = player.position.y - playerShield.height;
}

void SceneMain::renderEnemies()
{
    for (auto enemy : enemies) {
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, nullptr, &enemyRect);
    }
}

void SceneMain::shootEnemy(Enemy* enemy)
{
    auto projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;
    projectile->direction = getDirection(enemy);
    projectilesEnemy.push_back(projectile);
    Mix_PlayChannel(-1, sounds["enemy_shoot"], 0);
}

SDL_FPoint SceneMain::getDirection(Enemy* enemy)
{
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    auto length = sqrt(x * x + y * y);
    x /= length;
    y /= length;
    return SDL_FPoint{ x, y };
}

void SceneMain::enemyExplode(Enemy* enemy)
{
    auto currentTime = SDL_GetTicks();
    auto explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    explosion->startTime = currentTime;
    explosions.push_back(explosion);
    Mix_PlayChannel(-1, sounds["enemy_explode"], 0);
    float rate = dis(gen);
    if (rate > 0.9f) { // 10%概率掉落增强道具
        dropItem(enemy, ItemType::Boost);
        std::cout << "Boost" << std::endl;
    }
    else if (rate > 0.7f) { // 20%概率掉落生命道具
        dropItem(enemy, ItemType::Life);
        std::cout << "Life" << std::endl;
    }
    else if (rate > 0.4f) { // 30%概率掉落护盾道具
        dropItem(enemy, ItemType::Shield);
        std::cout << "Shield" << std::endl;
    }
    score += 10;
    delete enemy;
}

void SceneMain::updateExplosions(float)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();)
    {
        auto explosion = *it;
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
        if (explosion->currentFrame >= explosion->totalFrame) {
            delete explosion;
            it = explosions.erase(it);
        }
        else {
            ++it;
        }
    }
}

void SceneMain::renderExplosions()
{
    for (auto explosion : explosions)
    {
        SDL_Rect src = { explosion->currentFrame * explosion->width, 0, explosion->width / 2, explosion->height / 2 };
        SDL_Rect dst = {
            static_cast<int>(explosion->position.x),
            static_cast<int>(explosion->position.y),
            explosion->width,
            explosion->height
        };
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dst);
    }
}

void SceneMain::dropItem(Enemy* enemy, ItemType type) {
    Item* item = nullptr;
    switch (type) {
    case ItemType::Life:
        item = new Item(itemLifeTemplate);
        break;
    case ItemType::Boost:
        item = new Item(itemBoostTemplate);
        break;
    case ItemType::Shield:
        item = new Item(itemShieldTemplate);
        break;
    default:
        break;
    };
    if (item == nullptr) {
        return;
    }
    item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
    item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2;
    float angle = dis(gen) * 2 * PI;
    item->direction.x = cos(angle);
    item->direction.y = sin(angle);
    items.push_back(item);
}

void SceneMain::updateItems(float deltaTime)
{
    for (auto it = items.begin(); it != items.end();)
    {
        auto item = *it;
        // 更新位置
        item->position.x += item->direction.x * item->speed * deltaTime;
        item->position.y += item->direction.y * item->speed * deltaTime;
        // 处理屏幕边缘反弹
        if (item->position.x < 0 && item->bounceCount > 0) {
            item->direction.x = -item->direction.x;
            item->bounceCount--;
        }
        if (item->position.x + item->width > game.getWindowWidth() && item->bounceCount > 0) {
            item->direction.x = -item->direction.x;
            item->bounceCount--;
        }
        if (item->position.y < 0 && item->bounceCount > 0) {
            item->direction.y = -item->direction.y;
            item->bounceCount--;
        }
        if (item->position.y + item->height > game.getWindowHeight() && item->bounceCount > 0) {
            item->direction.y = -item->direction.y;
            item->bounceCount--;
        }
        // 如果超出屏幕范围则删除
        if (item->position.x + item->width < 0 ||
            item->position.x > game.getWindowWidth() ||
            item->position.y + item->height < 0 ||
            item->position.y > game.getWindowHeight()) {
            delete item;
            it = items.erase(it);
        }
        else {
            SDL_Rect itemRect = {
                static_cast<int>(item->position.x),
                static_cast<int>(item->position.y),
                item->width,
                item->height
            };
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            if (SDL_HasIntersection(&itemRect, &playerRect) && isDead == false)
            {
                playerGetItem(item);
                delete item;
                it = items.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::playerGetItem(Item* item)
{
    score += 5;
    switch (item->type) {
    case ItemType::Life:
        player.currentHealth += 1;
        if (player.currentHealth > player.maxHealth) {
            player.currentHealth = player.maxHealth;
        }
        break;
    case ItemType::Boost: {
        // float rate = dis(gen);
        // if (rate > 0.75) {
        //     player.boostRateOfFire = std::clamp(player.boostRateOfFire + 100, 0u, 500u);
        // }
        // else if (rate > 0.5) {
        //     player.boostSpeed = std::clamp(player.boostSpeed + 50, 0u, 300u);
        // }
        // else if (rate > 0.25) {
        //     player.boostDamage = std::clamp(player.boostDamage + 1, 0u, 4u);
        // }
        // else {
        player.boostProjectileNumber = std::clamp(player.boostProjectileNumber + 1, 0u, 3u);
        // }
        break;
    }
    case ItemType::Shield:
        playerShield.shieldStartTime = SDL_GetTicks();
        playerShield.shieldTime = 5000;
        std::cout << "Get Shield" << std::endl;
        break;
    default:
        break;
    }
    Mix_PlayChannel(-1, sounds["get_item"], 0);
}

void SceneMain::renderPlayer() {
    if (!isDead) {
        SDL_Rect playerRect = { static_cast<int>(player.position.x),
                        static_cast<int>(player.position.y),
                        player.width,
                        player.height };
        SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
    }
}

void SceneMain::renderPlayerShield() {
    if (playerShield.shieldTime > 0 && SDL_GetTicks() - playerShield.shieldStartTime < playerShield.shieldTime) {
        SDL_Rect rect = { static_cast<int>(playerShield.position.x),
                        static_cast<int>(playerShield.position.y),
                        playerShield.width,
                        playerShield.height };
        SDL_RenderCopy(game.getRenderer(), playerShield.texture, nullptr, &rect);
    }
}

void SceneMain::renderItems()
{
    for (auto& item : items)
    {
        SDL_Rect itemRect = {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width,
            item->height
        };
        SDL_RenderCopy(game.getRenderer(), item->texture, nullptr, &itemRect);
    }
}

void SceneMain::renderUI() {
    // 渲染血条
    int x = 10;
    int y = 10;
    int size = 32;
    int offset = 40;
    SDL_SetTextureColorMod(uiHealth, 100, 100, 100); // 颜色减淡
    for (int i = 0; i < player.maxHealth; i++)
    {
        SDL_Rect rect = { x + i * offset, y, size, size };
        SDL_RenderCopy(game.getRenderer(), uiHealth, nullptr, &rect);
    }
    SDL_SetTextureColorMod(uiHealth, 255, 255, 255); // reset color
    for (int i = 0; i < player.currentHealth; i++)
    {
        SDL_Rect rect = { x + i * offset, y, size, size };
        SDL_RenderCopy(game.getRenderer(), uiHealth, nullptr, &rect);
    }
    // 渲染得分
    auto text = "SCORE:" + std::to_string(score);
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderUTF8_Solid(scoreFont, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
    SDL_Rect rect = { game.getWindowWidth() - 10 - surface->w, 10, surface->w, surface->h };
    SDL_RenderCopy(game.getRenderer(), texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}