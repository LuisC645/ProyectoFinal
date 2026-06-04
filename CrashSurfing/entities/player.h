#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <QVector2D>

class Player : public Entity {
private:
    float speed;
    float jumpForce;
    float gravity;
    float floorY;

    int lives;
    int collectedFruits;
    bool isGrounded;

    // Invencibilidad
    bool isInvincible;
    float invincibilityTimer;
    float invincibilityDuration;

    bool isGlutton;
    int fruitsNeededForNextRush;

public:
    Player();
    virtual ~Player() override = default;

    virtual void update(float dt) override;
    virtual void onCollision(Entity* other) override;

    void jump();
    void takeDamage();
    void startInvincibility();
    void collectItem();
    void reset();

    // Getters
    int getLives() const { return lives; }
    int getCollectedFruits() const { return collectedFruits; }
    bool getIsGlutton() const { return isGlutton; }
    bool getIsInvincible() const { return isInvincible; }
    bool getIsGrounded() const { return isGrounded; }

};

#endif // PLAYER_H
