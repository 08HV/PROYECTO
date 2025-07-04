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
    qDebug() << "Destructor Nivel";
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

QList<Obstaculo*> Nivel::getObstaculos() const
{
    return obstaculos;
}


void Nivel::slotAcelerarObstaculos()
{
    acelerarObstaculos(0.05f);
}

void Nivel::slotObstaculoEliminado(Obstaculo* obs)
{
    eliminarObstaculo(obs);
}

void Nivel::iniciarTemporizadorNivel(int segundos)
{
    if (!timerNivel) {
        timerNivel = new QTimer(this);
        connect(timerNivel, &QTimer::timeout, this, &Nivel::slotActualizarTemporizadorNivel);
    }
    tiempoRestanteNivel = segundos;
    emit tiempoActualizadoNivel(tiempoRestanteNivel);
    timerNivel->start(1000); // cada segundo
}

void Nivel::detenerTemporizadorNivel()
{
    if (timerNivel) timerNivel->stop();
}

int Nivel::getTiempoRestanteNivel() const
{
    return tiempoRestanteNivel;
}

void Nivel::slotActualizarTemporizadorNivel()
{
    tiempoRestanteNivel--;
    emit tiempoActualizadoNivel(tiempoRestanteNivel);
    if (tiempoRestanteNivel <= 0) {
        timerNivel->stop();
        emit tiempoFinalizadoNivel();
    }
}
