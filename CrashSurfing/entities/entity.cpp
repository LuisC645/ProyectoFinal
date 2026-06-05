#include "entity.h"

Entity::Entity()
{
    position = QVector2D(0.0f, 0.0f);
    velocity = QVector2D(0.0f, 0.0f);
    active = true;
    width = 40;
    height = 40;

}

Entity::~Entity() {}
