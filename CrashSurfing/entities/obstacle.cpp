#include "obstacle.h"
#include <QDebug>
#include <cmath>

// Inicializa obstaculo
Obstacle::Obstacle(QString type, QVector2D position)
{
    this->type = type.toLower();
    this->position = position;
    this->active = true;
    this->initialY = position.y();

    if (this->type == "log")
    {
        width = 35.0f;
        height = 35.0f;
    }
    else if (this->type == "saw")
    {
        width = 40.0f;
        height = 40.0f;
    }
    else if (this->type == "floating")
    {
        width = 40.0f;
        height = 40.0f;
    }
    else if (this->type == "tronco")
    {
        width = 40.0f;
        height = 40.0f;
    }
    else
    {
        width = 40.0f;
        height = 40.0f;
    }
}

// Actualiza comportamiento
void Obstacle::update(float dt)
{
    if (!isActive())
    {
        return;
    }

    if (type == "floating")
    {
        static float tiempoDron = 0.0f;
        tiempoDron += dt * 0.05f;
        float nuevoY = 270.0f + std::sin(tiempoDron + (position.x() * 0.005f)) * 90.0f;

        position.setY(nuevoY);
    }
    else if (type == "saw" || type == "enemy_saw")
    {
        constexpr float GRAVEDAD = 980.0f;
        constexpr float SUELO_Y = 350.0f;
        constexpr float REBOTE = -500.0f;

        position.setX(position.x() + velocity.x() * dt);
        velocity.setY(velocity.y() + GRAVEDAD * dt);
        position.setY(position.y() + velocity.y() * dt);

        if (position.y() >= SUELO_Y)
        {
            position.setY(SUELO_Y);
            velocity.setY(REBOTE);
        }

        if (position.x() < -200.0f)
        {
            active = false;
        }
    }
    else if (type == "log")
    {
        // Estatico
    }
}

// Procesa colisiones
void Obstacle::onCollision(Entity* other)
{
    Q_UNUSED(other);
}
