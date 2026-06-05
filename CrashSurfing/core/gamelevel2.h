#ifndef GAMELEVEL2_H
#define GAMELEVEL2_H

#include <vector>

#include "../entities/playerlevel2.h"
#include "../entities/enemy2.h"
#include "../entities/item.h"
#include "../entities/obstacle.h"
#include "../entities/whirlpool.h"

enum class GameLevel2Status
{
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETE
};

class GameLevel2
{
public:
    GameLevel2();
    ~GameLevel2();

    void update(float dt);
    void reset();

    PlayerLevel2* getPlayer();
    Enemy2* getEnemy2();
    Whirlpool& getWhirlpool();

    const std::vector<Item*>& getFruits() const;
    const std::vector<Obstacle*>& getObstacles() const;
    const std::vector<Obstacle*>& getProjectiles() const;

    float getRemainingTime() const;

    GameLevel2Status getStatus() const;
    void setStatus(GameLevel2Status newStatus);

private:
    void loadLevel();
    void clearLevelEntities();

    void updateWhirlpoolForce(float dt);
    void updateEnemyDecision();
    void updateProjectiles(float dt);

    void spawnFruits();
    void spawnBoxes();
    void spawnSlowProjectile(const QVector2D& targetPosition);

    void checkCollisions();
    bool isColliding(const Entity* a, const Entity* b) const;

private:
    PlayerLevel2* player;
    Enemy2* enemy2;

    Whirlpool whirlpool;

    std::vector<Item*> fruits;
    std::vector<Obstacle*> obstacles;
    std::vector<Obstacle*> projectiles;

    float survivalTimer;
    float targetTime;

    GameLevel2Status status;
};

#endif // GAMELEVEL2_H
