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
    if (player) {
        player->update(dt);

        // Obtenemos la posición de Crash (asumiendo tamaño 40x40)
        float px = player->getPosition().x();
        float py = player->getPosition().y();
        float pw = 40.0f;
        float ph = 40.0f;

        // Revisamos colisiones con cada ítem
        for (Item* item : items) {
            if (!item->getIsCollected()) {

                // Coordenadas de la fruta
                float ix = item->getPosition().x();
                float iy = item->getPosition().y();
                float iw = item->getWidth();
                float ih = item->getHeight();

                // Algoritmo matemático AABB (Cajas de colisión alineadas)
                if (px < ix + iw && px + pw > ix &&
                    py < iy + ih && py + ph > iy)
                {
                    // ¡Hubo colisión!
                    item->onCollision(player); // La fruta se apaga

                    // Convertimos el puntero genérico a Player para activar su lógica
                    Player* p = dynamic_cast<Player*>(player);
                    if (p) {
                        // 2. ¿Es comida o es obstáculo?
                        if (item->getType() == "fruit") {
                            p->collectItem(); // Suma al glutton
                            qDebug() << "¡Fruta recogida!";
                        }
                        else if (item->getType() == "box") {
                            p->takeDamage();  // Quita una vida

                            // Opcional: Podrías reducir su velocidad temporalmente aquí para simular el tropiezo
                            // p->setVelocity(QVector2D(150.0f, 0.0f));
                        }
                    }
                }
            }
        }
    }
}

void Game::addEntity(Entity* e)
{
    if (e != nullptr) {
        entities.push_back(e);
    }
}

void Game::loadLevel()
{
    player = new Player();

    // Sembramos frutas flotando a diferentes distancias del río (coordenada Y = 380 para que queden flotando)
    items.push_back(new Item(600, 380, "fruit"));
    items.push_back(new Item(1000, 380, "fruit"));
    items.push_back(new Item(1400, 320, "fruit")); // Esta está más alta, obligará a saltar
    items.push_back(new Item(1800, 380, "fruit"));
    items.push_back(new Item(2200, 380, "fruit")); // Al agarrar esta (la 5ta), ¡se activará el Glutton!
    items.push_back(new Item(2600, 380, "fruit"));
    items.push_back(new Item(3000, 380, "fruit"));

    items.push_back(new Item(1400, 390, "box"));
    items.push_back(new Item(2000, 390, "box"));
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
