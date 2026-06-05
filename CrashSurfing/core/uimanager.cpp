#include "uimanager.h"

UIManager::UIManager(QGraphicsScene* scene)
{
    this->scene = scene;

    menuBackground = new QGraphicsPixmapItem();

    QPixmap menuPix(":/new/prefix1/resources/Menu.png");
    menuBackground->setPixmap(menuPix);
    menuBackground->setPixmap(menuPix.scaled(1600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation ));

    if (menuPix.isNull()){
        qDebug() << "Error con menu png";
    }

    scene->addItem(menuBackground);
    menuBackground->setZValue(100);

    QFont menuFont;
    menuFont.setPointSize(18);
    menuFont.setBold(true);

    menuLevelText = new QGraphicsTextItem();
    menuDifficultyText = new QGraphicsTextItem();

    menuLevelText->setFont(menuFont);
    menuDifficultyText->setFont(menuFont);

    menuLevelText = new QGraphicsTextItem();
    scene->addItem(menuLevelText);

    menuDifficultyText = new QGraphicsTextItem();
    scene->addItem(menuDifficultyText);

    pauseText = new QGraphicsTextItem( "PAUSA\n\n"  "ESC -> CONTINUAR\n" "R -> REINICIAR\n" "M -> MENU" );
    pauseText -> setDefaultTextColor(Qt::yellow);
    pauseText -> setZValue(60);

    scene->addItem(pauseText);
    gameOverText = new QGraphicsTextItem("GAME OVER\n\n" "R -> REINICIAR");
    gameOverText -> setDefaultTextColor(Qt::red);
    scene->addItem(gameOverText);

    levelCompleteText =  new QGraphicsTextItem( "NIVEL COMPLETADO\n\n" "R -> REINTENTAR");
    levelCompleteText -> setDefaultTextColor(Qt::green);
    scene->addItem(levelCompleteText);

    overlay = scene->addRect(0, 0, 1600, 600, Qt::NoPen, QBrush(QColor(0,0,0,160)));
    overlay -> setZValue(50);
    overlay -> setVisible(false);

    hideAll();
}

void UIManager::showMenu(LevelType level, Difficulty difficulty)
{
    QFont selectorFont;
    selectorFont.setPointSize(45);
    selectorFont.setBold(true);

    menuLevelText -> setFont(selectorFont);
    menuDifficultyText -> setFont(selectorFont);

    menuBackground->setPos(0, 0);
    menuBackground->setVisible(true);

    menuLevelText->setPos(275, 30);
    menuLevelText->setZValue(110);

    menuDifficultyText->setPos(1090, 30);
    menuDifficultyText->setZValue(110);

    menuLevelText->setPlainText(level == LevelType::LEVEL_1 ? "\n\n\n>       <\n " : "\n\n\n\n>      <");
    menuDifficultyText->setPlainText(difficulty == Difficulty::EASY ? "\n\n\n>        <\n " : "\n\n\n\n>        <");

    menuLevelText->setVisible(true);
    menuDifficultyText->setVisible(true);
}

void UIManager::showPause(float playerX)
{
    overlay -> setRect(playerX - 200, 0, 1600, 600);
    overlay -> setVisible(true);
    overlay -> setZValue(50);

    QFont selectorFontPause;
    selectorFontPause.setPointSize(30);
    selectorFontPause.setBold(true);

    pauseText -> setFont(selectorFontPause);
    pauseText -> setPos(playerX + 400, 180);
    pauseText -> setVisible(true);
    pauseText -> setZValue(60);
}

void UIManager::showGameOver(float playerX)
{
    overlay -> setRect(playerX - 200, 0, 1600, 600);
    overlay -> setVisible(true);
    overlay -> setZValue(50);

    QFont selectorFontOver;
    selectorFontOver.setPointSize(30);
    selectorFontOver.setBold(true);

    gameOverText -> setFont(selectorFontOver);
    gameOverText -> setPos(playerX + 150, 180);
    gameOverText -> setVisible(true);
    gameOverText -> setZValue(60);
}

void UIManager::showLevelComplete(float playerX)
{
    overlay -> setRect(playerX - 200, 0, 1600, 600);
    overlay -> setVisible(true);
    overlay -> setZValue(50);

    QFont selectorFontWin;
    selectorFontWin.setPointSize(30);
    selectorFontWin.setBold(true);

    levelCompleteText -> setFont(selectorFontWin);
    levelCompleteText -> setPos(playerX + 150, 180);
    levelCompleteText -> setVisible(true);
    levelCompleteText -> setVisible(true);
    levelCompleteText -> setZValue(60);
}

void UIManager::hideMenu()
{
    menuBackground->setVisible(false);
    menuLevelText->setVisible(false);
    menuDifficultyText->setVisible(false);
}

void UIManager::hidePause()
{
    overlay->setVisible(false);
    pauseText->setVisible(false);
}

void UIManager::hideGameOver()
{
    overlay->setVisible(false);
    gameOverText->setVisible(false);
}

void UIManager::hideLevelComplete()
{
    levelCompleteText->setVisible(false);
}

void UIManager::hideAll()
{
    hideMenu();
    hidePause();
    hideGameOver();
    hideLevelComplete();
}
