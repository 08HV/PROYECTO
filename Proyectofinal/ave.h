#ifndef AVE_H
#define AVE_H

#include "obstaculo.h"
#include <QPixmap>
#include <QTimer>

class Ave : public Obstaculo
{
    Q_OBJECT

public:
    Ave(float velocidadInicial, QGraphicsItem *parent = nullptr);
    ~Ave();

    void mover() override;
    void destruir();

    void updateSprite();
    void splitSprite();
    void animation();

private:
    QPixmap* spriteSheet = nullptr;
    QPixmap* body = nullptr;
    int column = 0;
    int totalFrames = 6;
    int scale = 3;
};

#endif // AVE_H
