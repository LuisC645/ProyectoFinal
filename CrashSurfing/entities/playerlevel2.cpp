#include "playerlevel2.h"
#include "entity.h"

PlayerLevel2::PlayerLevel2()
{
    position = QVector2D(1200.0f, 450.0f);

    velocity = QVector2D(0.0f, 0.0f);

    width = 50;
    height = 50;

    speed = 250.0f;

    lives = 3;

    collectedFruits = 0;

    arenaWidth = 1600.0f;
    arenaHeight = 600.0f;

    active = true;
}

PlayerLevel2::~PlayerLevel2()
{
}

void PlayerLevel2::update(float dt)
{
    position += velocity * dt;

    // Límites de la arena

    if(position.x() < 0)
        position.setX(0);

    if(position.y() < 0)
        position.setY(0);

    if(position.x() > arenaWidth)
        position.setX(arenaWidth);

    if(position.y() > arenaHeight)
        position.setY(arenaHeight);
}

void PlayerLevel2::onCollision(Entity* e)
{
    if(!e)
        return;

    // Se implementará más adelante para:
    // - Obstáculos
    // - Frutas
    // - Proyectiles
    // - Efectos de ralentización
}

void PlayerLevel2::moveUp()
{
    velocity.setY(-speed);
}

void PlayerLevel2::moveDown()
{
    velocity.setY(speed);
}

void PlayerLevel2::moveLeft()
{
    velocity.setX(-speed);
}

void PlayerLevel2::moveRight()
{
    velocity.setX(speed);
}

void PlayerLevel2::stopHorizontal()
{
    velocity.setX(0);
}

void PlayerLevel2::stopVertical()
{
    velocity.setY(0);
}

void PlayerLevel2::loseLife()
{
    lives--;

    if(lives < 0)
        lives = 0;
}

int PlayerLevel2::getLives() const
{
    return lives;
}

void PlayerLevel2::collectFruit()
{
    collectedFruits++;
}

int PlayerLevel2::getCollectedFruits() const
{
    return collectedFruits;
}

void PlayerLevel2::reset()
{
    position = QVector2D(1200.0f, 450.0f);

    velocity = QVector2D(0.0f, 0.0f);

    lives = 3;

    collectedFruits = 0;

    active = true;
}
