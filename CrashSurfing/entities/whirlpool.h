#ifndef WHIRLPOOL_H
#define WHIRLPOOL_H

#include <QVector2D>

class PlayerLevel2;

class Whirlpool
{
public:

    Whirlpool();

    void setCenter(const QVector2D& c);
    QVector2D getCenter() const;

    void setForce(float f);
    float getForce() const;

    void setDeathRadius(float r);
    float getDeathRadius() const;

    void applyForce(PlayerLevel2* player);

    bool isInsideDeathZone(PlayerLevel2* player);

private:

    QVector2D center;

    float force;

    float deathRadius;
};

#endif
