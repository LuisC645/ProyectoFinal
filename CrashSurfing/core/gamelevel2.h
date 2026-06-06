#ifndef GAMELEVEL2_H
#define GAMELEVEL2_H

#include "gamestate.h"

#include <vector>

#include "../entities/player2.h"
#include "../entities/item.h"
#include "../entities/obstacle.h"
#include "../entities/whirlpool.h"

class GameLevel2
{
public:
    // Inicializa nivel 2
    GameLevel2();

    ~GameLevel2();

    // Actualiza logica del nivel
    void update(float dt);

    // Reinicia partida
    void reset();

    // Entidades principales
    Player2* getPlayer();
    Whirlpool& getWhirlpool();

    // Contenedores
    const std::vector<Item*>& getFruits() const;
    const std::vector<Obstacle*>& getObstacles() const;

    // Estado
    GameStatus getStatus() const;
    void setStatus(GameStatus newStatus);

    // Tiempo restante
    float getRemainingTime() const;

    // Configura dificultad
    void setDifficulty(Difficulty difficulty);
    Difficulty getDifficult() const;

    // Objetivo de frutas
    unsigned short getFruits2Win() const;

    // Eventos de audio
    bool consumeFruitCollected();
    bool consumePlayerHit();

private:
    // Genera entidades iniciales
    void loadLevel();

    // Libera entidades
    void clearLevelEntities();

    // Aplica fuerza del remolino
    void updateWhirlpoolForce(float dt);

    // Genera frutas
    void spawnFruits();

    // Genera obstaculos
    void spawnBoxes();

    // Detecta colisiones
    void checkCollisions();

    // Verifica colision AABB
    bool isColliding(const Entity* a, const Entity* b) const;

private:

    // Jugador y remolino
    Player2* player;
    Whirlpool whirlpool;

    // Entidades del nivel
    std::vector<Item*> fruits;
    std::vector<Obstacle*> obstacles;

    // Temporizador
    float survivalTimer;
    float targetTime;

    // Estado actual
    GameStatus status;

    // Dificultad
    Difficulty difficult;

    // Objetivo de victoria
    unsigned short fruits2Win;

    // Eventos de audio
    bool fruitCollected;
    bool playerHit;
};

#endif // GAMELEVEL2_H
