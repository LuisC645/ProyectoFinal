#ifndef PLAYERLEVEL2_H
#define PLAYERLEVEL2_H

#include "entity.h"

class PlayerLevel2 : public Entity
{
public:
    PlayerLevel2();
    ~PlayerLevel2() override;

    void update(float dt) override;
    void onCollision(Entity* e) override;

    // Movimiento por input
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void stopHorizontal();
    void stopVertical();

    // Movimiento externo por frame
    void setExternalVelocity(const QVector2D& externalVel);

    // Estado del jugador
    void takeDamage();
    void loseLife();
    void startInvincibility();
    void applySlow();
    void collectFruit();
    void reset();

    // Getters
    int getLives() const;
    int getCollectedFruits() const;
    bool getIsInvincible() const;
    bool getIsSlowed() const;
    float getWhirlpoolResistance() const;
    float getCurrentSpeed() const;

private:
    void updateTimers(float dt);
    void updateCurrentSpeed();
    void clampToArena();

private:
    QVector2D inputDirection;
    QVector2D externalVelocity;

    float baseSpeed;
    float currentSpeed;

    int lives;
    int collectedFruits;

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
