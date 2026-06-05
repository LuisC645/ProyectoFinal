#include "mainwindowlevel2.h"

#include <QBrush>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPen>
#include <QTimer>

#include "../entities/enemy2.h"
#include "../entities/item.h"
#include "../entities/obstacle.h"
#include "../entities/playerlevel2.h"

namespace
{
constexpr int WINDOW_WIDTH = 1600;
constexpr int WINDOW_HEIGHT = 600;

constexpr int SCENE_X = 0;
constexpr int SCENE_Y = 0;
constexpr int SCENE_WIDTH = 1600;
constexpr int SCENE_HEIGHT = 600;

constexpr int FRAME_INTERVAL_MS = 16;
constexpr float DELTA_TIME = 0.016f;

constexpr int HUD_X = 20;
constexpr int HUD_Y = 20;
constexpr int HUD_FONT_SIZE = 16;

constexpr int STATE_X = 500;
constexpr int STATE_Y = 200;
constexpr int STATE_FONT_SIZE = 30;
}

MainWindowLevel2::MainWindowLevel2(QWidget *parent)
    : QMainWindow(parent)
    , game(new GameLevel2())
{
    setupWindow();
    setupScene();
    setupView();
    setupWhirlpool();
    setupDeathZone();
    setupPlayer();
    setupEnemy2();
    setupFruits();
    setupObstacles();
    setupHud();
    setupStateText();

    game->setStatus(GameLevel2Status::PLAYING);

    setupTimer();
}

MainWindowLevel2::~MainWindowLevel2()
{
    delete game;
}

void MainWindowLevel2::setupWindow()
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle("Crash Surfing - Nivel 2");
}

void MainWindowLevel2::setupScene()
{
    scene = new QGraphicsScene(SCENE_X, SCENE_Y, SCENE_WIDTH, SCENE_HEIGHT, this);
    scene->setBackgroundBrush(QColor(20, 80, 150));
}

void MainWindowLevel2::setupView()
{
    view = new QGraphicsView(scene, this);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);

    setCentralWidget(view);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

void MainWindowLevel2::setupWhirlpool()
{
    const QVector2D center = game->getWhirlpool().getCenter();
    const float radius = 300.0f;

    whirlpoolVisual = scene->addEllipse(
        center.x() - radius,
        center.y() - radius,
        radius * 2.0f,
        radius * 2.0f,
        QPen(Qt::darkGray, 3),
        QBrush(QColor(120, 120, 120, 80))
        );
}

void MainWindowLevel2::setupDeathZone()
{
    const QVector2D center = game->getWhirlpool().getCenter();
    const float radius = game->getWhirlpool().getDeathRadius();

    deathZoneVisual = scene->addEllipse(
        center.x() - radius,
        center.y() - radius,
        radius * 2.0f,
        radius * 2.0f,
        QPen(Qt::red, 3),
        QBrush(QColor(255, 0, 0, 120))
        );
}

void MainWindowLevel2::setupPlayer()
{
    PlayerLevel2 *player = game->getPlayer();

    if (!player)
        return;

    playerVisual = scene->addRect(
        0,
        0,
        player->getWidth(),
        player->getHeight(),
        QPen(Qt::blue, 2),
        QBrush(Qt::blue)
        );

    playerVisual->setPos(player->getPosition().x(), player->getPosition().y());
}

void MainWindowLevel2::setupEnemy2()
{
    Enemy2 *enemy2 = game->getEnemy2();

    if (!enemy2)
        return;

    enemy2Visual = scene->addRect(
        0,
        0,
        enemy2->getWidth(),
        enemy2->getHeight(),
        QPen(Qt::black, 2),
        QBrush(QColor(180, 50, 180))
        );

    enemy2Visual->setPos(enemy2->getPosition().x(), enemy2->getPosition().y());
}

void MainWindowLevel2::setupFruits()
{
    const std::vector<Item*>& fruits = game->getFruits();

    fruitVisuals.clear();
    fruitVisuals.reserve(fruits.size());

    for (Item* fruit : fruits)
    {
        QGraphicsEllipseItem *visual = scene->addEllipse(
            0,
            0,
            fruit->getWidth(),
            fruit->getHeight(),
            QPen(Qt::yellow, 2),
            QBrush(QColor(255, 200, 0))
            );

        visual->setPos(fruit->getPosition().x(), fruit->getPosition().y());
        fruitVisuals.push_back(visual);
    }
}

void MainWindowLevel2::setupObstacles()
{
    const std::vector<Obstacle*>& obstacles = game->getObstacles();

    obstacleVisuals.clear();
    obstacleVisuals.reserve(obstacles.size());

    for (Obstacle* obstacle : obstacles)
    {
        QGraphicsRectItem *visual = scene->addRect(
            0,
            0,
            obstacle->getWidth(),
            obstacle->getHeight(),
            QPen(Qt::darkRed, 2),
            QBrush(QColor(200, 80, 40))
            );

        visual->setPos(obstacle->getPosition().x(), obstacle->getPosition().y());
        obstacleVisuals.push_back(visual);
    }
}

void MainWindowLevel2::setupHud()
{
    hudText = scene->addText("");
    hudText->setDefaultTextColor(Qt::white);
    hudText->setFont(QFont("Arial", HUD_FONT_SIZE, QFont::Bold));
    hudText->setPos(HUD_X, HUD_Y);
}

void MainWindowLevel2::setupStateText()
{
    stateText = scene->addText("");
    stateText->setDefaultTextColor(Qt::yellow);
    stateText->setFont(QFont("Arial", STATE_FONT_SIZE, QFont::Bold));
    stateText->setPos(STATE_X, STATE_Y);
}

void MainWindowLevel2::setupTimer()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindowLevel2::updateGameLoop);
    timer->start(FRAME_INTERVAL_MS);
}

void MainWindowLevel2::keyPressEvent(QKeyEvent *event)
{
    PlayerLevel2 *player = game->getPlayer();

    if (!player)
        return;

    switch (event->key())
    {
    case Qt::Key_Up:
        player->moveUp();
        break;

    case Qt::Key_Down:
        player->moveDown();
        break;

    case Qt::Key_Left:
        player->moveLeft();
        break;

    case Qt::Key_Right:
        player->moveRight();
        break;

    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindowLevel2::keyReleaseEvent(QKeyEvent *event)
{
    PlayerLevel2 *player = game->getPlayer();

    if (!player)
        return;

    switch (event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_Down:
        player->stopVertical();
        break;

    case Qt::Key_Left:
    case Qt::Key_Right:
        player->stopHorizontal();
        break;

    default:
        QMainWindow::keyReleaseEvent(event);
        break;
    }
}

void MainWindowLevel2::updateGameLoop()
{
    game->update(DELTA_TIME);

    updatePlayerVisual();
    updateEnemy2Visual();
    updateFruitVisuals();
    updateObstacleVisuals();
    rebuildProjectilesIfNeeded();
    updateProjectileVisuals();
    updateHud();
    updateStateText();
}

void MainWindowLevel2::rebuildProjectilesIfNeeded()
{
    const std::vector<Obstacle*>& projectiles = game->getProjectiles();

    if (projectileVisuals.size() == projectiles.size())
        return;

    for (QGraphicsRectItem* visual : projectileVisuals)
    {
        scene->removeItem(visual);
        delete visual;
    }

    projectileVisuals.clear();
    projectileVisuals.reserve(projectiles.size());

    for (Obstacle* projectile : projectiles)
    {
        QGraphicsRectItem *visual = scene->addRect(
            0,
            0,
            projectile->getWidth(),
            projectile->getHeight(),
            QPen(Qt::green, 2),
            QBrush(QColor(80, 255, 80))
            );

        visual->setPos(projectile->getPosition().x(), projectile->getPosition().y());
        projectileVisuals.push_back(visual);
    }
}

void MainWindowLevel2::updatePlayerVisual()
{
    PlayerLevel2 *player = game->getPlayer();

    if (!player || !playerVisual)
        return;

    playerVisual->setPos(player->getPosition().x(), player->getPosition().y());

    if (player->getIsInvincible())
        playerVisual->setBrush(QBrush(QColor(100, 180, 255)));
    else if (player->getIsSlowed())
        playerVisual->setBrush(QBrush(QColor(80, 80, 255)));
    else
        playerVisual->setBrush(QBrush(Qt::blue));
}

void MainWindowLevel2::updateEnemy2Visual()
{
    Enemy2 *enemy2 = game->getEnemy2();

    if (!enemy2 || !enemy2Visual)
        return;

    enemy2Visual->setPos(enemy2->getPosition().x(), enemy2->getPosition().y());
}

void MainWindowLevel2::updateFruitVisuals()
{
    const std::vector<Item*>& fruits = game->getFruits();

    for (std::size_t i = 0; i < fruits.size() && i < fruitVisuals.size(); ++i)
    {
        if (!fruits[i] || !fruitVisuals[i])
            continue;

        fruitVisuals[i]->setVisible(fruits[i]->isActive());
        fruitVisuals[i]->setPos(fruits[i]->getPosition().x(), fruits[i]->getPosition().y());
    }
}

void MainWindowLevel2::updateObstacleVisuals()
{
    const std::vector<Obstacle*>& obstacles = game->getObstacles();

    for (std::size_t i = 0; i < obstacles.size() && i < obstacleVisuals.size(); ++i)
    {
        if (!obstacles[i] || !obstacleVisuals[i])
            continue;

        obstacleVisuals[i]->setVisible(obstacles[i]->isActive());
        obstacleVisuals[i]->setPos(obstacles[i]->getPosition().x(), obstacles[i]->getPosition().y());
    }
}

void MainWindowLevel2::updateProjectileVisuals()
{
    const std::vector<Obstacle*>& projectiles = game->getProjectiles();

    for (std::size_t i = 0; i < projectiles.size() && i < projectileVisuals.size(); ++i)
    {
        if (!projectiles[i] || !projectileVisuals[i])
            continue;

        projectileVisuals[i]->setVisible(projectiles[i]->isActive());
        projectileVisuals[i]->setPos(projectiles[i]->getPosition().x(), projectiles[i]->getPosition().y());
    }
}

void MainWindowLevel2::updateHud()
{
    PlayerLevel2 *player = game->getPlayer();

    if (!player || !hudText)
        return;

    const float resistancePercent = player->getWhirlpoolResistance() * 100.0f;

    hudText->setPlainText(
        QString("Tiempo: %1   Vidas: %2   Frutas: %3   Resistencia: %4%   Velocidad: %5")
            .arg(game->getRemainingTime(), 0, 'f', 1)
            .arg(player->getLives())
            .arg(player->getCollectedFruits())
            .arg(resistancePercent, 0, 'f', 0)
            .arg(player->getCurrentSpeed(), 0, 'f', 0)
        );
}

void MainWindowLevel2::updateStateText()
{
    if (!stateText)
        return;

    switch (game->getStatus())
    {
    case GameLevel2Status::GAME_OVER:
        stateText->setPlainText("GAME OVER");
        break;

    case GameLevel2Status::LEVEL_COMPLETE:
        stateText->setPlainText("NIVEL COMPLETADO");
        break;

    case GameLevel2Status::PLAYING:
    default:
        stateText->setPlainText("");
        break;
    }
}
