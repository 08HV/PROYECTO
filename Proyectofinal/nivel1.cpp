#include "nivel1.h"
#include <QRandomGenerator>
#include <QGraphicsScene>


Nivel1::Nivel1(Goku* goku,QObject *parent)
    :Nivel(parent), goku(goku)
{
    timerTortugas = new QTimer(this);
    frecuenciaTortugasMs = 3000;
    connect(timerTortugas, &QTimer::timeout, this, &Nivel1::slotCrearTortuga);
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
    int pisoY = 610;
    int anchoPantalla = escena->width();

    Tortuga* t = new Tortuga(3.0f);

    int posY = pisoY - t->boundingRect().height();
    t->setPos(anchoPantalla, posY);

    agregarObstaculo(t);
}
