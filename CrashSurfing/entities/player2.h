#ifndef PLAYERLEVEL2_H
#define PLAYERLEVEL2_H

#include "entity.h"

class Player2 : public Entity
{
public:
    Player2();
    ~Player2() override = default;

    // Actualiza movimiento y estados
    void update(float dt) override;

    // Procesa colisiones
    void onCollision(Entity* e) override;

    // Movimiento por input
    void moveUp() { inputDirection.setY(-1.0f); }
    void moveDown() { inputDirection.setY(1.0f); }
    void moveLeft() { inputDirection.setX(-1.0f); }
    void moveRight() { inputDirection.setX(1.0f); }

    void stopHorizontal() { inputDirection.setX(0.0f); }
    void stopVertical() { inputDirection.setY(0.0f); }

    // Aplica velocidad externa
    void setExternalVelocity(const QVector2D& externalVel)
    {
        externalVelocity = externalVel;
    }

    // Estado del jugador
    void takeDamage();
    void loseLife();
    void startInvincibility();
    void applySlow();
    void collectFruit();
    void reset();

    // Getters
    unsigned char getLives() const { return lives; }

    unsigned short getCollectedFruits() const
    {
        return collectedFruits;
    }

    bool getIsInvincible() const
    {
        return isInvincible;
    }

    bool getIsSlowed() const
    {
        return isSlowed;
    }

    float getWhirlpoolResistance() const;

    float getCurrentSpeed() const
    {
        return currentSpeed;
    }

private:
    // Actualiza temporizadores
    void updateTimers(float dt);

    // Calcula velocidad efectiva
    void updateCurrentSpeed();

    // Limita al area de juego
    void clampToArena();

private:
    QVector2D inputDirection;
    QVector2D externalVelocity;

    float baseSpeed;
    float currentSpeed;

    unsigned char lives;
    unsigned short collectedFruits;

    bool isInvincible;
    float invincibilityTimer;
    float invincibilityDuration;

    bool isSlowed;
    float slowTimer;
    float slowDuration;
    float slowFactor;

    float arenaWidth;
    float arenaHeight;
};

#endif
