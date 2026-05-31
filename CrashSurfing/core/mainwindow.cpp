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

    gameOverText = nullptr; // Inicializacion segura

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

    // 3. CREAR JUGADOR Y HUD
    visualPlayer = scene->addRect(0, 0, 40, 40, QPen(Qt::NoPen), QBrush(Qt::blue));

    if (game != nullptr && game->getPlayer() != nullptr && visualPlayer != nullptr) {
        float startX = game->getPlayer()->getPosition().x();
        float startY = game->getPlayer()->getPosition().y();
        visualPlayer->setPos(startX, startY);
    }

    hudText = scene->addText("Vidas: 3  |  Frutas: 0");
    hudText->setDefaultTextColor(Qt::white);
    hudText->setFont(QFont("Arial", 16, QFont::Bold));
    hudText->setZValue(10);

    // 4. MATERIALIZAR EL NIVEL
    createVisualEntities();

    // 5. INICIAR EL BUCLE DEL JUEGO
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGameLoop);
    timer->start(16); // 60 FPS
}

MainWindow::~MainWindow()
{
    delete game;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!game) return;

    // REINICIAR JUEGO (GAME OVER)
    if (game->getIsGameOver()) {
        if (event->key() == Qt::Key_R) {

            // 1. Borramos el texto de Game Over de la escena
            if (gameOverText) {
                scene->removeItem(gameOverText);
                delete gameOverText;
                gameOverText = nullptr;
            }

            // 2. Limpiamos gráficos dinámicos viejos de la escena de Qt
            for (QGraphicsItem* visualEnt : visualEntities) {
                if (visualEnt) {
                    scene->removeItem(visualEnt);
                    delete visualEnt;
                }
            }
            for (QGraphicsItem* visualIt : visualItems) {
                if (visualIt) {
                    scene->removeItem(visualIt);
                    delete visualIt;
                }
            }

            // 3. Reiniciamos la lógica del backend
            game->reset();

            // 4. Volvemos a leer el generador y creamos los nuevos gráficos
            createVisualEntities();

            // 5. Revivimos el render del jugador
            if (visualPlayer) {
                visualPlayer->setVisible(true);
                if (game->getPlayer()) {
                    visualPlayer->setPos(game->getPlayer()->getPosition().x(), game->getPlayer()->getPosition().y());
                }
            }

            timer->start(16);
        }
        return;
    }

    // === CONTROLES NORMALES ===
    Player* player = dynamic_cast<Player*>(game->getPlayer());
    if (!player) return;

    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) {
        player->jump();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::updateGameLoop()
{

    if (!game || !game->getPlayer()) return;

    // 1. ACTUALIZAR LÓGICA
    game->update(0.016f);

    Player* p = dynamic_cast<Player*>(game->getPlayer());
    if (!p) return; // Aseguramos que 'p' existe desde el principio

    // 2A. COMPROBAR VICTORIA (Límite de 50 frutas)
    if (p->getCollectedFruits() >= 30) {
        timer->stop();

        if (!gameOverText) {
            gameOverText = new QGraphicsTextItem("¡NIVEL COMPLETADO!\nPresiona 'R' para jugar de nuevo");
            gameOverText->setDefaultTextColor(Qt::green);
            QFont font = gameOverText->font();
            font.setPointSize(30);
            font.setBold(true);
            gameOverText->setFont(font);
            scene->addItem(gameOverText);
        }

        gameOverText->setPos(p->getPosition().x() - 200, 150);
        gameOverText->setVisible(true);
        return;
    }

    // 2B. COMPROBAR GAME OVER (Derrota normal)
    if (game->getIsGameOver()) {
        timer->stop();

        if (!gameOverText) {
            gameOverText = new QGraphicsTextItem("GAME OVER\nPresiona 'R' para reiniciar");
            gameOverText->setDefaultTextColor(Qt::red);
            QFont font = gameOverText->font();
            font.setPointSize(30);
            font.setBold(true);
            gameOverText->setFont(font);
            scene->addItem(gameOverText);
        }

        gameOverText->setPos(p->getPosition().x() - 150, 150);
        gameOverText->setVisible(true);
        return;
    }

    // 3. CONTINUAR DIBUJANDO EL JUEGO NORMALMENTE
    float newX = p->getPosition().x();
    float newY = p->getPosition().y();


    if (p) {
        // Actualización del HUD persistente en pantalla
        hudText->setPlainText(QString("VIDAS: %1   |   WUMPAS: %2   |   SCORE: %3")
                                  .arg(p->getLives())
                                  .arg(p->getCollectedFruits())
                                  .arg(game->getScore()));

        float cameraLeftEdge = newX - 350.0f;
        hudText->setPos(cameraLeftEdge, 30.0f);

        QGraphicsRectItem* rectPlayer = static_cast<QGraphicsRectItem*>(visualPlayer);
        if (rectPlayer) {

            rectPlayer->setRect(0, 0, 40, 40);

            if (p->getIsGlutton()) {
                rectPlayer->setBrush(QBrush(QColor(20, 20, 80)));
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
    }

    // Sincronizar Frutas y Cajas Visuales con el Backend
    const auto& backendItems = game->getItems();
    for (size_t i = 0; i < backendItems.size() && i < visualItems.size(); ++i) {
        if (visualItems[i] != nullptr) {
            if (backendItems[i]->getIsCollected() || !backendItems[i]->isActive()) {
                visualItems[i]->setVisible(false);
            } else {
                // Sincroniza la levitación senoidal del backend si la tiene
                visualItems[i]->setPos(backendItems[i]->getPosition().x(), backendItems[i]->getPosition().y());
                visualItems[i]->setVisible(true);
            }
        }
    }

    // Sincronizar Sierras y Drones Visuales con el Backend
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

    // Desplazar Jugador y Cámara Cinematográfica (Estilo Crash)
    if (visualPlayer) {
        visualPlayer->setPos(newX, newY);
    }
    view->centerOn(newX + 400, 300);
}

void MainWindow::createVisualEntities()
{
    // 1. Limpieza total de listas de renderizado
    visualEntities.clear();
    visualItems.clear();

    // 2. POBLAR LOS OBSTACULOS VISUALES
    const auto& backendEntities = game->getEntities();
    for (Entity* e : backendEntities) {
        Obstacle* obs = dynamic_cast<Obstacle*>(e);
        QGraphicsItem* visualEntity = nullptr;

        if (obs) {
            if (obs->getType() == "saw") {
                QGraphicsEllipseItem* saw = new QGraphicsEllipseItem(0, 0, 40, 40);
                saw->setBrush(QBrush(Qt::darkGray));
                saw->setPen(QPen(Qt::NoPen));
                visualEntity = saw;
            }
            else if (obs->getType() == "floating") {
                QGraphicsEllipseItem* fl = new QGraphicsEllipseItem(0, 0, 35, 35);
                fl->setBrush(QBrush(Qt::magenta));
                fl->setPen(QPen(Qt::NoPen));
                visualEntity = fl;
            }
            else if (obs->getType() == "log") {
                QGraphicsRectItem* logItem = new QGraphicsRectItem(0, 0, 80, 30);

                logItem->setBrush(QBrush(QColor(115, 66, 34)));

                logItem->setPen(QPen(Qt::NoPen));
                visualEntity = logItem;
            }
        }

        if (!visualEntity) {
            QGraphicsRectItem* fallback = new QGraphicsRectItem(0, 0, 40, 40);
            // Si algo no tiene tipo definido, aparecerá blanco chillón para que te des cuenta rápido.
            fallback->setBrush(QBrush(Qt::white));
            fallback->setPen(QPen(Qt::NoPen));
            visualEntity = fallback;
        }

        visualEntity->setPos(e->getPosition().x(), e->getPosition().y());
        scene->addItem(visualEntity);
        visualEntities.push_back(visualEntity);
    }

    // 3. POBLAR LOS ITEMS VISUALES (Frutas Wumpa y Cajas de madera)
    const auto& backendItems = game->getItems();
    for (Item* item : backendItems) {
        if (!item) continue;

        QGraphicsRectItem* visualItem = new QGraphicsRectItem(0, 0, item->getWidth(), item->getHeight());

        if (item->getType() == "fruit") {
            visualItem->setBrush(QBrush(QColor(255, 128, 0))); // Color Naranja Fruta Wumpa
        } else if (item->getType() == "box") {
            visualItem->setBrush(QBrush(QColor(139, 69, 19)));  // Color Marrón Caja
        }

        visualItem->setPen(QPen(Qt::NoPen));
        visualItem->setPos(item->getPosition().x(), item->getPosition().y());
        scene->addItem(visualItem);
        visualItems.push_back(visualItem);
    }
}
