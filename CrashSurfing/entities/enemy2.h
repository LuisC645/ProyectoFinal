#ifndef ENEMY2_H
#define ENEMY2_H

#include "entity.h"

class PlayerLevel2;
class Whirlpool;

class Enemy2 : public Entity
{
public:
    Enemy2();
    ~Enemy2() override;

    void update(float dt) override;
    void onCollision(Entity* e) override;

    // Evaluación de disparo
    bool canEvaluateShot() const;
    bool shouldShoot(const PlayerLevel2* player, const Whirlpool& whirlpool);
    void resetDecisionTimer();

    // Predicción y aprendizaje
    QVector2D predictPlayerPosition(const PlayerLevel2* player) const;
    void registerShotFired();
    void registerShotHit();
    void registerSuccessfulSlowdown();
    void adaptBehavior();

    // Getters
    QVector2D getLastPredictedPosition() const;
    float getDecisionCooldown() const;
    int getShotsFired() const;
    int getShotsHit() const;
    int getSuccessfulSlowdowns() const;

private:
    void updateMovement(float dt);
    float evaluateThreat(const PlayerLevel2* player, const Whirlpool& whirlpool) const;

private:
    float decisionTimer;
    float decisionCooldown;

    float orbitTime;
    QVector2D orbitCenter;
    float orbitRadiusX;
    float orbitRadiusY;
    float orbitSpeed;

    QVector2D lastPredictedPosition;

    int shotsFired;
    int shotsHit;
    int successfulSlowdowns;

    float aimError;
    float aggressiveness;
};

#endif // ENEMY2_H
