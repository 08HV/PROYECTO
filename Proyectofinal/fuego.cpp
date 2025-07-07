#include "fuego.h"
#include <resources.h>
#include <QPixmap>

Fuego::Fuego(QGraphicsItem *parent)
    : Obstaculo(parent)
{
    spriteSheet = new QPixmap(resources::Fuego);
    body = new QPixmap();
    column = 0;
    row = 0;
    scale = 6;

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Fuego::animation);
    timerAnimacion->start(160);

    splitSprite();
    setPixmap(body->scaled(scale * (spriteSheet->width()/2) / 5, scale * spriteSheet->height() / 5));
    setZValue(50);
}

Fuego::~Fuego()
{
    if (timerAnimacion) timerAnimacion->stop();
    delete spriteSheet;
    delete body;
}

void Fuego::updateSprite()
{
    if (column == 0)
        column = spriteSheet->width() / 2;
    else
        column = 0;
}

void Fuego::splitSprite()
{

    *body = spriteSheet->copy(column, 0, spriteSheet->width() / 2, spriteSheet->height());
}

void Fuego::animation()
{
    updateSprite();
    splitSprite();
    setPixmap(body->scaled(scale * (spriteSheet->width()/2) / 4, scale * spriteSheet->height() / 4));
}

void Fuego::mover() {
}
