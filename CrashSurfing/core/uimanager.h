#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QFont>

#include "gamestate.h"

class UIManager
{
private:

    QGraphicsScene* scene;

    QGraphicsTextItem* menuTitleText;
    QGraphicsTextItem* menuLevelText;
    QGraphicsTextItem* menuDifficultyText;
    QGraphicsTextItem* menuInfoText;

    QGraphicsPixmapItem* menuBackground;
    QGraphicsTextItem* pauseText;
    QGraphicsRectItem* overlay;

    QGraphicsTextItem* gameOverText;
    QGraphicsTextItem* levelCompleteText;

public:

    UIManager(QGraphicsScene* scene);

    void showMenu(LevelType level, Difficulty difficulty);
    void hideMenu();

    void showPause(float playerX);
    void hidePause();

    void showGameOver(float playerX);
    void hideGameOver();

    void showLevelComplete(float playerX);
    void hideLevelComplete();

    void hideAll();
};

#endif
