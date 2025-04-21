#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include <list>
#include <map>
#include <random>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "Scene.h"
#include "Game.h"
#include "Object.h"


class SceneMain : public Scene {
public:
    ~SceneMain();
    virtual void init() override;
    virtual void update(float) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event*) override;
private:
    // 渲染
    void renderItems(); // 渲染道具
    void renderEnemies(); // 渲染敌人
    void renderPlayerProjectiles(); // 渲染玩家子弹
    void renderEnemyProjectiles(); // 渲染敌人子弹
    void renderExplosions(); // 渲染爆炸
    void renderUI(); // 渲染UI
    // 更新
    void updateEnemies(float deltaTime); // 更新敌人
    void updateEnemyProjectiles(float deltaTime); // 更新敌人子弹
    void updatePlayer(float deltaTime); // 更新玩家
    void updatePlayerProjectiles(float deltaTime); // 更新玩家子弹
    void updateItems(float deltaTime); // 更新道具
    void updateExplosions(float deltaTime); // 更新爆炸
    // 处理事件
    void keyboardControl(float deltaTime); // 处理键盘事件
    void spawEnemy();   // 生成敌人
    void changeSceneDelayed(float deltaTime, float delay); // 延迟切换场景
    // 其它
    void playerGetItem(Item* item); // 玩家获取道具
    void shootPlayer(); // 玩家射击
    void shootEnemy(Enemy* enemy); // 敌人射击
    SDL_FPoint getDirection(Enemy* enemy); // 获取敌人射击方向
    void enemyExplode(Enemy* enemy); // 敌人爆炸
    void dropItem(Enemy* enemy); // 敌人掉落道具
private:
    Player player;
    Mix_Music* bgm = nullptr; // 音乐
    SDL_Texture* uiHealth;
    TTF_Font* scoreFont;
    int32_t score = 0; // 分数
    float timerEnd = 0.f;

    bool isDead = false; // 玩家是否死亡
    std::mt19937 gen; // 随机数生成器
    std::uniform_real_distribution<float> dis; // 随机数分布器

    // Objects Template
    Enemy enemyTemplate; // 敌人模板
    ProjectilePlayer projectilePlayerTemplate; // 玩家子弹模板
    ProjectileEnemy projectileEnemyTemplate; // 敌人子弹模板
    Explosion explosionTemplate; // 爆炸模板
    Item itemLifeTemplate; // 生命道具模板

    // Objects Capicity
    std::list<Enemy*> enemies; // 敌人列表
    std::list<ProjectilePlayer*> projectilesPlayer; // 玩家子弹列表
    std::list<ProjectileEnemy*> projectilesEnemy; // 敌人子弹列表
    std::list<Explosion*> explosions; // 爆炸列表
    std::list<Item*> items; // 道具列表
    std::map<std::string, Mix_Chunk*> sounds; // 音效列表
};

#endif