#include "obstacle.h"
#include <QDebug>
#include <cmath>

Obstacle::Obstacle(QString type, QVector2D position)
{
    this->type = type;
    this->position = position;
    this->active = true;
    this->velocity = QVector2D(0.0f, 0.0f);
    this->initialY = position.y();
}

void Obstacle::update(float dt)
{
    if (!isActive()) return;

    if (type == "floating") {
        static float tiempoDron = 0.0f;
        tiempoDron += dt * 0.05f;

        float nuevoY = 270.0f + std::sin(tiempoDron + (position.x() * 0.005f)) * 90.0f;
        position.setY(nuevoY);
    }

    else if (type == "saw") {
        const float GRAVEDAD = 980.0f;
        const float FUERZA_REBOTE = -600.0f;
        const float VELOCIDAD_IZQUIERDA = -600.0f;
        const float SUELO_Y = 350.0f;

        position.setX(position.x() + VELOCIDAD_IZQUIERDA * dt);

        velocity.setY(velocity.y() + GRAVEDAD * dt);
        position.setY(position.y() + velocity.y() * dt);

        if (position.y() >= SUELO_Y) {
            position.setY(SUELO_Y);
            velocity.setY(FUERZA_REBOTE);
        }
    }

    // 3. TRONCOS: Estáticos
    else if (type == "log") {
        // Se quedan fijos en su Y elevado
    }
}

void Obstacle::onCollision(Entity* other)
{
    Q_UNUSED(other);
}
