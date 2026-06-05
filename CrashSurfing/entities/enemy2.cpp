#include "enemy2.h"
#include "playerlevel2.h"
#include "whirlpool.h"
#include <QRandomGenerator>
#include <QtMath>

namespace
{
constexpr float START_X = 1400.0f;
constexpr float START_Y = 300.0f;
constexpr float ENEMY_WIDTH = 70.0f;
constexpr float ENEMY_HEIGHT = 70.0f;
constexpr float DECISION_COOLDOWN = 1.5f;
constexpr float ORBIT_CENTER_X = 800.0f;
constexpr float ORBIT_CENTER_Y = 300.0f;
constexpr float ORBIT_RADIUS_X = 700.0f;
constexpr float ORBIT_RADIUS_Y = 230.0f;
constexpr float ORBIT_SPEED = 1.0f;
constexpr float BASE_AIM_ERROR = 45.0f;
constexpr float MIN_AIM_ERROR = 8.0f;
constexpr float MAX_AIM_ERROR = 70.0f;
constexpr float BASE_AGGRESSIVENESS = 0.55f;
constexpr float MIN_AGGRESSIVENESS = 0.25f;
constexpr float MAX_AGGRESSIVENESS = 0.95f;
}

Enemy2::Enemy2()
{
    position = QVector2D(START_X, START_Y);
    velocity = QVector2D(0.0f, 0.0f);
    active = true;

    width = ENEMY_WIDTH;
    height = ENEMY_HEIGHT;

    decisionTimer = 0.0f;
    decisionCooldown = DECISION_COOLDOWN;

    orbitTime = 0.0f;
    orbitCenter = QVector2D(ORBIT_CENTER_X, ORBIT_CENTER_Y);
    orbitRadiusX = ORBIT_RADIUS_X;
    orbitRadiusY = ORBIT_RADIUS_Y;
    orbitSpeed = ORBIT_SPEED;

    lastPredictedPosition = position;

    shotsFired = 0;
    shotsHit = 0;
    successfulSlowdowns = 0;

    aimError = BASE_AIM_ERROR;
    aggressiveness = BASE_AGGRESSIVENESS;
}

Enemy2::~Enemy2()
{
}

void Enemy2::update(float dt)
{
    if (!active)
        return;

    decisionTimer += dt;
    updateMovement(dt);
}

void Enemy2::onCollision(Entity* e)
{
    Q_UNUSED(e);
}

bool Enemy2::canEvaluateShot() const
{
    return decisionTimer >= decisionCooldown;
}

bool Enemy2::shouldShoot(const PlayerLevel2* player, const Whirlpool& whirlpool)
{
    if (!active || !player || !player->isActive())
        return false;

    if (!canEvaluateShot())
        return false;

    lastPredictedPosition = predictPlayerPosition(player);

    const float threat = evaluateThreat(player, whirlpool);
    const float randomValue =
        static_cast<float>(QRandomGenerator::global()->generateDouble());

    const bool decision = (randomValue < threat);

    return decision;
}

void Enemy2::resetDecisionTimer()
{
    decisionTimer = 0.0f;
}

QVector2D Enemy2::predictPlayerPosition(const PlayerLevel2* player) const
{
    if (!player)
        return position;

    const QVector2D playerPos = player->getPosition();
    const QVector2D playerVel = player->getVelocity();

    float predictionTime = 0.75f;

    if (player->getIsSlowed())
        predictionTime = 0.95f;

    QVector2D predicted = playerPos + playerVel * predictionTime;

    const float randomOffsetX =
        -aimError + static_cast<float>(
            QRandomGenerator::global()->generateDouble() * (2.0 * aimError)
            );

    const float randomOffsetY =
        -aimError + static_cast<float>(
            QRandomGenerator::global()->generateDouble() * (2.0 * aimError)
            );

    predicted += QVector2D(randomOffsetX, randomOffsetY);

    return predicted;
}

void Enemy2::registerShotFired()
{
    ++shotsFired;
}

void Enemy2::registerShotHit()
{
    ++shotsHit;
    adaptBehavior();
}

void Enemy2::registerSuccessfulSlowdown()
{
    ++successfulSlowdowns;
    adaptBehavior();
}

void Enemy2::adaptBehavior()
{
    if (shotsFired <= 0)
        return;

    const float hitRate =
        static_cast<float>(shotsHit) / static_cast<float>(shotsFired);

    if (hitRate > 0.6f)
    {
        aimError -= 4.0f;
        aggressiveness += 0.03f;
    }
    else if (hitRate < 0.3f)
    {
        aimError += 3.0f;
        aggressiveness += 0.02f;
    }

    if (successfulSlowdowns > 3)
    {
        aggressiveness += 0.02f;
    }

    if (aimError < MIN_AIM_ERROR)
        aimError = MIN_AIM_ERROR;

    if (aimError > MAX_AIM_ERROR)
        aimError = MAX_AIM_ERROR;

    if (aggressiveness < MIN_AGGRESSIVENESS)
        aggressiveness = MIN_AGGRESSIVENESS;

    if (aggressiveness > MAX_AGGRESSIVENESS)
        aggressiveness = MAX_AGGRESSIVENESS;
}

QVector2D Enemy2::getLastPredictedPosition() const
{
    return lastPredictedPosition;
}

float Enemy2::getDecisionCooldown() const
{
    return decisionCooldown;
}

int Enemy2::getShotsFired() const
{
    return shotsFired;
}

int Enemy2::getShotsHit() const
{
    return shotsHit;
}

int Enemy2::getSuccessfulSlowdowns() const
{
    return successfulSlowdowns;
}

void Enemy2::updateMovement(float dt)
{
    orbitTime += dt * orbitSpeed;

    const float x =
        orbitCenter.x() + orbitRadiusX * qCos(orbitTime);
    const float y =
        orbitCenter.y() + orbitRadiusY * qSin(orbitTime);

    QVector2D newPosition(x, y);
    velocity = (newPosition - position) / dt;
    position = newPosition;
}

float Enemy2::evaluateThreat(const PlayerLevel2* player, const Whirlpool& whirlpool) const
{
    if (!player)
        return MIN_AGGRESSIVENESS;

    const QVector2D playerPos = player->getPosition();
    const QVector2D playerVel = player->getVelocity();
    const QVector2D center = whirlpool.getCenter();

    const float distanceToCenter = (center - playerPos).length();
    const float speedMagnitude = playerVel.length();

    float threat = aggressiveness;

    if (distanceToCenter < 260.0f)
        threat += 0.20f;
    else if (distanceToCenter < 400.0f)
        threat += 0.10f;

    if (speedMagnitude < 120.0f)
        threat += 0.10f;

    if (player->getLives() <= 2)
        threat += 0.08f;

    if (player->getCollectedFruits() < 5)
        threat += 0.08f;

    if (player->getIsSlowed())
        threat -= 0.12f;

    const float resistance = player->getWhirlpoolResistance();
    threat += (0.30f - resistance) * 0.35f;

    if (threat < 0.05f)
        threat = 0.05f;

    if (threat > 0.95f)
        threat = 0.95f;

    return threat;
}
