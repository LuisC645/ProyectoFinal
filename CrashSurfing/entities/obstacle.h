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
    // Inicializa obstaculo
    Obstacle(QString type, QVector2D position);

    virtual ~Obstacle() override = default;

    // Actualiza comportamiento
    virtual void update(float dt) override;

    // Procesa colisiones
    virtual void onCollision(Entity* other) override;

    QString getType() const { return type; }
};

#endif // OBSTACLE_H
