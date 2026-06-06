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

#include "../core/uimanager.h"
#include "../core/audiomanager.h"
UIManager* ui;


// Inicializa ventana y recursos
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    try
    {
        // Assets crash
        pixPlayerRun = QPixmap(":/new/prefix1/resources/crash_init.png");
        pixPlayerJump = QPixmap(":/new/prefix1/resources/crash_jump.png");
        pixPlayerGameOver = QPixmap(":/new/prefix1/resources/crash_gameOver.png");
        pixPlayerlvl2 = QPixmap(":/new/prefix1/resources/crash2.png");
        pixEnemy1 = QPixmap(":/new/prefix1/resources/enemy1.png");

        if(pixPlayerRun.isNull() || pixPlayerJump.isNull() || pixPlayerGameOver.isNull() || pixEnemy1.isNull() || pixPlayerlvl2.isNull())
        {
            throw std::runtime_error("error cargando sprites crash");
        }

        // Assets obstaculos
        pixObstacleLog = QPixmap(":/new/prefix1/resources/TNT.png");
        pixObstacleRock = QPixmap(":/new/prefix1/resources/Rock.png");
        pixObstacleSaw = QPixmap(":/new/prefix1/resources/floating.png");

        if(pixObstacleLog.isNull() || pixObstacleRock.isNull() || pixObstacleSaw.isNull())
        {
            throw std::runtime_error("error cargando obstaculos");
        }

        // Assets nivel 2
        pixGameOver2 = QPixmap(":/new/prefix1/resources/GameOver2.png");
        pixTronco = QPixmap(":/new/prefix1/resources/tronco.png");

        if(pixGameOver2.isNull() || pixTronco.isNull())
        {
            throw std::runtime_error("error cargando sprites nivel2");
        }

        // Assets items
        pixFruit = QPixmap(":/new/prefix1/resources/fruit.png");

        if(pixFruit.isNull())
        {
            throw std::runtime_error("error cargando fruta");
        }

        // Assets fondos
        QPixmap pixFar(":/new/prefix1/resources/bg1.png");
        QPixmap pixClose(":/new/prefix1/resources/bg3.png");
        QPixmap pixLevel2(":/new/prefix1/resources/bglvl2.png");

        if(pixFar.isNull() || pixClose.isNull() || pixLevel2.isNull())
        {
            throw std::runtime_error("error cargando fondos");
        }

        visualPlayer2 = nullptr;
        whirlpoolVisual = nullptr;
        deathZoneVisual = nullptr;

        // Inicializa backend
        game = new Game();
        game->loadLevel();

        gameLevel2 = new GameLevel2();

        // Configura escena
        scene = new QGraphicsScene(0, 0, 30000, 600, this);
        scene->setBackgroundBrush(Qt::black);

        // Hitbox crash
        physicsHitboxDebug = scene->addRect(0, 0, 30, 50, QPen(Qt::NoPen));
        physicsHitboxDebug->setZValue(20);

        enemyDebugRect = scene->addRect(0,0,60,100,QPen(Qt::NoPen));
        enemyDebugRect->setZValue(20);

        // Configura fondos
        bgLayer1Width = pixFar.width();
        bgLayer3Width = pixClose.width();
        bgLevel2Width = pixLevel2.width();

        bgLayer1 = new QGraphicsRectItem(0, 0, bgLayer1Width * 2, 600);
        bgLayer1->setBrush(QBrush(pixFar));
        bgLayer1->setPen(QPen(Qt::NoPen));
        bgLayer1->setZValue(-30);
        bgLayer1->setPos(0, -200);
        scene->addItem(bgLayer1);

        bgLayer3 = new QGraphicsRectItem(0, 0, bgLayer3Width * 2, 600);
        bgLayer3->setBrush(QBrush(pixClose));
        bgLayer3->setPen(QPen(Qt::NoPen));
        bgLayer3->setZValue(-10);
        scene->addItem(bgLayer3);

        bgLevel2 = new QGraphicsRectItem(0, 0, bgLevel2Width, 1000);
        bgLevel2->setBrush(QBrush(pixLevel2));
        bgLevel2->setPen(QPen(Qt::NoPen));
        bgLevel2->setZValue(-10);
        bgLevel2->setPos(-30, -190);
        scene->addItem(bgLevel2);

        bgLayer1->setVisible(false);
        bgLayer3->setVisible(false);
        bgLevel2->setVisible(false);

        // Configura vista
        view = new QGraphicsView(scene, this);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setFocusPolicy(Qt::NoFocus);

        setCentralWidget(view);
        setFixedSize(1600, 600);
        setWindowTitle("Crash Surfing");

        // Inicializa audio
        audio = new AudioManager();
        audio->playMenuMusic();

        // Crea jugador visual
        visualPlayer = new QGraphicsPixmapItem();
        visualPlayer->setPixmap(pixPlayerRun.scaled(180, 180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        visualPlayer->setZValue(10);

        // Crea enemigo visual
        visualEnemy1 = new QGraphicsPixmapItem();
        visualEnemy1->setPixmap(pixEnemy1.scaled(180, 180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        visualEnemy1->setZValue(100);

        // Inicializa UI
        ui = new UIManager(scene);

        hudText = new QGraphicsTextItem();
        hudText->setDefaultTextColor(Qt::white);
        scene->addItem(hudText);

        float startX = 0.0f;
        float startY = 0.0f;

        if(game && game->getPlayer())
        {
            Player* playerBackend = dynamic_cast<Player*>(game->getPlayer());

            if(playerBackend)
            {
                startX = playerBackend->getPosition().x();
                startY = playerBackend->getPosition().y();
            }

            visualPlayer->setPos(startX - 70.0f, startY - 80.0f);
        }

        menuSelectionText = new QGraphicsTextItem();
        menuSelectionText->setDefaultTextColor(Qt::white);

        QFont font;
        font.setPointSize(16);

        menuSelectionText->setFont(font);
        scene->addItem(menuSelectionText);

        // Inicia bucle
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::updateGameLoop);
        timer->start(16);
    }
    catch(const std::exception& e)
    {
        qDebug() << "EXCEPCION:" << e.what();
    }
}

MainWindow::~MainWindow()
{
    delete game;
    delete gameLevel2;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!game || !gameLevel2){ return; }
    GameStatus currentStatus = (selectedLevel == LevelType::LEVEL_1) ? game->getStatus() : gameLevel2->getStatus();

    // MENU
    if(currentStatus == GameStatus::MENU)
    {
        if(event->key() == Qt::Key_Up){ selectedLevel = LevelType::LEVEL_1; }
        else if(event->key() == Qt::Key_Down){ selectedLevel = LevelType::LEVEL_2; }
        else if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
        {
            selectedDifficulty = (selectedDifficulty == Difficulty::EASY) ? Difficulty::HARD : Difficulty::EASY;
        }
        else if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {
            clearAllVisuals();
            audio->stopMenuMusic();
            audio->playGameMusic();

            if(selectedLevel == LevelType::LEVEL_1)
            {
                clearLevel2Visuals();
                game->reset();
                game->setDifficulty(selectedDifficulty);
                createVisualEntities();
                game->setStatus(GameStatus::PLAYING);
            }
            else
            {
                clearVisualEntities();
                gameLevel2->reset();
                gameLevel2->setDifficulty(selectedDifficulty);
                createLevel2Visuals();
                gameLevel2->setStatus(GameStatus::PLAYING);
            }
            ui->hideMenu();
        }
        return;
    }

    // GAME OVER / WIN
    if(currentStatus == GameStatus::GAME_OVER || currentStatus == GameStatus::LEVEL_COMPLETE)
    {
        if(event->key() == Qt::Key_R)
        {
            clearAllVisuals();
            if(selectedLevel == LevelType::LEVEL_1)
            {
                game->reset();
                createVisualEntities();
                game->setStatus(GameStatus::PLAYING);
            }
            else
            {
                gameLevel2->reset();
                createLevel2Visuals();
                gameLevel2->setStatus(GameStatus::PLAYING);
            }
            ui->hideAll();
        }
        return;
    }

    // PAUSA
    if(currentStatus == GameStatus::PAUSED)
    {
        if(event->key() == Qt::Key_Escape)
        {
            if(selectedLevel == LevelType::LEVEL_1){ game->setStatus(GameStatus::PLAYING); }
            else { gameLevel2->setStatus(GameStatus::PLAYING); }
            ui->hidePause();
        }
        else if(event->key() == Qt::Key_R)
        {
            clearAllVisuals();
            if(selectedLevel == LevelType::LEVEL_1)
            {
                game->reset();
                createVisualEntities();
                game->setStatus(GameStatus::PLAYING);
            }
            else
            {
                gameLevel2->reset();
                createLevel2Visuals();
                gameLevel2->setStatus(GameStatus::PLAYING);
            }

            ui->hideAll();
        }

        else if(event->key() == Qt::Key_M)
        {
            clearAllVisuals();

            game->reset();
            gameLevel2->reset();

            game->setStatus(GameStatus::MENU);
            gameLevel2->setStatus(GameStatus::MENU);

            audio->stopGameMusic();
            audio->playMenuMusic();

            ui->hideAll();

            return;
        }
        return;
    }

    // PLAYING
    if(currentStatus != GameStatus::PLAYING){ return; }
    if(event->key() == Qt::Key_Escape)
    {
        if(selectedLevel == LevelType::LEVEL_1)
        {
            game->setStatus(GameStatus::PAUSED);
            ui->showPause(game->getPlayer()->getPosition().x());
        }
        else
        {
            gameLevel2->setStatus(GameStatus::PAUSED);
            ui->showPause(800.0f);
        }
        return;
    }

    // NIVEL 1
    if(selectedLevel == LevelType::LEVEL_1)
    {
        Player* player = dynamic_cast<Player*>(game->getPlayer());
        if(player && (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up)){ player->jump(); }
        return;
    }

    // Nivel 2
    Player2* player = gameLevel2->getPlayer();
    if(!player){ return; }
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_Up){ player->moveUp(); }
    else if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down){ player->moveDown(); }
    else if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left){ player->moveLeft(); }
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right){ player->moveRight(); }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(selectedLevel == LevelType::LEVEL_2)
    {
        Player2* player = gameLevel2->getPlayer();

        if(!player)
            return;

        if(event->key() == Qt::Key_A || event->key() == Qt::Key_D || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
        {
            player->stopHorizontal();
        }
        if(event->key() == Qt::Key_W || event->key() == Qt::Key_S || event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
        {
            player->stopVertical();
        }
    }
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::updateGameLoop()
{
    // DEBUG
    // qDebug() << "Nivel:" << (selectedLevel == LevelType::LEVEL_1 ? "LEVEL1" : "LEVEL2");
    GameStatus currentStatus;
    if(selectedLevel == LevelType::LEVEL_1)
    {
        currentStatus = game->getStatus();
    }
    else
    {
        currentStatus = gameLevel2->getStatus();
    }

    // MENU
    if(currentStatus == GameStatus::MENU)
    {
        ui->hideAll();
        ui->showMenu(selectedLevel, selectedDifficulty);
        view->centerOn(0, 0);
        return;
    }

    // Seleccionado
    if(selectedLevel == LevelType::LEVEL_1){
        updateLevel1();
    }
    else {
        updateLevel2();
    }
}

void MainWindow::updateLevel1()
{
    if(!game || !game->getPlayer()){ return; }

    Player* p = dynamic_cast<Player*>(game->getPlayer());

    if(!p){ return; }

    float playerX = p->getPosition().x();
    float playerY = p->getPosition().y();

    GameStatus currentStatus = game->getStatus();

    if(p->getCollectedFruits() >= game->getFruits2Win())
    {
        game->setStatus(GameStatus::LEVEL_COMPLETE);
    }
    if(currentStatus == GameStatus::PLAYING)
    {
        game->update(0.016f);
    }
    if(game->consumeFruitCollected())
    {
        audio->playFruitSound();
    }
    if(game->consumePlayerHit())
    {
        audio->playHitSound();
    }
    if(currentStatus == GameStatus::PAUSED)
    {
        ui->hideAll();
        ui->showPause(playerX);
        return;
    }
    if(currentStatus == GameStatus::MENU)
    {
        ui->hideAll();
        ui->showMenu(selectedLevel, selectedDifficulty);
        view->centerOn(0, 0);
        return;
    }
    if(currentStatus == GameStatus::GAME_OVER)
    {
        visualPlayer->setPixmap(pixPlayerGameOver.scaled(250, 200,  Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->hideAll();
        ui->showGameOver(playerX);
        return;
    }
    if(currentStatus == GameStatus::LEVEL_COMPLETE)
    {
        ui->hideAll();
        ui->showLevelComplete(playerX);
        return;
    }

    bgLayer1->setVisible(true); bgLayer3->setVisible(true); bgLevel2->setVisible(false);
    visualPlayer->show(); visualEnemy1->show();
    if(visualPlayer2){ visualPlayer2->hide();}

    float currentCrashX = playerX;
    float currentCrashY = playerY + 130.0f;

    float cameraLeft = playerX - 200.0f;
    // physicsHitboxDebug->setRect(playerX - playerW * 0.5f, playerY + 155.0f - playerH, playerW, playerH);

    float offset1 = fmod(cameraLeft * 0.15f, bgLayer1Width);
    bgLayer1->setPos(cameraLeft - offset1, -200.0f);

    float offset3 = fmod(cameraLeft, bgLayer3Width);
    bgLayer3->setPos(cameraLeft - offset3, 50.0f);

    QFont selectorFont;
    selectorFont.setPointSize(20);
    selectorFont.setBold(true);

    QString difficultyText =  game->getDifficult() == Difficulty::EASY ? "FACIL" : "DIFICIL";

    hudText->setPlainText(QString("VIDAS: %1 | WUMPAS: %2/%3 | Dificultad: %4")
                              .arg(p->getLives())
                              .arg(p->getCollectedFruits())
                              .arg(game->getFruits2Win())
                              .arg(difficultyText));
    hudText->setPos(cameraLeft + 50.0f, 30.0f);
    hudText->setFont(selectorFont);

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
            alignedX = currentCrashX - 90.0f;
            alignedY = currentCrashY - 60.0f;
        }
        else {
            pixmapPlayer->setPixmap(pixPlayerRun.scaled(180, 180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            alignedX = currentCrashX - 90.0f;
            alignedY = currentCrashY - 80.0f;
        }
        pixmapPlayer->setPos(alignedX, alignedY);
    }

    // 6. EFECTO INVENCIBILIDAD (PARPADEO)
    if (p->getIsInvincible() && currentStatus != GameStatus::GAME_OVER) {
        static int frameCounter = 0;
        frameCounter++;
        if (frameCounter % 7 == 0) {
            if (visualPlayer) visualPlayer->setVisible(!visualPlayer->isVisible());
        }
    } else {
        if (visualPlayer) visualPlayer->setVisible(true);
    }

    // Enemy
    Enemy* enemy = game->getEnemy();

    if(enemy)
    {
        // enemyDebugRect->setRect(enemy->getPosition().x(), enemy->getPosition().y(), enemy->getWidth(), enemy->getHeight());
        enemyDebugRect->setVisible(enemy->isActive());
    }

    QGraphicsPixmapItem* pixmapEnemy1 = static_cast<QGraphicsPixmapItem*>(visualEnemy1);
    if (pixmapEnemy1) {
        float enemyX = enemy->getPosition().x();
        float enemyY = enemy->getPosition().y() + 130.0f;
        pixmapEnemy1->setPixmap(pixEnemy1.scaled( 220, 160,  Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        float alignedX = enemyX - 90.0f;
        float alignedY = enemyY - 150.0f;
        pixmapEnemy1->setPos(alignedX, alignedY);
    }

    // Frutas
    while(visualEntities.size() < game->getEntities().size())
    {
        Entity* entity = game->getEntities()[visualEntities.size()];
        Obstacle* obs = dynamic_cast<Obstacle*>(entity);

        if(!obs){ break; }

        QString tipo = obs->getType().toLower();
        QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem();

        float spriteW = 130.0f;
        float spriteH = 110.0f;

        QPixmap texture;

        if(tipo == "saw")
        {
            texture = pixObstacleRock;
            spriteW = 130.0f;
            spriteH = 100.0f;
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

        // QGraphicsRectItem* physicsBox = scene->addRect( 0, 0, hitboxW, hitboxH, QPen(Qt::cyan,2));
        // physicsBox->setBrush(QColor(0,255,255,40));
        // physicsBox->setZValue(500);
        // obstaclePhysicsDebug.push_back(physicsBox);
    }

    const auto& backendItems = game->getItems();
    for (size_t i = 0; i < backendItems.size() && i < visualItems.size(); ++i)
    {
        if (!visualItems[i]){ continue; }
        if (backendItems[i]->getIsCollected() ||
            !backendItems[i]->isActive())
        {
            visualItems[i]->setVisible(false);
            if(i < fruitPhysicsDebug.size()){ fruitPhysicsDebug[i]->setVisible(false); }

            continue;
        }

        float itemX = backendItems[i]->getPosition().x();
        float itemY = backendItems[i]->getPosition().y() + 135.0f;

        // Hitbox
        if(i < fruitPhysicsDebug.size())
        {
            fruitPhysicsDebug[i]->setRect(itemX, itemY, backendItems[i]->getWidth(), backendItems[i]->getHeight());
            fruitPhysicsDebug[i]->setVisible(true);
        }

        QGraphicsPixmapItem* fruitSprite = dynamic_cast<QGraphicsPixmapItem*>(visualItems[i]);

        if(fruitSprite)
        {
            float offsetX = fruitSprite->data(1).toFloat();
            float offsetY = fruitSprite->data(2).toFloat();
            fruitSprite->setPos(itemX - offsetX, itemY - offsetY);
        }
        visualItems[i]->setVisible(true);
    }

    // 8. ACTUALIZACIÓN VISUAL DE OBSTÁCULOS
    const auto& backendEntities = game->getEntities();
    for (size_t i = 0; i < backendEntities.size() && i < visualEntities.size(); ++i)
    {
        if (!visualEntities[i]){ continue; }

        if (!backendEntities[i]->isActive())
        {
            visualEntities[i]->setVisible(false);
            if(i < obstaclePhysicsDebug.size()){ obstaclePhysicsDebug[i]->setVisible(false); }
            continue;
        }

        float entX = backendEntities[i]->getPosition().x();
        float entY = backendEntities[i]->getPosition().y() + 130.0f;

        // HITBOX REAL (CYAN)
        if(i < obstaclePhysicsDebug.size())
        {
            obstaclePhysicsDebug[i]->setRect(entX, entY,  backendEntities[i]->getWidth(), backendEntities[i]->getHeight()  );
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
                float alignedY = entY - offsetY + 35.0f;
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
                pixmapEnt->setTransformOriginPoint(pixmapEnt->boundingRect().center());
                pixmapEnt->setRotation(pixmapEnt->rotation() + 10.0f);
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
    scene->addItem(visualPlayer); scene->addItem(visualEnemy1);

    const auto& backendEntities = game->getEntities();

    for(Entity* entity : backendEntities)
    {
        Obstacle* obs = dynamic_cast<Obstacle*>(entity);
        if(!obs){ continue; }

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

        float hitboxW = entity->getWidth();
        float hitboxH = entity->getHeight();
        float offsetX = (spriteW - hitboxW) * 0.5f;
        float offsetY = spriteH - hitboxH;

        sprite->setPixmap(texture.scaled(spriteW, spriteH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        sprite->setData(1, offsetX);
        sprite->setData(2, offsetY);
        sprite->setZValue(5);

        scene->addItem(sprite);
        visualEntities.push_back(sprite);

        // QGraphicsRectItem* physicsBox = scene->addRect(0, 0, hitboxW, hitboxH, QPen(Qt::cyan, 2));
        // physicsBox->setBrush(QColor(0,255,255,40));
        // physicsBox->setZValue(20);
        // obstaclePhysicsDebug.push_back(physicsBox);
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

        fruitSprite->setPixmap(pixFruit.scaled( spriteW, spriteH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        fruitSprite->setData(1, offsetX);
        fruitSprite->setData(2, offsetY);
        fruitSprite->setZValue(8);

        scene->addItem(fruitSprite);
        visualItems.push_back(fruitSprite);

        // QGraphicsRectItem* physicsBox = scene->addRect( 0, 0, hitboxW, hitboxH, QPen(Qt::magenta, 2));
        // physicsBox->setBrush(QColor(255,0,255,40));
        // physicsBox->setZValue(20);
        // fruitPhysicsDebug.push_back(physicsBox);
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

void MainWindow::clearAllVisuals()
{
    clearVisualEntities();
    clearLevel2Visuals();
}

void MainWindow::loadLevel1()
{
    clearAllVisuals();
    createVisualEntities();
}

void MainWindow::loadLevel2()
{
    clearAllVisuals();
    createLevel2Visuals();
}

// NIVEL 2
void MainWindow::updateLevel2()
{
    if(!gameLevel2 || !gameLevel2->getPlayer()){ return; }

    Player2* p = gameLevel2->getPlayer();

    if(!p){ return; }

    GameStatus currentStatus = gameLevel2->getStatus();

    // Victoria por frutas
    if(p->getCollectedFruits() >= gameLevel2->getFruits2Win())
    {
        gameLevel2->setStatus(GameStatus::LEVEL_COMPLETE);
    }

    // Update backend
    if(currentStatus == GameStatus::PLAYING)
    {
        gameLevel2->update(0.016f);
    }

    // Sonidos
    if(gameLevel2->consumeFruitCollected())
    {
        audio->playFruitSound();
    }
    if(gameLevel2->consumePlayerHit())
    {
        audio->playHitSound();
    }

    // PAUSA
    if(currentStatus == GameStatus::PAUSED)
    {
        ui->hideAll();
        ui->showPause(200.0f);
        return;
    }

    // MENU
    if(currentStatus == GameStatus::MENU)
    {
        ui->hideAll();
        ui->showMenu(selectedLevel, selectedDifficulty);
        return;
    }

    // GAME OVER
    if(currentStatus == GameStatus::GAME_OVER)
    {
        visualPlayer2->setPixmap(pixGameOver2.scaled(180, 130,  Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->hideAll();
        ui->showGameOver(200.0f);
        return;
    }

    // LEVEL COMPLETE
    if(currentStatus == GameStatus::LEVEL_COMPLETE)
    {
        ui->hideAll();
        ui->showLevelComplete(200.0f);
        return;
    }

    // Visuales
    bgLayer1->setVisible(false); bgLayer3->setVisible(false); bgLevel2->setVisible(true);
    visualPlayer->hide(); visualEnemy1->hide(); visualPlayer2->show();
    float playerX = p->getPosition().x();
    float playerY = p->getPosition().y();

    visualPlayer2->setPos(playerX - 45.0f, playerY - 30.0f);
    player2HitboxDebug->setPos(playerX, playerY);

    // HUD
    QFont selectorFont;
    selectorFont.setPointSize(20);
    selectorFont.setBold(true);

    QString difficultyText = (gameLevel2->getDifficult() == Difficulty::EASY) ? "FACIL" : "DIFICIL";

    hudText->setPlainText(QString("VIDAS: %1 | FRUTAS: %2/%3 | TIEMPO: %4 | DIFICULTAD: %5")
            .arg(p->getLives())
            .arg(p->getCollectedFruits())
            .arg(gameLevel2->getFruits2Win())
            .arg((int)gameLevel2->getRemainingTime())
            .arg(difficultyText));

    hudText->setPos(50,30);
    hudText->setFont(selectorFont);

    // Frutas
    const auto& fruits = gameLevel2->getFruits();
    for(size_t i = 0; i < fruits.size() && i < visualLevel2Fruits.size(); i++)
    {
        visualLevel2Fruits[i]->setPos(fruits[i]->getPosition().x() - 55, fruits[i]->getPosition().y() - 30);
        visualLevel2Fruits[i]->setVisible(fruits[i]->isActive());
        level2FruitHitboxes[i]->setPos(fruits[i]->getPosition().x(), fruits[i]->getPosition().y());
        level2FruitHitboxes[i]->setVisible( fruits[i]->isActive());
    }

    // Obstáculos
    const auto& obstacles = gameLevel2->getObstacles();
    for(size_t i = 0;i < obstacles.size() && i < visualLevel2Obstacles.size(); i++)
    {
        visualLevel2Obstacles[i]->setPos(obstacles[i]->getPosition().x() - 30, obstacles[i]->getPosition().y() - 30);
        visualLevel2Obstacles[i]->setVisible(obstacles[i]->isActive());
        level2ObstacleHitboxes[i]->setPos(obstacles[i]->getPosition().x(), obstacles[i]->getPosition().y());
        level2ObstacleHitboxes[i]->setVisible(obstacles[i]->isActive());
    }
    // Cámara fija
    view->centerOn(800,300);
}

void MainWindow::createLevel2Visuals()
{
    if(!gameLevel2){ return; }
    bgLevel2->setVisible(true);

    // Player
    visualPlayer2 = new QGraphicsPixmapItem();
    visualPlayer2->setPixmap(pixPlayerlvl2.scaled(130, 100,  Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    visualPlayer2->setZValue(10);
    scene->addItem(visualPlayer2);

    player2HitboxDebug = scene->addRect( 0, 0, gameLevel2->getPlayer()->getWidth(), gameLevel2->getPlayer()->getHeight(), QPen(Qt::NoPen) );
    // player2HitboxDebug->setBrush( QColor(0,255,0,40) );
    player2HitboxDebug->setZValue(50);

    // Remolino
    QVector2D center = gameLevel2->getWhirlpool().getCenter();
    float radius = gameLevel2->getWhirlpool().getDeathRadius();

    whirlpoolVisual = scene->addEllipse( center.x() - 150,  center.y() - 150, 300, 300, QPen(Qt::NoPen));
    whirlpoolVisual->setZValue(1);

    deathZoneVisual =scene->addEllipse(center.x() - radius,center.y() - radius,radius * 2, radius * 2,QPen(Qt::NoPen));
    deathZoneVisual->setZValue(2);

    // Frutas
    const auto& fruits =gameLevel2->getFruits();
    for(Item* fruit : fruits)
    {
        QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem();
        sprite->setPixmap(pixFruit.scaled(130, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        sprite->setZValue(30);
        scene->addItem(sprite);
        visualLevel2Fruits.push_back(sprite);

        // HITBOX DEBUG
        QGraphicsRectItem* log = scene->addRect(0, 0, fruit->getWidth(), fruit->getHeight(), QPen(Qt::NoPen));
        //log->setBrush(QColor(255,0,255,40));
        log->setZValue(50);
        level2FruitHitboxes.push_back(log);
    }

    const auto& obstacles = gameLevel2->getObstacles();

    for(Obstacle* obstacle : obstacles)
    {
        QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem();

        sprite->setPixmap(pixTronco.scaled(100, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        sprite->setZValue(8);
        scene->addItem(sprite);
        visualLevel2Obstacles.push_back(sprite);

        // HITBOX DEBUG
        QGraphicsRectItem* box =scene->addRect(0,0,obstacle->getWidth(),obstacle->getHeight(),QPen(Qt::NoPen));
        //box->setBrush(QColor(0,255,255,40));
        box->setZValue(50);
        level2ObstacleHitboxes.push_back(box);
    }
}

void MainWindow::clearLevel2Visuals()
{
    for(auto item : visualLevel2Fruits)
    {
        scene->removeItem(item);
        delete item;
    }
    visualLevel2Fruits.clear();

    for(auto item : visualLevel2Obstacles)
    {
        scene->removeItem(item);
        delete item;
    }
    visualLevel2Obstacles.clear();

    if(visualPlayer2)
    {
        scene->removeItem(visualPlayer2);
        delete visualPlayer2;
        visualPlayer2 = nullptr;
    }
    if(whirlpoolVisual)
    {
        scene->removeItem(whirlpoolVisual);
        delete whirlpoolVisual;
        whirlpoolVisual = nullptr;
    }
    if(deathZoneVisual)
    {
        scene->removeItem(deathZoneVisual);
        delete deathZoneVisual;
        deathZoneVisual = nullptr;
    }
}

