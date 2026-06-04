#include "mainwindow.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QFont>
#include <cmath>

#include "../entities/obstacle.h"
#include "../entities/player.h"
#include "../entities/item.h"
#include "../entities/enemy.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // assets
    // Crash
    pixPlayerRun = QPixmap(":/new/prefix1/resources/crash_init.png");
    pixPlayerJump = QPixmap(":/new/prefix1/resources/crash_jump.png");
    pixPlayerGameOver = QPixmap(":/new/prefix1/resources/crash_gameOver.png");

    if (pixPlayerRun.isNull() || pixPlayerJump.isNull() || pixPlayerGameOver.isNull()) {
        qDebug() << "Error con sprites crash";
    }

    // Obstaculos
    pixObstacleLog = QPixmap(":/new/prefix1/resources/TNT.png");
    pixObstacleRock = QPixmap(":/new/prefix1/resources/Rock.png");
    pixObstacleSaw = QPixmap(":/new/prefix1/resources/floating.png");

    if (pixObstacleLog.isNull() || pixObstacleRock.isNull() || pixObstacleSaw.isNull()) {
        qDebug() << "Error con sprites obs";
    }

    pixFruit = QPixmap(":/new/prefix1/resources/fruit.png");

    if (pixFruit.isNull()) {
        qDebug() << "Error con sprites fruta";
    }


    // 1. INICIALIZAR BACKEND
    game = new Game();
    game->loadLevel();

    gameOverText = nullptr;
    menuTitleText = nullptr;
    menuInfoText = nullptr;

    // 2. CONFIGURAR ESCENA Y VISTA
    scene = new QGraphicsScene(0, 0, 30000, 600, this);
    scene->setBackgroundBrush(Qt::black);

    // Hitbox crash
    physicsHitboxDebug = scene->addRect(0, 0, 30, 50, QPen(Qt::yellow, 3));
    physicsHitboxDebug->setBrush(QColor(255,255,0,60));
    physicsHitboxDebug->setZValue(500);

    enemyDebugRect = scene->addRect(0,0,60,100,QPen(Qt::red,3));
    enemyDebugRect->setBrush(QColor(255,0,0,80));
    enemyDebugRect->setZValue(1000);

    // Debug piso
    //QGraphicsLineItem* floorDebug = scene->addLine(0, 400, 30000, 350, QPen(Qt::green, 3));
    //floorDebug->setZValue(100);

    // Assets fondo
    QPixmap pixFar(":/new/prefix1/resources/bg1.png");
    QPixmap pixClose(":/new/prefix1/resources/bg3.png");

    // Determinar anchos reales de las imagenes
    bgLayer1Width = pixFar.isNull() ? 1600 : pixFar.width();
    bgLayer3Width = pixClose.isNull() ? 600 : pixClose.width();

    // Capas
    bgLayer1 = new QGraphicsRectItem(0, 0, bgLayer1Width * 2, 600);
    bgLayer1->setBrush(QBrush(pixFar));
    bgLayer1->setPen(QPen(Qt::NoPen));
    bgLayer1->setZValue(-30);
    scene->addItem(bgLayer1);
    bgLayer1->setPos(0, -200);

    bgLayer3 = new QGraphicsRectItem(0, 0, bgLayer3Width * 2, 600);
    bgLayer3->setBrush(QBrush(pixClose));
    bgLayer3->setPen(QPen(Qt::NoPen));
    bgLayer3->setZValue(-10);
    scene->addItem(bgLayer3);

    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);

    setCentralWidget(view);
    setFixedSize(1600, 600);
    setWindowTitle("Crash Surfing");

    // 3. CREAR JUGADOR
    visualPlayer = new QGraphicsPixmapItem();
    visualPlayer->setPixmap(pixPlayerRun.scaled(180, 180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    visualPlayer->setZValue(10);
    scene->addItem(visualPlayer);

    float startX = 0.0f;
    float startY = 0.0f;

    if (game != nullptr && game->getPlayer() != nullptr) {

        Player* playerBackend = dynamic_cast<Player*>(game->getPlayer());

        if (playerBackend) {
            startX = playerBackend->getPosition().x();
            startY = playerBackend->getPosition().y();
        }

        float alignedStartX = startX - 70.0f;
        float alignedStartY = startY - 80.0f;
        visualPlayer->setPos(alignedStartX, alignedStartY);
    }

    // 4.
    hudText = scene->addText("VIDAS: 3  |  WUMPAS: 0");
    hudText->setDefaultTextColor(Qt::white);
    hudText->setFont(QFont("Arial", 16, QFont::Bold));
    hudText->setZValue(10);
    hudText->setVisible(false);

    menuTitleText = scene->addText("CRASH SURFING");
    menuTitleText->setDefaultTextColor(QColor(255, 102, 0));
    menuTitleText->setFont(QFont("Impact", 55, QFont::Bold));
    menuTitleText->setZValue(11);
    menuTitleText->setPos(startX + 180, 160);

    menuInfoText = scene->addText("PRESIONA 'ENTER' PARA COMENZAR");
    menuInfoText->setDefaultTextColor(Qt::white);
    menuInfoText->setFont(QFont("Arial", 16, QFont::Bold));
    menuInfoText->setZValue(11);
    menuInfoText->setPos(startX + 210, 270);

    // 5. HAcer el nivel
    createVisualEntities();

    // 6. INICIAR EL BUCLE DEL JUEGO
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGameLoop);
    timer->start(16);
}

