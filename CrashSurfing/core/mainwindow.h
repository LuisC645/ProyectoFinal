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
#include "gamestate.h"
#include "audiomanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Game* game;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QTimer* timer;

    std::vector<QGraphicsItem*> visualEntities;
    std::vector<QGraphicsItem*> visualItems;
    QGraphicsPixmapItem* visualPlayer;
    QGraphicsPixmapItem* visualEnemy1;

    QGraphicsRectItem* enemyDebugRect;

    AudioManager* audio;

    QPixmap pixPlayerRun;
    QPixmap pixPlayerJump;
    QPixmap pixPlayerGameOver;

    QPixmap pixEnemy1;

    QPixmap pixObstacleLog;
    QPixmap pixObstacleRock;
    QPixmap pixObstacleSaw;
    QPixmap pixFruit;

    QGraphicsTextItem* hudText;

    QGraphicsRectItem* physicsHitboxDebug;
    QVector<QGraphicsRectItem*> obstaclePhysicsDebug;
    QVector<QGraphicsRectItem*> fruitPhysicsDebug;

    LevelType selectedLevel = LevelType::LEVEL_1;
    Difficulty selectedDifficulty = Difficulty::EASY;
    QGraphicsTextItem* menuSelectionText;

    void createVisualEntities();
    void clearVisualEntities();

    // Contenedores gráficos para las capas de fondo
    QGraphicsRectItem* bgLayer1; // Capa 1: Mas lejana (Cielo y Nubes)
    QGraphicsRectItem* bgLayer2; // Capa 2: Intermedia (Paisaje y Montañas)
    QGraphicsRectItem* bgLayer3; // Capa 3: Superficie del río (Suelo)

    // Dimensiones de ancho de las imágenes para el cálculo de envoltura (wrapping)
    int bgLayer1Width;
    int bgLayer2Width;
    int bgLayer3Width;

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
