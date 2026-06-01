#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <vector>
#include "game.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Game* game;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QTimer* timer;

    QGraphicsItem* visualPlayer;
    QGraphicsTextItem* hudText;
    QGraphicsTextItem* gameOverText;

    std::vector<QGraphicsItem*> visualEntities;
    std::vector<QGraphicsItem*> visualItems;

    QGraphicsTextItem* menuTitleText;  // Textos del menú de inicio
    QGraphicsTextItem* menuInfoText;

    void createVisualEntities();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGameLoop();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
