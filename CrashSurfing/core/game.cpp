#include "game.h"
#include "../entities/player.h"
#include "../entities/entity.h"
#include "../entities/obstacle.h"
#include "../entities/item.h"
#include "../entities/enemy.h"

#include <QDebug>
#include <cstdlib>
#include <ctime>

Game::Game()
{
    player = new Player();
    enemy = new Enemy();
    score = 0;
    status = GameStatus::MENU;
}

Game::~Game()
{
    delete player;
    delete enemy;
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

        enemy->update(subDt);
        enemy->setPosition(
            QVector2D(
                player->getPosition().x() + 1000.0f,
                enemy->getPosition().y() - 100.0f
                )
            );

        if(enemy->canShoot())
        {
            qDebug() << "DISPARO";

            enemy->resetShootTimer();

            spawnProjectile();
        }

    }



}


void Game::reset()
{
    score = 0;
    status = GameStatus::MENU;

    Player* p = dynamic_cast<Player*>(player);
    if (p) p->reset();

    enemy->setPosition(QVector2D(1100.0f,250.0f));
    enemy->setActive(true);

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
    float floorY = 380.0f;

    while (currentX < endX) {
        float spacing = 200.0f + (std::rand() % 150);
        currentX += spacing;

        if (currentX >= endX) break;

        int spawnChance = std::rand() % 100;

        if (spawnChance < 5) {
            float randomY = std::rand() % 250;
            entities.push_back(new Obstacle("saw", QVector2D(currentX + 200.0f, randomY)));
        }
        else if (spawnChance < 20) {
            entities.push_back(new Obstacle("floating", QVector2D(currentX, 270.0f)));
        }
        else if (spawnChance < 60) {
            // Ahora restamos 40 a floorY para que el tronco de 40px de alto
            // quede reposando exactamente sobre la línea del piso.
            entities.push_back(new Obstacle("log", QVector2D(currentX, floorY - 15.0f)));        }
        else {
            float fruitHeight = 230.0f + (std::rand() % 136);
            items.push_back(new Item("fruit", QVector2D(currentX, fruitHeight), 25, 25));

            currentX += 50.0f;
        }
    }
}

void Game::checkCollisions()
{
    if (!player || !player->isActive())
        return;

    float pW = player->getWidth();
    float pH = player->getHeight();

    float pX = player->getPosition().x() - (pW * 0.5f);
    float pY = player->getPosition().y() - pH + 25.0f;

    for (Entity* ent : entities)
    {
        if (!ent->isActive())
            continue;

        float eX = ent->getPosition().x();
        float eY = ent->getPosition().y();

        float eW = ent->getWidth();
        float eH = ent->getHeight();

        if (pX < eX + eW &&
            pX + pW > eX &&
            pY < eY + eH &&
            pY + pH > eY)
        {
            player->onCollision(ent);
        }
    }

    for (Item* item : items)
    {
        if (!item->isActive())
            continue;

        float iX = item->getPosition().x();
        float iY = item->getPosition().y();

        float iW = item->getWidth();
        float iH = item->getHeight();

        if (pX < iX + iW &&
            pX + pW > iX &&
            pY < iY + iH &&
            pY + pH > iY)
        {
            player->onCollision(item);
        }
    }
}

void Game::spawnProjectile()
{
    if(!enemy || !player)
        return;

    // ==========================================
    // PERCEPCIÓN
    // ==========================================

    float playerX = player->getPosition().x();
    float playerY = player->getPosition().y();

    float playerVX = player->getVelocity().x();
    float playerVY = player->getVelocity().y();

    float enemyX = enemy->getPosition().x();
    float enemyY = enemy->getPosition().y();

    // ==========================================
    // PREDICCIÓN
    // ==========================================

    float predictionTime = 0.6f;

    float futureX =
        playerX +
        playerVX * predictionTime;

    float futureY =
        playerY +
        playerVY * predictionTime;

    // ==========================================
    // DECISIÓN PROBABILÍSTICA
    // ==========================================

    int chance = rand() % 100;

    if(chance > 35)
        return;

    // ==========================================
    // CREAR PROYECTIL
    // ==========================================

    Obstacle* projectile =
        new Obstacle(
            "saw",
            QVector2D(enemyX, enemyY)
            );

    // ==========================================
    // RAZONAMIENTO
    // ==========================================

    float dx =
        futureX -
        enemyX;

    float dy =
        futureY -
        enemyY;

    // Velocidad horizontal fija
    float vx = -650.0f;

    // Velocidad vertical adaptativa
    float vy =
        -650.0f +
        (dy * 0.40f);

    // Limitar valores extremos
    if(vy < -900.0f)
        vy = -900.0f;

    if(vy > -250.0f)
        vy = -250.0f;

    projectile->setVelocity(
        QVector2D(-2000.0f, -800.0f)
        );

    entities.push_back(projectile);
}
