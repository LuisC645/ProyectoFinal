#ifndef GAME_H
#define GAME_H

#include <vector>

// Forward declarations para evitar dependencias circulares
class Entity;
class Player;
class Item;

class Game {
private:
    Entity* player;
    std::vector<Entity*> entities;
    std::vector<Item*> items;

    int score;
    bool isGameOver;
    bool isLevelComplete;

public:
    Game();
    ~Game();

    void update(float dt);
    void addEntity(Entity* e);
    void loadLevel();
    void checkCollisions();
    void reset();

    // === GETTERS Y SETTERS ===
    Entity* getPlayer() const { return player; }
    const std::vector<Entity*>& getEntities() const { return entities; }
    const std::vector<Item*>& getItems() const { return items; }

    bool getIsGameOver() const { return isGameOver; }
    bool getIsLevelComplete() const { return isLevelComplete; }
    void setLevelComplete(bool complete) { isLevelComplete = complete; }

    int getScore() const { return score; }
};

#endif // GAME_H
