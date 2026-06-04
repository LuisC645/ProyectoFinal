#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Agent;

class Enemy : public Entity
{
private:

    // Puntero al agente
    Agent* agent;
    unsigned short health;

    float shootTimer;
    float shootCooldown;

    int misses;

    float throwForce;

    float phase;

public:
    Enemy();
    ~Enemy();

    void update(float dt) override;
    void onCollision(Entity* e) override;

    void setAgent(Agent* a) { agent = a; }
    Agent* getAgent() const { return agent; }

    unsigned short getHealth() const { return health; }
    void setHealth(unsigned short h) { health = h; }
    bool canShoot() const;
    void resetShootTimer();

};

#endif // ENEMY_H
