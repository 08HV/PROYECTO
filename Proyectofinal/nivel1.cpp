#include "nivel1.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <resources.h>
#include <QPixmap>


Nivel1::Nivel1(Goku* goku,QObject *parent)
    :Nivel(parent), goku(goku)
{
    timerTortugas = new QTimer(this);
    frecuenciaTortugasMs = 6000;
    connect(timerTortugas, &QTimer::timeout, this, &Nivel1::slotCrearTortuga);

    QPixmap fondoPixmap(resources::nivel1);
    fondo = new QGraphicsPixmapItem(fondoPixmap);
    fondo->setZValue(-100);
    fondo->setPos(0, 0);
    escena->addItem(fondo);

    escena->setSceneRect(0, 0, fondoPixmap.width(), fondoPixmap.height());

}



Nivel1::~Nivel1()
{
    timerTortugas->stop();
}

void Nivel1::iniciarNivel()
{

    for (Obstaculo* obs : obstaculos) {
        escena->removeItem(obs);
        obs->deleteLater();
    }
    obstaculos.clear();


    timerTortugas->start(frecuenciaTortugasMs);
    timerAceleracion->start(1000);


    slotCrearTortuga();
}

void Nivel1::slotCrearTortuga()
{
    int pisoY = fondo->pixmap().height();
    int anchoPantalla = fondo->pixmap().width();
    Tortuga* t = new Tortuga(3.0f);

    int posY = pisoY - t->boundingRect().height();
    t->setPos(anchoPantalla, posY);

    qDebug() << "Creando tortuga en x=" << anchoPantalla << ", y=" << posY;
    agregarObstaculo(t);
}

