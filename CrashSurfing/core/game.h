#ifndef GAME_H
#define GAME_H

#include <vector>

// Declaraciones anticipadas (Forward declarations)
class Entity;
class Player;
class Item;

// ✨ 1. NUESTRO ENUM DE ESTADOS
enum class GameStatus {
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETE
};

class Game {
public:
    Game();
    ~Game();

    void update(float dt);
    void addEntity(Entity* e);
    void loadLevel();
    void checkCollisions();
    void reset();

    // ✨ 2. NUEVOS GETTERS Y SETTERS PARA EL ESTADO
    GameStatus getStatus() const { return status; }
    void setStatus(GameStatus newStatus) { status = newStatus; }

    int getScore() const { return score; }

    Entity* getPlayer() const { return player; }
    const std::vector<Entity*>& getEntities() const { return entities; }
    const std::vector<Item*>& getItems() const { return items; }

private:
    Entity* player;
    std::vector<Entity*> entities;
    std::vector<Item*> items;

    int score;

    // ✨ 3. VARIABLE CENTRAL DE ESTADO (Reemplaza a isGameOver e isLevelComplete)
    GameStatus status;
};

#endif // GAME_H
