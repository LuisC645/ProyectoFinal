#include <QApplication>
#include <QDebug>
#include "entities/player.h"

int main(int argc, char *argv[])
{
    qDebug() << "test";

    Player crash;

    qDebug() << "Vidas init:" << crash.getLives();
    qDebug() << "Posicion init:" << crash.getPosition().x() << "," << crash.getPosition().y();

    crash.setVelocity(QVector2D(10.0f, 5.0f));
    qDebug() << "Vel:" << crash.getVelocity().x() << "," << crash.getVelocity().y();

    qDebug() << "Sim - 3 movimientos";

    float deltaTime = 1.0f; // paso 1s

    for (unsigned short i = 1; i <= 3; i++) {
        crash.update(deltaTime);

        qDebug() << "Frame" << i << "- Nueva Posicion -> X:" << crash.getPosition().x() << " Y:" << crash.getPosition().y();
    }

    qDebug() << "Funciona gracias a Lucho como siempre";

    return 0;
}
