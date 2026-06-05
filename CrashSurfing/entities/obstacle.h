#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entity.h"
#include <QString>
#include <QVector2D>

class Obstacle : public Entity {
private:
    QString type;
    float initialY;

public:
    Obstacle(QString type, QVector2D position);
    virtual ~Obstacle() override = default;

    virtual void update(float dt) override;
    virtual void onCollision(Entity* other) override;

    QString getType() const { return type; }
};

#endif // OBSTACLE_H
