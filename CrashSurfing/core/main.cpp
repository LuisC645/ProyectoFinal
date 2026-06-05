#include <QApplication>

#include "mainwindowlevel2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindowLevel2 w;

    w.show();

    return a.exec();
}
