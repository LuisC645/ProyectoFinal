#include "mainwindow.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QFont>

#include "../entities/obstacle.h"
#include "../entities/player.h"
#include "../entities/item.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. INICIALIZAR BACKEND
    game = new Game();
    game->loadLevel();

    gameOverText = nullptr;
    menuTitleText = nullptr;
    menuInfoText = nullptr;

    // 2. CONFIGURAR ESCENA Y VISTA
    scene = new QGraphicsScene(0, 0, 30000, 600, this);
    scene->setBackgroundBrush(Qt::black);

    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);

    setCentralWidget(view);
    setFixedSize(1600, 600);
    setWindowTitle("Crash Surfing - Procedural Engine v1.0");

    // 3. CREAR JUGADOR
    visualPlayer = scene->addRect(0, 0, 40, 40, QPen(Qt::NoPen), QBrush(Qt::blue));

    float startX = 0.0f;
    float startY = 0.0f;
    if (game != nullptr && game->getPlayer() != nullptr && visualPlayer != nullptr) {
        startX = game->getPlayer()->getPosition().x();
        startY = game->getPlayer()->getPosition().y();
        visualPlayer->setPos(startX, startY);
    }

    // 4. CREAR ELEMENTOS DE LA INTERFAZ (GUI)
    // HUD de juego
    hudText = scene->addText("VIDAS: 3  |  WUMPAS: 0  |  SCORE: 0");
    hudText->setDefaultTextColor(Qt::white);
    hudText->setFont(QFont("Arial", 16, QFont::Bold));
    hudText->setZValue(10);
    hudText->setVisible(false); // Oculto al inicio (estamos en menú)

    // Pantalla de Inicio: Título Principal
    menuTitleText = scene->addText("CRASH SURFING");
    menuTitleText->setDefaultTextColor(QColor(255, 102, 0)); // Color Naranja Crash clásico
    menuTitleText->setFont(QFont("Impact", 55, QFont::Bold));
    menuTitleText->setZValue(11);
    menuTitleText->setPos(startX + 180, 160);

    // Pantalla de Inicio: Instrucciones
    menuInfoText = scene->addText("PRESIONA 'ENTER' PARA COMENZAR");
    menuInfoText->setDefaultTextColor(Qt::white);
    menuInfoText->setFont(QFont("Arial", 16, QFont::Bold));
    menuInfoText->setZValue(11);
    menuInfoText->setPos(startX + 210, 270);

    // 5. MATERIALIZAR EL NIVEL
    createVisualEntities();

    // 6. INICIAR EL BUCLE DEL JUEGO (Nunca se detiene, cambia por estados)
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGameLoop);
    timer->start(16); // ~60 FPS
}

