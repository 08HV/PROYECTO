#ifndef OBSTACULOESTATICO_H
#define OBSTACULOESTATICO_H

#include "obstaculo.h"
#include <QPixmap>
#include <QTimer>

class ObstaculoEstatico : public Obstaculo
{
    Q_OBJECT

public:
    explicit ObstaculoEstatico(const QString& spritePath, int frameWidth, int frameHeight, int totalFrames, float scale = 1.0f, QGraphicsItem* parent = nullptr);
    ~ObstaculoEstatico();

    void mover() override;
    void animarSprite();

private:
    QPixmap spriteSheet;
    QTimer* timer;
    int currentFrame = 0;
    int totalFrames;
    int frameWidth;
    int frameHeight;
    float scale;
};

#endif // OBSTACULOESTATICO_H
