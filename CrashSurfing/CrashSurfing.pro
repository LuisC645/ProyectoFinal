QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/game.cpp \
    core/gamelevel2.cpp \
    core/mainwindowlevel2.cpp \
    core\main.cpp \
    core\mainwindow.cpp \
    entities/enemy.cpp \
    entities/entity.cpp \
    entities/item.cpp \
    entities/obstacle.cpp \
    entities/player.cpp \
    entities/playerlevel2.cpp \
    entities/whirlpool.cpp

HEADERS += \
    core/game.h \
    core/game_state.h \
    core/gamelevel2.h \
    core/mainwindowlevel2.h \
    core\mainwindow.h \
    entities/enemy.h \
    entities/entity.h \
    entities/item.h \
    entities/obstacle.h \
    entities/player.h \
    entities/playerlevel2.h \
    entities/whirlpool.h

FORMS += \
    core\mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc
