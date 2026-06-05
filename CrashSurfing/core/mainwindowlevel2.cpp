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

constexpr int WHIRLPOOL_X = 500;
constexpr int WHIRLPOOL_Y = 0;
constexpr int WHIRLPOOL_SIZE = 600;

constexpr int DEATH_ZONE_X = 750;
constexpr int DEATH_ZONE_Y = 250;
constexpr int DEATH_ZONE_SIZE = 100;

constexpr int PLAYER_START_X = 0;
constexpr int PLAYER_START_Y = 0;
constexpr int PLAYER_WIDTH = 50;
constexpr int PLAYER_HEIGHT = 50;

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
    setCentralWidget(view);
}

void MainWindowLevel2::setupWhirlpool()
{
    whirlpoolVisual = scene->addEllipse(
        WHIRLPOOL_X,
        WHIRLPOOL_Y,
        WHIRLPOOL_SIZE,
        WHIRLPOOL_SIZE,
        QPen(Qt::darkGray, 3),
        QBrush(QColor(120, 120, 120, 80))
        );
}

void MainWindowLevel2::setupDeathZone()
{
    deathZoneVisual = scene->addEllipse(
        DEATH_ZONE_X,
        DEATH_ZONE_Y,
        DEATH_ZONE_SIZE,
        DEATH_ZONE_SIZE,
        QPen(Qt::red, 3),
        QBrush(QColor(255, 0, 0, 120))
        );
}

void MainWindowLevel2::setupPlayer()
{
    playerVisual = scene->addRect(
        PLAYER_START_X,
        PLAYER_START_Y,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        QPen(Qt::blue, 2),
        QBrush(Qt::blue)
        );
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

    PlayerLevel2 *player = game->getPlayer();
    if (!player)
        return;

    updatePlayerVisual();
    updateHud();
    updateStateText();
}

void MainWindowLevel2::updatePlayerVisual()
{
    PlayerLevel2 *player = game->getPlayer();
    if (!player)
        return;

    playerVisual->setPos(player->getPosition().x(), player->getPosition().y());
}

void MainWindowLevel2::updateHud()
{
    hudText->setPlainText(QString("Tiempo: %1").arg(game->getRemainingTime(), 0, 'f', 1));
}

void MainWindowLevel2::updateStateText()
{
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
