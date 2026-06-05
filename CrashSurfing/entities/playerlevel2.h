#ifndef PLAYERLEVEL2_H
#define PLAYERLEVEL2_H

#include "entity.h"

class PlayerLevel2 : public Entity
{
public:
    PlayerLevel2();
    ~PlayerLevel2();

    void update(float dt) override;
    void onCollision(Entity* e) override;

    // Movimiento
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void stopHorizontal();
    void stopVertical();

    // Vidas
    void loseLife();
    int getLives() const;

    // Frutas
    void collectFruit();
    int getCollectedFruits() const;

    // Estado
    void reset();

private:

    float speed;

    int lives;

    int collectedFruits;

    float arenaWidth;
    float arenaHeight;
};

#endif
