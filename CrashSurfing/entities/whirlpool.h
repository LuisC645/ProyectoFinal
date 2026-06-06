#ifndef WHIRLPOOL_H
#define WHIRLPOOL_H

#include <QVector2D>

class Player2;

class Whirlpool
{
public:
    // Inicializa remolino
    Whirlpool();

    void setCenter(const QVector2D& c)
    {
        center = c;
    }

    QVector2D getCenter() const
    {
        return center;
    }

    void setForce(float f)
    {
        force = f;
    }

    float getForce() const
    {
        return force;
    }

    void setDeathRadius(float r)
    {
        deathRadius = r;
    }

    float getDeathRadius() const
    {
        return deathRadius;
    }

    // Calcula velocidad aplicada al jugador
    QVector2D computeFrameVelocity(const Player2* player, float dt) const;

    // Obtiene intensidad segun distancia
    float getForceMagnitude(float distance) const;

    // Verifica zona de muerte
    bool isInsideDeathZone(Player2* player) const;

private:
    QVector2D center;

    float force;
    float deathRadius;
};

#endif
