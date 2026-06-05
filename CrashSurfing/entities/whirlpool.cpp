#include "whirlpool.h"
#include "../entities/playerlevel2.h"

Whirlpool::Whirlpool()
{
    center = QVector2D(800.0f, 300.0f);

    force = 0.3f;

    deathRadius = 50.0f;
}

void Whirlpool::setCenter(const QVector2D& c)
{
    center = c;
}

QVector2D Whirlpool::getCenter() const
{
    return center;
}

void Whirlpool::setForce(float f)
{
    force = f;
}

float Whirlpool::getForce() const
{
    return force;
}

void Whirlpool::setDeathRadius(float r)
{
    deathRadius = r;
}

float Whirlpool::getDeathRadius() const
{
    return deathRadius;
}

void Whirlpool::applyForce(PlayerLevel2* player, float dt)
{
    if(!player)
        return;

    QVector2D r =
        center -
        player->getPosition();

    float distance = r.length();

    if(distance < 1.0f)
        return;

    QVector2D radial =
        r.normalized();

    QVector2D tangential(
        -radial.y(),
        radial.x()
        );

    // ==========================
    // ZONAS DE INFLUENCIA
    // ==========================

    float Kr;

    if(distance > 500.0f)
    {
        // Zona 1
        Kr = 1500.0f;
    }
    else if(distance > 350.0f)
    {
        // Zona 2
        Kr = 4000.0f;
    }
    else if(distance > 200.0f)
    {
        // Zona 3
        Kr = 8000.0f;
    }
    else
    {
        // Zona 4
        Kr = 15000.0f;
    }

    float Kt = 1200.0f;

    // ==========================
    // FUERZAS DEL REMOLINO
    // ==========================

    float radialStrength =
        Kr / distance;

    float tangentialStrength =
        Kt / distance;

    QVector2D acceleration =
        radial * radialStrength +
        tangential * tangentialStrength;

    // ==========================
    // ECUACION DE MOVIMIENTO
    // v = v + a·dt
    // ==========================

    QVector2D newVelocity =
        player->getVelocity()
        +
        acceleration * dt;

    // Limite para evitar explosiones numéricas

    float maxSpeed = 450.0f;

    if(newVelocity.length() > maxSpeed)
    {
        newVelocity.normalize();
        newVelocity *= maxSpeed;
    }

    player->setVelocity(newVelocity);
}

bool Whirlpool::isInsideDeathZone(PlayerLevel2* player)
{
    if(!player)
        return false;

    float distance =
        (center -
         player->getPosition()).length();

    return distance <= deathRadius;
}
