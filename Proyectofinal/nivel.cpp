#include "nivel.h"

Nivel::Nivel(QObject *parent)
    : QObject(parent)
{
    escena = new QGraphicsScene(this);
    timerAceleracion = new QTimer(this);
    connect(timerAceleracion, &QTimer::timeout, this, &Nivel::slotAcelerarObstaculos);

}

Nivel::~Nivel()
{
    obstaculos.clear();
}

void Nivel::agregarObstaculo(Obstaculo* obs)
{
    if (!obs) return;
    obstaculos.append(obs);
    escena->addItem(obs);

    connect(obs, &Obstaculo::obstaculoEliminado, this, &Nivel::slotObstaculoEliminado);
}

void Nivel::eliminarObstaculo(Obstaculo* obs)
{
    if (!obs) return;
    obstaculos.removeAll(obs);
    escena->removeItem(obs);
    obs->deleteLater();
}

void Nivel::acelerarObstaculos(float incremento)
{
    for (Obstaculo* obs : obstaculos) {
        if (obs) obs->aumentarVelocidad(incremento);
    }
}

QGraphicsScene* Nivel::getEscena() const
{
    return escena;
}

void Nivel::slotAcelerarObstaculos()
{
    acelerarObstaculos(0.05f);
}

void Nivel::slotObstaculoEliminado(Obstaculo* obs)
{
    eliminarObstaculo(obs);
}
