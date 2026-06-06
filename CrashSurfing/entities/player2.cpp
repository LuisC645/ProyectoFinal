#include "player2.h"

#include "item.h"
#include "obstacle.h"

#include <QtMath>

namespace
{
constexpr float START_X = 1200.0f;
constexpr float START_Y = 450.0f;

constexpr float PLAYER_WIDTH = 40.0f;
constexpr float PLAYER_HEIGHT = 40.0f;

constexpr float BASE_SPEED = 250.0f;

constexpr unsigned char INITIAL_LIVES = 5;
constexpr unsigned short MAX_EFFECTIVE_FRUITS = 10;

constexpr float ARENA_WIDTH = 1600.0f;
constexpr float ARENA_HEIGHT = 600.0f;

constexpr float INVINCIBILITY_DURATION = 1.0f;
constexpr float SLOW_DURATION = 1.0f;
constexpr float SLOW_FACTOR = 0.50f;
constexpr float FRUIT_RESISTANCE = 0.03f;
}

// Inicializa atributos del jugador
Player2::Player2()
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

// Actualiza movimiento y estados
void Player2::update(float dt)
{
    if (!active){ return; }

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

    externalVelocity = QVector2D(0.0f, 0.0f);
}

// Procesa colisiones
void Player2::onCollision(Entity* e)
{
    if (!e || !e->isActive()){ return; }

    Obstacle* obstacle = dynamic_cast<Obstacle*>(e);

    if (obstacle)
    {
        const QString type = obstacle->getType().toLower();

        if (type == "tronco")
        {
            takeDamage();
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

// Aplica daño al jugador
void Player2::takeDamage()
{
    if (isInvincible || !active){ return; }

    loseLife();
    startInvincibility();

    if (lives == 0)
    {
        active = false;
    }
}

// Reduce una vida
void Player2::loseLife()
{
    if (lives > 0)
    {
        --lives;
    }
}

// Activa invencibilidad
void Player2::startInvincibility()
{
    isInvincible = true;
    invincibilityTimer = invincibilityDuration;
}

// Aplica ralentizacion
void Player2::applySlow()
{
    isSlowed = true;
    slowTimer = slowDuration;
}

// Incrementa frutas recolectadas
void Player2::collectFruit()
{
    ++collectedFruits;
}

// Restaura atributos iniciales
void Player2::reset()
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

// Calcula resistencia al remolino
float Player2::getWhirlpoolResistance() const
{
    const unsigned short effectiveFruits =
        (collectedFruits > MAX_EFFECTIVE_FRUITS)
            ? MAX_EFFECTIVE_FRUITS
            : collectedFruits;

    return effectiveFruits * FRUIT_RESISTANCE;
}

// Actualiza temporizadores
void Player2::updateTimers(float dt)
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

// Calcula velocidad efectiva
void Player2::updateCurrentSpeed()
{
    currentSpeed = baseSpeed;

    if (isSlowed)
    {
        currentSpeed = baseSpeed * (1.0f - slowFactor);
    }
}

// Limita al area de juego
void Player2::clampToArena()
{
    if (position.x() < 0.0f){ position.setX(0.0f); }
    if (position.y() < 0.0f){ position.setY(0.0f); }

    if (position.x() > arenaWidth){ position.setX(arenaWidth); }
    if (position.y() > arenaHeight){ position.setY(arenaHeight); }
}
