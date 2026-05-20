#include "enemy.h"

Enemy::Enemy() : Entity()
{
    // Vacio pq no hay
    agent = nullptr;
    // Salud por defecto
    health = 1;
}

Enemy::~Enemy()
{
}

void Enemy::update(float dt)
{
    if (!active) return;

    // Test
    position += velocity * dt;
}

void Enemy::onCollision(Entity* e)
{
    // Falta
}
