#include "obstaculoestatico.h"
#include <QDebug>

ObstaculoEstatico::ObstaculoEstatico(const QString& spritePath, int frameW, int frameH, int totalF, float scale, QGraphicsItem* parent)
    : Obstaculo(parent),
    totalFrames(totalF),
    frameWidth(frameW),
    frameHeight(frameH),
    scale(scale)
{
    if (!spriteSheet.load(spritePath)) {
        qWarning() << "No se pudo cargar el sprite del obstáculo:" << spritePath;
    }


    setPixmap(spriteSheet.copy(0, 0, frameWidth, frameHeight)
    .scaled(frameWidth * scale, frameHeight * scale, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setZValue(1);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ObstaculoEstatico::animarSprite);
    timer->start(100);
}

ObstaculoEstatico::~ObstaculoEstatico()
{
    timer->stop();
    delete timer;
}

void ObstaculoEstatico::mover()
{

}

void ObstaculoEstatico::animarSprite()
{
    currentFrame = (currentFrame + 1) % totalFrames;
    int x = currentFrame * frameWidth;
    QPixmap frame = spriteSheet.copy(x, 0, frameWidth, frameHeight);
    setPixmap(frame.scaled(frameWidth * scale, frameHeight * scale, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
