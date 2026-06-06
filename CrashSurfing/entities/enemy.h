#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Agent;

class Enemy : public Entity
{
private:

    // Controlador IA
    Agent* agent;

    unsigned short health;

    float shootTimer;
    float shootCooldown;

    unsigned short misses;

    float throwForce;

    float phase;

public:
    // Inicializa enemigo
    Enemy();

    ~Enemy();

    // Actualiza comportamiento
    void update(float dt) override;

    // Procesa colisiones
    void onCollision(Entity* e) override;

    void setAgent(Agent* a) { agent = a; }
    Agent* getAgent() const { return agent; }

    unsigned short getHealth() const { return health; }
    void setHealth(unsigned short h) { health = h; }

    bool canShoot() const;

    void resetShootTimer();
};

#endif // ENEMY_H
