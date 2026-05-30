#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QKeyEvent>
#include <vector>

#include "core/game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGameLoop();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QGraphicsView* view;
    Game* game;

    QGraphicsRectItem* visualPlayer;
    QTimer* timer;

    std::vector<QGraphicsRectItem*> visualItems;
};
#endif // MAINWINDOW_H
