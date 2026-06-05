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

void Whirlpool::applyForce(PlayerLevel2* player)
{
    if(!player)
        return;

    QVector2D dir =
        center -
        player->getPosition();

    float distance = dir.length();

    if(distance < 1.0f)
        return;

    dir.normalize();

    player->setPosition(
        player->getPosition() +
        dir * force
        );
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
