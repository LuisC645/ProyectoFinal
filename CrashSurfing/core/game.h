#ifndef GAME_H
#define GAME_H

#include "gamestate.h"
#include <vector>

class Entity;
class Player;
class Item;
class Enemy;

class Game
{
private:
    Entity* player;
    Enemy* enemy;

    std::vector<Entity*> entities;
    std::vector<Item*> items;

    GameStatus status;

    Difficulty difficult = Difficulty::EASY;

    unsigned short fruits2Win = 20;

    bool fruitCollected = false;
    bool playerHit = false;

public:
    // Inicializa juego
    Game();

    ~Game();

    // Actualiza logica del juego
    void update(float dt);

    // Agrega entidad al nivel
    void addEntity(Entity* e);

    // Carga entidades iniciales
    void loadLevel();

    // Detecta colisiones
    void checkCollisions();

    // Reinicia partida
    void reset();

    GameStatus getStatus() const { return status; }
    void setStatus(GameStatus newStatus) { status = newStatus; }

    // Genera proyectil enemigo
    void spawnProjectile();

    Entity* getPlayer() const { return player; }
    Enemy* getEnemy() const { return enemy; }

    const std::vector<Entity*>& getEntities() const
    {
        return entities;
    }

    const std::vector<Item*>& getItems() const
    {
        return items;
    }

    bool consumeFruitCollected();
    bool consumePlayerHit();

    // Configura dificultad
    void setDifficulty(Difficulty difficulty);

    unsigned short getFruits2Win() const;

    Difficulty getDifficult() const;
};

#endif // GAME_H
