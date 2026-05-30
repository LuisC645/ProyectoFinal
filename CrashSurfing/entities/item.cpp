#include "item.h"

Item::Item(float x, float y, QString itemType)
{
    position = QVector2D(x, y);
    velocity = QVector2D(0.0f, 0.0f); // Las frutas se quedan flotando estáticas en el río
    type = itemType;
    isCollected = false;

    // Dimensiones de la caja de colisión (Hitbox)
    width = 30.0f;
    height = 30.0f;
}

void Item::update(float dt)
{
    // Las frutas no se mueven solas, así que no necesitan física por ahora
}

void Item::onCollision(Entity* e)
{
    // Cuando colisiona con algo, se marca como recolectada
    isCollected = true;
}
