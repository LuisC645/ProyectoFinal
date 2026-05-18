#include "player.h"
#include <QDebug>

Player::Player() : Entity()
{
    lives = 5;
    invulnerable = false;
}

Player::~Player() {}

void Player::update(float dt)
{
    if (!active) return;

    applyPhysics();

    position += velocity * dt;

    qDebug() << "Crash Position -> X:" << position.x() << " Y:" << position.y();
}

void Player::onCollision(Entity* e)
{
    // falta
}

void Player::input()
{
    // leer teclas
}

void Player::applyPhysics()
{
    // fisicas a cargo del fisico (marcos castillo)
}
