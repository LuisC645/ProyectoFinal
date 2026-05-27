#include "game.h"
#include "../entities/player.h"
#include "../entities/entity.h"
#include "../entities/obstacle.h"\

#include <QDebug>

Game::Game()
{
    player = new Player();
    score = 0;
}

Game::~Game()
{
    delete player;
    for (Entity* e : entities) { delete e; }
    entities.clear();
}

void Game::update(float dt)
{
    // Mover player
    if (player && player->isActive()) {
        player->update(dt);
    }

    // Mover entities
    for (Entity* e : entities) {
        if (e->isActive()) {
            e->update(dt);
        }
    }

    checkCollisions();
}

void Game::addEntity(Entity* e)
{
    if (e != nullptr) {
        entities.push_back(e);
    }
}

void Game::loadLevel()
{
    // test
    Obstacle* sierra = new Obstacle();
    sierra->setPosition(QVector2D(10.0f, 0.0f));
    addEntity(sierra);
}

void Game::checkCollisions()
{
    if (!player || !player->isActive()) return;

    for (Entity* e : entities) {
        if (!e->isActive()) continue;

        float distancia = player->getPosition().distanceToPoint(e->getPosition());

        if (distancia < 1.5f) {
            qDebug() << "colision en X:" << e->getPosition().x();

            // quitar vida
            player->setLives(player->getLives() - 1);

            // quitar sierra
            e->setActive(false);
        }
    }
}
