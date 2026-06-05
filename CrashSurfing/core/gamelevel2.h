#ifndef GAMELEVEL2_H
#define GAMELEVEL2_H

#include <vector>
#include "../entities/playerlevel2.h"
#include "../entities/entity.h"
#include "../entities/item.h"
#include "../entities/obstacle.h"
#include "../entities/whirlpool.h"

enum class GameLevel2Status
{
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETE
};

class GameLevel2
{
public:

    GameLevel2();
    ~GameLevel2();

    void update(float dt);

    void reset();

    PlayerLevel2* getPlayer();

    Whirlpool& getWhirlpool();

    float getRemainingTime() const;

    GameLevel2Status getStatus() const;
    void setStatus(GameLevel2Status newStatus);

private:

    PlayerLevel2* player;

    Whirlpool whirlpool;

    float survivalTimer;

    float targetTime;

    GameLevel2Status status;
};

#endif
