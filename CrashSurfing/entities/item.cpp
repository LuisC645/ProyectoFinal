#include "item.h"

Item::Item(QString type, QVector2D position, float width, float height)
{
    this->type = type;
    this->width = width;
    this->height = height;
    this->isCollected = false;

    // Variables heredadas de Entity
    this->position = position;
    this->active = true;
}

void Item::update(float dt)
{
    Q_UNUSED(dt);

    // ESTATICOS

}

void Item::onCollision(Entity* other)
{
    Q_UNUSED(other);

    // Cuando Crash choca con el item, lo marcamos como recogido y lo desactivamos
    if (!isCollected) {
        isCollected = true;
        this->active = false;
    }
}
