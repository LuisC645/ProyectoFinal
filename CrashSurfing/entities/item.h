#ifndef ITEM_H
#define ITEM_H

#include "entity.h"
#include <QString>

class Item : public Entity
{
private:

    QString type;     // Puede ser "fruit" o "box"
    bool isCollected; // Para saber si Crash ya la agarró y no volver a dibujarla
    float width;
    float height;

public:

    Item(float x, float y, QString itemType);

    void update(float dt) override;
    void onCollision(Entity* e) override;

    // Getters y Setters necesarios
    QString getType() const { return type; }
    bool getIsCollected() const { return isCollected; }
    void setCollected(bool status) { isCollected = status; }

    float getWidth() const { return width; }
    float getHeight() const { return height; }

};

#endif // ITEM_H
