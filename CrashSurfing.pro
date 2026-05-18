QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core\main.cpp \
    core\mainwindow.cpp \
    entities/entity.cpp \
    entities/obstacle.cpp \
    entities/player.cpp

HEADERS += \
    core\mainwindow.h \
    entities/entity.h \
    entities/obstacle.h \
    entities/player.h

FORMS += \
    core\mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
