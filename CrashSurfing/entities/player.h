#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
private:
    int lives;
    bool invulnerable;

    float startY;
    float v0y;
    float baseGravity;
    float gravity;
    float timeJumping;
    bool isJumping;

    bool isGlutton;     // Personalidad 1: Si come mucho, se vuelve pesado (gravedad aumenta)
    bool isScared;      // Personalidad 2: Si el enemigo está cerca, corre mas rapido
    int itemsCollected; // Contador para activar "isGlutton"

public:
    Player();
    ~Player();

    void update(float dt) override;
    void onCollision(Entity* e) override;

    void input();
    void applyPhysics(float dt);

    void jump();
    void collectItem();
    void checkScaredStatus(float enemyX, float enemyY);

    int getLives() const { return lives; }
    void setLives(int l) { lives = l; }
    bool getIsJumping() const { return isJumping; }
    bool getIsGlutton() const { return isGlutton; }
    bool getIsScared() const { return isScared; }
};

#endif // PLAYER_H
