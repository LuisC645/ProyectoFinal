#include "whirlpool.h"
#include "playerlevel2.h"

#include <QtMath>

namespace
{
constexpr float DEFAULT_CENTER_X = 800.0f;
constexpr float DEFAULT_CENTER_Y = 300.0f;

constexpr float DEFAULT_FORCE_SCALE = 1.35f;
constexpr float DEFAULT_DEATH_RADIUS = 50.0f;

// Más alto para que el empuje externo no quede capado
constexpr float MAX_EXTERNAL_SPEED = 700.0f;

// Poco giro, mucha succión
constexpr float TANGENTIAL_FACTOR = 0.14f;
}

Whirlpool::Whirlpool()
{
    center = QVector2D(DEFAULT_CENTER_X, DEFAULT_CENTER_Y);
    force = DEFAULT_FORCE_SCALE;
    deathRadius = DEFAULT_DEATH_RADIUS;
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

QVector2D Whirlpool::computeFrameVelocity(const PlayerLevel2* player, float dt) const
{
    if (!player || !player->isActive())
        return QVector2D(0.0f, 0.0f);

    QVector2D toCenter = center - player->getPosition();
    const float distance = toCenter.length();

    if (distance < 1.0f)
        return QVector2D(0.0f, 0.0f);

    QVector2D radial = toCenter.normalized();
    QVector2D tangential(-radial.y(), radial.x());

    const float radialStrength = getForceMagnitude(distance);
    const float tangentialStrength = radialStrength * TANGENTIAL_FACTOR;

    QVector2D frameVelocity =
        (radial * radialStrength + tangential * tangentialStrength) * dt;

    if (frameVelocity.length() > MAX_EXTERNAL_SPEED)
    {
        frameVelocity.normalize();
        frameVelocity *= MAX_EXTERNAL_SPEED;
    }

    return frameVelocity;
}

float Whirlpool::getForceMagnitude(float distance) const
{
    if (distance <= 1.0f)
        return 0.0f;

    float radialBase = 0.0f;

    // Expansión grande del radio útil:
    // prácticamente toda la arena ya siente presión
    if (distance > 950.0f)
    {
        radialBase = 140.0f;
    }
    else if (distance > 800.0f)
    {
        radialBase = 230.0f;
    }
    else if (distance > 650.0f)
    {
        radialBase = 360.0f;
    }
    else if (distance > 500.0f)
    {
        radialBase = 520.0f;
    }
    else if (distance > 380.0f)
    {
        radialBase = 760.0f;
    }
    else if (distance > 260.0f)
    {
        radialBase = 1080.0f;
    }
    else if (distance > 160.0f)
    {
        radialBase = 1500.0f;
    }
    else if (distance > 90.0f)
    {
        radialBase = 2100.0f;
    }
    else
    {
        radialBase = 2900.0f;
    }

    // Refuerzo continuo desde muy lejos
    float proximityBoost = 1.0f + ((1100.0f - distance) / 260.0f);

    if (proximityBoost < 1.0f)
        proximityBoost = 1.0f;

    return radialBase * proximityBoost * force;
}

bool Whirlpool::isInsideDeathZone(PlayerLevel2* player) const
{
    if (!player)
        return false;

    const float distance = (center - player->getPosition()).length();
    return distance <= deathRadius;
}
