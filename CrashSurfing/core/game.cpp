#include "game.h"
#include "../entities/player.h"
#include "../entities/entity.h"

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
}

void Game::addEntity(Entity* e)
{
    if (e != nullptr) {
        entities.push_back(e);
    }
}
