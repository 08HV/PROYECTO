#include "gokunube.h"
#include "resources.h"
#include <QDebug>

Gokunube::Gokunube() : Personaje()
{
    flySprite = new QPixmap(resources::gokuNube);
    column = 0;
    row = 0;
    scale = 5;
    if (timer) {
        timer->stop();
        disconnect(timer, nullptr, nullptr, nullptr);
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Gokunube::animation);
    timer->start(120);

    body = new QPixmap();
    splitSprite();
    setPos(50, 200);
    setZValue(10);

}

Gokunube::~Gokunube()
{
    delete flySprite;
    delete body;
}

void Gokunube::updateSprite()
{
    if (column == 0)
        column = flySprite->width() / 2;
    else
        column = 0;
}

void Gokunube::splitSprite()
{
    *body = flySprite->copy(column, 0, flySprite->width() / 2, flySprite->height());
}


void Gokunube::animation()
{
    updateSprite();
    splitSprite();
    setPixmap(body->scaled(scale * (flySprite->width()/2) / 5, scale * flySprite->height() / 5));
}


void Gokunube::updateFisica()
{
    if (espacioPresionado) {
        velocidadY -= fuerzaVuelo;
    } else {
        velocidadY += gravedad;
    }


    if (velocidadY > 10) velocidadY = 10;
    if (velocidadY < -10) velocidadY = -10;

    setPos(pos().x() + velocidadX, pos().y() + velocidadY);

    if (pos().y() < 0) setPos(pos().x(), 0);
    if (pos().y() > 550) setPos(pos().x(), 550);
}
