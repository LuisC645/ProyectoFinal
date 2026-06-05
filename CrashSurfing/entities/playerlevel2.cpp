#include "playerlevel2.h"

#include "item.h"
#include "obstacle.h"

#include <QtMath>

namespace
{
constexpr float START_X = 1200.0f;
constexpr float START_Y = 450.0f;

constexpr float PLAYER_WIDTH = 50.0f;
constexpr float PLAYER_HEIGHT = 50.0f;

constexpr float BASE_SPEED = 250.0f;

constexpr int INITIAL_LIVES = 5;
constexpr int MAX_EFFECTIVE_FRUITS = 10;

constexpr float ARENA_WIDTH = 1600.0f;
constexpr float ARENA_HEIGHT = 600.0f;

constexpr float INVINCIBILITY_DURATION = 1.0f;
constexpr float SLOW_DURATION = 4.0f;
constexpr float SLOW_FACTOR = 0.50f;
constexpr float FRUIT_RESISTANCE = 0.03f;
}

PlayerLevel2::PlayerLevel2()
{
    width = PLAYER_WIDTH;
    height = PLAYER_HEIGHT;

    arenaWidth = ARENA_WIDTH;
    arenaHeight = ARENA_HEIGHT;

    baseSpeed = BASE_SPEED;
    currentSpeed = BASE_SPEED;

    invincibilityDuration = INVINCIBILITY_DURATION;
    slowDuration = SLOW_DURATION;
    slowFactor = SLOW_FACTOR;

    reset();
}

PlayerLevel2::~PlayerLevel2()
{
}

void PlayerLevel2::update(float dt)
{
    if (!active)
        return;

    updateTimers(dt);
    updateCurrentSpeed();

    QVector2D inputVelocity(0.0f, 0.0f);

    if (!inputDirection.isNull())
    {
        inputVelocity = inputDirection.normalized() * currentSpeed;
    }

    velocity = inputVelocity + externalVelocity;
    position += velocity * dt;

    clampToArena();

    // La contribución externa se aplica solo en este frame
    externalVelocity = QVector2D(0.0f, 0.0f);
}

void PlayerLevel2::onCollision(Entity* e)
{
    if (!e || !e->isActive())
        return;

    Obstacle* obstacle = dynamic_cast<Obstacle*>(e);
    if (obstacle)
    {
        const QString type = obstacle->getType().toLower();

        // Cajas y proyectiles ralentizan; las cajas además hacen daño
        if (type == "log" || type == "tnt")
        {
            takeDamage();
            applySlow();
            return;
        }

        if (type == "enemy_slow" || type == "slow_projectile")
        {
            applySlow();
            obstacle->setActive(false);
            return;
        }
    }

    Item* item = dynamic_cast<Item*>(e);
    if (item && !item->getIsCollected())
    {
        const QString type = item->getType().toLower();

        if (type == "fruit")
        {
            collectFruit();
            item->setIsCollected(true);
            item->setActive(false);
        }
    }
}

void PlayerLevel2::moveUp()
{
    inputDirection.setY(-1.0f);
}

void PlayerLevel2::moveDown()
{
    inputDirection.setY(1.0f);
}

void PlayerLevel2::moveLeft()
{
    inputDirection.setX(-1.0f);
}

void PlayerLevel2::moveRight()
{
    inputDirection.setX(1.0f);
}

void PlayerLevel2::stopHorizontal()
{
    inputDirection.setX(0.0f);
}

void PlayerLevel2::stopVertical()
{
    inputDirection.setY(0.0f);
}

void PlayerLevel2::setExternalVelocity(const QVector2D& externalVel)
{
    externalVelocity = externalVel;
}

void PlayerLevel2::takeDamage()
{
    if (isInvincible || !active)
        return;

    loseLife();
    startInvincibility();

    if (lives <= 0)
    {
        lives = 0;
        active = false;
    }
}

void PlayerLevel2::loseLife()
{
    --lives;

    if (lives < 0)
        lives = 0;
}

void PlayerLevel2::startInvincibility()
{
    isInvincible = true;
    invincibilityTimer = invincibilityDuration;
}

void PlayerLevel2::applySlow()
{
    isSlowed = true;
    slowTimer = slowDuration;
}

void PlayerLevel2::collectFruit()
{
    ++collectedFruits;
}

void PlayerLevel2::reset()
{
    position = QVector2D(START_X, START_Y);
    velocity = QVector2D(0.0f, 0.0f);

    inputDirection = QVector2D(0.0f, 0.0f);
    externalVelocity = QVector2D(0.0f, 0.0f);

    lives = INITIAL_LIVES;
    collectedFruits = 0;

    baseSpeed = BASE_SPEED;
    currentSpeed = BASE_SPEED;

    isInvincible = false;
    invincibilityTimer = 0.0f;

    isSlowed = false;
    slowTimer = 0.0f;

    active = true;
}

int PlayerLevel2::getLives() const
{
    return lives;
}

int PlayerLevel2::getCollectedFruits() const
{
    return collectedFruits;
}

bool PlayerLevel2::getIsInvincible() const
{
    return isInvincible;
}

bool PlayerLevel2::getIsSlowed() const
{
    return isSlowed;
}

float PlayerLevel2::getWhirlpoolResistance() const
{
    const int effectiveFruits =
        (collectedFruits > MAX_EFFECTIVE_FRUITS)
            ? MAX_EFFECTIVE_FRUITS
            : collectedFruits;

    return effectiveFruits * FRUIT_RESISTANCE;
}

float PlayerLevel2::getCurrentSpeed() const
{
    return currentSpeed;
}

void PlayerLevel2::updateTimers(float dt)
{
    if (isInvincible)
    {
        invincibilityTimer -= dt;

        if (invincibilityTimer <= 0.0f)
        {
            invincibilityTimer = 0.0f;
            isInvincible = false;
        }
    }

    if (isSlowed)
    {
        slowTimer -= dt;

        if (slowTimer <= 0.0f)
        {
            slowTimer = 0.0f;
            isSlowed = false;
        }
    }
}

void PlayerLevel2::updateCurrentSpeed()
{
    currentSpeed = baseSpeed;

    if (isSlowed)
    {
        currentSpeed = baseSpeed * (1.0f - slowFactor);
    }
}

void PlayerLevel2::clampToArena()
{
    if (position.x() < 0.0f)
        position.setX(0.0f);

    if (position.y() < 0.0f)
        position.setY(0.0f);

    if (position.x() > arenaWidth)
        position.setX(arenaWidth);

    if (position.y() > arenaHeight)
        position.setY(arenaHeight);
}