MainWindow::~MainWindow()
{
    delete game;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!game) return;

    GameStatus currentStatus = game->getStatus();

    if (currentStatus == GameStatus::MENU) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            game->setStatus(GameStatus::PLAYING);

            if (menuTitleText) menuTitleText->setVisible(false);
            if (menuInfoText) menuInfoText->setVisible(false);
            if (hudText) hudText->setVisible(true);
        }
        return;
    }

    if (currentStatus == GameStatus::GAME_OVER || currentStatus == GameStatus::LEVEL_COMPLETE) {
        if (event->key() == Qt::Key_R) {

            if (gameOverText) {
                scene->removeItem(gameOverText);
                delete gameOverText;
                gameOverText = nullptr;
            }

            clearVisualEntities();
            game->reset();
            createVisualEntities();

            if (visualPlayer) {
                visualPlayer->setVisible(true);
                if (game->getPlayer()) {
                    visualPlayer->setPos(game->getPlayer()->getPosition().x(),
                                         game->getPlayer()->getPosition().y());
                }
            }

            float playerX = game->getPlayer()->getPosition().x();
            if (menuTitleText) { menuTitleText->setPos(playerX + 180, 160); menuTitleText->setVisible(true); }
            if (menuInfoText) { menuInfoText->setPos(playerX + 210, 270); menuInfoText->setVisible(true); }
            if (hudText) hudText->setVisible(false);
        }
        return;
    }

    if (currentStatus == GameStatus::PLAYING) {
        Player* player = dynamic_cast<Player*>(game->getPlayer());
        if (!player) return;

        if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) {
            player->jump();
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::updateGameLoop()
{
    if (!game || !game->getPlayer()) return;

    Player* p = dynamic_cast<Player*>(game->getPlayer());
    if (!p) return;

    GameStatus currentStatus = game->getStatus();

    // 1. Actualizar físicas del Backend
    game->update(0.016f);

    if (currentStatus == GameStatus::PLAYING && p->getCollectedFruits() >= 30) {
        game->setStatus(GameStatus::LEVEL_COMPLETE);
        currentStatus = GameStatus::LEVEL_COMPLETE;
    }

    Enemy* enemy = game->getEnemy();

    if(enemy)
    {
        enemyDebugRect->setRect(
            enemy->getPosition().x(),
            enemy->getPosition().y(),
            enemy->getWidth(),
            enemy->getHeight()
            );

        enemyDebugRect->setVisible(
            enemy->isActive()
            );
    }

    // 2. Control de Estados del Menú y Fin de Juego
    if (currentStatus == GameStatus::MENU) {
        view->centerOn(p->getPosition().x() + 400, 300);
        return;
    }

    if (currentStatus == GameStatus::LEVEL_COMPLETE) {
        if (!gameOverText) {
            gameOverText = new QGraphicsTextItem("¡NIVEL COMPLETADO!\nPresiona 'R' para jugar de nuevo");
            gameOverText->setDefaultTextColor(Qt::green);
            QFont font = gameOverText->font(); font.setPointSize(30); font.setBold(true);
            gameOverText->setFont(font); scene->addItem(gameOverText);
        }
        gameOverText->setPos(p->getPosition().x() + 150, 180);
        gameOverText->setVisible(true);
        return;
    }

    if (currentStatus == GameStatus::GAME_OVER) {
        if (!gameOverText) {
            gameOverText = new QGraphicsTextItem("GAME OVER\nPresiona 'R' para reiniciar");
            gameOverText->setDefaultTextColor(Qt::red);
            QFont font = gameOverText->font(); font.setPointSize(30); font.setBold(true);
            gameOverText->setFont(font); scene->addItem(gameOverText);
        }
        gameOverText->setPos(p->getPosition().x() + 200, 180);
        gameOverText->setVisible(true);
    }

    // 3. RUNTIME: CAMARA Y PARALLAX
    float currentCrashX = p->getPosition().x();
    float currentCrashY = p->getPosition().y() + 130.0f;
    float cameraLeft = currentCrashX - 200.0f;

    float pW = p->getWidth();
    float pH = p->getHeight();

    physicsHitboxDebug->setRect(currentCrashX - (pW * 0.5f), currentCrashY - pH + 25.0f, pW, pH);

    float offset1 = fmod(cameraLeft * 0.15f, bgLayer1Width);
    bgLayer1->setPos(cameraLeft - offset1, -200);

    float offset3 = fmod(cameraLeft, bgLayer3Width);
    bgLayer3->setPos(cameraLeft - offset3, 50);

    // 4. INTERFAZ (HUD)
    hudText->setPlainText(QString("VIDAS: %1   |   WUMPAS: %2") .arg(p->getLives()) .arg(p->getCollectedFruits()));
    hudText->setPos(cameraLeft + 50, 30.0f);

    // 5. ACTUALIZACIÓN VISUAL DE CRASH
    QGraphicsPixmapItem* pixmapPlayer = static_cast<QGraphicsPixmapItem*>(visualPlayer);
    if (pixmapPlayer) {
        float alignedX = 0.0f;
        float alignedY = 0.0f;

        if (currentStatus == GameStatus::GAME_OVER) {
            pixmapPlayer->setPixmap(pixPlayerGameOver.scaled(220, 160, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            pixmapPlayer->setOpacity(1.0);

            alignedX = currentCrashX - 60.0f; // Sprites correccion
            alignedY = currentCrashY - 40.0f;
        }
        else if (!p->getIsGrounded()) {
            pixmapPlayer->setPixmap(pixPlayerJump.scaled(220, 160, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            pixmapPlayer->setOpacity(p->getIsGlutton() ? 0.6 : 1.0);

            alignedX = currentCrashX - 90.0f;
            alignedY = currentCrashY - 60.0f;
        }
        else {
            pixmapPlayer->setPixmap(pixPlayerRun.scaled(180, 180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            pixmapPlayer->setOpacity(p->getIsGlutton() ? 0.6 : 1.0);

            alignedX = currentCrashX - 90.0f;
            alignedY = currentCrashY - 80.0f;
        }

        pixmapPlayer->setPos(alignedX, alignedY);
    }

    // 6. EFECTO INVENCIBILIDAD (PARPADEO)
    if (p->getIsInvincible() && currentStatus != GameStatus::GAME_OVER) {
        static int frameCounter = 0;
        frameCounter++;
        if (frameCounter % 6 == 0) {
            if (visualPlayer) visualPlayer->setVisible(!visualPlayer->isVisible());
        }
    } else {
        if (visualPlayer) visualPlayer->setVisible(true);
    }

    // =====================================================
    // ACTUALIZACIÓN VISUAL DE FRUTAS
    // =====================================================

    while(visualEntities.size() < game->getEntities().size())
    {
        Entity* entity = game->getEntities()[visualEntities.size()];

        Obstacle* obs = dynamic_cast<Obstacle*>(entity);

        if(!obs){ break; }

        QString tipo = obs->getType().toLower();

        QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem();

        float spriteW = 130.0f;
        float spriteH = 120.0f;

        QPixmap texture;

        if(tipo == "saw")
        {
            texture = pixObstacleSaw;
            spriteW = 120.0f;
            spriteH = 100.0f;
        }
        else if(tipo == "log")
        {
            texture = pixObstacleLog;
            spriteW = 130.0f;
            spriteH = 120.0f;
        }
        else if(tipo == "floating")
        {
            texture = pixObstacleRock;
            spriteW = 100.0f;
            spriteH = 67.0f;
        }

        float hitboxW = entity->getWidth();
        float hitboxH = entity->getHeight();

        float offsetX = (spriteW - hitboxW) * 0.5f;
        float offsetY = spriteH - hitboxH;

        sprite->setPixmap(texture.scaled( spriteW, spriteH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        sprite->setData(1, offsetX);
        sprite->setData(2, offsetY);
        sprite->setZValue(5);

        scene->addItem(sprite);

        visualEntities.push_back(sprite);

        QGraphicsRectItem* physicsBox = scene->addRect( 0, 0, hitboxW, hitboxH, QPen(Qt::cyan,2));
        physicsBox->setBrush(QColor(0,255,255,40));
        physicsBox->setZValue(500);

        obstaclePhysicsDebug.push_back(physicsBox);
    }

    const auto& backendItems = game->getItems();

    for (size_t i = 0;
         i < backendItems.size() &&
         i < visualItems.size();
         ++i)
    {
        if (!visualItems[i])
            continue;

        if (backendItems[i]->getIsCollected() ||
            !backendItems[i]->isActive())
        {
            visualItems[i]->setVisible(false);

            if(i < fruitPhysicsDebug.size())
                fruitPhysicsDebug[i]->setVisible(false);

            continue;
        }

        float itemX =
            backendItems[i]->getPosition().x();

        float itemY =
            backendItems[i]->getPosition().y() + 135.0f;

        // ====================================
        // HITBOX REAL
        // ====================================

        if(i < fruitPhysicsDebug.size())
        {
            fruitPhysicsDebug[i]->setRect(
                itemX,
                itemY,
                backendItems[i]->getWidth(),
                backendItems[i]->getHeight()
                );

            fruitPhysicsDebug[i]->setVisible(true);
        }

        QGraphicsPixmapItem* fruitSprite =
            dynamic_cast<QGraphicsPixmapItem*>(
                visualItems[i]
                );

        if(fruitSprite)
        {
            float offsetX =
                fruitSprite->data(1).toFloat();

            float offsetY =
                fruitSprite->data(2).toFloat();

            fruitSprite->setPos(
                itemX - offsetX,
                itemY - offsetY
                );
        }

        visualItems[i]->setVisible(true);
    }

    // 8. ACTUALIZACIÓN VISUAL DE OBSTÁCULOS
    const auto& backendEntities = game->getEntities();
    for (size_t i = 0;
         i < backendEntities.size() &&
         i < visualEntities.size();
         ++i)
    {
        if (!visualEntities[i])
            continue;

        if (!backendEntities[i]->isActive())
        {
            visualEntities[i]->setVisible(false);

            if(i < obstaclePhysicsDebug.size())
                obstaclePhysicsDebug[i]->setVisible(false);

            continue;
        }

        float entX = backendEntities[i]->getPosition().x();
        float entY = backendEntities[i]->getPosition().y() + 130.0f;

        // HITBOX FÍSICA REAL (CYAN)

        if(i < obstaclePhysicsDebug.size())
        {
            obstaclePhysicsDebug[i]->setRect(
                entX,
                entY,
                backendEntities[i]->getWidth(),
                backendEntities[i]->getHeight()
                );

            obstaclePhysicsDebug[i]->setVisible(true);
        }

        Obstacle* obs = dynamic_cast<Obstacle*>(backendEntities[i]);
        QGraphicsPixmapItem* pixmapEnt = dynamic_cast<QGraphicsPixmapItem*>(visualEntities[i]);

        if (obs && (obs->getType().toLower() == "log")) {

            if (pixmapEnt) {
                float offsetX = pixmapEnt->data(1).toFloat();
                float offsetY = pixmapEnt->data(2).toFloat();

                float alignedX = entX - offsetX;
                float alignedY = entY - offsetY + 35.0f;

                pixmapEnt->setPos(alignedX, alignedY);
            }
        }
        else if(obs && (obs->getType().toLower() == "saw")){

            if (pixmapEnt) {
                float offsetX = pixmapEnt->data(1).toFloat();
                float offsetY = pixmapEnt->data(2).toFloat();

                float alignedX = entX - offsetX + 0.0f;
                float alignedY = entY - offsetY + 47.0f;

                pixmapEnt->setPos(alignedX, alignedY);
            }
        }
        else if(obs && (obs->getType().toLower() == "floating")){

            if (pixmapEnt) {
                float offsetX = pixmapEnt->data(1).toFloat();
                float offsetY = pixmapEnt->data(2).toFloat();

                float alignedX = entX - offsetX + 0.0f;
                float alignedY = entY - offsetY + 15.0f;

                pixmapEnt->setPos(alignedX, alignedY);

                pixmapEnt->setPos(alignedX, alignedY);

                pixmapEnt->setTransformOriginPoint(
                    pixmapEnt->boundingRect().center()
                    );

                pixmapEnt->setRotation(
                    pixmapEnt->rotation() + 10.0f
                    );
            }
        }
        visualEntities[i]->setVisible(true);
    }

    // 9. CENTRAR CÁMARA
    view->centerOn(cameraLeft + 800, 300);
}

void MainWindow::createVisualEntities()
{
    visualEntities.clear();
    visualItems.clear();
    obstaclePhysicsDebug.clear();
    fruitPhysicsDebug.clear();

    const auto& backendEntities = game->getEntities();

    for(Entity* entity : backendEntities)
    {
        Obstacle* obs = dynamic_cast<Obstacle*>(entity);
        if(!obs) continue;

        QString tipo = obs->getType().toLower();

        QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem();

        float spriteW = 130.0f;
        float spriteH = 120.0f;

        QPixmap texture;

        if(tipo == "floating")
        {
            texture = pixObstacleSaw;

            spriteW = 105.0f;
            spriteH = 70.0f;
        }
        else if(tipo == "log")
        {
            texture = pixObstacleLog;

            spriteW = 130.0f;
            spriteH = 120.0f;
        }
        else if(tipo == "saw")
        {
            texture = pixObstacleRock;

            spriteW = 100.0f;
            spriteH = 67.0f;
        }

        float hitboxW = entity->getWidth();
        float hitboxH = entity->getHeight();

        float offsetX = (spriteW - hitboxW) * 0.5f;
        float offsetY = spriteH - hitboxH;

        sprite->setPixmap(
            texture.scaled(
                spriteW,
                spriteH,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation
                )
            );

        sprite->setData(1, offsetX);
        sprite->setData(2, offsetY);
        sprite->setZValue(5);

        scene->addItem(sprite);
        visualEntities.push_back(sprite);

        QGraphicsRectItem* physicsBox =
            scene->addRect(
                0,
                0,
                hitboxW,
                hitboxH,
                QPen(Qt::cyan, 2)
                );

        physicsBox->setBrush(QColor(0,255,255,40));
        physicsBox->setZValue(500);

        obstaclePhysicsDebug.push_back(physicsBox);
    }

    const auto& backendItems = game->getItems();

    for(Item* item : backendItems)
    {
        if(!item) continue;

        QGraphicsPixmapItem* fruitSprite = new QGraphicsPixmapItem();

        float spriteW = 110.0f;
        float spriteH = 100.0f;

        float hitboxW = item->getWidth();
        float hitboxH = item->getHeight();

        float offsetX = (spriteW - hitboxW) * 0.5f;
        float offsetY = (spriteH - hitboxH) * 0.5f;

        fruitSprite->setPixmap(
            pixFruit.scaled(
                spriteW,
                spriteH,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation
                )
            );

        fruitSprite->setData(1, offsetX);
        fruitSprite->setData(2, offsetY);
        fruitSprite->setZValue(8);

        scene->addItem(fruitSprite);
        visualItems.push_back(fruitSprite);

        QGraphicsRectItem* physicsBox =
            scene->addRect(
                0,
                0,
                hitboxW,
                hitboxH,
                QPen(Qt::magenta, 2)
                );

        physicsBox->setBrush(QColor(255,0,255,40));
        physicsBox->setZValue(700);

        fruitPhysicsDebug.push_back(physicsBox);
    }
}
void MainWindow::clearVisualEntities()
{
    for(auto item : visualEntities)
    {
        scene->removeItem(item);
        delete item;
    }

    for(auto item : visualItems)
    {
        scene->removeItem(item);
        delete item;
    }

    for(auto item : obstaclePhysicsDebug)
    {
        scene->removeItem(item);
        delete item;
    }

    for(auto item : fruitPhysicsDebug)
    {
        scene->removeItem(item);
        delete item;
    }

    visualEntities.clear();
    visualItems.clear();
    obstaclePhysicsDebug.clear();
    fruitPhysicsDebug.clear();
}
