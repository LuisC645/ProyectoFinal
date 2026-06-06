#include "item.h"

// Inicializa item
Item::Item(QString type, QVector2D position, float width, float height)
{
    this->type = type;
    isCollected = false;

    position = position;
    active = true;

    this->position = position;
    this->active = true;
}

// Actualiza comportamiento
void Item::update(float dt)
{
    Q_UNUSED(dt);
}

// Procesa colisiones
void Item::onCollision(Entity* other)
{
    Q_UNUSED(other);

    if (!isCollected)
    {
        isCollected = true;
        active = false;
    }
}
