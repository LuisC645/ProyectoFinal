#ifndef MAINWINDOWLEVEL2_H
#define MAINWINDOWLEVEL2_H

#include <QMainWindow>
#include <vector>

class QGraphicsScene;
class QGraphicsView;
class QGraphicsRectItem;
class QGraphicsEllipseItem;
class QGraphicsTextItem;
class QTimer;
class QKeyEvent;

#include "../core/gamelevel2.h"

class MainWindowLevel2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowLevel2(QWidget *parent = nullptr);
    ~MainWindowLevel2() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGameLoop();

private:
    void setupWindow();
    void setupScene();
    void setupView();
    void setupWhirlpool();
    void setupDeathZone();
    void setupPlayer();
    void setupEnemy2();
    void setupFruits();
    void setupObstacles();
    void setupHud();
    void setupStateText();
    void setupTimer();

    void rebuildProjectilesIfNeeded();

    void updatePlayerVisual();
    void updateEnemy2Visual();
    void updateFruitVisuals();
    void updateObstacleVisuals();
    void updateProjectileVisuals();
    void updateHud();
    void updateStateText();

private:
    GameLevel2 *game = nullptr;

    QGraphicsScene *scene = nullptr;
    QGraphicsView *view = nullptr;
    QTimer *timer = nullptr;

    QGraphicsRectItem *playerVisual = nullptr;
    QGraphicsRectItem *enemy2Visual = nullptr;

    QGraphicsEllipseItem *whirlpoolVisual = nullptr;
    QGraphicsEllipseItem *deathZoneVisual = nullptr;

    QGraphicsTextItem *hudText = nullptr;
    QGraphicsTextItem *stateText = nullptr;

    std::vector<QGraphicsEllipseItem*> fruitVisuals;
    std::vector<QGraphicsRectItem*> obstacleVisuals;
    std::vector<QGraphicsRectItem*> projectileVisuals;
};

#endif
