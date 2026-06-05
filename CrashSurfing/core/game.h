#ifndef GAME_H
#define GAME_H

#include "gamestate.h"
#include <vector>

class Entity;
class Player;
class Item;
class Enemy;

class Game {    
private:
    Entity* player;
    Enemy* enemy;
    std::vector<Entity*> entities;
    std::vector<Item*> items;

    int score;
    GameStatus status;
    Difficulty difficult = Difficulty::EASY;
    int fruits2Win = 20;

    bool fruitCollected = false;
    bool playerHit = false;

public:
    Game();
    ~Game();

    void update(float dt);
    void addEntity(Entity* e);
    void loadLevel();
    void checkCollisions();
    void reset();

    GameStatus getStatus() const { return status; }
    void setStatus(GameStatus newStatus) { status = newStatus; }

    // int getScore() const { return score; }

    void spawnProjectile();

    Entity* getPlayer() const { return player; }
    Enemy* getEnemy() const { return enemy; }
    const std::vector<Entity*>& getEntities() const { return entities; }
    const std::vector<Item*>& getItems() const { return items; }

    bool consumeFruitCollected();
    bool consumePlayerHit();

    void setDifficulty(Difficulty difficulty);
    int getFruits2Win() const;

    Difficulty getDifficult() const;

};

#endif // GAME_H
