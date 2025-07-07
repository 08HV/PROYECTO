#ifndef FUEGO_H
#define FUEGO_H

#include <QtCore/qglobal.h>

#include "obstaculo.h"
#include <QTimer>
#include <QPixmap>

class Fuego : public Obstaculo
{
    Q_OBJECT
public:
    Fuego(QGraphicsItem *parent = nullptr);
    ~Fuego();

    void updateSprite();
    void splitSprite();
    void animation();
    void mover() override;

private:
    QPixmap* spriteSheet;
    QPixmap* body;
    int column = 0;
    int row = 0;
    int scale = 4;
    QTimer* timerAnimacion = nullptr;
};

#endif // FUEGO_H
