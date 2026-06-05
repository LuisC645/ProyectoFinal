#include "gamelevel2.h"

GameLevel2::GameLevel2()
{
    player = new PlayerLevel2();

    survivalTimer = 0.0f;

    targetTime = 60.0f;

    status = GameLevel2Status::MENU;
}

GameLevel2::~GameLevel2()
{
    delete player;
}

void GameLevel2::update(float dt)
{
    if(status != GameLevel2Status::PLAYING)
        return;

    player->update(dt);

    whirlpool.applyForce(player);

    survivalTimer += dt;

    if(whirlpool.isInsideDeathZone(player))
    {
        status = GameLevel2Status::GAME_OVER;
        return;
    }

    if(survivalTimer >= targetTime)
    {
        status = GameLevel2Status::LEVEL_COMPLETE;
        return;
    }
}

void GameLevel2::reset()
{
    if(player)
        player->reset();

    survivalTimer = 0.0f;

    status = GameLevel2Status::MENU;
}

PlayerLevel2* GameLevel2::getPlayer()
{
    return player;
}

Whirlpool& GameLevel2::getWhirlpool()
{
    return whirlpool;
}

float GameLevel2::getRemainingTime() const
{
    return targetTime - survivalTimer;
}

GameLevel2Status GameLevel2::getStatus() const
{
    return status;
}

void GameLevel2::setStatus(GameLevel2Status newStatus)
{
    status = newStatus;
}
