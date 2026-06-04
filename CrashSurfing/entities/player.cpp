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

    width = 30;
    height = 60;

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

    /// 2. SISTEMA DE ESCALONES (Fijado estrictamente a un máximo de 15 wumpas)
    int effectiveFruits = collectedFruits;
    if (effectiveFruits > 10) {
        effectiveFruits = 10; // Cap máximo a 15 frutas
    }

    int tier = effectiveFruits / 5; // Tier máximo será 3 (15 / 5)
    isGlutton = (collectedFruits >= 5); // El glotón se activa desde las 5 frutas reales

    // 3. LOGICA DE VELOCIDAD ACUMULATIVA
    // Con 15 frutas (tier 3), la velocidad máxima exacta será: 200 + (3 * 60) = 380.0f
    float targetSpeed = 200.0f + (tier * 60.0f);

    // 4. LOGICA DE GRAVEDAD ACUMULATIVA
    float currentGravity = 980.0f + (tier * 80.0f);
    if (currentGravity > 1500.0f) currentGravity = 1500.0f;

    // Aceleración suave hacia la velocidad objetivo
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

    // === 1. SI ES UN OBSTÁCULO (Sierra, Drone, Tronco) ===
    Obstacle* obs = dynamic_cast<Obstacle*>(other);
    if (obs) {
        QString type = obs->getType().toLower();

        // Los obstáculos SIEMPRE hacen daño. No les importa si eres Glotón.
        if (type == "saw" || type == "floating" || type == "log" || type == "tronco") {
            if (!isInvincible) {
                takeDamage();
            } else {
                qDebug() << "Tocaste un obstáculo, pero estabas Invencible.";
            }

            // Como mencionas que el tronco desaparece al chocar,
            // lo desactivamos aquí manualmente después de aplicar el daño:
            obs->setActive(false);
        }
        return;
    }

    // === 2. SI ES UN OBJETO (Caja, Fruta o Tronco por si se creó como Item) ===
    Item* item = dynamic_cast<Item*>(other);
    if (item && !item->getIsCollected()) {
        QString type = item->getType().toLower();

        // LÓGICA EXCLUSIVA DE LA CAJA
        if (type == "box") {
            if (isGlutton) {
                item->setActive(false);
                qDebug() << "Tienes más de 5 frutas (Glotón activo). Rompes la caja a salvo.";
            } else {
                if (!isInvincible) {
                    takeDamage();
                }
            }
        }
        // LÓGICA EXCLUSIVA DE LA FRUTA
        else if (type == "fruit") {
            item->setActive(false);
            collectItem();

            if (collectedFruits >= fruitsNeededForNextRush) {
                fruitsNeededForNextRush += 5;
                qDebug() << "Mas velocidad." << fruitsNeededForNextRush;
            }
        }
        // ¡CANDADO DE SEGURIDAD! Si por error creaste el tronco como "Item":
        // Evitamos que entre a la lógica de la caja y lo obligamos a hacer daño.
        else if (type == "log" || type == "tronco") {
            if (!isInvincible) {
                takeDamage();
            }
            item->setActive(false); // Desaparece tras hacerte daño
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
