#include "whirlpool.h"
#include "player2.h"

#include <QtMath>

namespace
{
constexpr float DEFAULT_CENTER_X = 800.0f;
constexpr float DEFAULT_CENTER_Y = 300.0f;

constexpr float DEFAULT_FORCE_SCALE = 1.15f;
constexpr float DEFAULT_DEATH_RADIUS = 50.0f;

constexpr float MAX_EXTERNAL_SPEED = 560.0f;
constexpr float TANGENTIAL_FACTOR = 0.16f;
}

// Inicializa remolino
Whirlpool::Whirlpool()
{
    center = QVector2D(DEFAULT_CENTER_X, DEFAULT_CENTER_Y);

    force = DEFAULT_FORCE_SCALE;
    deathRadius = DEFAULT_DEATH_RADIUS;
}

// Calcula velocidad aplicada al jugador
QVector2D Whirlpool::computeFrameVelocity(const Player2* player, float dt) const
{
    if (!player || !player->isActive())
    {
        return QVector2D(0.0f, 0.0f);
    }

    QVector2D toCenter = center - player->getPosition();

    const float distance = toCenter.length();

    if (distance < 1.0f)
    {
        return QVector2D(0.0f, 0.0f);
    }

    QVector2D radial = toCenter.normalized();

    QVector2D tangential( -radial.y(), radial.x() );

    const float radialStrength = getForceMagnitude(distance);

    const float tangentialStrength = radialStrength * TANGENTIAL_FACTOR;

    QVector2D frameVelocity = (radial * radialStrength + tangential * tangentialStrength) * dt;

    if (frameVelocity.length() > MAX_EXTERNAL_SPEED)
    {
        frameVelocity.normalize();
        frameVelocity *= MAX_EXTERNAL_SPEED;
    }

    return frameVelocity;
}

// Obtiene intensidad segun distancia
float Whirlpool::getForceMagnitude(float distance) const
{
    if (distance <= 1.0f)
    {
        return 0.0f;
    }

    float radialBase = 0.0f;

    if (distance > 900.0f)
    {
        radialBase = 90.0f;
    }
    else if (distance > 760.0f)
    {
        radialBase = 150.0f;
    }
    else if (distance > 620.0f)
    {
        radialBase = 240.0f;
    }
    else if (distance > 480.0f)
    {
        radialBase = 360.0f;
    }
    else if (distance > 340.0f)
    {
        radialBase = 520.0f;
    }
    else if (distance > 220.0f)
    {
        radialBase = 760.0f;
    }
    else if (distance > 130.0f)
    {
        radialBase = 1050.0f;
    }
    else
    {
        radialBase = 1450.0f;
    }

    float proximityBoost =
        1.0f +
        ((950.0f - distance) / 340.0f);

    if (proximityBoost < 1.0f)
    {
        proximityBoost = 1.0f;
    }

    return radialBase * proximityBoost * force;
}

// Verifica zona de muerte
bool Whirlpool::isInsideDeathZone(Player2* player) const
{
    if (!player)
    {
        return false;
    }

    const float distance = (center - player->getPosition()).length();

    return distance <= deathRadius;
}
