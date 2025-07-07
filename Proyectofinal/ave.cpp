#include "ave.h"
#include <resources.h>
#include <QPixmap>
#include <QDebug>

Ave::Ave(float velocidadInicial, QGraphicsItem *parent)
    : Obstaculo(parent)
{
    velocidad = velocidadInicial;
    velocidadMaxima = 12.0f;

    spriteSheet = new QPixmap(resources::ave);
    body = new QPixmap();
    column = 0;
    scale = 3;
    totalFrames = 6;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Ave::animation);
    timer->start(120);

    splitSprite();

    int frameWidth = spriteSheet->width() / totalFrames;
    int frameHeight = spriteSheet->height();
    setPixmap(body->scaled(
        scale * frameWidth,
        scale * frameHeight,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        ));
}

Ave::~Ave()
{
    if (timer){
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    delete spriteSheet;
    spriteSheet = nullptr;
    delete body;
    body = nullptr;
}

void Ave::updateSprite()
{
    int frameWidth = spriteSheet->width() / totalFrames;
    column += frameWidth;
    if (column >= spriteSheet->width())
        column = 0;
}

void Ave::splitSprite()
{
    int frameWidth = spriteSheet->width() / totalFrames;
    int frameHeight = spriteSheet->height();
    *body = spriteSheet->copy(column, 0, frameWidth, frameHeight);
}

void Ave::animation()
{
    mover();
    updateSprite();
    splitSprite();
    int frameWidth = spriteSheet->width() / totalFrames;
    int frameHeight = spriteSheet->height();
    setPixmap(body->scaled(
        scale * frameWidth,
        scale * frameHeight,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        ));
}

void Ave::mover()
{
    setX(x() - velocidad);

    if (x() + boundingRect().width() < 0) {
        destruir();
    }
}

void Ave::destruir()
{
    emit obstaculoEliminado(this);
    deleteLater();
}
