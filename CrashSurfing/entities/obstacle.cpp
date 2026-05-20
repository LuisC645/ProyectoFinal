#include "obstacle.h"

Obstacle::Obstacle() : Entity()
{
    damage = 1; // Obstaculo quina 1 vida
}

Obstacle::~Obstacle() {}

void Obstacle::update(float dt)
{
    if (!active) return;

    // Faltan fisicas aqui uwu

    position += velocity * dt;
}

void Obstacle::onCollision(Entity* e)
{
    // Faltan colisiones (Luis Castillo no hace nada)
}
