#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entity.h"

class Obstacle : public Entity
{
private:
    unsigned short damage;

public:
    Obstacle();
    ~Obstacle();

    void update(float dt) override;
    void onCollision(Entity* e) override;

    // Getters - Setters
    unsigned short getDamage() const {
        return damage;
    }
    void setDamage(unsigned short d) {
        damage = d;
    }
};

#endif // OBSTACLE_H
