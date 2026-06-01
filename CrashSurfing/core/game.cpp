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

    // ✨ Iniciamos el juego en modo MENU
    status = GameStatus::MENU;
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
    // 1. Si no estamos jugando, no hacemos nada
    if (status != GameStatus::PLAYING) return;

    // CAP DE SEGURIDAD PARA EL LAG: Si el juego se congela un momento,
    // evitamos que el 'dt' sea gigantesco y rompa las físicas.
    if (dt > 0.1f) dt = 0.1f;

    // === EL TRUCO DE ALTA VELOCIDAD: SUB-STEPPING ===
    // En lugar de mover a Crash un tramo grande de golpe, dividimos el tiempo
    // de este fotograma en 4 "mini-pasos".
    const int SUB_STEPS = 4;
    float subDt = dt / SUB_STEPS;

    for (int paso = 0; paso < SUB_STEPS; ++paso)
    {
        // A) Mover al jugador un mini-tramo
        if (player) {
            player->update(subDt);
        }

        // B) Mover los obstáculos y objetos un mini-tramo
        for (Entity* e : entities) {
            if (e->isActive()) e->update(subDt);
        }
        for (Item* i : items) {
            if (!i->getIsCollected()) i->update(subDt);
        }

        // C) ¡REVISAR COLISIONES EN CADA MINI-PASO!
        // Al revisar las colisiones 4 veces por frame, Crash nunca avanzará más de
        // 2-3 píxeles entre cada chequeo, haciendo imposible que atraviese nada.
        checkCollisions();

        // D) Verificar estados críticos del juego en cada sub-paso
        Player* p = dynamic_cast<Player*>(player);
        if (p) {
            if (p->getLives() <= 0) {
                status = GameStatus::GAME_OVER;
                return; // Salimos inmediatamente si muere
            }
            if (p->getPosition().x() >= 25000.0f) {
                status = GameStatus::LEVEL_COMPLETE;
                return; // Salimos inmediatamente si gana
            }
        }
    }
}


void Game::reset()
{
    score = 0;

    // ✨ Al reiniciar, volvemos a poner el juego en el menú
    status = GameStatus::MENU;

    Player* p = dynamic_cast<Player*>(player);
    if (p) p->reset();

    loadLevel();
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

    // 🔥 DEFINIMOS EL SUELO AQUÍ 🔥
    // Ajusta este número (por ejemplo 380.0f o 400.0f) según dónde camine Crash
    float floorY = 380.0f;

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
            // Ahora restamos 40 a floorY para que el tronco de 40px de alto
            // quede reposando exactamente sobre la línea del piso.
            entities.push_back(new Obstacle("log", QVector2D(currentX, floorY - 30.0f)));        }
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

    float pX = player->getPosition().x();
    float pY = player->getPosition().y();

    // Mantenemos la hitbox de Crash ligeramente ajustada (36x36)
    // para que los saltos se sientan fluidos y justos en las esquinas.
    float pW = 36.0f;
    float pH = 36.0f;

    // === 1. REVISAR OBSTÁCULOS ===
    for (Entity* ent : entities) {
        if (!ent->isActive()) continue;

        float eX = ent->getPosition().x();
        float eY = ent->getPosition().y();

        // Medida estándar para sierras, flotantes y ahora tus troncos de 40x40
        float eW = 40.0f;
        float eH = 40.0f;

        Obstacle* obs = dynamic_cast<Obstacle*>(ent);
        if (obs) {
            QString type = obs->getType().toLower();
            if (type == "log" || type == "tronco") {
                // 🎯 AJUSTE DE REGLA EN 40x40:
                // Dejamos el ancho en 40, pero bajamos el alto matemático a 35.
                // Esos 5 píxeles menos en el techo evitan que Crash colisione
                // falsamente si su pie roza el borde superior al saltar a toda velocidad.
                eW = 40.0f;
                eH = 35.0f;
            }
        }

        // Algoritmo de colisión AABB estándar
        if (pX < eX + eW && pX + pW > eX &&
            pY < eY + eH && pY + pH > eY)
        {
            player->onCollision(ent);
        }
    }

    // === 2. REVISAR ITEMS ===
    for (Item* item : items) {
        if (item->getIsCollected() || !item->isActive()) continue;

        float iX = item->getPosition().x();
        float iY = item->getPosition().y();
        float iW = item->getWidth();
        float iH = item->getHeight();

        if (pX < iX + iW && pX + pW > iX &&
            pY < iY + iH && pY + pH > iY)
        {
            player->onCollision(item);
        }
    }
}
