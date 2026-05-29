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

    //
    scene = new QGraphicsScene(0, 0, 5000, 600, this);
    scene->setBackgroundBrush(Qt::black);

    view = new QGraphicsView(scene, this);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(view);

    setFixedSize(1024, 600);
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

    Player* player = dynamic_cast<Player*>(game->getPlayer());
    if (!player) return;

    // Parabola salto
    else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) {
        player->jump();
    }

    // pesado (test)
    else if (event->key() == Qt::Key_C) {
        player->collectItem();
        if (player->getIsGlutton()) {
            qDebug() << "Pesado";
        }
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

    // Actualizar
    float newX = game->getPlayer()->getPosition().x();
    float newY = game->getPlayer()->getPosition().y();

    visualPlayer->setPos(newX, newY);
    view->centerOn(newX + 450, 300);

}
