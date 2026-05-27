#ifndef GAME_H
#define GAME_H

#include <vector>
#include "../entities/player.h"
#include "../entities/entity.h"

using namespace std;

class Game
{
private:
    Player* player;
    vector<Entity*> entities; // Lista polimorfica que guarda enemys y obstacles
    unsigned short score;

public:
    Game();
    ~Game();

    void update(float dt);
    void addEntity(Entity* e);

    // olision
    void loadLevel();         // colocar obj
    void checkCollisions();   // colision

    // Getters
    Player* getPlayer() const { return player; }
    unsigned short getScore() const { return score; }
};

#endif // GAME_H
