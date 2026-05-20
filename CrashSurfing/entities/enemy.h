#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

// Falta por definir (pero si no no compila xd)
class Agent;

class Enemy : public Entity
{
private:

    // Puntero al agente
    Agent* agent;
    // Optimización de memoria
    unsigned short health;

public:
    Enemy();
    ~Enemy();

    void update(float dt) override;
    void onCollision(Entity* e) override;

    void setAgent(Agent* a) { agent = a; }
    Agent* getAgent() const { return agent; }

    unsigned short getHealth() const { return health; }
    void setHealth(unsigned short h) { health = h; }
};

#endif // ENEMY_H
