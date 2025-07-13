QT       += core gui
QT += multimedia
QT += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ave.cpp \
    fuego.cpp \
    goku.cpp \
    gokunube.cpp \
    maestro.cpp \
    main.cpp \
    mainwindow.cpp \
    nivel.cpp \
    nivel1.cpp \
    nivel2.cpp \
    nivel3.cpp \
    obstaculo.cpp \
    obstaculoestatico.cpp \
    personaje.cpp \
    resources.cpp \
    tortuga.cpp

HEADERS += \
    ave.h \
    fuego.h \
    goku.h \
    gokunube.h \
    maestro.h \
    mainwindow.h \
    nivel.h \
    nivel1.h \
    nivel2.h \
    nivel3.h \
    obstaculo.h \
    obstaculoestatico.h \
    personaje.h \
    resources.h \
    tortuga.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
