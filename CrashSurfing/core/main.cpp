#include <QApplication>
#include <QDebug>
#include "core/game.h"
#include "entities/player.h"
#include "entities/obstacle.h"
#include "entities/enemy.h"

int main(int argc, char *argv[])
{
    qDebug() << "Test";

    Game game;
    qDebug() << "Juego iniciado. Puntaje inicial:" << game.getScore();

    // Vel a crash
    game.getPlayer()->setVelocity(QVector2D(5.0f, 0.0f));

    // otras entidades
    Obstacle* sierra = new Obstacle();
    sierra->setVelocity(QVector2D(-2.0f, 0.0f));
    game.addEntity(sierra);

    Enemy* dron = new Enemy();
    dron->setVelocity(QVector2D(0.0f, 3.0f));
    game.addEntity(dron);

    qDebug() << "Entidades in Game.";
    qDebug() << "Testeando 1 frame";

    game.update(1.0f);

    // Se movio crash?
    qDebug() << "Nueva Posicion Crash -> X:" << game.getPlayer()->getPosition().x();

    qDebug() << "Funciona gracias a Lucho como siempre.";

    return 0;
}
