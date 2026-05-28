#include "mainwindow.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    game = new Game();
    game->loadLevel();

    scene = new QGraphicsScene(0, 0, 800, 600, this);
    scene->setBackgroundBrush(Qt::black);

    view = new QGraphicsView(scene, this);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(view);

    setFixedSize(800, 600);
    setWindowTitle("Crash Surfing - Pre-Alpha");

    visualPlayer = scene->addRect(0, 0, 40, 40, QPen(Qt::NoPen), QBrush(Qt::blue));

    if (game != nullptr && game->getPlayer() != nullptr) {
        float startX = game->getPlayer()->getPosition().x();
        float startY = game->getPlayer()->getPosition().y();
        visualPlayer->setPos(startX, startY);
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGameLoop);
    timer->start(16);
}

MainWindow::~MainWindow()
{

    delete game;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!game || !game->getPlayer()) return;

    if (event->key() == Qt::Key_Right) {
        game->getPlayer()->setVelocity(QVector2D(200.0f, 0.0f));
    }
    else if (event->key() == Qt::Key_Left) {
        game->getPlayer()->setVelocity(QVector2D(-200.0f, 0.0f));
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!game || !game->getPlayer()) return;

    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Left) {
        game->getPlayer()->setVelocity(QVector2D(0.0f, 0.0f));
    }
}

void MainWindow::updateGameLoop()
{
    if (!game || !game->getPlayer()) return;

    game->update(0.016f);

    float newX = game->getPlayer()->getPosition().x();
    float newY = game->getPlayer()->getPosition().y();
    visualPlayer->setPos(newX, newY);
}
