#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <QVector2D>

class Player : public Entity
{
private:
    bool isJumping;
    bool isGlutton;
    int fruitCount;
    int lives;

    // Estados de control
    bool isStunned;
    float stunTimer;

public:
    Player();

    void update(float dt) override;
    void onCollision(Entity* e) override;

    void jump();
    void collectItem();
    void takeDamage();

    // Getters básicos
    bool getIsJumping() const { return isJumping; }
    int getLives() const { return lives; }

    void setLives(int newLives) { lives = newLives; }
    bool getIsGlutton() const { return isGlutton; }
};

#endif // PLAYER_H
