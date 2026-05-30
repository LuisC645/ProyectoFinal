#include "player.h"
#include <QDebug>

Player::Player()
{
    position = QVector2D(100.0f, 400.0f); // Posición inicial en el río
    velocity = QVector2D(300.0f, 0.0f);   // Velocidad inicial estándar

    isJumping = false;
    isGlutton = false;
    fruitCount = 0;
    lives = 3;

    isStunned = false;
    stunTimer = 0.0f;
}

void Player::jump()
{
    // Permite saltar si esta en elsuelo
    if (!isJumping) {
        velocity.setY(-550.0f); // Impulso vertical del salto
        isJumping = true;
        qDebug() << "¡Crash saltó!";
    }
}

void Player::takeDamage()
{
    lives--;
    qDebug() << "Colision --- Vidas:" << lives;

    // Activamos el estado de aturdimiento y su temporizador
    isStunned = true;
    stunTimer = 1.5f; // Duración del freno en s

    if (lives <= 0) {
        qDebug() << "¡GAME OVER!";
    }
}

void Player::update(float dt)
{
    // Stuned
    if (isStunned) {
        stunTimer -= dt;
        if (stunTimer <= 0.0f) {
            isStunned = false;
            qDebug() << "Freno, ya acabo";
        }
    }

    float targetSpeedX = 300.0f; // Velocidad base del juego

    if (isGlutton) {
        targetSpeedX *= 0.7f;    // Si pesado va un 30% más lento (210)
    }

    if (isStunned) {
        targetSpeedX = 80.0f;    // Si esta aturdido, va 80% mas lento (80)
    }

    velocity.setX(targetSpeedX); // Se aplica limpiamente en este frame

    // Fisicas
    float gravity = isGlutton ? 2000.0f : 1500.0f;

    if (position.y() < 400.0f || velocity.y() < 0.0f) {
        // Esta en el aire
        velocity.setY(velocity.y() + gravity * dt);
        isJumping = true;
    } else {
        // Esta tocando el suelo (Río)
        velocity.setY(0.0f);
        position.setY(400.0f);
        isJumping = false; // Quitar estado de salto
    }

    position += velocity * dt;
}

void Player::collectItem()
{
    if (isGlutton) return;

    fruitCount++;
    if (fruitCount >= 5) {
        isGlutton = true;
        qDebug() << "¡Crash ahora es GLUTTON!";
    }
}

void Player::onCollision(Entity* e) {}
