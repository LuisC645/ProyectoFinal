#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "ok 1";

    MainWindow w;
    qDebug() << "ok 2";

    w.show();
    qDebug() << "ok 3";

    return a.exec();
}
