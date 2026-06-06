#include "player.h"
#include "obstacle.h"
#include "item.h"
#include <QDebug>

// Inicializa atributos del jugador
Player::Player()
{
    speed = 200.0f;
    jumpForce = -450.0f;
    gravity = 980.0f;
    floorY = 350.0f;

    invincibilityDuration = 2.0f;

    width = 30.0f;
    height = 60.0f;

    reset();
}

// Actualiza movimiento y estados
void Player::update(float dt)
{
    if (!isActive()) return;

    if (isInvincible)
    {
        invincibilityTimer -= dt;

        if (invincibilityTimer <= 0.0f)
        {
            isInvincible = false;
            invincibilityTimer = 0.0f;
        }
    }

    unsigned short effectiveFruits = collectedFruits;

    if (effectiveFruits > 10)
    {
        effectiveFruits = 10;
    }

    unsigned short tier = effectiveFruits / 5;

    isGlutton = (collectedFruits >= 5);

    float targetSpeed = 200.0f + (tier * 60.0f);
    float currentGravity = 980.0f + (tier * 80.0f);

    if (currentGravity > 1500.0f)
    {
        currentGravity = 1500.0f;
    }

    if (speed < targetSpeed)
    {
        speed += 250.0f * dt;

        if (speed > targetSpeed)
        {
            speed = targetSpeed;
        }
    }

    velocity.setX(speed);

    if (!isGrounded)
    {
        velocity.setY(velocity.y() + currentGravity * dt);
    }
    else
    {
        velocity.setY(0.0f);
    }

    position += velocity * dt;

    if (position.y() >= floorY)
    {
        position.setY(floorY);
        isGrounded = true;
        velocity.setY(0.0f);
    }
}

// Ejecuta salto
void Player::jump()
{
    if (isGrounded)
    {
        velocity.setY(jumpForce);
        isGrounded = false;
    }
}

// Aplica daño al jugador
void Player::takeDamage()
{
    if (isInvincible)
    {
        return;
    }

    lives--;

    speed = 30.0f;
    velocity.setX(speed);

    startInvincibility();

    if (lives <= 0)
    {
        lives = 0;
        setActive(false);
    }
}

// Activa invencibilidad
void Player::startInvincibility()
{
    isInvincible = true;
    invincibilityTimer = invincibilityDuration;
}

// Incrementa frutas recolectadas
void Player::collectItem()
{
    collectedFruits++;
}

// Procesa colisiones
void Player::onCollision(Entity* other)
{
    if (!other->isActive())
    {
        return;
    }

    Obstacle* obs = dynamic_cast<Obstacle*>(other);

    if (obs)
    {
        QString type = obs->getType().toLower();

        if (type == "saw" || type == "floating" || type == "log")
        {
            if (!isInvincible)
            {
                takeDamage();
            }
            else
            {
                qDebug() << "Colision pero invencible";
            }

            obs->setActive(false);
        }

        return;
    }

    Item* item = dynamic_cast<Item*>(other);

    if (item && !item->getIsCollected())
    {
        QString type = item->getType().toLower();

        if (type == "fruit")
        {
            item->setActive(false);

            collectItem();

            if (collectedFruits >= fruitsNeededForNextRush)
            {
                fruitsNeededForNextRush += 5;
            }
        }

        item->setActive(false);
    }
}

// Restaura atributos iniciales
void Player::reset()
{
    lives = 5;
    collectedFruits = 0;

    isInvincible = false;
    invincibilityTimer = 0.0f;

    isGrounded = true;
    active = true;

    isGlutton = false;

    speed = 200.0f;

    position = QVector2D(100.0f, floorY);
    velocity = QVector2D(speed, 0.0f);

    fruitsNeededForNextRush = 5;

    qDebug() << "Reset stats";
}
