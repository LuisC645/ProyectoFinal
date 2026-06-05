#ifndef ITEM_H
#define ITEM_H

#include "entity.h"
#include <QString>
#include <QVector2D>

class Item : public Entity {
private:
    QString type;
    float width;
    float height;
    bool isCollected;

public:
    Item(QString type, QVector2D position, float width, float height);
    virtual ~Item() override = default;

    virtual void update(float dt) override;
    virtual void onCollision(Entity* other) override;

    // === GETTERS Y SETTERS ===
    QString getType() const { return type; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    bool getIsCollected() const { return isCollected; }
    void setIsCollected(bool collected) { isCollected = collected; }
};

#endif // ITEM_H
