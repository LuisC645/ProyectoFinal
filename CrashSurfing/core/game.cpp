#include "game.h"
#include "../entities/player.h"
#include "../entities/entity.h"
#include "../entities/obstacle.h"
#include "../entities/item.h"

#include <QDebug>
#include <cstdlib>
#include <ctime>

Game::Game()
{
    player = new Player();
    score = 0;
    isGameOver = false;
    isLevelComplete = false;
}

Game::~Game()
{
    delete player;
    for (Entity* e : entities) { delete e; }
    entities.clear();
    for (Item* i : items) { delete i; }
    items.clear();
}

void Game::update(float dt)
{
    Player* p = dynamic_cast<Player*>(player);

    if (p && p->getLives() <= 0) {
        isGameOver = true;
        return;
    }

    if (p && p->getPosition().x() >= 25000.0f) {
        isLevelComplete = true;
        return;
    }

    if (player && !isGameOver && !isLevelComplete) {
        player->update(dt);

        for (Entity* e : entities) {
            if (e->isActive()) e->update(dt);
        }
        for (Item* i : items) {
            if (!i->getIsCollected()) i->update(dt);
        }

        float px = player->getPosition().x();
        float py = player->getPosition().y();

        // Ajustamos dinámicamente el tamaño de la caja de colisión si está Glotón
        float pw = (p && p->getIsGlutton()) ? 55.0f : 40.0f;
        float ph = (p && p->getIsGlutton()) ? 55.0f : 40.0f;

        for (Item* item : items) {
            if (!item->getIsCollected()) {
                float ix = item->getPosition().x();
                float iy = item->getPosition().y();
                float iw = item->getWidth();
                float ih = item->getHeight();

                if (px < ix + iw && px + pw > ix &&
                    py < iy + ih && py + ph > iy)
                {
                    if (p) {
                        if (item->getType() == "fruit") {
                            item->onCollision(player);
                            p->collectItem();
                            score += 10;
                            qDebug() << "¡Fruta recogida! Puntuación:" << score;
                        }
                        else if (item->getType() == "box") {
                            if (p->getIsGlutton()) {
                                item->onCollision(player); // Devora la caja
                                //p->resetGlutton();         // Pierde el estado glotón gastando sus frutas
                                //score += 50;               // Premio de puntos
                                p->startInvincibility();   // Breve ventana de seguridad
                                qDebug() << "¡Crash Glotón devoró la caja sin sufrir daño! Puntos:" << score;
                            }
                            else if (!p->getIsInvincible()) {
                                p->takeDamage();
                                p->startInvincibility();
                                qDebug() << "¡Daño por caja! Vidas restantes:" << p->getLives();
                            }
                        }
                    }
                }
            }
        }

        checkCollisions();
    }
}

void Game::addEntity(Entity* e)
{
    if (e != nullptr) entities.push_back(e);
}

void Game::loadLevel()
{
    for (Entity* e : entities) delete e;
    entities.clear();
    for (Item* i : items) delete i;
    items.clear();

    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }

    float currentX = 600.0f;
    float endX = 25000.0f;

    float boxY = 350.0f;

    while (currentX < endX) {
        float spacing = 200.0f + (std::rand() % 150);
        currentX += spacing;

        if (currentX >= endX) break;

        int spawnChance = std::rand() % 100;

        if (spawnChance < 10) {
            float randomY = std::rand() % 250;
            entities.push_back(new Obstacle("saw", QVector2D(currentX + 200.0f, randomY)));
        }
        else if (spawnChance < 35) {
            entities.push_back(new Obstacle("floating", QVector2D(currentX, 270.0f)));
        }
        else if (spawnChance < 55) {
            items.push_back(new Item("box", QVector2D(currentX, boxY), 40, 40));
        }
        else {
            float fruitHeight = 230.0f + (std::rand() % 136);
            items.push_back(new Item("fruit", QVector2D(currentX, fruitHeight), 25, 25));

            currentX += 50.0f;
        }
    }
}

void Game::checkCollisions()
{
    if (!player || !player->isActive()) return;

    Player* p = dynamic_cast<Player*>(player);
    if (!p) return;

    float px = player->getPosition().x();
    float py = player->getPosition().y();
    float pw = p->getIsGlutton() ? 55.0f : 40.0f;
    float ph = p->getIsGlutton() ? 55.0f : 40.0f;

    for (Entity* e : entities) {
        if (!e->isActive()) continue;

        Obstacle* obs = dynamic_cast<Obstacle*>(e);
        if (obs) {
            float ox = obs->getPosition().x();
            float oy = obs->getPosition().y();

            float ow = (obs->getType() == "log") ? 80.0f : 40.0f;
            float oh = (obs->getType() == "log") ? 30.0f : 40.0f;

            if (px < ox + ow && px + pw > ox &&
                py < oy + oh && py + ph > oy)
            {
                if (!p->getIsInvincible()) {
                    qDebug() << "¡Colisión con obstáculo en X:" << ox;
                    p->takeDamage();
                    p->startInvincibility();
                }
                obs->setActive(false);
                obs->onCollision(player);
            }
        }
    }
}

void Game::reset()
{
    isGameOver = false;
    isLevelComplete = false;
    score = 0;
    Player* p = dynamic_cast<Player*>(player);
    if (p) p->reset();
    loadLevel();
}
