#include "enemy.h"

Enemy::Enemy() : Entity()
{
    agent = nullptr;

    health = 1;

    active = true;

    position = QVector2D(1100.0f,200.0f);

    velocity = QVector2D(0.0f,0.0f);

    width = 60.0f;
    height = 100.0f;

    shootTimer = 0.0f;
    shootCooldown = 10.0f;

}

Enemy::~Enemy()
{
}

void Enemy::onCollision(Entity* e)
{
    Q_UNUSED(e);
}

void Enemy::update(float dt)
{
    if(!active)
        return;

    shootTimer += dt;

    static float tiempoEnemy = 0.0f;

    tiempoEnemy += dt * 0.5f;

    position.setY(120.0f + std::sin(tiempoEnemy) * 60.0f);
}

bool Enemy::canShoot() const
{
    return shootTimer >= shootCooldown;
}

void Enemy::resetShootTimer()
{
    shootTimer = 0.0f;
}
