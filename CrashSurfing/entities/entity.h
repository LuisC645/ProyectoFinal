#ifndef ENTITY_H
#define ENTITY_H

#include <QVector2D>

class Entity
{
protected:
    QVector2D position;
    QVector2D velocity;
    bool active;

    float width;
    float height;

public:
    Entity();
    virtual ~Entity();

    virtual void update(float dt) = 0;
    virtual void onCollision(Entity* e) = 0;

    QVector2D getPosition() const { return position; }
    void setPosition(const QVector2D &pos){ position = pos; }

    QVector2D getVelocity() const { return velocity; }
    void setVelocity(const QVector2D &vel) { velocity = vel; }

    bool isActive() const { return active; }
    void setActive(bool act) { active = act; }

    float getWidth() const { return width; }
    float getHeight() const { return height; }

    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }
};

#endif // ENTITY_H
