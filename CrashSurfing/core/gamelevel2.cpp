#include "gamelevel2.h"
#include <QDebug>
#include <QRandomGenerator>

namespace
{
constexpr float TARGET_TIME = 60.0f;

constexpr unsigned char FRUIT_COUNT = 30;
constexpr unsigned char BOX_COUNT = 18;

constexpr float MIN_X = 80.0f;
constexpr float MAX_X = 1480.0f;
constexpr float MIN_Y = 80.0f;
constexpr float MAX_Y = 520.0f;
}

// Inicializa nivel 2
GameLevel2::GameLevel2()
{
    player = new Player2();

    survivalTimer = 0.0f;
    targetTime = TARGET_TIME;
    status = GameStatus::MENU;
    difficult = Difficulty::EASY;

    fruits2Win = 15;
    fruitCollected = false;
    playerHit = false;

    loadLevel();
}

// Libera memoria del nivel
GameLevel2::~GameLevel2()
{
    clearLevelEntities();
    delete player;
}

// Actualiza logica del nivel
void GameLevel2::update(float dt)
{
    if(status != GameStatus::PLAYING){ return; }

    updateWhirlpoolForce(dt);

    if(player){ player->update(dt); }

    checkCollisions();
    survivalTimer += dt;

    if(!player || !player->isActive() || whirlpool.isInsideDeathZone(player))
    {
        status = GameStatus::GAME_OVER;
        return;
    }

    // Verifica victoria
    if(survivalTimer >= targetTime)
    {
        if(player->getCollectedFruits() >= fruits2Win)
        {
            status = GameStatus::LEVEL_COMPLETE;
        }
        else
        {
            status = GameStatus::GAME_OVER;
        }

        return;
    }
}

// Reinicia partida
void GameLevel2::reset()
{
    if(player){ player->reset(); }

    clearLevelEntities();

    loadLevel();

    survivalTimer = 0.0f;

    status = GameStatus::MENU;

    fruitCollected = false;
    playerHit = false;
}

// Obtiene jugador
Player2* GameLevel2::getPlayer(){ return player; }

// Obtiene remolino
Whirlpool& GameLevel2::getWhirlpool(){ return whirlpool; }

// Obtiene frutas
const std::vector<Item*>& GameLevel2::getFruits() const { return fruits; }

// Obtiene obstaculos
const std::vector<Obstacle*>& GameLevel2::getObstacles() const { return obstacles; }

// Obtiene tiempo restante
float GameLevel2::getRemainingTime() const
{
    return targetTime - survivalTimer;
}

// Genera entidades iniciales
void GameLevel2::loadLevel()
{
    spawnFruits();
    spawnBoxes();
}

// Libera entidades
void GameLevel2::clearLevelEntities()
{
    for (Item* fruit : fruits){ delete fruit; }
    fruits.clear();

    for (Obstacle* obstacle : obstacles){ delete obstacle; }
    obstacles.clear();
}

// Aplica fuerza del remolino
void GameLevel2::updateWhirlpoolForce(float dt)
{
    if (!player){ return; }

    QVector2D frameContribution = whirlpool.computeFrameVelocity(player, dt);
    frameContribution *= (1.0f - player->getWhirlpoolResistance());
    player->setExternalVelocity(frameContribution);
}

// Genera frutas
void GameLevel2::spawnFruits()
{
    for (unsigned char i = 0; i < FRUIT_COUNT; ++i)
    {
        const float x = static_cast<float>(MIN_X + QRandomGenerator::global()->generateDouble() * (MAX_X - MIN_X));
        const float y = static_cast<float>(MIN_Y + QRandomGenerator::global()->generateDouble() * (MAX_Y - MIN_Y));

        Item* fruit = new Item("fruit", QVector2D(x, y), 28.0f, 28.0f);
        fruits.push_back(fruit);
    }
}

// Genera obstaculos
void GameLevel2::spawnBoxes()
{
    for (unsigned char i = 0; i < BOX_COUNT; ++i)
    {
        const float x = static_cast<float>(MIN_X + QRandomGenerator::global()->generateDouble() * (MAX_X - MIN_X));
        const float y = static_cast<float>(MIN_Y + QRandomGenerator::global()->generateDouble() * (MAX_Y - MIN_Y));

        Obstacle* box = new Obstacle("tronco", QVector2D(x, y));

        box->setWidth(40.0f);
        box->setHeight(40.0f);

        obstacles.push_back(box);
    }
}

// Detecta colisiones
void GameLevel2::checkCollisions()
{
    if (!player || !player->isActive()){ return; }

    for (Item* fruit : fruits)
    {
        if (!fruit || !fruit->isActive()){ continue; }

        if (isColliding(player, fruit))
        {
            player->onCollision(fruit);
            fruitCollected = true;
        }
    }

    for (Obstacle* obstacle : obstacles)
    {
        if (!obstacle || !obstacle->isActive()){ continue; }

        if (isColliding(player, obstacle))
        {
            bool wasInvincible = player->getIsInvincible();

            player->onCollision(obstacle);

            if(!wasInvincible && player->getIsInvincible())
            {
                playerHit = true;
            }
        }
    }
}

// Verifica colision AABB
bool GameLevel2::isColliding(const Entity* a, const Entity* b) const
{
    if (!a || !b){ return false; }

    const float ax1 = a->getPosition().x();
    const float ay1 = a->getPosition().y();
    const float ax2 = ax1 + a->getWidth();
    const float ay2 = ay1 + a->getHeight();

    const float bx1 = b->getPosition().x();
    const float by1 = b->getPosition().y();
    const float bx2 = bx1 + b->getWidth();
    const float by2 = by1 + b->getHeight();

    return (ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1);
}

// Configura dificultad
void GameLevel2::setDifficulty(Difficulty difficulty)
{
    difficult = difficulty;

    if(difficulty == Difficulty::EASY)
    {
        whirlpool.setForce(1.9f);
        fruits2Win = 20;
        targetTime = 45.0f;
    }
    else
    {
        whirlpool.setForce(4.0f);
        fruits2Win = 25;
        targetTime = 60.0f;
    }
}

// Obtiene dificultad
Difficulty GameLevel2::getDifficult() const
{
    return difficult;
}

// Obtiene objetivo de frutas
unsigned short GameLevel2::getFruits2Win() const
{
    return fruits2Win;
}

// Consume evento de fruta
bool GameLevel2::consumeFruitCollected()
{
    if(fruitCollected)
    {
        fruitCollected = false;
        return true;
    }

    return false;
}

// Consume evento de daño
bool GameLevel2::consumePlayerHit()
{
    if(playerHit)
    {
        playerHit = false;
        return true;
    }

    return false;
}

// Actualiza estado
void GameLevel2::setStatus(GameStatus newStatus)
{
    status = newStatus;
}

// Obtiene estado
GameStatus GameLevel2::getStatus() const
{
    return status;
}
