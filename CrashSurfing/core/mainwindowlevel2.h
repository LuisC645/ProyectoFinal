#ifndef MAINWINDOWLEVEL2_H
#define MAINWINDOWLEVEL2_H

#include <QMainWindow>

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
    void setupHud();
    void setupStateText();
    void setupTimer();
    void updatePlayerVisual();
    void updateHud();
    void updateStateText();

private:
    GameLevel2 *game = nullptr;

    QGraphicsScene *scene = nullptr;
    QGraphicsView *view = nullptr;

    QTimer *timer = nullptr;

    // Crash
    QGraphicsRectItem *playerVisual = nullptr;

    // Remolino
    QGraphicsEllipseItem *whirlpoolVisual = nullptr;

    // Centro
    QGraphicsEllipseItem *deathZoneVisual = nullptr;

    // HUD
    QGraphicsTextItem *hudText = nullptr;

    // Mensajes
    QGraphicsTextItem *stateText = nullptr;
};

#endif
