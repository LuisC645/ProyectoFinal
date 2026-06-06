#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <vector>

// LVL2
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

#include "game.h"
#include "gamestate.h"
#include "audiomanager.h"
#include "gameLevel2.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Game* game;
    GameLevel2* gameLevel2;

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
    QPixmap pixTronco;
    QPixmap pixGameOver2;

    QGraphicsTextItem* hudText;

    QGraphicsRectItem* physicsHitboxDebug;

    QVector<QGraphicsRectItem*> obstaclePhysicsDebug;
    QVector<QGraphicsRectItem*> fruitPhysicsDebug;

    LevelType selectedLevel = LevelType::LEVEL_1;
    Difficulty selectedDifficulty = Difficulty::EASY;

    QGraphicsTextItem* menuSelectionText;

    // Crea sprites nivel 1
    void createVisualEntities();

    // Libera sprites nivel 1
    void clearVisualEntities();

    // Actualiza fondos
    void updateBackgrounds();

    // Fondos nivel 1
    QGraphicsRectItem* bgLayer1;
    QGraphicsRectItem* bgLayer2;
    QGraphicsRectItem* bgLayer3;

    // Anchos de fondos
    unsigned short bgLayer1Width;
    unsigned short bgLayer2Width;
    unsigned short bgLayer3Width;

    // Libera todos los sprites
    void clearAllVisuals();

    // Carga nivel 1
    void loadLevel1();

    // Carga nivel 2
    void loadLevel2();

    // Nivel 2
    QGraphicsPixmapItem* visualPlayer2;
    QPixmap pixPlayerlvl2;

    QGraphicsEllipseItem* whirlpoolVisual;
    QGraphicsEllipseItem* deathZoneVisual;

    QPixmap pixWhirlpool;

    // Crea sprites nivel 2
    void createLevel2Visuals();

    // Libera sprites nivel 2
    void clearLevel2Visuals();

    bool level2VisualsCreated = false;

    std::vector<QGraphicsPixmapItem*> visualLevel2Fruits;
    std::vector<QGraphicsPixmapItem*> visualLevel2Obstacles;

    QGraphicsRectItem* player2HitboxDebug;

    QVector<QGraphicsRectItem*> level2FruitHitboxes;
    QVector<QGraphicsRectItem*> level2ObstacleHitboxes;

    QGraphicsRectItem* bgLevel2;

    unsigned short bgLevel2Width;

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private slots:

    // Actualiza bucle principal
    void updateGameLoop();

    // Actualiza nivel 1
    void updateLevel1();

    // Actualiza nivel 2
    void updateLevel2();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
