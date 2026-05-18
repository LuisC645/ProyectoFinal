#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
private:
    int lives;
    bool invulnerable;

public:
    Player();
    ~Player();

    void update(float dt);
    void onCollision(Entity* e);

    void input();
    void applyPhysics();

    int getLives() const {
        return lives;
    }
    void setLives(int l) {
        lives = l;
    }
};

#endif // PLAYER_H
