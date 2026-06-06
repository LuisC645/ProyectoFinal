#ifndef ITEM_H
#define ITEM_H

#include "entity.h"
#include <QString>
#include <QVector2D>

class Item : public Entity {
private:
    QString type;
    bool isCollected;

public:
    // Inicializa item
    Item(QString type, QVector2D position, float width, float height);

    virtual ~Item() override = default;

    // Actualiza comportamiento
    virtual void update(float dt) override;

    // Procesa colisiones
    virtual void onCollision(Entity* other) override;

    QString getType() const { return type; }

    bool getIsCollected() const { return isCollected; }
    void setIsCollected(bool collected) { isCollected = collected; }
};

#endif // ITEM_H