MainWindow::~MainWindow()
{
    delete game;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!game) return;

    GameStatus currentStatus = game->getStatus();

    // 🕹️ ACCIÓN EN ESTADO: MENU
    if (currentStatus == GameStatus::MENU) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            // Iniciamos la carrera
            game->setStatus(GameStatus::PLAYING);

            // Transición limpia de visibilidad de pantallas
            if (menuTitleText) menuTitleText->setVisible(false);
            if (menuInfoText) menuInfoText->setVisible(false);
            if (hudText) hudText->setVisible(true);
        }
        return;
    }

    // 💀 ACCIÓN EN ESTADO: GAME OVER / VICTORIA (REINICIAR)
    if (currentStatus == GameStatus::GAME_OVER || currentStatus == GameStatus::LEVEL_COMPLETE) {
        if (event->key() == Qt::Key_R) {

            // 1. Borramos el texto de fin de juego
            if (gameOverText) {
                scene->removeItem(gameOverText);
                delete gameOverText;
                gameOverText = nullptr;
            }

            // 2. Limpiamos gráficos dinámicos viejos
            for (QGraphicsItem* visualEnt : visualEntities) {
                if (visualEnt) { scene->removeItem(visualEnt); delete visualEnt; }
            }
            for (QGraphicsItem* visualIt : visualItems) {
                if (visualIt) { scene->removeItem(visualIt); delete visualIt; }
            }

            // 3. Reiniciamos lógica (el backend regresará automáticamente a GameStatus::MENU)
            game->reset();

            // 4. Regeneramos el mapa procedural
            createVisualEntities();

            // 5. Reposicionamos al jugador de forma segura
            if (visualPlayer) {
                visualPlayer->setVisible(true);
                if (game->getPlayer()) {
                    visualPlayer->setPos(game->getPlayer()->getPosition().x(), game->getPlayer()->getPosition().y());
                }
            }

            // 6. Mostramos el menú de inicio nuevamente en la nueva posición inicial
            float playerX = game->getPlayer()->getPosition().x();
            if (menuTitleText) { menuTitleText->setPos(playerX + 180, 160); menuTitleText->setVisible(true); }
            if (menuInfoText) { menuInfoText->setPos(playerX + 210, 270); menuInfoText->setVisible(true); }
            if (hudText) hudText->setVisible(false);
        }
        return;
    }

    // 🏃 CONTROLES EN ESTADO: PLAYING (NORMAL)
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

    // 1. ACTUALIZAR EL BACKEND (Él sabe si debe congelarse o no según su estado interno)
    game->update(0.016f);

    // 2. FORCE-SINCRONIZAR CONDICIÓN DE VICTORIA DEL FRONTEND AL BACKEND
    if (currentStatus == GameStatus::PLAYING && p->getCollectedFruits() >= 30) {
        game->setStatus(GameStatus::LEVEL_COMPLETE);
        currentStatus = GameStatus::LEVEL_COMPLETE;
    }

    // ==========================================
    // RENDERS Y ENTRADAS SEGÚN EL ESTADO VISUAL
    // ==========================================

    // 🎪 PANTALLA: MENÚ DE INICIO
    if (currentStatus == GameStatus::MENU) {
        view->centerOn(p->getPosition().x() + 400, 300); // Cámara fija mirando al jugador en la salida
        return; // Detiene el procesamiento gráfico dinámico del loop
    }

    // 🏆 PANTALLA: VICTORIA
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

    // 💀 PANTALLA: GAME OVER
    if (currentStatus == GameStatus::GAME_OVER) {
        if (!gameOverText) {
            gameOverText = new QGraphicsTextItem("GAME OVER\nPresiona 'R' para reiniciar");
            gameOverText->setDefaultTextColor(Qt::red);
            QFont font = gameOverText->font(); font.setPointSize(30); font.setBold(true);
            gameOverText->setFont(font); scene->addItem(gameOverText);
        }
        gameOverText->setPos(p->getPosition().x() + 200, 180);
        gameOverText->setVisible(true);
        return;
    }

    // 🏃 RUNTIME: ACTUALIZACIÓN GRÁFICA EN VIVO (PLAYING)
    float newX = p->getPosition().x();
    float newY = p->getPosition().y();

    // Actualización del HUD persistente en pantalla con seguimiento de cámara
    hudText->setPlainText(QString("VIDAS: %1   |   WUMPAS: %2   |   SCORE: %3")
                              .arg(p->getLives())
                              .arg(p->getCollectedFruits())
                              .arg(game->getScore()));

    float cameraLeftEdge = newX - 350.0f;
    hudText->setPos(cameraLeftEdge, 30.0f);

    // Ajustar el tamaño y render del jugador (Modo Glotón)
    QGraphicsRectItem* rectPlayer = static_cast<QGraphicsRectItem*>(visualPlayer);
    if (rectPlayer) {
        rectPlayer->setRect(0, 0, 40, 40);
        if (p->getIsGlutton()) {
            rectPlayer->setBrush(QBrush(QColor(20, 20, 80))); // Color azul pesado
        } else {
            rectPlayer->setBrush(QBrush(Qt::blue));
        }
    }

    // Efecto visual de parpadeo por invencibilidad temporal
    if (p->getIsInvincible()) {
        static int frameCounter = 0;
        frameCounter++;
        if (frameCounter % 6 == 0) {
            visualPlayer->setVisible(!visualPlayer->isVisible());
        }
    } else {
        if (visualPlayer) visualPlayer->setVisible(true);
    }

    // Sincronizar ítems (Cajas/Frutas)
    const auto& backendItems = game->getItems();
    for (size_t i = 0; i < backendItems.size() && i < visualItems.size(); ++i) {
        if (visualItems[i] != nullptr) {
            if (backendItems[i]->getIsCollected() || !backendItems[i]->isActive()) {
                visualItems[i]->setVisible(false);
            } else {
                visualItems[i]->setPos(backendItems[i]->getPosition().x(), backendItems[i]->getPosition().y());
                visualItems[i]->setVisible(true);
            }
        }
    }

    // Sincronizar obstáculos (Sierras/Drones)
    const auto& backendEntities = game->getEntities();
    for (size_t i = 0; i < backendEntities.size() && i < visualEntities.size(); ++i) {
        if (visualEntities[i] != nullptr) {
            if (backendEntities[i]->isActive()) {
                visualEntities[i]->setPos(backendEntities[i]->getPosition().x(), backendEntities[i]->getPosition().y());
                visualEntities[i]->setVisible(true);
            } else {
                visualEntities[i]->setVisible(false);
            }
        }
    }

    // Desplazamiento de la cámara cinematográfica
    if (visualPlayer) {
        visualPlayer->setPos(newX, newY);
    }
    view->centerOn(newX + 400, 300);
}

