#include <QApplication>
#include <QDebug>
#include "core/game.h"
#include "entities/player.h"

int main(int argc, char *argv[])
{
    qDebug() << "test olision";

    Game game;
    game.loadLevel(); // sierra en X = 10

    game.getPlayer()->setVelocity(QVector2D(5.0f, 0.0f));
    qDebug() << "Crash inicia en X:" << game.getPlayer()->getPosition().x() << "| Vidas:" << game.getPlayer()->getLives();

    // frame 1
    qDebug() << "\n[frame 1]";
    game.update(1.0f);
    qDebug() << "Crash Pos X:" << game.getPlayer()->getPosition().x() << "| Vidas:" << game.getPlayer()->getLives();

    // olision
    qDebug() << "\n[frame 2]";
    game.update(1.0f);
    qDebug() << "Crash Pos X:" << game.getPlayer()->getPosition().x() << "| Vidas:" << game.getPlayer()->getLives();

    qDebug() << "\nMelo, Gracias a Lucho como siempre.";
    return 0;
}
