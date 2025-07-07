#include "obstaculo.h"

Obstaculo::Obstaculo(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), velocidad(0.0f), velocidadMaxima(10.0f)
{
    timer = new QTimer(this);
}

Obstaculo::~Obstaculo()
{
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
}

float Obstaculo::getVelocidad() const
{
    return velocidad;
}

void Obstaculo::setVelocidad(float v)
{
    velocidad = v;
}

void Obstaculo::aumentarVelocidad(float incremento)
{
    if (velocidad + incremento < velocidadMaxima)
        velocidad += incremento;
    else
        velocidad = velocidadMaxima;
}