void MainWindow::createVisualEntities()
{
    visualEntities.clear();
    visualItems.clear();

    const auto& backendEntities = game->getEntities();
    for (Entity* e : backendEntities) {
        Obstacle* obs = dynamic_cast<Obstacle*>(e);
        QGraphicsItem* visualEntity = nullptr;

        if (obs) {
            if (obs->getType() == "saw") {
                QGraphicsEllipseItem* saw = new QGraphicsEllipseItem(0, 0, 40, 40);
                saw->setBrush(QBrush(Qt::darkGray)); saw->setPen(QPen(Qt::NoPen));
                visualEntity = saw;
            }
            else if (obs->getType() == "floating") {
                QGraphicsEllipseItem* fl = new QGraphicsEllipseItem(0, 0, 35, 35);
                fl->setBrush(QBrush(Qt::magenta)); fl->setPen(QPen(Qt::NoPen));
                visualEntity = fl;
            }
            else if (obs->getType() == "log") {
                QGraphicsRectItem* logItem = new QGraphicsRectItem(0, 0, 40, 40);
                logItem->setBrush(QBrush(QColor(115, 66, 34))); logItem->setPen(QPen(Qt::NoPen));
                visualEntity = logItem;
            }
        }

        if (!visualEntity) {
            QGraphicsRectItem* fallback = new QGraphicsRectItem(0, 0, 40, 40);
            fallback->setBrush(QBrush(Qt::white)); fallback->setPen(QPen(Qt::NoPen));
            visualEntity = fallback;
        }

        visualEntity->setPos(e->getPosition().x(), e->getPosition().y());
        scene->addItem(visualEntity);
        visualEntities.push_back(visualEntity);
    }

    const auto& backendItems = game->getItems();
    for (Item* item : backendItems) {
        if (!item) continue;

        QGraphicsRectItem* visualItem = new QGraphicsRectItem(0, 0, item->getWidth(), item->getHeight());
        if (item->getType() == "fruit") {
            visualItem->setBrush(QBrush(QColor(255, 128, 0)));
        }

        visualItem->setPen(QPen(Qt::NoPen));
        visualItem->setPos(item->getPosition().x(), item->getPosition().y());
        scene->addItem(visualItem);
        visualItems.push_back(visualItem);
    }
}
