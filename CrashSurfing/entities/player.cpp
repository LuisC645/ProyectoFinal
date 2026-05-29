#include "player.h"
#include <QDebug>

Player::Player() : Entity()
{
    lives = 5;
    invulnerable = false;

    startY = 400.0f;
    position = QVector2D(0.0f, startY);
    velocity = QVector2D(300.0f, 0.0f);

    v0y = -850.0f;
    baseGravity = 2500.0f;
    gravity = baseGravity;
    timeJumping = 0.0f;
    isJumping = false;

    isGlutton = false;
    isScared = false;
    itemsCollected = 0;
}

Player::~Player() {}

void Player::jump()
{
    if (!isJumping) {
        isJumping = true;
        timeJumping = 0.0f; // Reiniciamos t del tiro parabólico
    }
}

void Player::collectItem()
{
    itemsCollected++;
    // Si recoge 5 items, se vuelve pesado
    if (itemsCollected >= 5 && !isGlutton) {
        isGlutton = true;
        gravity = baseGravity * 1.5f;
    }
}

void Player::checkScaredStatus(float enemyX, float enemyY)
{
    // Enemigo está a menos de 200px, asustado
    float dist = position.distanceToPoint(QVector2D(enemyX, enemyY));
    isScared = (dist < 200.0f);
}

void Player::applyPhysics(float dt)
{
    // 1. Movimiento rectilíneo horizontal
    float speedMultiplier = isScared ? 1.5f : 1.0f;
    float newX = position.x() + (velocity.x() * speedMultiplier * dt);

    if (newX < 0.0f) {
        newX = 0.0f;
    }
    else if (newX > 4960.0f) { // 5000 - 40px (ancho del jugador)
        newX = 4960.0f;
    }

    position.setX(newX);
    // Fisica salto
    if (isJumping) {
        timeJumping += dt;

        // y = y0 + v0*t + 0.5*g*t^2
        float currentY = startY + (v0y * timeJumping) + (0.5f * gravity * timeJumping * timeJumping);
        position.setY(currentY);

        // Colision con el piso
        if (position.y() >= startY) {
            position.setY(startY);
            isJumping = false;
        }
    }
}

void Player::update(float dt)
{
    if (!active) return;

    applyPhysics(dt);

    // position += velocity * dt;

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

