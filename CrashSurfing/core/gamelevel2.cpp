#include "gamelevel2.h"

#include <QRandomGenerator>
#include <QVector2D>

namespace
{
constexpr float TARGET_TIME = 60.0f;

constexpr int FRUIT_COUNT = 30;
constexpr int BOX_COUNT = 18;

constexpr float MIN_X = 80.0f;
constexpr float MAX_X = 1480.0f;
constexpr float MIN_Y = 80.0f;
constexpr float MAX_Y = 520.0f;

constexpr float PROJECTILE_SPEED = 360.0f;
}

GameLevel2::GameLevel2()
{
    player = new PlayerLevel2();
    enemy2 = new Enemy2();

    survivalTimer = 0.0f;
    targetTime = TARGET_TIME;
    status = GameLevel2Status::MENU;

    loadLevel();
}

GameLevel2::~GameLevel2()
{
    clearLevelEntities();

    delete enemy2;
    delete player;
}

void GameLevel2::update(float dt)
{
    if (status != GameLevel2Status::PLAYING)
        return;

    updateWhirlpoolForce(dt);

    if (player)
        player->update(dt);

    if (enemy2)
        enemy2->update(dt);

    updateEnemyDecision();
    updateProjectiles(dt);
    checkCollisions();

    survivalTimer += dt;

    if (!player || !player->isActive() || whirlpool.isInsideDeathZone(player))
    {
        status = GameLevel2Status::GAME_OVER;
        return;
    }

    if (survivalTimer >= targetTime)
    {
        status = GameLevel2Status::LEVEL_COMPLETE;
        return;
    }
}

void GameLevel2::reset()
{
    if (player)
        player->reset();

    if (enemy2)
    {
        delete enemy2;
        enemy2 = new Enemy2();
    }

    clearLevelEntities();
    loadLevel();

    survivalTimer = 0.0f;
    status = GameLevel2Status::MENU;
}

PlayerLevel2* GameLevel2::getPlayer()
{
    return player;
}

Enemy2* GameLevel2::getEnemy2()
{
    return enemy2;
}

Whirlpool& GameLevel2::getWhirlpool()
{
    return whirlpool;
}

const std::vector<Item*>& GameLevel2::getFruits() const
{
    return fruits;
}

const std::vector<Obstacle*>& GameLevel2::getObstacles() const
{
    return obstacles;
}

const std::vector<Obstacle*>& GameLevel2::getProjectiles() const
{
    return projectiles;
}

float GameLevel2::getRemainingTime() const
{
    return targetTime - survivalTimer;
}

GameLevel2Status GameLevel2::getStatus() const
{
    return status;
}

void GameLevel2::setStatus(GameLevel2Status newStatus)
{
    status = newStatus;
}

void GameLevel2::loadLevel()
{
    spawnFruits();
    spawnBoxes();
}

void GameLevel2::clearLevelEntities()
{
    for (Item* fruit : fruits)
        delete fruit;
    fruits.clear();

    for (Obstacle* obstacle : obstacles)
        delete obstacle;
    obstacles.clear();

    for (Obstacle* projectile : projectiles)
        delete projectile;
    projectiles.clear();
}

void GameLevel2::updateWhirlpoolForce(float dt)
{
    if (!player)
        return;

    QVector2D frameContribution =
        whirlpool.computeFrameVelocity(player, dt);

    frameContribution *= (1.0f - player->getWhirlpoolResistance());

    player->setExternalVelocity(frameContribution);
}

void GameLevel2::updateEnemyDecision()
{
    if (!enemy2 || !player)
        return;

    if (!enemy2->canEvaluateShot())
        return;

    const bool shouldFire = enemy2->shouldShoot(player, whirlpool);

    if (shouldFire)
    {
        spawnSlowProjectile(enemy2->getLastPredictedPosition());
        enemy2->registerShotFired();
    }

    enemy2->resetDecisionTimer();
}

void GameLevel2::updateProjectiles(float dt)
{
    for (Obstacle* projectile : projectiles)
    {
        if (!projectile || !projectile->isActive())
            continue;

        projectile->update(dt);

        const QVector2D pos = projectile->getPosition();

        if (pos.x() < -50.0f || pos.x() > 1650.0f ||
            pos.y() < -50.0f || pos.y() > 650.0f)
        {
            projectile->setActive(false);
        }
    }
}

void GameLevel2::spawnFruits()
{
    for (int i = 0; i < FRUIT_COUNT; ++i)
    {
        const float x = static_cast<float>(
            MIN_X +
            QRandomGenerator::global()->generateDouble() * (MAX_X - MIN_X)
            );

        const float y = static_cast<float>(
            MIN_Y +
            QRandomGenerator::global()->generateDouble() * (MAX_Y - MIN_Y)
            );

        Item* fruit = new Item("fruit", QVector2D(x, y), 28.0f, 28.0f);
        fruits.push_back(fruit);
    }
}

void GameLevel2::spawnBoxes()
{
    for (int i = 0; i < BOX_COUNT; ++i)
    {
        const float x = static_cast<float>(
            MIN_X +
            QRandomGenerator::global()->generateDouble() * (MAX_X - MIN_X)
            );

        const float y = static_cast<float>(
            MIN_Y +
            QRandomGenerator::global()->generateDouble() * (MAX_Y - MIN_Y)
            );

        Obstacle* box = new Obstacle("log", QVector2D(x, y));
        box->setWidth(40.0f);
        box->setHeight(40.0f);
        obstacles.push_back(box);
    }
}

void GameLevel2::spawnSlowProjectile(const QVector2D& targetPosition)
{
    if (!enemy2)
        return;

    Obstacle* projectile =
        new Obstacle("slow_projectile", enemy2->getPosition());

    projectile->setWidth(18.0f);
    projectile->setHeight(18.0f);

    QVector2D direction = targetPosition - enemy2->getPosition();

    if (direction.length() > 0.001f)
        direction.normalize();
    else
        direction = QVector2D(-1.0f, 0.0f);

    projectile->setVelocity(direction * PROJECTILE_SPEED);
    projectiles.push_back(projectile);
}

void GameLevel2::checkCollisions()
{
    if (!player || !player->isActive())
        return;

    for (Item* fruit : fruits)
    {
        if (!fruit || !fruit->isActive())
            continue;

        if (isColliding(player, fruit))
        {
            player->onCollision(fruit);
        }
    }

    for (Obstacle* obstacle : obstacles)
    {
        if (!obstacle || !obstacle->isActive())
            continue;

        if (isColliding(player, obstacle))
        {
            player->onCollision(obstacle);
        }
    }

    for (Obstacle* projectile : projectiles)
    {
        if (!projectile || !projectile->isActive())
            continue;

        if (isColliding(player, projectile))
        {
            const bool wasSlowed = player->getIsSlowed();

            player->onCollision(projectile);

            if (!wasSlowed && player->getIsSlowed() && enemy2)
            {
                enemy2->registerShotHit();
                enemy2->registerSuccessfulSlowdown();
            }
        }
    }
}

bool GameLevel2::isColliding(const Entity* a, const Entity* b) const
{
    if (!a || !b)
        return false;

    const float ax1 = a->getPosition().x();
    const float ay1 = a->getPosition().y();
    const float ax2 = ax1 + a->getWidth();
    const float ay2 = ay1 + a->getHeight();

    const float bx1 = b->getPosition().x();
    const float by1 = b->getPosition().y();
    const float bx2 = bx1 + b->getWidth();
    const float by2 = by1 + b->getHeight();

    return (ax1 < bx2 && ax2 > bx1 &&
            ay1 < by2 && ay2 > by1);
}
