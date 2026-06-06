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

    unsigned char lives;
    unsigned short collectedFruits;
    bool isGrounded;

    bool isInvincible;
    float invincibilityTimer;
    float invincibilityDuration;

    bool isGlutton;
    unsigned short fruitsNeededForNextRush;

public:
    Player();
    virtual ~Player() override = default;

    // Actualiza fisica y estados
    virtual void update(float dt) override;

    // Procesa colisiones
    virtual void onCollision(Entity* other) override;

    // Realiza salto
    void jump();

    // Aplica daño
    void takeDamage();

    // Activa invencibilidad
    void startInvincibility();

    // Recolecta fruta
    void collectItem();

    // Reinicia estadisticas
    void reset();

    // Getters
    unsigned char getLives() const { return lives; }

    unsigned short getCollectedFruits() const
    {
        return collectedFruits;
    }

    bool getIsGlutton() const { return isGlutton; }

    bool getIsInvincible() const
    {
        return isInvincible;
    }

    bool getIsGrounded() const
    {
        return isGrounded;
    }
};

#endif // PLAYER_H
