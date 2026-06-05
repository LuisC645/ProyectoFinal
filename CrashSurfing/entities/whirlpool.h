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

    // Nueva arquitectura:
    // calcula la contribución de velocidad del remolino en este frame
    QVector2D computeFrameVelocity(const PlayerLevel2* player, float dt) const;

    // Magnitud radial útil para GameLevel2 / IA
    float getForceMagnitude(float distance) const;

    bool isInsideDeathZone(PlayerLevel2* player) const;

private:
    QVector2D center;
    float force;
    float deathRadius;
};

#endif
