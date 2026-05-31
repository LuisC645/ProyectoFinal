#include "player.h"
#include "obstacle.h"
#include "item.h"
#include <QDebug>

Player::Player()
{
    // Parámetros de diseño de físicas
    speed = 200.0f;        // Velocidad constante hacia la derecha
    jumpForce = -450.0f;   // Fuerza del impulso hacia arriba
    gravity = 980.0f;      // Fuerza de gravedad
    floorY = 350.0f;       // Coordenada Y exacta del suelo

    invincibilityDuration = 2.0f;

    reset(); // Inicializa todos los valores limpios
}

void Player::update(float dt)
{
    if (!isActive()) return;

    // Temporizador de invencibilidad (Con blindaje extra)
    if (isInvincible) {
        invincibilityTimer -= dt;
        if (invincibilityTimer <= 0.0f) {
            isInvincible = false;
            invincibilityTimer = 0.0f; // Evita que se quede en valores negativos
            qDebug() << "Listo para recibir daño.";
        }
    }

    // 2. SISTEMA DE ESCALONES
    int tier = collectedFruits / 5;
    isGlutton = (tier > 0);

    // 3. LOGICA DE VELOCIDAD ACUMULATIVA
    float targetSpeed = 200.0f + (tier * 60.0f);
    if (targetSpeed > 600.0f) targetSpeed = 600.0f;

    // 4. LOGICA DE GRAVEDAD ACUMULATIVA (Caida mas pesada)
    float currentGravity = 980.0f + (tier * 80.0f);
    if (currentGravity > 1500.0f) currentGravity = 1500.0f;

    // Aceleracion y recuperacion suave
    if (speed < targetSpeed) {
        speed += 250.0f * dt;
        if (speed > targetSpeed) speed = targetSpeed;
    }

    velocity.setX(speed);

    // 5. APLICAR GRAVEDAD
    if (!isGrounded) {
        velocity.setY(velocity.y() + currentGravity * dt);
    } else {
        velocity.setY(0.0f);
    }

    // 6. ACTUALIZAR POSICION
    position += velocity * dt;

    // 7. DETECCIÓN DE SUELO
    if (position.y() >= floorY) {
        position.setY(floorY);
        isGrounded = true;
        velocity.setY(0.0f);
    }
}

void Player::jump()
{
    if (isGrounded) {
        velocity.setY(jumpForce);
        isGrounded = false;
        qDebug() << "¡Crash Saltó!";
    }
}

void Player::takeDamage()
{
    if (isInvincible) return; // Salvaguarda absoluta

    lives--;

    // Al chocar frena a 30 de velocidad
    this->speed = 30.0f;
    this->velocity.setX(this->speed);

    // Activa la invencibilidad al recibir daño
    startInvincibility();

    qDebug() << "¡Crash recibió daño! Vidas restantes:" << lives;

    if (lives <= 0) {
        lives = 0;
        setActive(false);
        qDebug() << "¡GAME OVER!";
    }
}

void Player::startInvincibility()
{
    isInvincible = true;
    invincibilityTimer = invincibilityDuration;
}

void Player::collectItem()
{
    collectedFruits++;
}

void Player::onCollision(Entity* other)
{
    if (!other->isActive()) return;

    // === COLISIÓN CON OBSTÁCULOS (Sierras / Drones) ===
    Obstacle* obs = dynamic_cast<Obstacle*>(other);
    if (obs) {
        QString type = obs->getType();
        if (type == "saw" || type == "floating") {
            if (!isInvincible) {
                takeDamage();
            } else {
                // Si chocas pero no pierdes vida, te dirá esto en rojo:
                qDebug() << "Tocaste un obstáculo, pero estabas Invencible.";
            }
        }
        return;
    }

    // === COLISIÓN CON OBJETOS (Frutas / Cajas) ===
    Item* item = dynamic_cast<Item*>(other);
    if (item && !item->getIsCollected()) {
        QString type = item->getType();

        if (type == "box") {
            if (isGlutton) {
                item->setActive(false);
                qDebug() << "Tienes más de 5 frutas (Glotón activo).";
            } else {
                if (!isInvincible) {
                    takeDamage();
                }
            }
        }
        else if (type == "fruit") {
            item->setActive(false);
            collectItem();

            if (collectedFruits >= fruitsNeededForNextRush) {
                fruitsNeededForNextRush += 5;
                qDebug() << "Mas velocidad." << fruitsNeededForNextRush;
            }
        }
    }
}

void Player::reset()
{
    lives = 5;
    collectedFruits = 0;
    isInvincible = false;
    invincibilityTimer = 0.0f;
    isGrounded = true;
    active = true;

    isGlutton = false;

    speed = 200.0f; // Restaurar velocidad original
    position = QVector2D(100.0f, floorY);
    velocity = QVector2D(speed, 0.0f);

    fruitsNeededForNextRush = 5;

    qDebug() << "Estadisticas de Crash restauradas.";
}
